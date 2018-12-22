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
/* Liquid War homepage : http://www.ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* nom           : team.c                                           */
/* content       : menu used to choose teams                        */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <ctype.h>

#include "back.h"
#include "config.h"
#include "dialog.h"
#include "disk.h"
#include "help.h"
#include "joystick.h"
#include "keyboard.h"
#include "menu.h"
#include "mouse.h"
#include "sound.h"
#include "team.h"
#include "log.h"
#include "lang.h"
#include "netmess.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

static char *MENU_KEY_NAMES[] = {
  "None",
  "A",
  "B",
  "C",
  "D",
  "E",
  "F",
  "G",
  "H",
  "I",
  "J",
  "K",
  "L",
  "M",
  "N",
  "O",
  "P",
  "Q",
  "R",
  "S",
  "T",
  "U",
  "V",
  "W",
  "X",
  "Y",
  "Z",
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "Pad0",
  "Pad1",
  "Pad2",
  "Pad3",
  "Pad4",
  "Pad5",
  "Pad6",
  "Pad7",
  "Pad8",
  "Pad9",
  "F1",
  "F2",
  "F3",
  "F4",
  "F5",
  "F6",
  "F7",
  "F8",
  "F9",
  "F10",
  "F11",
  "F12",
  "ESC",
  "~",
  "-",
  "=",
  "Back",
  "Tab",
  "{",
  "}",
  "Enter",
  ":",
  "'",
  "\\",
  "\\",
  ",",
  ".",
  "/",
  "Space",
  "Ins",
  "Del",
  "Home",
  "End",
  "Pgup",
  "Pgdn",
  "<",
  ">",
  "/\\",
  "\\/",
  "Pad/",
  "*",
  "Pad-",
  "Pad+",
  "Pad.",
  "PadE",
  "PScr",
  "Pause",
  "LShf",
  "RShf",
  "LCnt",
  "RCnt",
  "Alt",
  "Altgr",
  "LWin",
  "RWin",
  "Menu",
  "SLock",
  "NLock",
  "CLock",
  "None",
  "None",
  "None",
  "None",
  "J1/\\",
  "J1>",
  "J1\\/",
  "J1<",
  "J1B1",
  "J1B2",
  "J1B3",
  "J1B4",
  "J2/\\",
  "J2>",
  "J2\\/",
  "J2<",
  "J2B1",
  "J2B2",
  "M/\\",
  "M>",
  "M\\/",
  "M<"
};

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
cleanup_player_names ()
{
  int i;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      lw_netmess_cleanup_arg (CONFIG_PLAYER_NAME[i]);
    }
}

/*------------------------------------------------------------------*/
static int
suppress_conflicting_color (int color, int replacement_color)
{
  int i, n = -1;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (CONFIG_TEAM_COLOR[i] == color)
        {
          CONFIG_TEAM_COLOR[n = i] = replacement_color;
        }
    }

  return n;
}

/*------------------------------------------------------------------*/
static char *
get_key_name (int num)
{
  if (MENU_KEY_NAMES[num] != NULL)
    return MENU_KEY_NAMES[num];
  else
    return "None";
}

/*------------------------------------------------------------------*/
static void
change_key (DIALOG * d, int *val)
{
  lw_mouse_reset_control ();
  wait_no_key ();
  (*val) = wait_key_pressed ();
  play_click ();
  clear_keybuf ();
  d->dp = get_key_name (*val);
}

/*------------------------------------------------------------------*/
static void
update_team_box (DIALOG * d, int number)
{
  switch (CONFIG_CONTROL_TYPE[number])
    {
    case CONFIG_CONTROL_TYPE_OFF:
      d[15].dp = lw_lang_string (LW_LANG_STRING_TEAM_OFF);
      break;
    case CONFIG_CONTROL_TYPE_HUMAN:
      d[15].dp = lw_lang_string (LW_LANG_STRING_TEAM_HUMAN);
      break;
    case CONFIG_CONTROL_TYPE_CPU:
      d[15].dp = lw_lang_string (LW_LANG_STRING_TEAM_CPU);
      break;
    default:
      d[15].dp = "???";
    }
  if (CONFIG_CONTROL_TYPE[number] == CONFIG_CONTROL_TYPE_OFF)
    {
      d[0].bg = MENU_BG;
    }
  else
    {
      d[0].bg = 128 + (CONFIG_TEAM_COLOR[number]) * 10;
    }
}

/*------------------------------------------------------------------*/
static void
redraw_team_box (DIALOG * d, int number)
{
  int i;

  update_team_box (d, number);

  scare_mouse ();
  for (i = 0; i < 20; ++i)
    {
      (d + i)->proc (MSG_DRAW, d + i, 0);
    }
  unscare_mouse ();
}

