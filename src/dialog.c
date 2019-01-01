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
/* nom           : dialog.c                                         */
/* contenu       : utilitaires gui                                  */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#ifdef UNIX
#include <unistd.h>
#endif

#include "dialog.h"
#include "sound.h"
#include "back.h"
#include "lang.h"
#include "exit.h"
#include "keyboard.h"
#include "capture.h"
#include "joystick.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
menu_real_x (int x)
{
  return ((x * SCREEN_W) / MENU_VIRTUAL_W);
}

/*------------------------------------------------------------------*/
int
menu_real_y (int y)
{
  return ((y * SCREEN_H) / MENU_VIRTUAL_H);
}

/*------------------------------------------------------------------*/
void
menu_real_coord (DIALOG * d)
{
  d->x = menu_real_x (d->x);
  d->y = menu_real_y (d->y);
  d->w = menu_real_x (d->w);
  d->h = menu_real_y (d->h);
}

/*------------------------------------------------------------------*/
/* gestion generique des menus                                      */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
int
find_center (int pos, int num, int space, int size)
{
  int center;

  center = (size - num * space) / 2;
  center += pos * space + space / 2;

  return center;
}

/*------------------------------------------------------------------*/
void
color_one_dialog (DIALOG * d)
{
  d->fg = MENU_FG;
  d->bg = MENU_BG;
}

/*------------------------------------------------------------------*/
void
center_one_dialog (DIALOG * d, int x, int y, int w, int h)
{
  d->x = x - w / 2;
  d->y = y - h / 2;
  d->w = w;
  d->h = h;
}

/*------------------------------------------------------------------*/
void
quick_buttons (DIALOG * d)
{
  int i;

  d[MENU_QUICK_QUIT].dp = lw_lang_string (LW_LANG_STRING_DIALOG_QUIT);
  d[MENU_QUICK_BACK].dp = lw_lang_string (LW_LANG_STRING_DIALOG_BACK);
  d[MENU_QUICK_PLAY].dp = lw_lang_string (LW_LANG_STRING_DIALOG_PLAY);
  d[MENU_QUICK_MAIN].dp = lw_lang_string (LW_LANG_STRING_DIALOG_MAIN);
  d[MENU_QUICK_QUIT].key = 'q';
  d[MENU_QUICK_BACK].key = 'b';
  d[MENU_QUICK_PLAY].key = 'h';
  d[MENU_QUICK_MAIN].key = 'm';

  d[MENU_QUICK_BACK].x = d[MENU_QUICK_QUIT].x = MENU_W_SPACE / 2;
  d[MENU_QUICK_PLAY].x =
    d[MENU_QUICK_MAIN].x = MENU_VIRTUAL_W - 1
    - MENU_W_SPACE / 2 - MENU_W_QUICK;
  d[MENU_QUICK_BACK].y = d[MENU_QUICK_PLAY].y = MENU_H_SPACE / 2;
  d[MENU_QUICK_QUIT].y =
    d[MENU_QUICK_MAIN].y = MENU_VIRTUAL_H - 1
    - MENU_H_SPACE / 2 - MENU_H_QUICK;
  for (i = 0; i < 4; ++i)
    {
      d[i].proc = my_button_proc;
      d[i].flags = D_EXIT;
      d[i].h = MENU_H_QUICK;
      d[i].w = MENU_W_QUICK;
      color_one_dialog (d + i);
      menu_real_coord (d + i);
    }
}

/*------------------------------------------------------------------*/
void
standard_button (DIALOG * d, int x_pos, int y_pos, int x_num, int y_num)
{
  d->proc = my_button_proc;

  center_one_dialog (d,
                     find_center (x_pos, x_num,
                                  MENU_W_STANDARD + MENU_W_SPACE,
                                  MENU_VIRTUAL_W), find_center (y_pos, y_num,
                                                                MENU_H_STANDARD
                                                                +
                                                                MENU_H_SPACE,
                                                                MENU_VIRTUAL_H),
                     MENU_W_STANDARD, MENU_H_STANDARD);
  color_one_dialog (d);
  menu_real_coord (d);
  d->key = 0;
  d->dp = "";
  d->flags = D_EXIT;
}

