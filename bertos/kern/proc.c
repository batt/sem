/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2001, 2004 Develer S.r.l. (http://www.develer.com/)
 * Copyright 1999, 2000, 2001, 2008 Bernie Innocenti <bernie@codewiz.org>
 * -->
 *
 * \brief Simple cooperative multitasking scheduler.
 *
 * \version $Id: proc.c 1776 2008-09-03 14:11:01Z batt $
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Stefano Fedrigo <aleph@develer.com>
 */

#include "proc_p.h"
#include "proc.h"

#include "cfg/cfg_arch.h"  // ARCH_EMUL
#include "cfg/cfg_kern.h"
#include <cfg/macros.h>    // ROUND_UP2
#include <cfg/module.h>
#include <cfg/depend.h>    // CONFIG_DEPEND()

#include <cpu/irq.h>
#include <cpu/types.h>
#include <cpu/attr.h>
#include <cpu/frame.h>

#if CONFIG_KERN_HEAP
	#include <struct/heap.h>
#endif

#include <string.h>           /* memset() */

// Check config dependencies
CONFIG_DEPEND(CONFIG_KERN_SIGNALS,    CONFIG_KERN_SCHED);
CONFIG_DEPEND(CONFIG_KERN_SEMAPHORES, CONFIG_KERN_SIGNALS);
CONFIG_DEPEND(CONFIG_KERN_MONITOR,    CONFIG_KERN_SCHED);


/*
 * The scheduer tracks ready processes by enqueuing them in the
 * ready list.
 *
 * \note Access to the list must occur while interrupts are disabled.
 */
REGISTER List ProcReadyList;

/*
 * Holds a pointer to the TCB of the currently running process.
 *
 * \note User applications should use proc_current() to retrieve this value.
 */
REGISTER Process *CurrentProcess;

#if (ARCH & ARCH_EMUL)
/*
 * In some hosted environments, we must emulate the stack on the real
 * process stack to satisfy consistency checks in system libraries and
 * because some ABIs place trampolines on the stack.
 *
 * Access to this list must be protected by PROC_ATOMIC().
 */
List StackFreeList;

#define NPROC 8
cpu_stack_t proc_stacks[NPROC][(64 * 1024) / sizeof(cpu_stack_t)];
#endif

/** The main process (the one that executes main()). */
struct Process MainProcess;


static void proc_init_struct(Process *proc)
{
	/* Avoid warning for unused argument. */
	(void)proc;

#if CONFIG_KERN_SIGNALS
	proc->sig_recv = 0;
#endif

#if CONFIG_KERN_HEAP
	proc->flags = 0;
#endif

#if CONFIG_KERN_PRI
	proc->link.pri = 0;
#endif
}

MOD_DEFINE(proc);

void proc_init(void)
{
	LIST_INIT(&ProcReadyList);

#if ARCH & ARCH_EMUL
	LIST_INIT(&StackFreeList);
	for (int i = 0; i < NPROC; i++)
		ADDTAIL(&StackFreeList, (Node *)proc_stacks[i]);
#endif

	/*
	 * We "promote" the current context into a real process. The only thing we have
	 * to do is create a PCB and make it current. We don't need to setup the stack
	 * pointer because it will be written the first time we switch to another process.
	 */
	proc_init_struct(&MainProcess);
	CurrentProcess = &MainProcess;

#if CONFIG_KERN_MONITOR
	monitor_init();
	monitor_add(CurrentProcess, "main");
#endif

#if CONFIG_KERN_PREEMPT
	preempt_init();
#endif

	MOD_INIT(proc);
}

/**
 * Create a new process, starting at the provided entry point.
 *
 * \return Process structure of new created process
 *         if successful, NULL otherwise.
 */
struct Process *proc_new_with_name(UNUSED_ARG(const char *, name), void (*entry)(void), iptr_t data, size_t stack_size, cpu_stack_t *stack_base)
{
	Process *proc;
	const size_t PROC_SIZE_WORDS = ROUND_UP2(sizeof(Process), sizeof(cpu_stack_t)) / sizeof(cpu_stack_t);
#if CONFIG_KERN_HEAP
	bool free_stack = false;
#endif
	TRACEMSG("name=%s", name);

#if (ARCH & ARCH_EMUL)
	/* Ignore stack provided by caller and use the large enough default instead. */
	PROC_ATOMIC(stack_base = (cpu_stack_t *)list_remHead(&StackFreeList));

	stack_size = CONFIG_KERN_MINSTACKSIZE;
#elif CONFIG_KERN_HEAP
	/* Did the caller provide a stack for us? */
	if (!stack_base)
	{
		/* Did the caller specify the desired stack size? */
		if (!stack_size)
			stack_size = CONFIG_KERN_MINSTACKSIZE;

		/* Allocate stack dinamically */
		if (!(stack_base = heap_alloc(stack_size)))
			return NULL;

		free_stack = true;
	}

#else // !ARCH_EMUL && !CONFIG_KERN_HEAP

	/* Stack must have been provided by the user */
	ASSERT_VALID_PTR(stack_base);
	ASSERT(stack_size);

#endif // !ARCH_EMUL && !CONFIG_KERN_HEAP

#if CONFIG_KERN_MONITOR
	/*
	 * Fill-in the stack with a special marker to help debugging.
	 * On 64bit platforms, CONFIG_KERN_STACKFILLCODE is larger
	 * than an int, so the (int) cast is required to silence the
	 * warning for truncating its size.
	 */
	memset(stack_base, (int)CONFIG_KERN_STACKFILLCODE, stack_size);
#endif

	/* Initialize the process control block */
	if (CPU_STACK_GROWS_UPWARD)
	{
		proc = (Process *)stack_base;
		proc->stack = stack_base + PROC_SIZE_WORDS;
		if (CPU_SP_ON_EMPTY_SLOT)
			proc->stack++;
	}
	else
	{
		proc = (Process *)(stack_base + stack_size / sizeof(cpu_stack_t) - PROC_SIZE_WORDS);
		proc->stack = (cpu_stack_t *)proc;
		if (CPU_SP_ON_EMPTY_SLOT)
			proc->stack--;
	}

	proc_init_struct(proc);
	proc->user_data = data;

#if CONFIG_KERN_HEAP | CONFIG_KERN_MONITOR | (ARCH & ARCH_EMUL)
	proc->stack_base = stack_base;
	proc->stack_size = stack_size;
	#if CONFIG_KERN_HEAP
	if (free_stack)
		proc->flags |= PF_FREESTACK;
	#endif
#endif

	#if CONFIG_KERN_PREEMPT

		getcontext(&proc->context);
		proc->context.uc_stack.ss_sp = proc->stack;
		proc->context.uc_stack.ss_size = stack_size - PROC_SIZE_WORDS - 1;
		proc->context.uc_link = NULL;
		makecontext(&proc->context, (void (*)(void))proc_entry, 1, entry);

	#else // !CONFIG_KERN_PREEMPT
	{
		size_t i;

		/* Initialize process stack frame */
		CPU_PUSH_CALL_FRAME(proc->stack, proc_exit);
		CPU_PUSH_CALL_FRAME(proc->stack, entry);

		/* Push a clean set of CPU registers for asm_switch_context() */
		for (i = 0; i < CPU_SAVED_REGS_CNT; i++)
			CPU_PUSH_WORD(proc->stack, CPU_REG_INIT_VALUE(i));
	}
	#endif // CONFIG_KERN_PREEMPT

	#if CONFIG_KERN_MONITOR
		monitor_add(proc, name);
	#endif

	/* Add to ready list */
	ATOMIC(SCHED_ENQUEUE(proc));

	return proc;
}

/**
 * Return the name of the specified process.
 *
 * NULL is a legal argument and will return the name "<NULL>".
 */
const char *proc_name(struct Process *proc)
{
	#if CONFIG_KERN_MONITOR
		return proc ? proc->monitor.name : "<NULL>";
	#else
		(void)proc;
		return "---";
	#endif
}

/// Return the name of the currently running process
const char *proc_currentName(void)
{
	return proc_name(proc_current());
}

/// Rename a process
void proc_rename(struct Process *proc, const char *name)
{
#if CONFIG_KERN_MONITOR
	monitor_rename(proc, name);
#else
	(void)proc; (void)name;
#endif
}


#if CONFIG_KERN_PRI
/**
 * Change the scheduling priority of a process.
 *
 * Process piorities are signed ints, whereas a larger integer value means
 * higher scheduling priority.  The default priority for new processes is 0.
 * The idle process runs with the lowest possible priority: INT_MIN.
 *
 * A process with a higher priority always preempts lower priority processes.
 * Processes of equal priority share the CPU time according to a simple
 * round-robin policy.
 *
 * As a general rule to maximize responsiveness, compute-bound processes
 * should be assigned negative priorities and tight, interactive processes
 * should be assigned positive priorities.
 *
 * To avoid interfering with system background activities such as input
 * processing, application processes should remain within the range -10
 * and +10.
 */
void proc_setPri(struct Process *proc, int pri)
{
		if (proc->link.pri == pri)
			return;

		proc->link.pri = pri;

		if (proc != CurrentProcess)
		{
				//proc_forbid();
				//TODO: re-enqueue process
				//pric_permit();
		}
}
#endif // CONFIG_KERN_PRI

/**
 * Terminate the current process
 */
void proc_exit(void)
{
	TRACEMSG("%p:%s", CurrentProcess, proc_currentName());

#if CONFIG_KERN_MONITOR
	monitor_remove(CurrentProcess);
#endif

#if CONFIG_KERN_HEAP
	/*
	 * The following code is BROKEN.
	 * We are freeing our own stack before entering proc_schedule()
	 * BAJO: A correct fix would be to rearrange the scheduler with
	 *  an additional parameter which frees the old stack/process
	 *  after a context switch.
	 */
	if (CurrentProcess->flags & PF_FREESTACK)
		heap_free(CurrentProcess->stack_base, CurrentProcess->stack_size);
	heap_free(CurrentProcess);
#endif

#if (ARCH & ARCH_EMUL)
	/* Reinsert process stack in free list */
	PROC_ATOMIC(ADDHEAD(&StackFreeList, (Node *)CurrentProcess->stack_base));

	/*
	 * NOTE: At this point the first two words of what used
	 * to be our stack contain a list node. From now on, we
	 * rely on the compiler not reading/writing the stack.
	 */
#endif /* ARCH_EMUL */

	CurrentProcess = NULL;
	proc_switch();
	/* not reached */
}


/**
 * Get the pointer to the user data of the current process
 */
iptr_t proc_currentUserData(void)
{
	return CurrentProcess->user_data;
}
