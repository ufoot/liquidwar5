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
/* nom           : time.c                                           */
/* contenu       : gestion du temps de jeu                          */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "code.h"
#include "config.h"
#include "sound.h"
#include "ticker.h"
#include "time.h"

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

static int TIME_TABLE[17] = { 15, 30, 45, 60,
  90, 120, 150, 180,
  240, 300, 360, 480,
  600, 900, 1200, 1800,
  3600
};

static int TICKER_PAUSED;
static int TICKER_START;
static int TICKER_PAUSE_START;
static int BELL_RINGING;

int TIME_ELAPSED;
int TIME_LEFT;
int GLOBAL_TICKER;
int GLOBAL_CLOCK = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
update_play_time (void)
{
  GLOBAL_TICKER = get_ticker () - TICKER_START;
  if (PAUSE_ON)
    {
      if (TICKER_PAUSE_START == 0)
        TICKER_PAUSE_START = get_ticker ();
    }
  else
    {
      if (TICKER_PAUSE_START != 0)
        {
          TICKER_PAUSED += get_ticker () - TICKER_PAUSE_START;
          TICKER_PAUSE_START = 0;
        }
      else
        TIME_ELAPSED = (get_ticker () - TICKER_START - TICKER_PAUSED) / 1000;
    }
  TIME_LEFT = TIME_TABLE[LW_CONFIG_CURRENT_RULES.game_time] - TIME_ELAPSED;

  if (TIME_LEFT < 0)
    TIME_LEFT = 0;
  if (TIME_LEFT <= 5 && BELL_RINGING == 0)
    {
      BELL_RINGING = 1;
      play_time ();
    }
}

/*------------------------------------------------------------------*/
void
start_play_time (void)
{
  GLOBAL_CLOCK = 2;
  TICKER_START = get_ticker ();
  TICKER_PAUSED = 0;
  TICKER_PAUSE_START = 0;
  TIME_ELAPSED = 0;
  TIME_LEFT = TIME_TABLE[LW_CONFIG_CURRENT_RULES.game_time] + 1;
  BELL_RINGING = 0;
  update_play_time ();
}

/*------------------------------------------------------------------*/
void
write_time (int time, char *buffer)
{
  int min, sec;

  min = time / 60;
  sec = time % 60;
  buffer[0] = '0' + min / 10;
  buffer[1] = '0' + min % 10;
  buffer[2] = ':';
  buffer[3] = '0' + sec / 10;
  buffer[4] = '0' + sec % 10;
  buffer[5] = 0;
}
