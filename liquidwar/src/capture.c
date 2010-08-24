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
/* name          : capture.c                                        */
/* content       : capture the video output and dump it on the disk */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdio.h>

#include <allegro.h>

#include "capture.h"
#include "viewport.h"
#include "log.h"
#include "init.h"
#include "palette.h"
#include "config.h"
#include "ticker.h"
#include "macro.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_CAPTURE_FILENAME_FORMAT_STRING "lw%06d.bmp"
#define LW_CAPTURE_FILENAME_BUFFER_SIZE   100

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

static int LW_CAPTURE_MODE = 0;

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Returns the capture mode
 */
int
lw_capture_get_mode ()
{
  return LW_CAPTURE_MODE;
}

/*------------------------------------------------------------------*/
/*
 * Sets the capture mode
 */
void
lw_capture_set_mode (int mode)
{
  LW_CAPTURE_MODE = mode;
}

/*------------------------------------------------------------------*/
/*
 * Dumps the current video output on the disk, but performs some
 * checks which make it adapted to a menu context, such as not
 * calling the function "too often".
 */
int
lw_capture_dump_menu ()
{
  int result = 1;
  static int last_dump_time = 0;
  int dummy;

  /*
   * We don't dump stuff if the last dump is too recent.
   * To determine "how often" we may dump, we use the
   * same settings than the in-game "frame per sec" limit.
   */
  if (get_ticker () >= last_dump_time +
      DISPLAY_DELAY_MIN[CONFIG_FRAMES_PER_SEC_LIMIT])
    {
      last_dump_time = get_ticker ();

      /*
       * We do nothing if we're not in capture mode
       */
      if (LW_CAPTURE_MODE)
	{
	  lw_capture_dump_game ();
	}
    }

  /*
   * Now we check manually for F9 for there's no way
   * to do it easily within the generic menu code.
   */
  if (key[KEY_F9])
    {
      while (key[KEY_F9])
	{
	  /*
	   * Force a all to poll_keyboard if needed
	   */
	  dummy = keypressed ();

	  rest (10);
	}
      LW_CAPTURE_MODE = !LW_CAPTURE_MODE;
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Dumps the current video output on the disk
 */
int
lw_capture_dump_game ()
{
  int result = 1;
  char buffer[LW_CAPTURE_FILENAME_BUFFER_SIZE];
  static int frame_number = 0;
  BITMAP *screenshot;

  /*
   * We do nothing if we're not in capture mode
   */
  if (LW_CAPTURE_MODE)
    {
      /*
       * Prepare a nice filename
       */
      LW_MACRO_SPRINTF1 (buffer, LW_CAPTURE_FILENAME_FORMAT_STRING,
			 frame_number++);

      /*
       * Log on the console
       */
      log_print_str ("Dumping video output to \"");
      log_print_str (buffer);
      log_print_str ("\"");

      /*
       * Create a sub-bitmap with the right size
       */
      screenshot =
	create_sub_bitmap (screen, 0, PAGE_FLIP_H, SCREEN_W, SCREEN_H);

      if (screenshot)
	{
	  /*
	   * Write to disk
	   */
	  result = !save_bitmap (buffer, screenshot, GLOBAL_PALETTE);
	  destroy_bitmap (screenshot);
	}
      else
	{
	  result = 0;
	}

      display_success (result);
    }

  return result;
}
