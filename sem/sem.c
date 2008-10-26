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
#include "states.h"

#include "hw/hw_input.h"
#include "cfg/cfg_ser.h"

#include <cfg/macros.h>

#include <drv/buzzer.h>
#include <drv/timer.h>
#include <drv/kbd.h>
#include <drv/ser.h>
#include <drv/sipo.h>

int main(void)
{
	Serial fd_ser;
	Serial tag_ser;

	kdbg_init();
	sipo_init();
	timer_init();
	IRQ_ENABLE;
	INPUT_INIT;
	kbd_init();
	buz_init();
	whistle_init();

	/* Open the main communication port */
	ser_init(&fd_ser, CONFIG_TRIFACE_PORT);
	ser_setbaudrate(&fd_ser, CONFIG_TRIFACE_BAUDRATE);

	ser_init(&tag_ser, TAG_SER_PORT);
	ser_setbaudrate(&tag_ser, TAG_SER_BAUDRATE);


	LightTimes times;
	times.shootline_time  = 20000UL;
	times.green_time  = 90000UL;
	times.orange_time = 30000UL;

	state_run(&times);

	/* La macchina a stati non dovrebbe ritornare mai, in ogni caso... */
	resetout(GREEN_OUT | ORANGE_OUT);
	setout(RED_OUT);
	ASSERT(0);

	return 0;
}


