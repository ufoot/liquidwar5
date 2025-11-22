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
/* name          : music.c                                          */
/* content       : music related functions                          */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>

#include "backport.h"
#include "config.h"
#include "disk.h"
#include "startup.h"
#include "music.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int MUSIC_VOLUME = 0;

static int MUSIC_VOLUME_VALUES[17] = { 0,
  16, 32, 48, 64,
  80, 96, 112, 128,
  144, 160, 176, 192,
  208, 224, 240, 255
};

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
start_music (void)
{
  static int last_volume = -1;
  static int last_file = -1;
  int result = 1;
  int i;

  /*
   * we pick up a random music, letting the user choose the music
   * would imply work on the GUI, which is very time consuming 8-(
   */
  if (STARTUP_MUSIC_STATE && MIDI_MUSIC_NUMBER >= 0)
    {
      /*
       * we try and pick up a music that is different from
       * the last music played
       */
      if (MIDI_MUSIC_NUMBER >= 2)
        {
          do
            {
              i = random () % MIDI_MUSIC_NUMBER;
            }
          while (i == last_file);
        }
      else
        {
          /*
           * we have only 1 midi file loaded...
           */
          i = 0;
        }
      last_file = i;

      if (CONFIG_MUSIC_VOLUME != last_volume)
        {
          set_volume (-1, MUSIC_VOLUME_VALUES[CONFIG_MUSIC_VOLUME]);
          last_volume = CONFIG_MUSIC_VOLUME;
        }

      result = play_midi (MIDI_MUSIC[i], TRUE);
    }

  return result;
}

/*------------------------------------------------------------------*/
void
stop_music (void)
{
  if (STARTUP_MUSIC_STATE)
    {
      /*
       * Calling play_midi with a NULL arg ends the midi output
       */
      play_midi (NULL, FALSE);
    }
}
