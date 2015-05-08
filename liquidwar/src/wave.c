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
/* nom           : wave.c                                           */
/* contenu       : choix des options de vagues                      */
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
#include "wave.h"
#include "lang.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
wave_options (void)
{
  int *temp;
  DIALOG d[24];
  DIALOG_PLAYER *dp;
  int i, j, k, choix = 4, retour = 0;

  memset (d, 0, sizeof (d));

  display_back_image ();
  for (i = 0; i < 5; ++i)
    for (j = 0; j < 4; ++j)
      if (i > 0 || j > 0)
	{
	  k = i * 4 + j + 3;
	  standard_small_button (d + k, j, i, 4, 5);
	  if (i > 0 && j > 0)
	    {
	      d[k].proc = my_slider_proc;
	      d[k].dp = NULL;
	      d[k].dp2 = slider_int;
	      switch (j)
		{
		case 1:
		  d[k].d1 = 16;
		  d[k].dp3 = &(CONFIG_WAVE_AMPLI[i - 1]);
		  break;
		case 2:
		  d[k].d1 = 4;
		  d[k].dp3 = &(CONFIG_WAVE_NUMBER[i - 1]);
		  break;
		case 3:
		  d[k].d1 = 16;
		  d[k].dp3 = &(CONFIG_WAVE_SPEED[i - 1]);
		  break;
		}
	      temp = d[k].dp3;
	      d[k].d2 = *temp;
	    }
	  else
	    d[k].proc = my_textbox_proc;
	}
  d[23].proc = NULL;

  quick_buttons (d);
  d[4].dp = lw_lang_string (LW_LANG_STRING_WAVE_AMPLI);
  d[5].dp = lw_lang_string (LW_LANG_STRING_WAVE_NUMBER);
  d[6].dp = lw_lang_string (LW_LANG_STRING_WAVE_SPEED);
  d[7].dp = lw_lang_string (LW_LANG_STRING_WAVE_WX);
  d[11].dp = lw_lang_string (LW_LANG_STRING_WAVE_HY);
  d[15].dp = lw_lang_string (LW_LANG_STRING_WAVE_WY);
  d[19].dp = lw_lang_string (LW_LANG_STRING_WAVE_HX);

  display_back_image ();
  while (retour == 0)
    {
      dp = my_init_dialog (d, choix);
      while (my_update_dialog (dp))
	;
      choix = shutdown_dialog (dp);
      //choix=my_do_dialog (d,choix);
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

  if (retour > 0)
    retour--;

  return retour;
}
