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
/* nom           : init.c                                           */
/* contenu       : installation d'allegro                           */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>
#include <stdlib.h>
#include <time.h>
#ifdef DOS
#include <dos.h>
#endif

#include "base.h"
#include "config.h"
#include "gfxmode.h"
#include "init.h"
#include "exit.h"
#include "joystick.h"
#include "log.h"
#include "startup.h"
#include "ticker.h"
#include "sockgen.h"
#include "lang.h"
#include "asm.h"
#include "capture.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int LW_INIT_ALLEGRO_OK = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
display_success (int success)
{
  if (success)
    log_println_str (" - success!");
  else
    log_println_str (" - failed...");

  log_flush ();
  /* delay (100); */
}

/*------------------------------------------------------------------*/
void
display_success_driver (char *driver)
{
  log_print_str (" (driver=\"");
  log_print_str (driver);
  log_println_str ("\") - success!");

  log_flush ();
  /* delay (100); */
}

/*------------------------------------------------------------------*/
int
init_all ()
{
  int result = 0;
  int graphics = 1, assembly = 1, config = 1, timer = 1, keyboard = 1,
    mouse = 1, sound = 1, joystick = 1, network = 1;

  /*
   * we use srand to garantee that random() won't return the same values
   * each time the game is run
   */
  srandom (time (NULL));

  set_startup ();
  log_init ();
  lw_lang_init ();
  set_uformat (U_ASCII);

  log_print_str ("Starting Allegro (http://www.talula.demon.co.uk/allegro)");
  display_success (graphics = !allegro_init ());
  log_println ();

  if (graphics)
    {
      LW_INIT_ALLEGRO_OK = 1;

#ifdef DOS
      set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
#endif
      log_print_str ("Allegro ID : ");
      log_println_str (allegro_id);
      log_println ();

      set_color_depth (8);
      set_color_conversion (COLORCONV_REDUCE_TO_256);

      /*
       * We check for potential struct size errors
       */
      assembly = lw_asm_check_struct_align ();

      log_print_str ("Loading config options from \"");
      log_print_str (STARTUP_CFG_PATH);
      log_print_str ("\"");

      /*
       * We call this now only (after loading the options), for
       * options must be loaded for my_exit_close_button to work
       */
      //set_window_close_button (TRUE); deprecated
      //set_window_close_hook (my_exit_close_button); deprecated
      set_close_button_callback (my_exit_close_button);

      display_success (config = load_config_options ());

      log_print_str ("Installing timer");
      timer = !install_timer ();
      if (timer)
        {
          display_success_driver ((char *) timer_driver->ascii_name);
        }
      else
        {
          display_success (0);
        }

      start_ticker ();

      log_print_str ("Installing keyboard");
      keyboard = !install_keyboard ();
      if (keyboard)
        {
          display_success_driver ((char *) keyboard_driver->ascii_name);
        }
      else
        {
          display_success (0);
        }

      log_print_str ("Installing mouse");
      mouse = install_mouse () != -1;
      if (mouse)
        {
          display_success_driver ((char *) mouse_driver->ascii_name);
        }
      else
        {
          display_success (0);
        }

      if (STARTUP_DIGI_CARD != DIGI_NONE || STARTUP_MIDI_CARD != MIDI_NONE)
        {
          log_print_str ("Installing sound");
          sound = !install_sound (STARTUP_DIGI_CARD, STARTUP_MIDI_CARD, "");
          if (!sound)
            {
              /*
               * OK, first init failed, now we try without the MIDI card, which
               * is where problems usually come from, especially under Linux
               */
              sound = !install_sound (STARTUP_DIGI_CARD, MIDI_NONE, "");
            }
          if (sound)
            {
              log_print_str (" (digi=\"");
              log_print_str ((char *) digi_driver->ascii_name);
              log_print_str ("\", midi=\"");
              log_print_str ((char *) midi_driver->ascii_name);
              log_print_str ("\")");
            }
          display_success (sound);
        }

      if (STARTUP_JOYSTICK_ON)
        {
          log_print_str ("Installing joystick");
          if (STARTUP_JOYSTICK_ON)
            joystick = !my_initialise_joystick ();
          if (joystick)
            {
              display_success_driver ((char *) joystick_driver->ascii_name);
            }
          else
            {
              display_success (0);
            }
        }

      log_print_str ("Setting up network");
      display_success (network = lw_sock_init ());

      /*
       * We update the "capture" mode
       */
      lw_capture_set_mode (STARTUP_CAPTURE);
    }

  result = graphics
    && (assembly || !STARTUP_CHECK)
    && timer
    && keyboard
    && mouse && (sound || !STARTUP_CHECK) && (joystick || !STARTUP_CHECK);

  return (result);
}

/*------------------------------------------------------------------*/
int
start_graphic (void)
{
  int retour;

  retour = (!set_resolution (CONFIG_GFX_MENU, 0, CONFIG_FULLSCREEN, NULL));

  return retour;
}
