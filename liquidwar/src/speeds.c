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
/* nom           : speeds.c                                         */
/* contenu       : menu used to set up speeds                       */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdio.h>

#include "advanced.h"
#include "config.h"
#include "dialog.h"
#include "disk.h"
#include "help.h"
#include "menu.h"
#include "rules.h"
#include "sound.h"
#include "time.h"
#include "lang.h"
#include "macro.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_SPEEDS_FPS_SIZE 33
#define LW_SPEEDS_RPS_SIZE 33

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
write_fps_setting (DIALOG * d)
{
  char *str;

  str = d->dp;
  if (DISPLAY_DELAY_MIN[CONFIG_FRAMES_PER_SEC_LIMIT] != 0)
    {
      LW_MACRO_SNPRINTF2 (str,
			  LW_SPEEDS_FPS_SIZE,
			  "%d %s",
			  1000 /
			  (DISPLAY_DELAY_MIN[CONFIG_FRAMES_PER_SEC_LIMIT]),
			  lw_lang_string (LW_LANG_STRING_SPEEDS_FPS));
    }
  else
    {
      LW_MACRO_SNPRINTF0 (str,
			  LW_SPEEDS_FPS_SIZE,
			  lw_lang_string (LW_LANG_STRING_SPEEDS_NOFPSLIMIT));
    }
}

/*------------------------------------------------------------------*/
static int
slider_fps (void *dp3, int d2)
{
  DIALOG *d;

  d = dp3;

  CONFIG_FRAMES_PER_SEC_LIMIT = d2;
  write_fps_setting (d);

  scare_mouse ();
  my_textbox_proc (MSG_DRAW, d, 0);
  unscare_mouse ();

  play_click ();

  return 0;
}

/*------------------------------------------------------------------*/
static void
write_rps_setting (DIALOG * d)
{
  char *str;

  str = d->dp;
  if (LOGIC_DELAY_MIN[CONFIG_ROUNDS_PER_SEC_LIMIT] != 0)
    {
      LW_MACRO_SNPRINTF2 (str,
			  LW_SPEEDS_RPS_SIZE,
			  "%d %s",
			  1000 /
			  (LOGIC_DELAY_MIN[CONFIG_ROUNDS_PER_SEC_LIMIT]),
			  lw_lang_string (LW_LANG_STRING_SPEEDS_RPS));
    }
  else
    {
      LW_MACRO_SNPRINTF0 (str,
			  LW_SPEEDS_RPS_SIZE,
			  lw_lang_string (LW_LANG_STRING_SPEEDS_NORPSLIMIT));
    }
}

/*------------------------------------------------------------------*/
static int
slider_rps (void *dp3, int d2)
{
  DIALOG *d;

  d = dp3;

  CONFIG_ROUNDS_PER_SEC_LIMIT = d2;
  write_rps_setting (d);

  scare_mouse ();
  my_textbox_proc (MSG_DRAW, d, 0);
  unscare_mouse ();

  play_click ();

  return 0;
}

/*------------------------------------------------------------------*/
int
speeds_options (void)
{
  int *temp;
  DIALOG d[9];
  int i, choix = 4, retour = 0;
  char fps_buffer[LW_SPEEDS_FPS_SIZE];
  char rps_buffer[LW_SPEEDS_RPS_SIZE];

  memset (d, 0, sizeof (d));

  for (i = 0; i < 2; ++i)
    {
      standard_button (d + i + 4, 0, i, 2, 2);
      d[i + 4].proc = my_textbox_proc;
    }
  for (i = 0; i < 2; ++i)
    {
      standard_button (d + i + 6, 1, i, 2, 2);
      d[i + 6].proc = my_slider_proc;
    }

  quick_buttons (d);
  d[4].dp = fps_buffer;
  d[5].dp = rps_buffer;
  d[6].d1 = 8;
  d[7].d1 = 8;
  d[6].dp3 = &CONFIG_FRAMES_PER_SEC_LIMIT;
  d[7].dp3 = &CONFIG_ROUNDS_PER_SEC_LIMIT;
  for (i = 0; i < 2; ++i)
    {
      d[i + 6].dp = NULL;
      d[i + 6].dp2 = slider_int;
      temp = d[i + 6].dp3;
      d[i + 6].d2 = *temp;
      d[i + 6].dp3 = d + i + 4;
    }

  d[6].dp2 = slider_fps;
  d[7].dp2 = slider_rps;

  write_fps_setting (d + 4);
  write_rps_setting (d + 5);

  d[8].proc = 0;

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
	}
    }

  if (retour > 0)
    retour--;

  return retour;
}
