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
 * Copyright 2005 Develer S.r.l. (http://www.develer.com/)
 *
 * -->
 *
 * \version $Id: timer_avr.c 1792 2008-09-05 16:12:40Z batt $
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Francesco Sacchi <batt@develer.com>
 *
 * \brief Low-level timer module for AVR (implementation).
 *
 * This module is automatically included so no need to include 
 * in test list.
 * notest: avr
 */

#include <drv/timer_avr.h>
#include <cfg/macros.h> // BV()

#include <cpu/types.h>
#include <cpu/irq.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#if CPU_AVR_ATMEGA1281 || CPU_AVR_ATMEGA168
	#define REG_TIFR0 TIFR0
	#define REG_TIFR2 TIFR2

	#define REG_TIMSK0 TIMSK0
	#define REG_TIMSK2 TIMSK2

	#define REG_TCCR2A TCCR2A
	#define REG_TCCR2B TCCR2B

	#define REG_OCR2A  OCR2A

	#define BIT_OCF0A  OCF0A
	#define BIT_OCF2A  OCF2A

	#define BIT_OCIE0A OCIE0A
	#define BIT_OCIE2A OCIE2A
#else
	#define REG_TIFR0 TIFR
	#define REG_TIFR2 TIFR

	#define REG_TIMSK0 TIMSK
	#define REG_TIMSK2 TIMSK

	#define REG_TCCR2A TCCR2
	#define REG_TCCR2B TCCR2

	#define REG_OCR2A  OCR2

	#define BIT_OCF0A  OCF0
	#define BIT_OCF2A  OCF2

	#define BIT_OCIE0A OCIE0
	#define BIT_OCIE2A OCIE2
#endif

#if CPU_AVR_ATMEGA128 || CPU_AVR_ATMEGA64 || CPU_AVR_ATMEGA103
    /* These ATMega have different prescaler options. */
    #define TIMER0_PRESCALER_64 BV(CS02)
    #define TIMER2_PRESCALER_64 (BV(CS21) | BV(CS20))
#else
    #define TIMER0_PRESCALER_64 (BV(CS01) | BV(CS00))
    #define TIMER2_PRESCALER_64 BV(CS22)
#endif

/** HW dependent timer initialization  */
#if (CONFIG_TIMER == TIMER_ON_OUTPUT_COMPARE0)

	static void timer_hw_init(void)
	{
		cpu_flags_t flags;
		IRQ_SAVE_DISABLE(flags);

		/* Reset Timer flags */
		REG_TIFR0 = BV(BIT_OCF0A) | BV(TOV0);

		/* Setup Timer/Counter interrupt */
		ASSR = 0x00;                  /* Internal system clock */
		TCCR0 = BV(WGM01)             /* Clear on Compare match */
			#if TIMER_PRESCALER == 64
				| TIMER0_PRESCALER_64
			#else
				#error Unsupported value of TIMER_PRESCALER
			#endif
		;
		TCNT0 = 0x00;                 /* Initialization of Timer/Counter */
		OCR0 = OCR_DIVISOR;           /* Timer/Counter Output Compare Register */

		/* Enable timer interrupts: Timer/Counter2 Output Compare (OCIE2) */
		REG_TIMSK0 &= ~BV(TOIE0);
		REG_TIMSK0 |= BV(OCIE0);

		IRQ_RESTORE(flags);
	}

	INLINE hptime_t timer_hw_hpread(void)
	{
		return TCNT0;
	}

#elif (CONFIG_TIMER == TIMER_ON_OVERFLOW1)

	static void timer_hw_init(void)
	{
		cpu_flags_t flags;
		IRQ_SAVE_DISABLE(flags);

		/* Reset Timer overflow flag */
		TIFR |= BV(TOV1);

		/* Fast PWM mode, 9 bit, 24 kHz, no prescaling. */
		#if (TIMER_PRESCALER == 1) && (TIMER_HW_BITS == 9)
			TCCR1A |= BV(WGM11);
			TCCR1A &= ~BV(WGM10);
			TCCR1B |= BV(WGM12) | BV(CS10);
			TCCR1B &= ~(BV(WGM13) | BV(CS11) | BV(CS12));
		/* Fast PWM mode, 8 bit, 24 kHz, no prescaling. */
		#elif (TIMER_PRESCALER == 1) && (TIMER_HW_BITS == 8)
			TCCR1A |= BV(WGM10);
			TCCR1A &= ~BV(WGM11);
			TCCR1B |= BV(WGM12) | BV(CS10);
			TCCR1B &= ~(BV(WGM13) | BV(CS11) | BV(CS12));
		#else
			#error Unsupported value of TIMER_PRESCALER or TIMER_HW_BITS
		#endif

		TCNT1 = 0x00;         /* initialization of Timer/Counter */

		/* Enable timer interrupt: Timer/Counter1 Overflow */
		TIMSK |= BV(TOIE1);

		IRQ_RESTORE(flags);
	}

	INLINE hptime_t timer_hw_hpread(void)
	{
		return TCNT1;
	}

#elif (CONFIG_TIMER == TIMER_ON_OUTPUT_COMPARE2)
	static void timer_hw_init(void)
	{
		cpu_flags_t flags;
		IRQ_SAVE_DISABLE(flags);

		/* Reset Timer flags */
		REG_TIFR2 = BV(BIT_OCF2A) | BV(TOV2);

		/* Setup Timer/Counter interrupt */
		REG_TCCR2A = 0;	// TCCR2 reg could be separate or a unique register with both A & B values, this is needed to
		REG_TCCR2B = 0; // ensure correct initialization.

		REG_TCCR2A = BV(WGM21);
		#if TIMER_PRESCALER == 64
			REG_TCCR2B |= TIMER2_PRESCALER_64;
		#else
			#error Unsupported value of TIMER_PRESCALER
		#endif

		/* Clear on Compare match & prescaler = 64, internal sys clock.
		   When changing prescaler change TIMER_HW_HPTICKS_PER_SEC too */
		TCNT2 = 0x00;         /* initialization of Timer/Counter */
		REG_OCR2A = OCR_DIVISOR;   /* Timer/Counter Output Compare Register */

		/* Enable timer interrupts: Timer/Counter2 Output Compare (OCIE2) */
		REG_TIMSK2 &= ~BV(TOIE2);
		REG_TIMSK2 |= BV(BIT_OCIE2A);

		IRQ_RESTORE(flags);
	}

	INLINE hptime_t timer_hw_hpread(void)
	{
		return TCNT2;
	}
#elif (CONFIG_TIMER == TIMER_ON_OVERFLOW3)

	static void timer_hw_init(void)
	{
		cpu_flags_t flags;
		IRQ_SAVE_DISABLE(flags);

		/* Reset Timer overflow flag */
		TIFR |= BV(TOV3);

		/* Fast PWM mode, 9 bit, 24 kHz, no prescaling. */
		#if (TIMER_PRESCALER == 1) && (TIMER_HW_BITS == 9)
			TCCR3A |= BV(WGM31);
			TCCR3A &= ~BV(WGM30);
			TCCR3B |= BV(WGM32) | BV(CS30);
			TCCR3B &= ~(BV(WGM33) | BV(CS31) | BV(CS32));
		/* Fast PWM mode, 8 bit, 24 kHz, no prescaling. */
		#elif (TIMER_PRESCALER == 1) && (TIMER_HW_BITS == 8)
			TCCR3A |= BV(WGM30);
			TCCR3A &= ~BV(WGM31);
			TCCR3B |= BV(WGM32) | BV(CS30);
			TCCR3B &= ~(BV(WGM33) | BV(CS31) | BV(CS32));
		#else
			#error Unsupported value of TIMER_PRESCALER or TIMER_HW_BITS
		#endif

		TCNT3 = 0x00;         /* initialization of Timer/Counter */

		/* Enable timer interrupt: Timer/Counter3 Overflow */
		/* ATTENTION! TOIE3 is only on ETIMSK, not TIMSK */
		ETIMSK |= BV(TOIE3);

		IRQ_RESTORE(flags);
	}

	INLINE hptime_t timer_hw_hpread(void)
	{
		return TCNT3;
	}

#else
	#error Unimplemented value for CONFIG_TIMER
#endif /* CONFIG_TIMER */

