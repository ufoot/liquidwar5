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
/* Copyright (C) 1998-2019 Christian Mauduit                                 */
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
/* nom           : help.c                                           */
/* contenu       : aide en ligne                                    */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "back.h"
#include "config.h"
#include "dialog.h"
#include "disk.h"
#include "help.h"
#include "menu.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
prepare_help (DIALOG d[6], char *str)
{
  quick_buttons (d);

  color_one_dialog (d + 4);
  d[4].proc = my_textbox_proc;
  d[4].d2 = 0;
  d[4].dp = str;
  d[4].key = 0;
  d[4].flags = 0;
  d[4].x = (MENU_W_SPACE * 3) / 2 + MENU_W_QUICK;
  d[4].y = (MENU_H_SPACE * 3) / 2 + MENU_H_QUICK;
  d[4].w = MENU_VIRTUAL_W - MENU_W_SPACE * 3 - MENU_W_QUICK * 2;
  d[4].h = MENU_VIRTUAL_H - MENU_H_SPACE * 3 - MENU_H_QUICK * 2;
  menu_real_coord (d + 4);

  d[5].proc = NULL;
}

/*------------------------------------------------------------------*/
int
display_help (char *str)
{
  int retour = 0, choix = MENU_QUICK_BACK;
  DIALOG d[6];

  memset (d, 0, sizeof (d));

  prepare_help (d, str);

  display_back_image ();

  while (retour == 0)
    {
      choix = my_do_dialog_no_clear (d, choix);
      switch (choix)
        {
        case -1:
        case MENU_QUICK_BACK:
          retour = 1;
          break;
        case MENU_QUICK_PLAY:
          retour = MENU_PLAY;
          break;
        case MENU_QUICK_MAIN:
          retour = MENU_TOP;
          break;
        case MENU_QUICK_QUIT:
          if (confirm_quit ())
            retour = MENU_EXIT;
          break;
        }
    }
  if (retour > 0)
    retour--;
  return retour;
}
