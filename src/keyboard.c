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
/* nom           : keyboard.c                                       */
/* contenu       : gestion du clavier                               */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>

#include "config.h"
#include "joystick.h"
#include "keyboard.h"
#include "mouse.h"
#include "ticker.h"
#include "log.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

/*
 * 1 second limit when polling the keyboard for "no key pressed"
 */
#define LW_KEYBOARD_WAITNOKEY_TICK_LIMIT 1000

/*
 * 10 seconds limit when polling the keyboard for a key
 */
#define LW_KEYBOARD_WAITKEYPRESSED_TICK_LIMIT 10000

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

static char ALLOWED_KEYBOARD_KEY[128] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
is_touched_key (int i)
{
  int retval = 0;
  int found = 0;

  if (CONFIG_JOYSTICK_ON)
    switch (i)
      {
      case ALLEGRO_KEY_JOY1_UP:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY1_UP;
        break;
      case ALLEGRO_KEY_JOY1_RIGHT:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY1_RIGHT;
        break;
      case ALLEGRO_KEY_JOY1_DOWN:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY1_DOWN;
        break;
      case ALLEGRO_KEY_JOY1_LEFT:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY1_LEFT;
        break;

      case ALLEGRO_KEY_JOY1_B1:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY1_B1;
        break;
      case ALLEGRO_KEY_JOY1_B2:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY1_B2;
        break;
      case ALLEGRO_KEY_JOY1_B3:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY1_B3;
        break;
      case ALLEGRO_KEY_JOY1_B4:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY1_B4;
        break;

      case ALLEGRO_KEY_JOY2_UP:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY2_UP;
        break;
      case ALLEGRO_KEY_JOY2_RIGHT:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY2_RIGHT;
        break;
      case ALLEGRO_KEY_JOY2_DOWN:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY2_DOWN;
        break;
      case ALLEGRO_KEY_JOY2_LEFT:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY2_LEFT;
        break;

      case ALLEGRO_KEY_JOY2_B1:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY2_B1;
        break;
      case ALLEGRO_KEY_JOY2_B2:
        found = 1;
        retval = JOYSTICK_CONTROL_JOY2_B2;
        break;
      }

  if (!found)
    switch (i)
      {
      case ALLEGRO_KEY_MOUSE_UP:
        retval = MOUSE_CONTROL_UP;
        break;
      case ALLEGRO_KEY_MOUSE_RIGHT:
        retval = MOUSE_CONTROL_RIGHT;
        break;
      case ALLEGRO_KEY_MOUSE_DOWN:
        retval = MOUSE_CONTROL_DOWN;
        break;
      case ALLEGRO_KEY_MOUSE_LEFT:
        retval = MOUSE_CONTROL_LEFT;
        break;
      default:


        if (i >= 128)
          {
            i -= 128;
            retval = (key[i] & KB_EXTENDED) && ALLOWED_KEYBOARD_KEY[i];
          }
        else
          retval = (key[i] & KB_NORMAL) && ALLOWED_KEYBOARD_KEY[i];

      }


  return retval ? 1 : 0;
}

/*------------------------------------------------------------------*/
void
wait_no_key (void)
{
  int touched, i;
  int start_tick;

  start_tick = get_ticker ();

  lw_mouse_reset_control ();
  do
    {
      /*
       * Force a all to poll_keyboard if needed
       */
      keypressed ();

      touched = 0;
      lw_mouse_update_control ();
      my_poll_joystick ();
      for (i = 0; i < 256; ++i)
        if (is_touched_key (i))
          touched = 1;
    }
  while (touched &&
         get_ticker () < start_tick + LW_KEYBOARD_WAITNOKEY_TICK_LIMIT);
}

/*------------------------------------------------------------------*/
int
wait_key_pressed (void)
{
  int touched = 0, i, result = 0;
  int start_tick;

  start_tick = get_ticker ();

  lw_mouse_reset_control ();
  while ((!touched) &&
         get_ticker () < start_tick + LW_KEYBOARD_WAITKEYPRESSED_TICK_LIMIT)
    {
      /*
       * Force a all to poll_keyboard if needed
       */
      keypressed ();

      lw_mouse_update_control ();
      my_poll_joystick ();
      for (i = 0; i < 256 && !touched; ++i)
        if (is_touched_key (i))
          {
            touched = 1;
            result = i;
          }
    }

  /*
   * We map all the >= 128 keys to [0-127] since the strongest bit
   * is used by allegro as a "modifier" which we do not care about
   * at all
   */
  if (result >= 128)
    {
      result -= 128;
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Ugly hack to fix a UNIX bug. When one switches between workspaces,
 * under X, the keyboard functions go crazy. One solution: uninstall
 * and then reinstall the keyboad. It's ugly but it works...
 */
void
lw_keyboard_reset_hack (void)
{
#ifdef UNIX
  {
    int keyboard_state = 0;

    clear_keybuf ();
    remove_keyboard ();
    keyboard_state = install_keyboard ();

    if (keyboard_state)
      {
        log_println_str ("Error: unable to reinstall keyboard");
      }
  }
#endif
}
