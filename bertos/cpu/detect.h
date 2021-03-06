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
 * Copyright 2004, 2005, 2006, 2007 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2004 Giovanni Bajo
 *
 * -->
 *
 * \brief CPU detection through special preprocessor macros
 */
#ifndef CPU_DETECT_H
#define CPU_DETECT_H

#if defined(__arm__) /* GCC */ \
	|| defined(__ARM4TM__) /* IAR: defined for all cores >= 4tm */
	#define CPU_ARM                 1
	#define CPU_ID                  arm

	// AT91SAM7S core family
	#if defined(__ARM_AT91SAM7S32__)
		#define CPU_ARM_AT91         1
		#define CPU_ARM_AT91SAM7S32  1
	#else
		#define CPU_ARM_AT91SAM7S32  0
	#endif

	#if defined(__ARM_AT91SAM7S64__)
		#define CPU_ARM_AT91         1
		#define CPU_ARM_AT91SAM7S64  1
	#else
		#define CPU_ARM_AT91SAM7S64  0
	#endif

	#if defined(__ARM_AT91SAM7S128__)
		#define CPU_ARM_AT91         1
		#define CPU_ARM_AT91SAM7S128 1
	#else
		#define CPU_ARM_AT91SAM7S128 0
	#endif

	#if defined(__ARM_AT91SAM7S256__)
		#define CPU_ARM_AT91         1
		#define CPU_ARM_AT91SAM7S256 1
	#else
		#define CPU_ARM_AT91SAM7S256 0
	#endif

	// AT91SAM7X core family
	#if defined(__ARM_AT91SAM7X128__)
		#define CPU_ARM_AT91         1
		#define CPU_ARM_AT91SAM7X128 1
	#else
		#define CPU_ARM_AT91SAM7X128 0
	#endif

	#if defined(__ARM_AT91SAM7X256__)
		#define CPU_ARM_AT91         1
		#define CPU_ARM_AT91SAM7X256 1
	#else
		#define CPU_ARM_AT91SAM7X256 0
	#endif

	#if defined (__ARM_LM3S1968__)
		#define CPU_ARM_LM3S        1
		#define CPU_ARM_LM3S1968    1
	#else
		#define CPU_ARM_LM3S1968    0
	#endif



	#if defined(CPU_ARM_AT91)
		#if CPU_ARM_AT91SAM7S32 + CPU_ARM_AT91SAM7S64 \
		+ CPU_ARM_AT91SAM7S128 + CPU_ARM_AT91SAM7S256 \
		+ CPU_ARM_AT91SAM7X128 + CPU_ARM_AT91SAM7X256 != 1
			#error ARM CPU configuration error
		#endif
		#define CPU_ARM_LM3S        0

	#elif defined (CPU_ARM_LM3S)
		#if CPU_ARM_LM3S1968 + 0 != 1
			#error Luminary ARM CPU configuration error
		#endif
		#define CPU_ARM_AT91        0
	/* #elif Add other ARM families here */
	#else
		#define CPU_ARM_AT91        0
		#define CPU_ARM_LM3S        0
	#endif


	#if CPU_ARM_AT91 + CPU_ARM_LM3S + 0 /* Add other ARM families here */ != 1
		#error ARM CPU configuration error
	#endif
#else
	#define CPU_ARM                 0

	/* ARM Families */
	#define CPU_ARM_AT91            0
	#define CPU_ARM_LM3S            0

	/* ARM CPUs */
	#define CPU_ARM_AT91SAM7S32     0
	#define CPU_ARM_AT91SAM7S64     0
	#define CPU_ARM_AT91SAM7S128    0
	#define CPU_ARM_AT91SAM7S256    0
	#define CPU_ARM_AT91SAM7X128    0
	#define CPU_ARM_AT91SAM7X256    0

	#define CPU_ARM_LM3S1968        0
#endif

#if (defined(__IAR_SYSTEMS_ICC__) || defined(__IAR_SYSTEMS_ICC)) \
	&& !defined(__ARM4TM__) /* IAR: if not ARM assume I196 */
	#warning Assuming CPU is I196
	#define CPU_I196		1
	#define CPU_ID                  i196
