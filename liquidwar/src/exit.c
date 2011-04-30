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
/* nom           : exit.c                                           */
/* contenu       : fin d'allero et messages d'adieu                 */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#ifdef WIN32
#include "popupgen.h"
#endif

#ifdef DOS
#include <conio.h>
#endif

#include "basicopt.h"
#include "config.h"
#include "exit.h"
#include "init.h"
#include "log.h"
#include "sound.h"
#include "startup.h"
#include "ticker.h"
#include "sockgen.h"
#include "viewport.h"

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

static int LW_EXIT_FORCE_SHUTDOWN = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* fonction de desinstallation d'allegro                            */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static void
exit_all (void)
{
  if (LW_INIT_ALLEGRO_OK)
    {
      /*
       * we take a rest before and after calling set_gfx_mode(GFX_TEXT,...)
       * This is an attempt to get rid of a hideous bug under X-Win,
       * which said that there was a "bad file descriptor"
       */
      rest (10);
      last_flip ();
      set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
      rest (10);

      log_println ();
      log_println_str
	("Leaving Allegro (http://www.talula.demon.co.uk/allegro)");
      save_config_options ();
      stop_water ();
      stop_ticker ();
      remove_sound ();
      remove_mouse ();
      clear_keybuf ();
      remove_keyboard ();
      remove_timer ();
      allegro_exit ();
      lw_sock_exit ();
    }
}

/*------------------------------------------------------------------*/
static void
disp_base_message (void)
{
  lw_basicopt_legal_info ("Liquid War by U-Foot");
}

/*------------------------------------------------------------------*/
static void
disp_ok_message (void)
{
  log_println_str ("Thanks for playing Liquid War, I hope you enjoyed it!");
}

/*------------------------------------------------------------------*/
static void
disp_error_message (void)
{
  log_println_str
    ("Thanks for trying to play, sorry about the trouble Liquid War is causing you.");
}

/*------------------------------------------------------------------*/
static void
disp_init_fail_message (void)
{
  log_println_str ("An error occured during the init process.");
  log_println ();
  log_println_str ("Try \"lw -silent\" to disable sound management.");
  log_println_str ("Try \"lw -nojoy\" to disable joystick control.");
  log_println ();
  log_print_str ("Try to delete the \"");
  log_print_str (STARTUP_CFG_PATH);
  log_println_str ("\" file and restart Liquid War.");
#ifdef WIN32
  lw_popup_init_fail ();
#endif
}

/*------------------------------------------------------------------*/
static void
disp_common_memory_message (void)
{
  log_println_str ("Try \"lw -auto\" if you don't want to waste your time.");
  log_println ();
  log_println_str
    ("Try \"lw -mem n\" where 'n' is the number of Mb to be allocated by Liquid War");
  log_println_str
    ("to do all his calculus. The lower it is, the safer it is in general, but you");
  log_println_str
    ("won't be able to play with all the levels if 'n' is too small.");
  log_println_str ("'n' can range from 1 to 32, default is 8.");
  log_println ();
  log_println_str
    ("If you run Liquid War from Windows, try and give it more DPMI memory.");
  log_println ();
  log_println_str ("Try \"lw -noback\" to skip background image loading.");
  log_println_str ("Try \"lw -notex\" to skip texture loading.");
  log_println_str ("Try \"lw -nowater\" to skip water sounds loading.");
  log_println_str ("Try \"lw -nosfx\" to skip sound fx loading.");
  log_println ();
  log_println_str
    ("Try \"lw -nice\" if \"lw -auto\" doesn't work correctly.");
  log_println_str ("Try \"lw -safe\" if nothing else works.");
}

/*------------------------------------------------------------------*/
static void
disp_mem_trouble_message (void)
{
  log_println_str ("Liquid War is running short of memory for its bitmaps.");
  log_println ();
  disp_common_memory_message ();
#ifdef WIN32
  lw_popup_mem_trouble ();
#endif
}

/*------------------------------------------------------------------*/
static void
disp_big_data_message (void)
{
  log_println_str
    ("Liquid War was unable to allocate memory for its calculus.");
  log_println ();
  disp_common_memory_message ();
#ifdef WIN32
  lw_popup_big_data ();
#endif
}

/*------------------------------------------------------------------*/
static void
disp_load_fail_message (void)
{
  log_println_str ("Liquid War was unable to load its datafile.");
  log_println ();
  log_print_str ("Please first check that \"");
  log_print_str (STARTUP_DAT_PATH);
  log_print_str ("\" exists.");
  log_println ();
  log_println ();
  disp_common_memory_message ();
#ifdef WIN32
  lw_popup_load_fail ();
#endif
}

/*------------------------------------------------------------------*/
static void
disp_custom_fail_message (void)
{
  log_println_str ("Liquid War was unable to load your custom maps.");
  log_println ();
  log_println_str ("This can happen when:");
  log_println ();
  log_println_str
    ("The path is wrong, use \"lw -tex path\" or \"lw -map path\".");
  log_println_str ("Your bitmaps are not correct, just change them.");
  log_println_str ("You don't have enough memory.");
#ifdef WIN32
  lw_popup_custom_fail ();
#endif
}

/*------------------------------------------------------------------*/
static void
disp_graphics_fail_message (void)
{
  log_println_str ("Unable to set up a graphic mode.");
  log_println ();
  log_println_str ("Liquid War _needs_ to set up a graphic mode.");
  log_println_str ("Please check that your config is supported by Allegro.");
#ifdef WIN32
  lw_popup_graphics_fail ();
#endif
}

/*------------------------------------------------------------------*/
void
my_exit_close_button (void)
{
  LW_EXIT_FORCE_SHUTDOWN = 1;
}

/*------------------------------------------------------------------*/
void
my_exit_poll ()
{
  /*
   * We exit if the close button has been clicked or
   * F10 has been pressed
   */
  if (LW_EXIT_FORCE_SHUTDOWN || key[KEY_F10])
    {
      my_exit (EXIT_CODE_OK);
    }
}

/*------------------------------------------------------------------*/
void
my_exit (int code)
{
  exit_all ();
  if (code == EXIT_CODE_OK)
    {
      disp_base_message ();
      log_println ();
      disp_ok_message ();
    }
  else
    {
      log_println ();
      log_flush ();
      switch (code)
	{
	case EXIT_CODE_INIT_FAIL:
	  disp_init_fail_message ();
	  break;
	case EXIT_CODE_MEM_TROUBLE:
	  disp_mem_trouble_message ();
	  break;
	case EXIT_CODE_BIG_DATA:
	  disp_big_data_message ();
	  break;
	case EXIT_CODE_LOAD_FAIL:
	  disp_load_fail_message ();
	  break;
	case EXIT_CODE_CUSTOM_FAIL:
	  disp_custom_fail_message ();
	  break;
	case EXIT_CODE_GRAPHICS_FAIL:
	  disp_graphics_fail_message ();
	  break;
	}
#ifdef DOS
      getch ();
#endif
      log_println ();
      log_println ();
      log_println ();
      disp_base_message ();
      log_println ();
      disp_error_message ();
    }

  log_exit ();

  exit (code);
}
