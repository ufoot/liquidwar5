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
/* Copyright (C) 1998-2008 Christian Mauduit                                 */
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
/* nom           : rules.c                                          */
/* contenu       : choix des regles de jeu                          */
/* date de modif : 3 mai 98                                         */
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
/* constantes                                                       */
/*==================================================================*/

#define LW_RULES_CIS_SIZE   30
#define LW_RULES_TIME_SIZE  14
#define LW_RULES_MMR_SIZE   50

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
write_cis_setting (DIALOG * d)
{
  char *str;

  str = d->dp;
  LW_MACRO_SNPRINTF2 (str,
		      LW_RULES_CIS_SIZE,
		      "%s x%d",
		      lw_lang_string (LW_LANG_STRING_RULES_CURSOR),
		      CONFIG_CURSOR_INCREASE_SPEED + 1);
}

/*------------------------------------------------------------------*/
static int
slider_cis (void *dp3, int d2)
{
  DIALOG *d;

  d = dp3;

  CONFIG_CURSOR_INCREASE_SPEED = d2;
  write_cis_setting (d);

  scare_mouse ();
  my_textbox_proc (MSG_DRAW, d, 0);
  unscare_mouse ();

  play_click ();

  return 0;
}

/*------------------------------------------------------------------*/
static void
write_time_setting (DIALOG * d)
{
  char *str;

  str = d->dp;
  start_play_time ();
  write_time (TIME_LEFT, str + 8);
}

/*------------------------------------------------------------------*/
static int
slider_time (void *dp3, int d2)
{
  DIALOG *d;

  d = dp3;

  CONFIG_GAME_TIME = d2;
  /*
   * Following line needed because start_play_time() uses the
   * value in LW_CONFIG_CURRENT_RULES
   */
  LW_CONFIG_CURRENT_RULES.game_time = CONFIG_GAME_TIME;

  write_time_setting (d);

  scare_mouse ();
  my_textbox_proc (MSG_DRAW, d, 0);
  unscare_mouse ();

  play_click ();

  return 0;
}

/*------------------------------------------------------------------*/
static void
write_mmr_setting (DIALOG * d)
{
  char *str;

  str = d->dp;
  LW_MACRO_SNPRINTF3 (str,
		      LW_RULES_MMR_SIZE,
		      "%s%dx%d",
		      lw_lang_string (LW_LANG_STRING_RULES_MINMAPRES),
		      MIN_MAP_RES_W[CONFIG_MIN_MAP_RES],
		      MIN_MAP_RES_H[CONFIG_MIN_MAP_RES]);
}

/*------------------------------------------------------------------*/
static int
slider_mmr (void *dp3, int d2)
{
  DIALOG *d;

  d = dp3;

  CONFIG_MIN_MAP_RES = d2;
  write_mmr_setting (d);

  scare_mouse ();
  my_textbox_proc (MSG_DRAW, d, 0);
  unscare_mouse ();

  play_click ();

  return 0;
}

/*------------------------------------------------------------------*/
int
rules_options (void)
{
  int *temp;
  DIALOG d[15];
  int i, choix = 4, retour = 0;
  static int y_pos[4] = { 0, 1, 2, 3 };
  char cis_buffer[LW_RULES_CIS_SIZE];
  char time_str[LW_RULES_TIME_SIZE];
  char mmr_buffer[LW_RULES_MMR_SIZE];

  memset (d, 0, sizeof (d));

  LW_MACRO_SPRINTF1 (time_str, "%-8s00:00",
		     lw_lang_string (LW_LANG_STRING_RULES_TIME));

  for (i = 0; i < 4; ++i)
    {
      standard_button (d + i + 4, 0, y_pos[i], 2, 8);
      d[i + 4].proc = my_textbox_proc;
    }
  for (i = 0; i < 4; ++i)
    {
      standard_button (d + i + 8, 1, y_pos[i], 2, 8);
      d[i + 8].proc = my_slider_proc;
    }

  standard_button (d + 12, 0, 5, 1, 8);
  d[12].dp = lw_lang_string (LW_LANG_STRING_RULES_ADVANCED);

  standard_button (d + 13, 0, 7, 1, 8);
  d[13].dp = lw_lang_string (LW_LANG_STRING_RULES_DEFAULT);

  quick_buttons (d);
  d[4].dp = time_str;
  d[5].dp = lw_lang_string (LW_LANG_STRING_RULES_ARMYSIZE);
  d[6].dp = cis_buffer;
  d[7].dp = mmr_buffer;
  d[8].d1 = 16;
  d[9].d1 = 32;
  d[10].d1 = 2;
  d[11].d1 = 8;
  d[8].dp3 = &CONFIG_GAME_TIME;
  d[9].dp3 = &CONFIG_FIGHTER_NUMBER;
  d[10].dp3 = &CONFIG_CURSOR_INCREASE_SPEED;
  d[11].dp3 = &CONFIG_MIN_MAP_RES;
  for (i = 8; i < 12; ++i)
    {
      d[i].dp = NULL;
      d[i].dp2 = slider_int;
      temp = d[i].dp3;
      d[i].d2 = *temp;
    }
  d[8].dp2 = slider_time;
  d[8].dp3 = d + 4;
  d[10].dp2 = slider_cis;
  d[10].dp3 = d + 6;
  d[11].dp2 = slider_mmr;
  d[11].dp3 = d + 7;

  write_time_setting (d + 4);
  write_cis_setting (d + 6);
  write_mmr_setting (d + 7);

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
	  break;
	case MENU_QUICK_PLAY:
	  retour = MENU_PLAY;
	  break;
	case 12:
	  retour = advanced_options ();
	  break;
	case 13:
	  load_default_rules ();

	  /*
	   * Needed because the slider function works in a weird way
	   */
	  LW_CONFIG_CURRENT_RULES.game_time = CONFIG_GAME_TIME;

	  d[8].d2 = CONFIG_GAME_TIME;
	  d[9].d2 = CONFIG_FIGHTER_NUMBER;
	  d[10].d2 = CONFIG_CURSOR_INCREASE_SPEED;
	  d[11].d2 = CONFIG_MIN_MAP_RES;
	  write_time_setting (d + 4);
	  write_cis_setting (d + 6);
	  write_mmr_setting (d + 7);
	  break;
	}
    }

  if (retour > 0)
    retour--;

  return retour;
}