#else
	#define CPU_I196                0
#endif

#if defined(__i386__) /* GCC */ \
	|| (defined(_M_IX86) && !defined(_WIN64)) /* MSVC */
	#define CPU_X86                 1
	#define CPU_X86_32              1
	#define CPU_X86_64              0
	#define CPU_ID                  x86
#elif defined(__x86_64__) /* GCC */ \
	|| (defined(_M_IX86) && defined(_WIN64)) /* MSVC */
	#define CPU_X86                 1
	#define CPU_X86_32              0
	#define CPU_X86_64              1
	#define CPU_ID                  x86
#else
	#define CPU_X86                 0
	#define CPU_I386                0
	#define CPU_X86_64              0
#endif

#if defined (_ARCH_PPC) || defined(_ARCH_PPC64)
	#define CPU_PPC                 1
	#define CPU_ID                  ppc
	#if defined(_ARCH_PPC)
		#define CPU_PPC32       1
	#else
	        #define CPU_PPC32       0
	#endif
	#if defined(_ARCH_PPC64)
		#define CPU_PPC64       1
	#else
	        #define CPU_PPC64       0
	#endif
#else
	#define CPU_PPC                 0
	#define CPU_PPC32               0
	#define CPU_PPC64               0
#endif

#if defined(__m56800E__) || defined(__m56800__)
	#define CPU_DSP56K              1
	#define CPU_ID                  dsp56k
#else
	#define CPU_DSP56K              0
#endif

#if defined (__AVR__)
	#define CPU_AVR                 1
	#define CPU_ID                  avr

	#if defined(__AVR_ATmega32__)
		#define CPU_AVR_ATMEGA32    1
	#else
		#define CPU_AVR_ATMEGA32    0
	#endif

	#if defined(__AVR_ATmega64__)
		#define CPU_AVR_ATMEGA64    1
	#else
		#define CPU_AVR_ATMEGA64    0
	#endif

	#if defined(__AVR_ATmega103__)
		#define CPU_AVR_ATMEGA103   1
	#else
		#define CPU_AVR_ATMEGA103   0
	#endif

	#if defined(__AVR_ATmega128__)
		#define CPU_AVR_ATMEGA128   1
	#else
		#define CPU_AVR_ATMEGA128   0
	#endif

	#if defined(__AVR_ATmega8__)
		#define CPU_AVR_ATMEGA8     1
	#else
		#define CPU_AVR_ATMEGA8     0
	#endif

	#if defined(__AVR_ATmega168__)
		#define CPU_AVR_ATMEGA168   1
	#else
		#define CPU_AVR_ATMEGA168   0
	#endif

	#if defined(__AVR_ATmega1281__)
		#define CPU_AVR_ATMEGA1281  1
	#else
		#define CPU_AVR_ATMEGA1281  0
	#endif

	#if CPU_AVR_ATMEGA32 + CPU_AVR_ATMEGA64 + CPU_AVR_ATMEGA103 + CPU_AVR_ATMEGA128 \
	  + CPU_AVR_ATMEGA8 + CPU_AVR_ATMEGA168 + CPU_AVR_ATMEGA1281 != 1
		#error AVR CPU configuration error
	#endif
#else
	#define CPU_AVR                 0
	#define CPU_AVR_ATMEGA8         0
	#define CPU_AVR_ATMEGA168       0
	#define CPU_AVR_ATMEGA32        0
	#define CPU_AVR_ATMEGA64        0
	#define CPU_AVR_ATMEGA103       0
	#define CPU_AVR_ATMEGA128       0
	#define CPU_AVR_ATMEGA1281      0
#endif


/* Self-check for the detection: only one CPU must be detected */
#if CPU_ARM + CPU_I196 + CPU_X86 + CPU_PPC + CPU_DSP56K + CPU_AVR == 0
	#error Unknown CPU
#elif !defined(CPU_ID)
	#error CPU_ID not defined
#elif CPU_ARM + CPU_I196 + CPU_X86 + CPU_PPC + CPU_DSP56K + CPU_AVR != 1
	#error Internal CPU configuration error
#endif


#endif /* CPU_DETECT_H */
