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
 *
 * -->
 *
 * \version $Id: wdt.h 1571 2008-08-08 08:16:30Z bernie $
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 *
 * \brief Watchdog interface
 */

#ifndef DRV_WDT_H
#define DRV_WDT_H

#include "cfg/cfg_wdt.h"
#include <cfg/compiler.h> // INLINE
#include "cfg/cfg_arch.h"

/* Configury sanity check */
#if !defined(CONFIG_WATCHDOG) || (CONFIG_WATCHDOG != 0 && CONFIG_WATCHDOG != 1)
	#error CONFIG_WATCHDOG must be defined to either 0 or 1
#endif

#if CONFIG_WATCHDOG
	#include <cpu/detect.h>
	#include <cfg/os.h>

	#if OS_QT
			#include <QtGui/QApplication>
	#elif OS_POSIX
		#include <sys/select.h>
	#elif CPU_AVR
		#include <avr/io.h>
		#include <cfg/macros.h> // BV()
		#if CPU_AVR_ATMEGA1281  // Name is different in atmega1281
			#define WDTCR WDTCSR
		#endif
	#elif defined(ARCH_FREERTOS) && (ARCH & ARCH_FREERTOS)
		#include <task.h> /* taskYIELD() */
	#else
		#error unknown CPU
	#endif
#endif /* CONFIG_WATCHDOG */

/**
 * Reset the watchdog timer.
 */
INLINE void wdt_reset(void)
{
#if CONFIG_WATCHDOG
	#if OS_QT
		// Let Qt handle events
		ASSERT(qApp);
		qApp->processEvents();
	#elif OS_POSIX
		static struct timeval tv = { 0, 0 };
		select(0, NULL, NULL, NULL, &tv);
	#elif defined(ARCH_FREERTOS) && (ARCH & ARCH_FREERTOS)
		vTaskDelay(1);
	#elif CPU_AVR
		__asm__ __volatile__ ("wdr");
	#else
		#error unknown CPU
	#endif
#endif /* CONFIG_WATCHDOG */
}

/**
 * Set watchdog timer timeout.
 *
 * \param timeout  0: 16.3ms, 7: 2.1s
 */
INLINE void wdt_init(uint8_t timeout)
{
#if CONFIG_WATCHDOG
	#if OS_QT
		// Create a dummy QApplication object
		if (!qApp)
		{
			int argc;
			new QApplication(argc, (char **)NULL);
		}
		(void)timeout;
	#elif OS_POSIX
		(void)timeout; // NOP
	#elif defined(ARCH_FREERTOS) && (ARCH & ARCH_FREERTOS)
		/* nop */
	#elif CPU_AVR
		WDTCR |= BV(WDCE) | BV(WDE);
		WDTCR = timeout;
	#else
		#error unknown CPU
	#endif
#else
	(void)timeout;
#endif /* CONFIG_WATCHDOG */
}

INLINE void wdt_start(void)
{
#if CONFIG_WATCHDOG
	#if OS_QT
		// NOP
	#elif OS_POSIX
		// NOP
	#elif defined(ARCH_FREERTOS) && (ARCH & ARCH_FREERTOS)
		/* nop */
	#elif CPU_AVR
		WDTCR |= BV(WDE);
	#else
		#error unknown CPU
	#endif
#endif /* CONFIG_WATCHDOG */
}

INLINE void wdt_stop(void)
{
#if CONFIG_WATCHDOG
	#if OS_QT
		// NOP
	#elif OS_POSIX
		// NOP
	#elif defined(ARCH_FREERTOS) && (ARCH & ARCH_FREERTOS)
		/* nop */
	#elif CPU_AVR
		WDTCR |= BV(WDCE) | BV(WDE);
		WDTCR &= ~BV(WDE);
	#else
		#error unknown CPU
	#endif
#endif /* CONFIG_WATCHDOG */
}

#endif /* DRV_WDT_H */
