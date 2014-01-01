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
/* Copyright (C) 1998-2014 Christian Mauduit                                 */
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
/* nom           : controls.c                                       */
/* content       : controls menu                                    */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "back.h"
#include "base.h"
#include "config.h"
#include "dialog.h"
#include "menu.h"
#include "options.h"
#include "lang.h"
#include "controls.h"
#include "mouse.h"
#include "sound.h"
#include "joystick.h"
#include "log.h"

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static int
slider_mouse (void *dp3, int d2)
{
  int *val;

  val = dp3;
  *val = d2;

  lw_mouse_reset_control ();

  play_click ();

  return 0;
}


/*------------------------------------------------------------------*/
int
controls_options (void)
{
  DIALOG d[9];
  int choix = 4, retour = 0;

  memset (d, 0, sizeof (d));

  while (retour == 0)
    {
      standard_button (d + 4, 0, 0, 2, 4);
      standard_button (d + 5, 1, 0, 2, 4);
      standard_big_button (d + 6, 0, 2, 1, 4);
      standard_button (d + 7, 0, 3, 1, 4);

      quick_buttons (d);

      d[4].proc = my_textbox_proc;
      d[4].dp = lw_lang_string (LW_LANG_STRING_CONTROLS_JOYSTICK);

      switch (CONFIG_JOYSTICK_ON)
	{
	case 1:
	  d[5].dp = lw_lang_string (LW_LANG_STRING_CONTROLS_2JOYSTICKS);
	  break;
	case 2:
	  d[5].dp = lw_lang_string (LW_LANG_STRING_CONTROLS_4BUTTONS);
	  break;
	case 3:
	  d[5].dp = lw_lang_string (LW_LANG_STRING_CONTROLS_STANDARD);
	  break;
	default:
	  d[5].dp = lw_lang_string (LW_LANG_STRING_CONTROLS_OFF);
	  break;
	}

      d[6].proc = my_textbox_proc;
      d[6].dp = lw_lang_string (LW_LANG_STRING_CONTROLS_MOUSESENSIB);

      d[7].proc = my_slider_proc;
      d[7].dp = NULL;
      d[7].d1 = 8;
      d[7].d2 = CONFIG_MOUSE_GAP;
      d[7].dp2 = slider_mouse;
      d[7].dp3 = &CONFIG_MOUSE_GAP;

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
	case 5:
	  CONFIG_JOYSTICK_ON++;
	  if (CONFIG_JOYSTICK_ON > 3)
	    CONFIG_JOYSTICK_ON = 0;
	  my_initialise_joystick ();
	  break;
	}
    }

  if (retour > 0)
    retour--;

  return retour;
}
