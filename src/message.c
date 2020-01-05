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
/* nom           : message.c                                        */
/* contenu       : gestion des messages pendant le jeu              */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>
#include <stdio.h>
#include <string.h>

#include "message.h"
#include "ticker.h"
#include "viewport.h"
#include "macro.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define MESSAGE_MAX 12
#define MESSAGE_SIZE 64

static int MESSAGE_NB = 0;
static char MESSAGE[MESSAGE_MAX][MESSAGE_SIZE + 1];
static int MESSAGE_TICKER[MESSAGE_MAX];
static int MESSAGE_DISPLAYED[MESSAGE_MAX];
static int MESSAGE_DELAY = 1000;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
clear_message (void)
{
  MESSAGE_NB = 0;
}

/*------------------------------------------------------------------*/
void
display_message (void)
{
  int i, h, j, new_ticker;

  new_ticker = get_ticker ();
  for (i = 0; i < MESSAGE_NB; ++i)
    if (!MESSAGE_DISPLAYED[i])
      {
        MESSAGE_DISPLAYED[i] = 1;
        MESSAGE_TICKER[i] = new_ticker + MESSAGE_DELAY * (i + 1);
      }

  while (MESSAGE_NB > 0
         && MESSAGE_DISPLAYED[0] && new_ticker > MESSAGE_TICKER[0])
    {
      for (i = 0; i < MESSAGE_NB - 1; ++i)
        for (j = 0; j < MESSAGE_SIZE + 1; ++j)
          {
            MESSAGE[i][j] = MESSAGE[i + 1][j];
            MESSAGE_TICKER[i] = MESSAGE_TICKER[i + 1];
          }
      MESSAGE_NB--;
    }

  h = text_height (font);
  //text_mode (-1); deprecated
  for (i = 0; i < MESSAGE_NB; ++i)
    textout_ex (NEXT_SCREEN, font, MESSAGE[i],
                NEXT_SCREEN->w - text_length (font, MESSAGE[i]),
                NEXT_SCREEN->h - (MESSAGE_NB - i) * h, -1, -1);
}

/*------------------------------------------------------------------*/
void
message_str (char *str)
{
  int i;

  while (MESSAGE_NB >= MESSAGE_MAX)
    {
      for (i = 0; i < MESSAGE_NB - 1; ++i)
        {
          LW_MACRO_STRCPY (MESSAGE[i], MESSAGE[i + 1]);
          MESSAGE_TICKER[i] = MESSAGE_TICKER[i + 1] - MESSAGE_DELAY;
        }
      MESSAGE_NB--;
    }

  MESSAGE_DISPLAYED[MESSAGE_NB] = 0;
  LW_MACRO_STRCPY (MESSAGE[MESSAGE_NB], str);

  MESSAGE_NB++;
}

/*------------------------------------------------------------------*/
void
message_int (int val)
{
  static char buffer[33 + 1];

  LW_MACRO_SPRINTF1 (buffer, "%d", val);
  message_str (buffer);
}

/*------------------------------------------------------------------*/
void
message_str_int (char *str, int val)
{
  static char buffer[MESSAGE_SIZE + 33 + 1];

  LW_MACRO_SPRINTF2 (buffer, "%s%d", str, val);
  message_str (buffer);
}
