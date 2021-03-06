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
 * Copyright 2004 Develer S.r.l. (http://www.develer.com/)
 * Copyright 1999, 2000, 2001, 2008 Bernie Innocenti <bernie@codewiz.org>
 * -->
 *
 * \brief Heap subsystem (public interface).
 *
 * \todo Heap memory could be defined as an array of MemChunk, and used
 * in this form also within the implementation. This would probably remove
 * memory alignment problems, and also some aliasing issues.
 *
 * \version $Id: heap.h 1532 2008-08-04 07:21:26Z bernie $
 * \author Bernie Innocenti <bernie@codewiz.org>
 */

#ifndef STRUCT_HEAP_H
#define STRUCT_HEAP_H

#include "cfg/cfg_heap.h"
#include <cfg/compiler.h>

struct _MemChunk;

/// A heap
struct Heap
{
	struct _MemChunk *FreeList;     ///< Head of the free list
};


/// Initialize \a heap within the buffer pointed by \a memory which is of \a size bytes
void heap_init(struct Heap* heap, void* memory, size_t size);

/// Allocate a chunk of memory of \a size bytes from the heap
void *heap_allocmem(struct Heap* heap, size_t size);

/// Free a chunk of memory of \a size bytes from the heap
void heap_freemem(struct Heap* heap, void *mem, size_t size);


#define HNEW(heap, type) \
	(type*)heap_allocmem(heap, sizeof(type))

#define HNEWVEC(heap, type, nelem) \
	(type*)heap_allocmem(heap, sizeof(type) * (nelem))

#define HDELETE(heap, type, mem) \
	heap_freemem(heap, mem, sizeof(type))

#define HDELETEVEC(heap, type, nelem, mem) \
	heap_freemem(heap, mem, sizeof(type) * (nelem))


#if CONFIG_HEAP_MALLOC

void *heap_malloc(struct Heap* heap, size_t size);
void *heap_calloc(struct Heap* heap, size_t size);
void heap_free(struct Heap* heap, void * mem);

#endif

#endif /* STRUCT_HEAP_H */
