#
# $Id: armtest.mk 18234 2007-10-08 13:39:48Z rasky $
# Copyright 2006 Develer S.r.l. (http://www.develer.com/)
# All rights reserved.
#
# Makefile fragment for DevLib armtest application.
#
# Author: Bernie Innocenti <bernie@codewiz.org>
#
#

# Set to 1 for debug builds
armtest_DEBUG = 1

# Our target application
TRG += armtest

#include arm c and asm sources
include ./app/test/arm_src.mk

armtest_CSRC = $(arm_CSRC) app/test/empty_main.c

armtest_CPPASRC = $(arm_ASRC)

armtest_CROSS = arm-elf-

armtest_CPPAFLAGS = -O0 -g -gdwarf-2 -g -gen-debug
armtest_CPPFLAGS = -O0 -D'ARCH=ARCH_NIGHTTEST' -D__ARM_AT91SAM7S256__ -g3 -gdwarf-2 -fverbose-asm -Iapp/test -Ibertos/cpu/arm
armtest_LDFLAGS = -nostartfiles -T bertos/cpu/arm/scripts/at91sam7_256_rom.ld -Wl,--no-warn-mismatch

armtest_CPU = arm7tdmi

