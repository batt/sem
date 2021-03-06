#
# $Id: at91sam7s.mk 18234 2007-10-08 13:39:48Z rasky $
# Copyright 2006 Develer S.r.l. (http://www.develer.com/)
# All rights reserved.
#
# Makefile fragment for DevLib at91sam7s application.
#
# Author: Bernie Innocenti <bernie@codewiz.org>
#
#

# Set to 1 for debug builds
at91sam7s_DEBUG = 1

# Our target application
TRG += at91sam7s

at91sam7s_CSRC = \
	app/at91sam7s/at91sam7s.c \
	bertos/drv/timer.c \
	bertos/drv/ser.c \
	bertos/cpu/arm/drv/sysirq_at91.c \
	bertos/cpu/arm/drv/ser_at91.c \
	bertos/mware/event.c \
	bertos/mware/formatwr.c \
	bertos/mware/hex.c \
	bertos/kern/kfile.c \
	bertos/kern/proc.c \
	bertos/kern/coop.c \
	bertos/kern/proc_test.c \
	bertos/kern/monitor.c \
	bertos/kern/signal.c

at91sam7s_CPPASRC = \
	bertos/cpu/arm/hw/crtat91sam7_rom.S \
	bertos/cpu/arm/hw/switch_arm.S

at91sam7s_CROSS = arm-none-eabi-

at91sam7s_CPPAFLAGS = -O0 -g -gdwarf-2 -g -gen-debug
at91sam7s_CPPFLAGS = -O0 -D'ARCH=0' -D__ARM_AT91SAM7S256__ -g3 -gdwarf-2 -fverbose-asm -Iapp/at91sam7s/hw -Iapp/at91sam7s -Ibertos/cpu/arm
at91sam7s_LDFLAGS = -nostartfiles -T bertos/cpu/arm/scripts/at91sam7_256_ram.ld -Wl,--no-warn-mismatch

at91sam7s_CPU = arm7tdmi

