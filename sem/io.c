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
 * \author Marco Benelli <marco@develer.com>
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Daniele Basile <asterix@develer.com>
 *
 * \brief Windowing system test.
 */

#include "io.h"
#include "hw/hw_buzzer.h"

#include <cfg/macros.h>

#include <drv/timer.h>
#include <drv/buzzer.h>
#include <drv/sipo.h>

static volatile uint8_t whistle_count = 0;
static Timer whistle_timer;
static volatile uint8_t data_buf = 0;

/**
 * I bit a 1 in \a data verranno settati sul dato
 * correntemente in uscita.
 * Usare questa funzione per comandare l'IO perché è
 * thread safe.
 */
void setout(uint8_t data)
{
	ATOMIC(
		data_buf |= data;
		sipo_putchar(data_buf);
	);
}

/**
 * I bit a 1 in \a data verranno azzerati sul dato
 * correntemente in uscita.
 * Usare questa funzione per comandare l'IO perché è
 * thread safe.
 */
void resetout(uint8_t data)
{
	ATOMIC(
		data_buf &= ~data;
		sipo_putchar(data_buf);
	);
}


#define WHISTLE_ON_TIME  ms_to_ticks(200)
#define WHISTLE_OFF_TIME ms_to_ticks(300)


static void whistle_softint(void)
{
	if (whistle_timer._delay == WHISTLE_ON_TIME)
	{
		resetout(WHISTLE_OUT);
		BUZZER_OFF;
		timer_setDelay(&whistle_timer, WHISTLE_OFF_TIME);
		timer_add(&whistle_timer);
	}
	else
	{
		if (--whistle_count == 0)
			return;

		setout(WHISTLE_OUT);
		BUZZER_ON;
		timer_setDelay(&whistle_timer, WHISTLE_ON_TIME);
		timer_add(&whistle_timer);
	}
}

void whistle(uint8_t count)
{
	if (!count)
		return;

	while (whistle_count)
		/* aspetta un'eventuale fischiettamento in corso */;

	whistle_count = count;
	setout(WHISTLE_OUT);
	BUZZER_ON;
	timer_setDelay(&whistle_timer, WHISTLE_ON_TIME);
	timer_add(&whistle_timer);
}

void whistle_init(void)
{
	BUZZER_HW_INIT;
	resetout(WHISTLE_OUT);
	timer_setSoftint(&whistle_timer, (Hook)whistle_softint, NULL);
}
