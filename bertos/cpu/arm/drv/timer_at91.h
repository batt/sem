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
 * Copyright 2007 Develer S.r.l. (http://www.develer.com/)
 *
 * -->
 *
 * \version $Id: timer_at91.h 1750 2008-08-29 14:54:43Z batt $
 *
 * \author Francesco Sacchi <batt@develer.com>
 *
 * \brief Low-level timer module for Atmel AT91 (interface).
 */

#ifndef DRV_AT91_TIMER_H
#define DRV_AT91_TIMER_H

#include "hw/hw_cpu.h"            /* CLOCK_FREQ */

#include "cfg/cfg_timer.h"     /* CONFIG_TIMER */
#include <cfg/compiler.h>      /* uint8_t */

/**
 * \name Values for CONFIG_TIMER.
 *
 * Select which hardware timer interrupt to use for system clock and softtimers.
 *
 * \{
 */
#define TIMER_ON_PIT 1  ///< System timer on Periodic interval timer

#define TIMER_DEFAULT TIMER_ON_PIT  ///< Default system timer
/* \} */

/*
 * Hardware dependent timer initialization.
 */
#if (CONFIG_TIMER == TIMER_ON_PIT)

	void timer_handler(void);

	#define DEFINE_TIMER_ISR     void timer_handler(void)
	#define TIMER_TICKS_PER_SEC  1000
	#define TIMER_HW_CNT         (CLOCK_FREQ / (16 * TIMER_TICKS_PER_SEC) - 1)

	/** Frequency of the hardware high-precision timer. */
	#define TIMER_HW_HPTICKS_PER_SEC (CLOCK_FREQ / 16)

	/// Type of time expressed in ticks of the hardware high-precision timer
	typedef uint32_t hptime_t;
#else

	#error Unimplemented value for CONFIG_TIMER
#endif /* CONFIG_TIMER */


#endif /* DRV_TIMER_AT91_H */
