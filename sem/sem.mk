#
# $Id: sem.mk 1719 2008-08-27 13:53:03Z batt $
# Copyright 2006 Develer S.r.l. (http://www.develer.com/)
# All rights reserved.
#
# Makefile fragment for DevLib sem application.
#
# Author: Bernie Innocenti <bernie@codewiz.org>
#
#

# Set to 1 for debug builds
sem_DEBUG = 1

# Our target application
TRG += sem

sem_hfuse = 0x88
sem_lfuse = 0xff
sem_efuse = 0xff
sem_lock = 0x2f
sem_CSRC = \
	sem/sem.c \
	sem/io.c \
	sem/states.c \
	bertos/drv/timer.c \
	bertos/drv/ser.c \
	bertos/drv/kbd.c \
	bertos/drv/buzzer.c \
	bertos/cpu/avr/drv/ser_avr.c \
	bertos/cpu/avr/drv/sipo.c \
	bertos/mware/formatwr.c \
	bertos/mware/hex.c \
	bertos/mware/event.c \
	bertos/kern/kfile.c \
	#

sem_PCSRC += bertos/mware/formatwr.c


sem_CFLAGS = -O2 -D'ARCH=(ARCH_SEM)' -fno-strict-aliasing -Isem -Ibertos/cpu/avr
sem_LDFLAGS = -Wl

sem_MCU = atmega64
sem_CROSS = avr-

