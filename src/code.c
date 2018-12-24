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
/* nom           : code.c                                           */
/* contenu       : codes et touches pendant le jeu                  */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <allegro5/allegro.h>

#include "config.h"
#include "decal.h"
#include "message.h"
#include "profile.h"
#include "code.h"
#include "watchdog.h"
#include "lang.h"
#include "capture.h"
#include "network.h"
#include "macro.h"

/*==================================================================*/
/* constantes                                                       */
/*==================================================================*/

#define LW_CODE_KEYWORD_SPEED "speed"
#define LW_CODE_KEYWORD_GRADN "gradn"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int GRAD_TO_DISP = 0;
int PAUSE_ON = 0;
int CHANGED_INFO_BAR = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
display_special_info (void)
{
  int global, spread, move, disp, flip;

  if (WATCHDOG_SCANCODE[ALLEGRO_KEY_F5])
    {
      global = get_profile (GLOBAL_PROFILE);
      if (global <=0)
        global = 1;
      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_FPS),
                       1000 / global);
    }

  if (WATCHDOG_SCANCODE[ALLEGRO_KEY_F6])
    {
      global = get_logic_rate ();
      if (global <=0)
        global = 1;
      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_RPS),
                       1000 / global);
    }

  if (WATCHDOG_SCANCODE[ALLEGRO_KEY_F7])
    {
      global = get_profile (GLOBAL_PROFILE);
      spread = get_profile (SPREAD_PROFILE);
      move = get_profile (MOVE_PROFILE);
      if (global <=0)
        global = 1;
      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_PERCENTCALC),
                       (100 * (spread + move)) / global);
    }

  if (WATCHDOG_SCANCODE[ALLEGRO_KEY_F8])
    {
      global = get_profile (GLOBAL_PROFILE);
      disp = get_profile (DISP_PROFILE);
      flip = get_profile (FLIP_PROFILE);
      if (global <=0)
        global = 1;
      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_PERCENTDISPLAY),
                       (100 * (disp + flip)) / global);
    }

  if (watchdog_is_keyword (LW_CODE_KEYWORD_SPEED))
    {
      global = get_profile (GLOBAL_PROFILE);
      spread = get_profile (SPREAD_PROFILE);
      move = get_profile (MOVE_PROFILE);
      disp = get_profile (DISP_PROFILE);
      flip = get_profile (FLIP_PROFILE);

      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_SPREADGRADTIME),
                       spread);
      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_MOVEARMYTIME),
                       move);
      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_DISPLAYTIME),
                       disp);
      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_FLIPTIME), flip);
      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_OTHERCALCTIME),
                       global -spread - move - disp - flip);
      message_str_int (lw_lang_string (LW_LANG_STRING_CODE_GLOBALTIME),
                       global);
    }
}

/*------------------------------------------------------------------*/
static void
check_grad_to_disp (void)
{
  char grad_codes[6];
  int i, old;

  LW_MACRO_STRCPY (grad_codes, LW_CODE_KEYWORD_GRADN);
  old = GRAD_TO_DISP;
  for (i = 0; i <= 8; ++i)
    {
      grad_codes[4] = '0' + i;
      if (watchdog_is_keyword (grad_codes))
        GRAD_TO_DISP = i;
    }
  if (GRAD_TO_DISP > 0 && GRAD_TO_DISP <= 6 && GRAD_TO_DISP > PLAYING_TEAMS)
    GRAD_TO_DISP = 0;

  if (old != GRAD_TO_DISP)
    switch (GRAD_TO_DISP)
      {
      case 0:
        message_str (lw_lang_string (LW_LANG_STRING_CODE_STANDARDDISPLAY));
        break;
      case 1:
        message_str (lw_lang_string (LW_LANG_STRING_CODE_GRADIENT1));
        break;
      case 2:
        message_str (lw_lang_string (LW_LANG_STRING_CODE_GRADIENT2));
        break;
      case 3:
        message_str (lw_lang_string (LW_LANG_STRING_CODE_GRADIENT3));
        break;
      case 4:
        message_str (lw_lang_string (LW_LANG_STRING_CODE_GRADIENT4));
        break;
      case 5:
        message_str (lw_lang_string (LW_LANG_STRING_CODE_GRADIENT5));
        break;
      case 6:
        message_str (lw_lang_string (LW_LANG_STRING_CODE_GRADIENT6));
        break;
      case 7:
        message_str (lw_lang_string (LW_LANG_STRING_CODE_MESHSIZE));
        break;
      case 8:
        message_str (lw_lang_string (LW_LANG_STRING_CODE_MESHSTRUCTURE));
        break;
      }
}

/*------------------------------------------------------------------*/
static void
check_boolean_codes (void)
{
  /*
   * If we press F3 and we're not in network mode -> toggle pause mode
   */
  if (WATCHDOG_SCANCODE[ALLEGRO_KEY_F3] && !LW_NETWORK_ON)
    {
      if (PAUSE_ON)
        {
          PAUSE_ON = 0;
          message_str (lw_lang_string (LW_LANG_STRING_CODE_PAUSEOFF));
        }
      else
        {
          PAUSE_ON = 1;
          message_str (lw_lang_string (LW_LANG_STRING_CODE_PAUSEON));
        }
    }
  if (WATCHDOG_SCANCODE[ALLEGRO_KEY_F4])
    {
      if (CONFIG_WAVE_ON)
        {
          CONFIG_WAVE_ON = 0;
          message_str (lw_lang_string (LW_LANG_STRING_CODE_WAVEEFFECTSOFF));
        }
      else
        {
          CONFIG_WAVE_ON = 1;
          message_str (lw_lang_string (LW_LANG_STRING_CODE_WAVEEFFECTSON));
        }
    }
  if (WATCHDOG_SCANCODE[ALLEGRO_KEY_F9])
    {
      if (lw_capture_get_mode ())
        {
          lw_capture_set_mode (0);
          message_str (lw_lang_string (LW_LANG_STRING_CODE_CAPTUREOFF));
        }
      else
        {
          lw_capture_set_mode (1);
          message_str (lw_lang_string (LW_LANG_STRING_CODE_CAPTUREON));
        }
    }
}

/*------------------------------------------------------------------*/
void
reset_code (void)
{
  GRAD_TO_DISP = 0;
  PAUSE_ON = 0;
}

/*------------------------------------------------------------------*/
void
check_code (void)
{
  display_special_info ();
  check_grad_to_disp ();
  check_boolean_codes ();
}
