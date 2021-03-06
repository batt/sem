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
 * Copyright 2003, 2004, 2006 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2000 Bernie Innocenti <bernie@codewiz.org>
 * All Rights Reserved.
 * -->
 *
 * \version $Id: menu.h 1532 2008-08-04 07:21:26Z bernie $
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Stefano Fedrigo <aleph@develer.com>
 *
 * \brief Common menu handling API
 */

/*#*
 *#* $Log$
 *#* Revision 1.5  2007/09/19 16:23:27  batt
 *#* Fix doxygen warnings.
 *#*
 *#* Revision 1.4  2006/09/13 13:58:33  bernie
 *#* Add RenderHook support.
 *#*
 *#* Revision 1.3  2006/07/19 12:56:27  bernie
 *#* Convert to new Doxygen style.
 *#*
 *#* Revision 1.2  2006/06/03 13:58:02  bernie
 *#* Fix recursive timeout and add exit status information.
 *#*
 *#* Revision 1.1  2006/05/15 07:20:54  bernie
 *#* Move menu to gui/.
 *#*
 *#* Revision 1.4  2006/04/11 00:07:32  bernie
 *#* Implemenent MF_SAVESEL flag.
 *#*
 *#* Revision 1.3  2006/03/22 09:49:51  bernie
 *#* Simplifications from project_grl.
 *#*
 *#* Revision 1.2  2006/03/20 17:48:35  bernie
 *#* Implement support for ROM menus.
 *#*
 *#* Revision 1.1  2006/02/10 12:29:36  bernie
 *#* Add menu system.
 *#*
 *#* Revision 1.20  2005/11/16 18:10:19  bernie
 *#* Move top-level headers to cfg/ as in DevLib.
 *#*
 *#* Revision 1.19  2005/02/11 19:11:38  aleph
 *#* Move menu_displaymsg() in new displaymsg module
 *#*
 *#* Revision 1.18  2005/01/13 16:56:36  aleph
 *#* Fix progmem includes.
 *#*
 *#* Revision 1.17  2004/12/14 12:52:45  aleph
 *#* Add exclude menu flags
 *#*
 *#* Revision 1.16  2004/10/01 14:04:59  customer_pw
 *#* Add accessor functions for menu flags
 *#*
 *#* Revision 1.15  2004/09/09 08:31:36  customer_pw
 *#* Add disabled item type
 *#*
 *#* Revision 1.14  2004/08/29 21:46:12  bernie
 *#* CVSSILENT: Mark CVS log blocks.
 *#*
 *#* Revision 1.13  2004/08/25 15:35:23  customer_pw
 *#* IPTR -> iptr_t conversion.
 *#*
 *#* Revision 1.12  2004/08/25 13:23:45  bernie
 *#* IPTR -> iptr_t conversion.
 *#*/
#ifndef MWARE_MENU_H
#define MWARE_MENU_H

#include <cfg/compiler.h>

/* Fwd decl */
struct Bitmap;

/** Menu callback function */
typedef iptr_t (*MenuHook)(iptr_t userdata);

/**
 * Menu item description.
 */
typedef struct MenuItem
{
	const_iptr_t label;    /**< Item label (ID or ptr to string, 0 to disable) */
	int          flags;    /**< See MIF_#? definitions below */
	MenuHook     hook;     /**< Callback function (NULL to terminate item list) */
	iptr_t       userdata; /**< User data to be passed back to the hook */
} MenuItem;

/** Render hook callback function prototype */
typedef void (*RenderHook)(struct Bitmap *bitmap, int ypos, bool selected, const struct MenuItem *item);

/**
 * \name Flags for MenuItem.flags.
 * \{
 */
#define MIF_EXCLUDE_MASK    0x00FF /**< Mask for mutual exclusion map (shared with priority). */
#define MIF_PRI_MASK        0x00FF /**< Mask for priority value (shared with mutual exclusion). */
#define MIF_PRI(x)          ((x) & MIF_PRI_MASK) /**< Set menu item priority. */
#define MIF_EXCLUDE_0       BV(0)  /**< Exclude item 0 when this item is checked */
#define MIF_EXCLUDE_1       BV(1)  /**< Exclude item 1 when this item is checked */
#define MIF_EXCLUDE_2       BV(2)  /**< Exclude item 2 when this item is checked */
#define MIF_EXCLUDE_3       BV(3)  /**< Exclude item 3 when this item is checked */
#define MIF_EXCLUDE_4       BV(4)  /**< Exclude item 4 when this item is checked */
#define MIF_EXCLUDE_5       BV(5)  /**< Exclude item 5 when this item is checked */
#define MIF_EXCLUDE_6       BV(6)  /**< Exclude item 6 when this item is checked */
#define MIF_EXCLUDE_7       BV(7)  /**< Exclude item 7 when this item is checked */
#define MIF_CHECKED         BV(8)  /**< Item is currently checked */
#define MIF_CHECKIT         BV(9)  /**< Automatically check this item when selected */
#define MIF_TOGGLE          BV(10) /**< Toggle MIF_CHECKED when item is selected */
#define MIF_HIDDEN          BV(11) /**< This menu item is not visible */
#define MIF_DISABLED        BV(12) /**< This menu item is not visible */
#define MIF_RAMLABEL        BV(13) /**< Item label is stored in RAM, not in program memory */
#define MIF_RENDERHOOK      BV(14) /**< Menu render function is passed in label field */
/* \} */

/**
 * Menu description.
 */
typedef struct Menu
{
	MenuItem        *items;    /**< Array of items (end with a NULL hook) */
	const_iptr_t     title;    /**< Menu title (ID or ptr to string, 0 to disable) */
	int              flags;    /**< See MF_#? definitions below */
	struct Bitmap   *bitmap;   /**< Bitmap where the menu is rendered */
	int              selected; /**< Initial selection (written to if MF_SAVESEL is set). */
} Menu;

/**
 * \name Flags for Menu.flags.
 * \{
 */
#define MF_STICKY   BV(0)  /**< Stay in the menu when the items called return */
#define MF_TOPLEVEL BV(1)  /**< Top-level menu (do not display "back" label) */
#define MF_ROMITEMS BV(2)  /**< Menu items are stored in ROM (default is RAM) */
#define MF_SAVESEL  BV(3)  /**< Remember the selected item across invocations. */
/* \} */

/**
 * \name Special result codes for menu_handle().
 * \{
 */
#define MENU_OK       ((iptr_t)0)
#define MENU_CANCEL   ((iptr_t)-1)
#define MENU_TIMEOUT  ((iptr_t)-2)
#define MENU_ABORT    ((iptr_t)-3)
#define MENU_DISABLED ((iptr_t)-4)
/* \} */

/* Function prototypes */
iptr_t menu_handle(const struct Menu *menu);
int menu_setFlags(struct Menu *menu, int idx, int flags);
int menu_clearFlags(struct Menu *menu, int idx, int flags);

#endif /* MWARE_MENU_H */
