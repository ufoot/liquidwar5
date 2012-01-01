/********************************************************************/
/*                                                                  */
/*            L   I  QQ  U U I DD    W   W  A  RR    555            */
/*            L   I Q  Q U U I D D   W   W A A R R   5              */
/*            L   I Q  Q U U I D D   W W W AAA RR    55             */
/*            L   I Q Q  U U I D D   WW WW A A R R     5            */
/*            LLL I  Q Q  U  I DD    W   W A A R R   55             */
/*                                                                  */
/*                             b                                    */
/*                             bb  y y                              */
/*                             b b yyy                              */
/*                             bb    y                              */
/*                                 yy                               */
/*                                                                  */
/*                     U U       FFF  O   O  TTT                    */
/*                     U U       F   O O O O  T                     */
/*                     U U TIRET FF  O O O O  T                     */
/*                     U U       F   O O O O  T                     */
/*                      U        F    O   O   T                     */
/*                                                                  */
/********************************************************************/

/*****************************************************************************/
/* Liquid War is a multiplayer wargame                                       */
/* Copyright (C) 1998-2012 Christian Mauduit                                 */
/*                                                                           */
/* This program is free software; you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation; either version 2 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; if not, write to the Free Software               */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */
/*                                                                           */
/* Liquid War homepage : http://www.ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* nom           : lw6.c                                            */
/* contenu       : advertising for Liquid War 6                     */
/* date de modif : 2010                                             */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "back.h"
#include "dialog.h"
#include "help.h"
#include "menu.h"
#include "lw6.h"
#include "options.h"
#include "play.h"
#include "error.h"
#include "about.h"
#include "lang.h"
#include "log.h"

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
lw6_advertise_menu (int fade_in)
{
  int ret = 0;
  DIALOG d[3];

  memset (d, 0, sizeof (d));

  display_back_image ();

  if (fade_in)
    {
      my_fade_in ();
    }

  d[2].proc = NULL;
  standard_button (d, 0, 0, 1, 1);
  standard_button (d + 1, 0, 9, 1, 10);

  d[0].proc = my_textbox_proc;
  d[0].d2 = 0;
  d[0].key = 0;
  d[0].flags = 0;
  d[0].x = MENU_W_SPACE / 2;
  d[0].y = (MENU_H_SPACE * 3) / 2 + MENU_H_STANDARD * 2;
  d[0].w = MENU_VIRTUAL_W - MENU_W_SPACE;
  d[0].h = (MENU_H_SPACE * 6) + MENU_H_STANDARD * 7;
  menu_real_coord (d);

  d[0].dp = lw_lang_string (LW_LANG_STRING_LW6_INFO);
  d[1].dp = lw_lang_string (LW_LANG_STRING_LW6_OK);

  ret = (my_do_dialog (d, 0) == 1) ? 1 : 0;

  return ret;
}
