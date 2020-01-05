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
/* Copyright (C) 1998-2020 Christian Mauduit                                 */
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
/* nom           : main.c                                           */
/* contenu       : procedure main                                   */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>
#ifdef DOS
#include <dos.h>
#endif

#include "bigdata.h"
#include "config.h"
#include "disk.h"
#include "exit.h"
#include "init.h"
#include "log.h"
#include "menu.h"
#include "lw6.h"
#include "parser.h"
#include "sound.h"
#include "startup.h"
#include "basicopt.h"

/*==================================================================*/
/* construct pour reduire la place d'allegro                        */
/*==================================================================*/
#ifdef DOS

BEGIN_GFX_DRIVER_LIST
  GFX_DRIVER_VGA
  GFX_DRIVER_MODEX
  GFX_DRIVER_VBEAF
  GFX_DRIVER_VESA2L
  GFX_DRIVER_VESA2B
  GFX_DRIVER_VESA1
  END_GFX_DRIVER_LIST
  BEGIN_COLOR_DEPTH_LIST
  COLOR_DEPTH_8
  COLOR_DEPTH_15
  COLOR_DEPTH_16
  COLOR_DEPTH_24
  COLOR_DEPTH_32
  END_COLOR_DEPTH_LIST
  BEGIN_DIGI_DRIVER_LIST
  DIGI_DRIVER_SB
  DIGI_DRIVER_SOUNDSCAPE
  DIGI_DRIVER_AUDIODRIVE
  END_DIGI_DRIVER_LIST BEGIN_MIDI_DRIVER_LIST END_MIDI_DRIVER_LIST
#endif
/*==================================================================*/
/* functions                                                        */
/*==================================================================*/
/*------------------------------------------------------------------*/
  int
main (int argc, char **argv)
{
  int init_success = 0;
  int big_data_success = 0;
  int load_dat_success = 0;
  int load_custom_success = 0;

  read_command_line (argc, argv);

  if (lw_basicopt_process ("Liquid War - a unique multiplayer wargame"))
    {
      init_success = init_all ();

      if (init_success)
        {
          big_data_success = (malloc_big_data () != NULL);
          log_println ();
          load_dat_success = load_dat ();
          log_println ();
          load_custom_success = load_custom ();
          log_println ();

#ifdef DOS
          if (STARTUP_STOP)
            readkey ();
          else
            delay (500);
#endif
        }

      if (!init_success)
        my_exit (EXIT_CODE_INIT_FAIL);
      if (!big_data_success)
        my_exit (EXIT_CODE_BIG_DATA);
      if (!load_dat_success)
        my_exit (EXIT_CODE_LOAD_FAIL);
      if (!load_custom_success)
        my_exit (EXIT_CODE_CUSTOM_FAIL);

      order_map ();
      check_config_range ();

      check_free_memory ();

      start_graphic ();
      start_water ();
      if (CONFIG_LW6_ADVERTISING_DONE || CONFIG_LW6_ADVERTISING_SKIP)
        {
          main_menu (1);
        }
      else
        {
          CONFIG_LW6_ADVERTISING_DONE = lw6_advertise_menu (1);
          main_menu (0);
        }

      my_exit (EXIT_CODE_OK);
    }

  return 0;
}

#ifdef WIN32
END_OF_MAIN ()
#endif
#ifdef UNIX
  END_OF_MAIN ();
#endif
