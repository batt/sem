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
 * Copyright 1999, 2001 Bernie Innocenti <bernie@codewiz.org>
 *
 * -->
 * \brief X-Modem serial transmission protocol (interface)
 *
 * \version $Id: xmodem.h 1771 2008-08-31 21:45:08Z bernie $
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Francesco Sacchi <batt@develer.com>
 */


#ifndef NET_XMODEM_H
#define NET_XMODEM_H

#include <cfg/compiler.h>
#include <kern/kfile.h>

/**
 * Called to know if we want to abort data tranfer.
 * Redefine to whatever you need.
 * \{
 */
#ifndef XMODEM_CHECK_ABORT
#define XMODEM_CHECK_ABORT (false)
#endif
/*\}*/


/**
 * Called to printf progress messages.
 * Default to kprintf debug, redefine to whatever you need.
 * \{
 */
#ifndef XMODEM_PROGRESS
#define XMODEM_PROGRESS(msg, ...) kprintf(msg, ## __VA_ARGS__)
#endif


/* fwd decl */
struct Serial;

bool xmodem_recv(struct Serial *port, KFile *fd);
bool xmodem_send(struct Serial *port, KFile *fd);

#endif /* NET_XMODEM_H */