/*------------------------------------------------------------------*/
static void
team_param (DIALOG * d, int x, int y, int number)
{
  int w1, w2, w3, h, i;

  d[0].x = x;
  d[0].y = y;

  w1 = (menu_real_x (MENU_W_TEAM - 2 * MENU_W_SPACE) + 2) / 12;
  w2 = (menu_real_x (MENU_W_TEAM - 2 * MENU_W_SPACE) + 2) / 2;
  w3 = menu_real_x (MENU_W_TEAM - 2 * MENU_W_SPACE) + 2;
  h = (menu_real_y (MENU_H_TEAM - 2 * MENU_H_SPACE) + 2) / 6;

  x += 2 + menu_real_x (MENU_W_SPACE);
  y += 2 + menu_real_y (MENU_H_SPACE);

  for (i = 0; i < 20; ++i)
    {
      d[i].proc = my_button_proc;
      d[i].key = 0;
      d[i].dp = "";
      d[i].flags = D_EXIT;
      d[i].h = h - 2;
      color_one_dialog (d + i);
    }

  d[0].proc = d_box_proc;
  d[0].w = menu_real_x (MENU_W_TEAM);
  d[0].h = menu_real_y (MENU_H_TEAM);

  d[1].proc = d_box_proc;
  d[1].x = d[2].x = x;
  d[1].y = d[2].y = y;
  d[1].w = d[2].w = w3 - 2;

  for (i = 2; i < 14; ++i)
    {
      d[i].x = x + (i - 2) * w1;
      d[i].y = y + 2 * h;
      d[i].w = w1 - 2;
      d[i].bg = 128 + (i - 2) * 10;
    }

  for (i = 16; i < 20; ++i)
    d[i].w = w2 - 2;

  d[14].x = d[1].x + 1;
  d[14].y = d[1].y + 1;
  d[14].w = d[1].w - 2;

  d[15].x = x + w2 / 4;
  d[15].y = y + h;
  d[15].w = (3 * w2) / 2 - 2;

  d[14].proc = my_edit_proc;
  d[14].dp = CONFIG_PLAYER_NAME[number];
  d[14].d1 = NAME_SIZE;
  d[14].flags = 0;

  d[16].x = d[19].x = x + w2 / 2;
  d[17].x = x;
  d[18].x = x + w2;
  d[16].y = y + 3 * h;
  d[17].y = d[18].y = y + 4 * h;
  d[19].y = y + 5 * h;

  d[16].dp = get_key_name (CONFIG_KEY_UP[number]);
  d[17].dp = get_key_name (CONFIG_KEY_LEFT[number]);
  d[18].dp = get_key_name (CONFIG_KEY_RIGHT[number]);
  d[19].dp = get_key_name (CONFIG_KEY_DOWN[number]);

  update_team_box (d, number);
}

/*------------------------------------------------------------------*/
int
choose_teams (void)
{
  int retour = 0;
  DIALOG d[125];
  int i, choix = 0;
  int x1, x2, x3, y1, y2;
  int button_team, button_type;

  memset (d, 0, sizeof (d));

  quick_buttons (d);

  x1 = (MENU_VIRTUAL_W - (3 * MENU_W_TEAM + 2 * MENU_W_SPACE)) / 2;
  x2 = x1 + MENU_W_TEAM + MENU_W_SPACE;
  x3 = x2 + MENU_W_TEAM + MENU_W_SPACE;
  y1 = (MENU_VIRTUAL_H - (2 * MENU_H_TEAM + MENU_H_SPACE)) / 2;
  y2 = y1 + MENU_H_TEAM + MENU_H_SPACE;

  x1 = menu_real_x (x1);
  x2 = menu_real_x (x2);
  x3 = menu_real_x (x3);
  y1 = menu_real_y (y1);
  y2 = menu_real_y (y2);

  team_param (d + 4, x1, y1, 0);
  team_param (d + 24, x2, y1, 1);
  team_param (d + 44, x3, y1, 2);
  team_param (d + 64, x1, y2, 3);
  team_param (d + 84, x2, y2, 4);
  team_param (d + 104, x3, y2, 5);

  d[124].proc = NULL;

  set_palette_for_choose_color ();
  my_set_palette ();
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
        default:
          button_team = (choix - 4) / 20;
          button_type = (choix - 4) % 20;
          switch (button_type)
            {
            case 15:
              switch (CONFIG_CONTROL_TYPE[button_team])
                {
                case CONFIG_CONTROL_TYPE_OFF:
                  CONFIG_CONTROL_TYPE[button_team] =
                    CONFIG_CONTROL_TYPE_HUMAN;
                  break;
                case CONFIG_CONTROL_TYPE_HUMAN:
                  CONFIG_CONTROL_TYPE[button_team] = CONFIG_CONTROL_TYPE_CPU;
                  break;
                default:
                  CONFIG_CONTROL_TYPE[button_team] = CONFIG_CONTROL_TYPE_OFF;
                }
              redraw_team_box (d + 4 + 20 * button_team, button_team);
              break;
            case 16:
              change_key (d + choix, &CONFIG_KEY_UP[button_team]);
              break;
            case 17:
              change_key (d + choix, &CONFIG_KEY_LEFT[button_team]);
              break;
            case 18:
              change_key (d + choix, &CONFIG_KEY_RIGHT[button_team]);
              break;
            case 19:
              change_key (d + choix, &CONFIG_KEY_DOWN[button_team]);
              break;
            default:
              if (button_type >= 2 && button_type < 14)
                {
                  i = suppress_conflicting_color
                    (button_type - 2, CONFIG_TEAM_COLOR[button_team]);
                  if (i >= 0)
                    redraw_team_box (d + 4 + 20 * i, i);
                  CONFIG_TEAM_COLOR[button_team] = button_type - 2;
                  redraw_team_box (d + 4 + 20 * button_team, button_team);
                }
            }
        }
    }

  cleanup_player_names ();

  if (retour > 0)
    retour--;

  return retour;
}
