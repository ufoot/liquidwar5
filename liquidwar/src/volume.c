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
/* Copyright (C) 1998-2015 Christian Mauduit                                 */
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
/* nom           : volume.c                                         */
/* contenu       : reglage du volume du son                         */
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
#include "sound.h"
#include "volume.h"
#include "lang.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
sound_options (void)
{
  int *temp;
  DIALOG d[15];
  int i, choix = 4, retour = 0;
  static int y_pos[5] = { 0, 2, 4, 5, 7 };

  memset (d, 0, sizeof (d));

  for (i = 0; i < 5; ++i)
    {
      standard_button (d + i + 4, 0, y_pos[i], 2, 8);
      d[i + 4].proc = my_textbox_proc;
    }
  for (i = 0; i < 5; ++i)
    {
      standard_button (d + i + 9, 1, y_pos[i], 2, 8);
      d[i + 9].proc = my_slider_proc;
    }

  quick_buttons (d);

  d[4].dp = lw_lang_string (LW_LANG_STRING_VOLUME_SFX);
  d[5].dp = lw_lang_string (LW_LANG_STRING_VOLUME_CLICK);
  d[6].dp = lw_lang_string (LW_LANG_STRING_VOLUME_GAMEWATER);
  d[7].dp = lw_lang_string (LW_LANG_STRING_VOLUME_MENUWATER);
  d[8].dp = lw_lang_string (LW_LANG_STRING_VOLUME_MUSIC);
  d[9].dp3 = &CONFIG_SFX_VOLUME;
  d[10].dp3 = &CONFIG_CLICK_VOLUME;
  d[11].dp3 = &CONFIG_WATER_VOLUME_GAME;
  d[12].dp3 = &WATER_VOLUME;
  d[13].dp3 = &CONFIG_MUSIC_VOLUME;
  for (i = 9; i < 14; ++i)
    {
      d[i].d1 = 16;
      d[i].dp = NULL;
      d[i].dp2 = slider_int;
      temp = d[i].dp3;
      d[i].d2 = *temp;
    }

  d[14].proc = 0;

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
	  else
	    display_back_image ();
	  break;
	case MENU_QUICK_PLAY:
	  retour = MENU_PLAY;
	  break;
	}
    }

  CONFIG_WATER_VOLUME_MENU = WATER_VOLUME;	// realign current volume
  // and config volume
  if (retour > 0)
    retour--;

  return retour;
}
