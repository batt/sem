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
 * \version $Id: at91sam7s.c 1717 2008-08-27 13:35:20Z batt $
 *
 * \author Francesco Sacchi <batt@develer.com>
 *
 * \brief AT91SAM7S-EK porting test.
 */

#include <cfg/macros.h>
#include <drv/timer.h>
#include <drv/sysirq_at91.h>
#include <kern/proc.h>
#include <drv/ser.h>
#include <cfg/macros.h>
#include <io/arm.h>

Timer leds_timer;
Serial ser_fd;

static void leds_toggle(void)
{
	uint8_t a = (~PIOA_ODSR & 0x0f);

	if (a)
	{
		PIOA_SODR = a;
		PIOA_CODR = a << 1;
	}
	else
	{
		PIOA_SODR  =  0x0f;
		/* turn first led on */
		PIOA_CODR  = 0x00000001;
	}

	/* Wait for interval time */
	timer_setDelay(&leds_timer, ms_to_ticks(100));
	timer_add(&leds_timer);
}


int main(void)
{
	char msg[]="BeRTOS, be fast be beatiful be realtime";
	kdbg_init();
	timer_init();

	proc_init();
	ASSERT(!IRQ_ENABLED());

	/* Open the main communication port */
	ser_init(&ser_fd, 0);
	ser_setbaudrate(&ser_fd, 115200);
	ser_setparity(&ser_fd, SER_PARITY_NONE);


	IRQ_ENABLE;
	ASSERT(IRQ_ENABLED());

	/* Disable all pullups */
	PIOA_PUDR = 0xffffffff;
	/* Set PA0..3 connected to PIOA */
	PIOA_PER  = 0x0000001f;
	/* Set PA0..3 as output */
	PIOA_OER  = 0x0000001f;
	/* Disable multidrive on all pins */
	PIOA_MDDR = 0x0000001f;

	/* Set PA0..3 to 1 to turn off leds */
	PIOA_SODR  = 0x0000000f;
	/* turn first led on */
	PIOA_CODR  = 0x00000001;

 	timer_setSoftint(&leds_timer, (Hook)leds_toggle, 0);
 	timer_setDelay(&leds_timer, ms_to_ticks(100));
 	timer_add(&leds_timer);

	ASSERT(proc_testRun() == 0);
	// Main loop
	for(;;)
	{
		kfile_printf(&ser_fd.fd, "From serial 0: %s\r\n", msg);
	}
	return 0;
}
