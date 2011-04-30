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
/* Copyright (C) 1998-2011 Christian Mauduit                                 */
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
/* nom           : options.c                                        */
/* contenu       : choix des options                                */
/* date de modif : 3 mai 98                                         */
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
#include "sound.h"
#include "rules.h"
#include "volume.h"
#include "speeds.h"
#include "language.h"
#include "controls.h"
#include "lang.h"
#include "gfxmode.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
options (void)
{
  DIALOG d[12];
  DIALOG_PLAYER *dp;
  int i, choix = 4, retour = 0;
  int old_gfx_menu;
  int old_fullscreen;
  int gfxmode_change = 0;

  memset (d, 0, sizeof (d));

  while (retour == 0)
    {
      for (i = 0; i < 6; ++i)
	standard_button (d + i + 4, 0, i, 1, 8);
      standard_button (d + 10, 0, 7, 1, 8);

      quick_buttons (d);
      d[4].dp = lw_lang_string (LW_LANG_STRING_OPTIONS_GRAPHICS);
      d[5].dp = lw_lang_string (LW_LANG_STRING_OPTIONS_SOUND);
      d[6].dp = lw_lang_string (LW_LANG_STRING_OPTIONS_RULES);
      d[7].dp = lw_lang_string (LW_LANG_STRING_OPTIONS_SPEEDS);
      d[8].dp = lw_lang_string (LW_LANG_STRING_OPTIONS_CONTROLS);
      d[9].dp = lw_lang_string (LW_LANG_STRING_OPTIONS_LANGUAGE);
      d[10].dp = lw_lang_string (LW_LANG_STRING_OPTIONS_DEFAULT);
      d[11].proc = 0;

      old_gfx_menu = CONFIG_GFX_MENU;
      old_fullscreen = CONFIG_FULLSCREEN;

      display_back_image ();
      if (gfxmode_change)
	{
	  gfxmode_change = 0;
	  display_back_image ();
	  dp = my_init_dialog (d, choix);
	  my_fade_in ();
	}
      else
	{
	  dp = my_init_dialog (d, choix);
	}

      show_mouse (screen);
      while (my_update_dialog (dp))
	;
      choix = shutdown_dialog (dp);

      play_click ();

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
	  retour = graphic_options ();
	  break;
	case 5:
	  retour = sound_options ();
	  break;
	case 6:
	  retour = rules_options ();
	  break;
	case 7:
	  retour = speeds_options ();
	  break;
	case 8:
	  retour = controls_options ();
	  break;
	case 9:
	  retour = language_options ();
	  break;
	case 10:
	  load_default_options ();

	  /*
	   * The volume might have changed, so we update it
	   */
	  WATER_VOLUME = CONFIG_WATER_VOLUME_MENU;
	  /*
	   * If the graphics settings changed, we reflect them
	   */
	  if (old_gfx_menu != CONFIG_GFX_MENU ||
	      old_fullscreen != CONFIG_FULLSCREEN)
	    {
	      gfxmode_change = 1;
	      set_resolution (CONFIG_GFX_MENU, 0, CONFIG_FULLSCREEN, NULL);
	    }
	  retour = 0;
	  break;
	}
    }

  if (retour > 0)
    retour--;

  return retour;
}
