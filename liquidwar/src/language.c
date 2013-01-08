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
/* Copyright (C) 1998-2013 Christian Mauduit                                 */
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
/* nom           : language.c                                       */
/* contenu       : menu used to set up language options             */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "language.h"
#include "config.h"
#include "dialog.h"
#include "disk.h"
#include "help.h"
#include "menu.h"
#include "lang.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
update_language (DIALOG * d)
{
  int i;

  quick_buttons (d);

  d[4].dp = lw_lang_string (LW_LANG_STRING_LANGUAGE_AUTO);
  for (i = 0; i < LW_LANG_NB_LANGUAGES; ++i)
    {
      d[i + 5].dp = lw_lang_language (i);
    }
}

/*------------------------------------------------------------------*/
int
language_options (void)
{
  DIALOG d[6 + LW_LANG_NB_LANGUAGES];
  int i, choix = 4, retour = 0;

  memset (d, 0, sizeof (d));

  standard_button (d + 4, 0, 0, 1, 2 + LW_LANG_NB_LANGUAGES);
  for (i = 0; i < LW_LANG_NB_LANGUAGES; ++i)
    {
      standard_button (d + i + 5, 0, 2 + i, 1, 2 + LW_LANG_NB_LANGUAGES);
    }

  update_language (d);

  d[5 + LW_LANG_NB_LANGUAGES].proc = 0;

  while (retour == 0)
    {
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
	  break;
	case MENU_QUICK_PLAY:
	  retour = MENU_PLAY;
	  break;
	case 4:
	  CONFIG_LANGUAGE = LW_LANG_LANGUAGE_AUTO;
	  update_language (d);
	  break;
	default:
	  if (choix >= 5 && choix < 5 + LW_LANG_NB_LANGUAGES)
	    {
	      CONFIG_LANGUAGE = choix - 5;
	      update_language (d);
	    }
	}
    }

  if (retour > 0)
    retour--;

  return retour;
}
