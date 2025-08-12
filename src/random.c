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
/* Copyright (C) 1998-2025 Christian Mauduit                                 */
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
/* name          : random.c                                         */
/* content       : wrapper for the functions in ../utils/lwmpagen/  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "backport.h"
#include "random.h"
#include "alleg2.h"
#include "map.h"
#include "config.h"
#include "disk.h"
#include "log.h"
#include "macro.h"
#include "startup.h"
#include "init.h"
#include "exec2.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_RANDOM_RUN_DELAY 100
#define LW_RANDOM_RUN_RETRIES 50
#ifdef UNIX
#define LW_RANDOM_COMMAND_END " > /dev/null 2>&1"
#else
#define LW_RANDOM_COMMAND_END ""
#endif
#define LW_COMMAND_EXTRA_SIZE 100

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

void *LW_RANDOM_RAW_MAP = NULL;
int LW_RANDOM_ON = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Generates a black&white palette which is suitable for
 * the game to handle nicely maps generated by lwmapgen. Basically
 * all we need is color 0 == black and color 2555 = white, but
 * we do a full grayscale, just to say the palette is not full of
 * garbage.
 */
static void
generate_bw_palette (PALETTE pal)
{
  int i;

  for (i = 0; i < 256; ++i)
    {
      pal[i].r = i;
      pal[i].g = i;
      pal[i].b = i;
    }
}

/*------------------------------------------------------------------*/
static ALLEGRO_BITMAP *
generate_bitmap ()
{
  ALLEGRO_BITMAP *bmp;
  PALETTE pal;

  char filename[LW_STARTUP_MAX_PATH_LENGTH];
  char command[LW_STARTUP_MAX_PATH_LENGTH + LW_COMMAND_EXTRA_SIZE];

  int size;
  int ok = 0;
  int temp_fd;

  size = random () % 6;

  // Create secure temporary file using mkstemp
  // First create template without .bmp extension for mkstemp
  char temp_name[] = "/tmp/lwmapgen_XXXXXX";
  temp_fd = mkstemp(temp_name);
  if (temp_fd == -1) {
    log_println_str("Error: Could not create temporary file");
    return NULL;
  }
  close(temp_fd); // Close the file descriptor, we just need the name
  
  // Remove the temporary file created by mkstemp (we just wanted the unique name)
  delete_file(temp_name);
  
  // Add .bmp extension to the filename
  LW_MACRO_SPRINTF1(filename, "%s.bmp", temp_name);
  LW_MACRO_SPRINTF4 (command,
                     "%s --out %s --size %d%s",
                     STARTUP_GEN_PATH, filename, size, LW_RANDOM_COMMAND_END);

  log_print_str ("Generating random map with command: \"");
  log_print_str (command);
  log_print_str ("\"");

  ok = lw_exec2_run (STARTUP_GEN_PATH, command);
#ifdef WIN32
  {
    /*
     * Under Windows we need to wait a bit until the map generator is done
     * with generating the map, until an easy way is found to "wait until the
     * program is finished".
     */
    int i;

    for (i = 0; i < LW_RANDOM_RUN_RETRIES && !exists (filename); ++i)
      {
        rest (LW_RANDOM_RUN_DELAY);
      }
  }
#endif
  display_success (ok);

  if (ok)
    {
      bmp = load_bitmap (filename, pal);
    }
  else
    {
      bmp = NULL;
    }

  delete_file (filename);

#ifdef WIN32
  {
    /*
     * Sometimes files aren't deleted properly on Windows, so we wait
     * "a little more" just to be sure.
     */
    int i;

    for (i = 0; i < LW_RANDOM_RUN_RETRIES && exists (filename); ++i)
      {
        rest (LW_RANDOM_RUN_DELAY);
        delete_file (filename);
      }
  }
#endif

  return bmp;
}

/*------------------------------------------------------------------*/
int
lw_random_generate_map ()
{
  ALLEGRO_BITMAP *bmp;
  PALETTE pal;
  void *raw_map = NULL;
  int result = 0;

  bmp = generate_bitmap ();
  generate_bw_palette (pal);

  if (bmp)
    {
      raw_map = lw_map_archive_raw_bmp (bmp, pal, "lwmapgen");
    }

  if (raw_map)
    {
      if (LW_RANDOM_RAW_MAP)
        {
          free (LW_RANDOM_RAW_MAP);
        }
      LW_RANDOM_RAW_MAP = raw_map;
      result = 1;
    }

  return result;
}
