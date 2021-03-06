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
 * All Rights Reserved.
 * -->
 *
 * \brief Some simple delay routines.
 *
 * Simple serial driver
 * \version $Id: timer_simple_avr.c 1398 2008-05-27 16:36:05Z asterix $
 * \author Francesco Sacchi <batt@develer.com>
 */

#warning FIXME:This module is obsolete, yuo must refactor it.

#if 0
#include "hw/hw_cpu.h"
#include "timer_simple_avr.h"
#include <drv/wdt.h>
#include <cfg/compiler.h>
#include <cfg/macros.h> /* BV() */
#include <hw_cpu.h>  /* CLOCK_FREQ */

#include <avr/io.h>


#define MS_PER_SEC       1000UL
#define TIMER_PRESCALER  64UL
#define TIMER_DELAY_1MS  (255 - CLOCK_FREQ / TIMER_PRESCALER / MS_PER_SEC)

/**
 * Wait \a time ms using timer 0.
 *
 */
void timer_delay(mtime_t time)
{
	/* Set timer clock to clock_freq/64 */
	TCCR0 = BV(CS02);

	while (time--)
	{
		/* Initialize timer counter register */
		TCNT0 = TIMER_DELAY_1MS;
		/* Clear overflow bit. */
		TIFR |= BV(TOV0);
		/* Wait overflow. */
		while (!(TIFR & BV(TOV0)));
#if CONFIG_WATCHDOG
		wdt_reset();
#endif
	}
}
#endif

