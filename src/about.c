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
/* Copyright (C) 1998-2018 Christian Mauduit                                 */
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
/* Liquid War homepage : https://ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* nom           : about.c                                          */
/* content       : about menu                                       */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "back.h"
#include "base.h"
#include "config.h"
#include "decal.h"
#include "dialog.h"
#include "disk.h"
#include "graphics.h"
#include "help.h"
#include "menu.h"
#include "options.h"
#include "lang.h"
#include "about.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
lw_about (void)
{
  DIALOG d[9];
  int i, choix = 4, retour = 0;

  memset (d, 0, sizeof (d));

  while (retour == 0)
    {
      for (i = 0; i < 4; ++i)
        standard_button (d + i + 4, 0, i, 1, 4);

      quick_buttons (d);
      d[4].dp = lw_lang_string (LW_LANG_STRING_ABOUT_CREDITS);
      d[5].dp = lw_lang_string (LW_LANG_STRING_ABOUT_COPYRIGHT);
      d[6].dp = lw_lang_string (LW_LANG_STRING_ABOUT_LICENSE);
      d[7].dp = lw_lang_string (LW_LANG_STRING_ABOUT_VERSION);
      d[8].proc = 0;

      choix = my_do_dialog (d, choix);
      switch (choix)
        {
        case -1:
        case MENU_QUICK_BACK:
          retour = 1;
          break;
        case MENU_QUICK_MAIN:
          retour = MENU_TOP;
          break;
        case MENU_QUICK_QUIT:
          if (confirm_quit ())
            retour = MENU_EXIT;
          else
            display_back_image ();
          break;
        case MENU_QUICK_PLAY:
          retour = MENU_PLAY;
          break;
        case 4:
          retour =
            display_help (lw_lang_string (LW_LANG_STRING_HELP_CREDITS));
          break;
        case 5:
          retour =
            display_help (lw_lang_string (LW_LANG_STRING_HELP_COPYRIGHT));
          break;
        case 6:
          retour =
            display_help (lw_lang_string (LW_LANG_STRING_HELP_LICENSE));
          break;
        case 7:
          retour =
            display_help (lw_lang_string (LW_LANG_STRING_HELP_VERSION));
          break;
        }
    }

  if (retour > 0)
    retour--;

  return retour;
}
