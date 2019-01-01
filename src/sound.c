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
/* name          : sound.c                                          */
/* content       : low-level sound routines                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "config.h"
#include "disk.h"
#include "sound.h"
#include "mutxgen.h"
#include "log.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int WATER_VOLUME = 0;

#define HASARD_SIZE 1024
static int HASARD_VALUE[HASARD_SIZE];
static int HASARD_INDEX;

static int SOUND_VOLUME_VALUES[17] = { 0,
  8, 10, 13, 16,
  20, 25, 32, 40,
  51, 64, 81, 101,
  128, 161, 204, 255
};

static int WATER_DELAY = 2000;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* mise en place des bruits de fond                                 */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static void
water_handler (void)
{
  int pan;
  int hasard;

  HASARD_INDEX++;
  HASARD_INDEX = HASARD_INDEX % HASARD_SIZE;

  if (WATER_VOLUME > 0)
    {
      if (HASARD_INDEX % 2)
        pan = 192;
      else
        pan = 64;

      hasard = HASARD_VALUE[HASARD_INDEX] % SAMPLE_WATER_NUMBER;
      play_sample (SAMPLE_WATER[hasard],
                   SOUND_VOLUME_VALUES[WATER_VOLUME], pan, 1000, 0);
    }
}

END_OF_FUNCTION (water_handler);

/*------------------------------------------------------------------*/
int
start_water (void)
{
  int result = 1;
  int i;

  for (i = 0; i < HASARD_SIZE; ++i)
    HASARD_VALUE[i] = random () + i;

  if (LOADED_WATER)
    {
      WATER_VOLUME = CONFIG_WATER_VOLUME_MENU;

      LOCK_FUNCTION (water_handler);
      LOCK_VARIABLE (HASARD_INDEX);
      LOCK_VARIABLE (HASARD_VALUE);
      LOCK_VARIABLE (SAMPLE_WATER);
      LOCK_VARIABLE (WATER_VOLUME);
      LOCK_VARIABLE (SOUND_VOLUME_VALUES);

#ifdef DOS
      _go32_dpmi_lock_data (HASARD_VALUE, HASARD_SIZE * sizeof (int));
      _go32_dpmi_lock_data (SOUND_VOLUME_VALUES, 17 * sizeof (int));
#endif

      result = install_int_ex (water_handler, MSEC_TO_TIMER (WATER_DELAY));
      water_handler ();
    }
  return result;
}

/*------------------------------------------------------------------*/
void
stop_water (void)
{
  remove_int (water_handler);
}

/*------------------------------------------------------------------*/
static void
my_play_sample (ALLEGRO_SAMPLE * s, int vol)
{
  if (vol > 0)
    play_sample (s, SOUND_VOLUME_VALUES[vol - 1], 128, 1000, 0);
}

/*------------------------------------------------------------------*/
void
play_click (void)
{
  static LW_MUTEX_DATA mutex = { NULL };

  if (LOADED_SFX)
    {
      /*
       * We use a mutex for play_click since playing several clicks
       * together makes no sense. Additionnally, weird bugs (crashes...)
       * under windows when cycling "too fast" within the maps make
       * me think that it could be related to playing too many sounds
       * simultaneously from several threads.
       */
      if (lw_mutex_trylock (&mutex))
        {
          my_play_sample (SAMPLE_SFX_CLICK, CONFIG_CLICK_VOLUME);
          lw_mutex_unlock (&mutex);
        }
      else
        {
          log_println_str ("Warning: too many calls to \"play_click\"...");
        }
    }
}

/*------------------------------------------------------------------*/
void
play_go (void)
{
  if (LOADED_SFX)
    my_play_sample (SAMPLE_SFX_GO, CONFIG_SFX_VOLUME);
}

/*------------------------------------------------------------------*/
void
play_win (void)
{
  if (LOADED_SFX)
    my_play_sample (SAMPLE_SFX_WIN, CONFIG_SFX_VOLUME);
}

/*------------------------------------------------------------------*/
void
play_time (void)
{
  if (LOADED_SFX)
    my_play_sample (SAMPLE_SFX_TIME, CONFIG_SFX_VOLUME);
}

/*------------------------------------------------------------------*/
void
play_loose (void)
{
  if (LOADED_SFX)
    my_play_sample (SAMPLE_SFX_LOOSE, CONFIG_SFX_VOLUME);
}

/*------------------------------------------------------------------*/
void
play_connect (void)
{
  if (LOADED_SFX)
    my_play_sample (SAMPLE_SFX_CONNECT, CONFIG_SFX_VOLUME);
}
