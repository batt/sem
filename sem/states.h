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
 * \author Francesco Sacchi <batt@develer.com>
 *
 * \brief Signal light state machine.
 */

#ifndef SEM_STATES_H
#define SEM_STATES_H

#include <cfg/compiler.h>

typedef enum LightStates
{
	LS_RESET,
	LS_SHOOTINGLINE,
	LS_SHOOTING,
	LS_TIMEEXPIRING,
	LS_STOPSHOOTING,
} LightStates;

typedef struct LightTimes
{
	mtime_t shooting_line;
	mtime_t shooting;
	mtime_t time_expiring;
} LightTimes;

void NORETURN state_run(LightTimes *times);

#endif /*SEM_STATES_H */