/*------------------------------------------------------------------*/
void
standard_small_button (DIALOG * d, int x_pos, int y_pos, int x_num, int y_num)
{
  d->proc = my_button_proc;

  center_one_dialog (d,
                     find_center (x_pos, x_num,
                                  MENU_W_STANDARD / 2 + MENU_W_SPACE,
                                  MENU_VIRTUAL_W), find_center (y_pos, y_num,
                                                                MENU_H_STANDARD
                                                                +
                                                                MENU_H_SPACE,
                                                                MENU_VIRTUAL_H),
                     MENU_W_STANDARD / 2, MENU_H_STANDARD);
  color_one_dialog (d);
  menu_real_coord (d);
  d->key = 0;
  d->dp = "";
  d->flags = D_EXIT;
}

/*------------------------------------------------------------------*/
void
standard_big_button (DIALOG * d, int x_pos, int y_pos, int x_num, int y_num)
{
  d->proc = my_button_proc;

  center_one_dialog (d,
                     find_center (x_pos, x_num,
                                  MENU_W_STANDARD * 2 + MENU_W_SPACE,
                                  MENU_VIRTUAL_W), find_center (y_pos, y_num,
                                                                MENU_H_STANDARD
                                                                +
                                                                MENU_H_SPACE,
                                                                MENU_VIRTUAL_H),
                     MENU_W_STANDARD * 2, MENU_H_STANDARD);
  color_one_dialog (d);
  menu_real_coord (d);
  d->key = 0;
  d->dp = "";
  d->flags = D_EXIT;
}

/*------------------------------------------------------------------*/
void
main_message (DIALOG * d, char *str)
{
  standard_big_button (d, 0, 0, 1, 1);

  d->dp = str;
  d->key = 0;
  d->proc = my_textbox_proc;
  d->flags = 0;
}

/*------------------------------------------------------------------*/
void
main_info (DIALOG * d, char *str)
{
  color_one_dialog (d);
  d->proc = my_textbox_proc;
  d->dp = str;
  d->key = 0;
  d->flags = 0;
}

/*------------------------------------------------------------------*/
DIALOG_PLAYER *
my_init_dialog (DIALOG * d, int f)
{
  DIALOG_PLAYER *dp;

  lw_keyboard_reset_hack ();

  dp = init_dialog (d, f);

  return dp;
}

/*------------------------------------------------------------------*/
int
my_do_dialog_no_clear (DIALOG * d, int f)
{
  int result;
  void *player;

  player = my_init_dialog (d, f);

  while (my_update_dialog (player))
    {
      /*
       * Nothing here...
       */
    }

  result = shutdown_dialog (player);

  play_click ();

  return result;
}

/*------------------------------------------------------------------*/
int
my_do_dialog (DIALOG * d, int f)
{
  int result;

  display_back_image ();
  result = my_do_dialog_no_clear (d, f);

  return result;
}

/*------------------------------------------------------------------*/
int
my_update_dialog (DIALOG_PLAYER * player)
{
  int result;

  result = update_dialog (player);
  lw_capture_dump_menu ();
  my_poll_joystick ();
  my_exit_poll ();

#ifdef UNIX
  /*
   * The usleep calls prevents the game from eating 100% of the
   * CPU time on UNIX platforms.
   */
  usleep (100);
#endif
  //yield_timeslice (); deprecated
  rest (0);

  return result;
}

/*------------------------------------------------------------------*/
void
display_center_message (char *str)
{
  DIALOG d[2];
  DIALOG_PLAYER *dp;

  memset (d, 0, sizeof (d));

  display_back_image ();
  main_message (d, str);
  d[1].proc = 0;
  dp = my_init_dialog (d, 0);
  my_update_dialog (dp);
  shutdown_dialog (dp);

  /*
   * We wait a little bit, so that the player can see the message
   */
  //rest(100);
}

/*------------------------------------------------------------------*/
int
slider_int (void *dp3, int d2)
{
  int *val;

  val = dp3;
  *val = d2;

  play_click ();

  return 0;
}
