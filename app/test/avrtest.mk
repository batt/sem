#
# $Id: avrtest.mk,v 1.3 2006/06/12 21:37:02 marco Exp $
# Copyright 2006 Develer S.r.l. (http://www.develer.com/)
# All rights reserved.
#
# Makefile fragment for DevLib avrtest application.
#
# Author: Bernie Innocenti <bernie@codewiz.org>
#



# Set to 1 for debug builds
avrtest_DEBUG = 1

# Our target application
TRG += avrtest

avrtest_MCU = atmega64

#include avr c and asm sources
include app/test/avr_src.mk

avrtest_CSRC = $(avr_CSRC) app/test/empty_main.c

avrtest_CPPASRC = $(avr_ASRC)

avrtest_PCSRC += bertos/mware/formatwr.c

avrtest_CROSS = avr-
avrtest_CPPFLAGS = -D'ARCH=ARCH_NIGHTTEST' -Iapp/test -Ibertos/cpu/avr 
avrtest_CFLAGS = -Os -mcall-prologues -fno-strict-aliasing
