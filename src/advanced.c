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
/* nom           : advanced.c                                       */
/* contenu       : choix des options de jeu avancees                */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "advanced.h"
#include "backport.h"
#include "config.h"
#include "dialog.h"
#include "disk.h"
#include "help.h"
#include "time.h"
#include "menu.h"
#include "log.h"
#include "lang.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*------------------------------------------------------------------*/
static char *
get_asm_algorithm_str (void)
{
  if (CONFIG_ASM_ALGORITHM)
    return (lw_lang_string (LW_LANG_STRING_ADVANCED_ASSEMBLY));
  else
    return (lw_lang_string (LW_LANG_STRING_ADVANCED_STANDARDC));
}

/*------------------------------------------------------------------*/
static char *
get_cpu_vs_human_str (void)
{
  char *result = "";

  switch (CONFIG_CPU_VS_HUMAN)
    {
    case CONFIG_CPU_VS_HUMAN_ALWAYS:
      result = (lw_lang_string (LW_LANG_STRING_ADVANCED_ALWAYS));
      break;
    case CONFIG_CPU_VS_HUMAN_NEVER:
      result = (lw_lang_string (LW_LANG_STRING_ADVANCED_NEVER));
      break;
    default:
      result = (lw_lang_string (LW_LANG_STRING_ADVANCED_RANDOM));
      break;
    }

  return result;
}

/*------------------------------------------------------------------*/
static char *
get_allow_network_bots_str (void)
{
  if (CONFIG_ALLOW_NETWORK_BOTS)
    return (lw_lang_string (LW_LANG_STRING_ADVANCED_YES));
  else
    return (lw_lang_string (LW_LANG_STRING_ADVANCED_NO));
}

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
advanced_options (void)
{
  int *temp;
  DIALOG d[21];
  int i, retour = 0, choix = 4;
  static int y_pos[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

  memset (d, 0, sizeof (d));

  for (i = 0; i < 8; ++i)
    {
      standard_button (d + i + 4, 0, y_pos[i], 2, 8);
      d[i + 4].proc = my_textbox_proc;
    }
  for (i = 0; i < 8; ++i)
    {
      standard_button (d + i + 12, 1, y_pos[i], 2, 8);
      d[i + 12].proc = my_slider_proc;
    }
  d[17].proc = d[18].proc = d[19].proc = my_button_proc;

  quick_buttons (d);
  d[4].dp = lw_lang_string (LW_LANG_STRING_ADVANCED_ATTACK);
  d[5].dp = lw_lang_string (LW_LANG_STRING_ADVANCED_DEFENSE);
  d[6].dp = lw_lang_string (LW_LANG_STRING_ADVANCED_BASEHEALTH);
  d[7].dp = lw_lang_string (LW_LANG_STRING_ADVANCED_WINNERHELP);
  d[8].dp = lw_lang_string (LW_LANG_STRING_ADVANCED_CPUSTRENGTH);
  d[9].dp = lw_lang_string (LW_LANG_STRING_ADVANCED_CPUVSHUMAN);
  d[10].dp = lw_lang_string (LW_LANG_STRING_ADVANCED_ALLOWNETWORKBOTS);
  d[11].dp = lw_lang_string (LW_LANG_STRING_ADVANCED_ASMALGORITHM);
  d[12].d1 = d[13].d1 = d[14].d1 = d[15].d1 = 16;
  d[16].d1 = 4;
  d[12].dp3 = &CONFIG_FIGHTER_ATTACK;
  d[13].dp3 = &CONFIG_FIGHTER_DEFENSE;
  d[14].dp3 = &CONFIG_FIGHTER_NEW_HEALTH;
  d[15].dp3 = &CONFIG_NUMBER_INFLUENCE;
  d[16].dp3 = &CONFIG_CPU_ADVANTAGE;
  d[17].dp = get_cpu_vs_human_str ();
  d[18].dp = get_allow_network_bots_str ();
  d[19].dp = get_asm_algorithm_str ();
  for (i = 12; i < 17; ++i)
    {
      d[i].dp = NULL;
      d[i].dp2 = slider_int;
      temp = d[i].dp3;
      d[i].d2 = *temp;
    }

#ifndef ASM
  /*
   * If no assembly code has been compiled, we disable the button
   */
  d[10].flags = D_HIDDEN;
  d[18].flags = D_HIDDEN;
#endif

  d[20].proc = 0;

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
        case 17:
          CONFIG_CPU_VS_HUMAN = (CONFIG_CPU_VS_HUMAN + 1) % 3;
          d[17].dp = get_cpu_vs_human_str ();
          scare_mouse ();
          my_button_proc (MSG_DRAW, d + 17, 0);
          unscare_mouse ();
          break;
        case 18:
          CONFIG_ALLOW_NETWORK_BOTS = CONFIG_ALLOW_NETWORK_BOTS ? 0 : 1;
          d[18].dp = get_allow_network_bots_str ();
          scare_mouse ();
          my_button_proc (MSG_DRAW, d + 18, 0);
          unscare_mouse ();
          break;
        case 19:
          CONFIG_ASM_ALGORITHM = CONFIG_ASM_ALGORITHM ? 0 : 1;
          d[19].dp = get_asm_algorithm_str ();
          scare_mouse ();
          my_button_proc (MSG_DRAW, d + 19, 0);
          unscare_mouse ();
          break;
        }
    }

  if (retour > 0)
    retour--;

  return retour;
}
