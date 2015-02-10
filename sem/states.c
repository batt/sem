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
 * Copyright 2003, 2004, 2006, 2008 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2000 Bernie Innocenti <bernie@codewiz.org>
 *
 * -->
 *
 * \version $Id: triface.c 1550 2008-08-05 15:28:24Z qwert $
 *
 * \author Francesco Sacchi <batt@develer.com>
 *
 * \brief Signal light state machine.
 */

#include "states.h"
#include "io.h"
#include "hw/hw_buzzer.h"

#include <cfg/macros.h>
#include <drv/buzzer.h>
#include <drv/timer.h>
#include <drv/kbd.h>

/* Mettere a 1 per mandare il ciclo in automatico */
#define SEM_TEST 0

static LightStates state_reset(UNUSED_ARG(LightTimes *, times))
{
	TRACE;
	resetout(GREEN_OUT | ORANGE_OUT);
	setout(RED_OUT);
	buz_beep(200);

	#if SEM_TEST
	static int i = 1;
	kprintf("Ciclo %d\n", i++);

	ticks_t start = timer_clock();
	while ((!(kbd_peek() & K_START))
	&& (timer_clock() - start < ms_to_ticks(10000)));
	#else
	while (!(kbd_get() & K_START));
	#endif

	return LS_SHOOTINGLINE;
}

static LightStates state_shootingLine(LightTimes *times)
{
	TRACE;
	resetout(GREEN_OUT | ORANGE_OUT);
	setout(RED_OUT);
	whistle(2);
	ticks_t start = timer_clock();

	for (;;)
	{
		keymask_t k = kbd_peek();
		if (k & K_STOP)
			return LS_RESET;

		if ((k & K_START) ||  (timer_clock() - start >= ms_to_ticks(times->shootline_time)))
			return LS_SHOOTING;

		if (!((timer_clock() - start) % ms_to_ticks(1000)))
		{
			if (timer_clock() - start < ms_to_ticks(times->shootline_time))
				buz_beep(100);
			else
				buz_beep(500);
		}
		#if SEM_TEST
		if (timer_clock() - start >= ms_to_ticks(15000))
			return LS_SHOOTING;
		#endif
	}
}

static LightStates state_shooting(LightTimes *times)
{
	TRACE;
	resetout(RED_OUT | ORANGE_OUT);
	setout(GREEN_OUT);
	whistle(1);
	ticks_t start = timer_clock();

	for (;;)
	{
		keymask_t k = kbd_peek();
		if (k & K_STOP)
			return LS_STOPSHOOTING;

		if (k & K_START)
			return LS_SHOOTINGLINE;

		if (timer_clock() - start >= ms_to_ticks(times->green_time))
			return LS_TIMEEXPIRING;
	}
}


static LightStates state_timeExpiring(LightTimes *times)
{
	TRACE;
	resetout(RED_OUT | GREEN_OUT);
	setout(ORANGE_OUT);
	ticks_t start = timer_clock();

	for (;;)
	{
		keymask_t k = kbd_peek();

		if ((k & K_STOP) || (timer_clock() - start >= ms_to_ticks(times->orange_time)))
			return LS_STOPSHOOTING;

		if (k & K_START)
			return LS_SHOOTINGLINE;

		/* Fai un beep ogni secondo a partire dagli ultimi 5 secondi */
		if ((timer_clock() - start >= ms_to_ticks(times->orange_time - 5000))
			&& !((timer_clock() - start) % ms_to_ticks(1000)))
		{
			if (timer_clock() - start < ms_to_ticks(times->orange_time))
				buz_beep(100);
			else
				BUZZER_ON;
		}
		#if SEM_TEST
		if (timer_clock() - start >= ms_to_ticks(times->orange_time))
			return LS_STOPSHOOTING;
		#endif
	}
}


static LightStates state_stopShooting(UNUSED_ARG(LightTimes *, times))
{
	TRACE;
	resetout(ORANGE_OUT | GREEN_OUT);
	setout(RED_OUT);
	whistle(3);

	return LS_RESET;
}

typedef LightStates (*state_callback)(LightTimes *);

state_callback machine[] =
{
	/* LS_RESET */        state_reset,
	/* LS_SHOOTINGLINE */ state_shootingLine,
	/* LS_SHOOTING */     state_shooting,
	/* LS_TIMEEXPIRING */ state_timeExpiring,
	/* LS_STOPSHOOTING */ state_stopShooting,
};

void NORETURN state_run(LightTimes *times)
{
	state_callback hook = state_reset;
	for (;;)
			hook = machine[hook(times)];
}
