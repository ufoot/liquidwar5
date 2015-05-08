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
/* Copyright (C) 1998-2015 Christian Mauduit                                 */
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
/* nom           : watchdog.c                                       */
/* contenu       : gestion des codes secrets pendant le jeu         */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>
#include <string.h>

#include "watchdog.h"
#include "log.h"
#include "macro.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define WATCHDOG_NUMBER 10
#define WATCHDOG_BUFFER_SIZE 1024
#define WATCHDOG_MAX_WORD 64
#define WATCHDOG_KEYWORD "ufoot"

char WATCHDOG_ASCII[256];
char WATCHDOG_SCANCODE[128];
static int WATCHDOG_BUFFER[WATCHDOG_BUFFER_SIZE];
static int WATCHDOG_KEYS_IN_BUFFER;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
watchdog_update ()
{
  int i, n, old_pos;

  old_pos = WATCHDOG_KEYS_IN_BUFFER;

  for (i = 0; i < 256; ++i)
    WATCHDOG_ASCII[i] = 0;
  for (i = 0; i < 128; ++i)
    WATCHDOG_SCANCODE[i] = 0;

  if ((n = (WATCHDOG_KEYS_IN_BUFFER - WATCHDOG_MAX_WORD)) > WATCHDOG_MAX_WORD)
    {
      for (i = n; i < WATCHDOG_KEYS_IN_BUFFER; ++i)
	WATCHDOG_BUFFER[i - n] = WATCHDOG_BUFFER[i];
      WATCHDOG_KEYS_IN_BUFFER -= n;
      old_pos -= n;
    }

  while (keypressed () && WATCHDOG_KEYS_IN_BUFFER < WATCHDOG_BUFFER_SIZE)
    {
      WATCHDOG_BUFFER[WATCHDOG_KEYS_IN_BUFFER++] = readkey ();
    }

  for (i = old_pos; i < WATCHDOG_KEYS_IN_BUFFER; ++i)
    {
      WATCHDOG_ASCII[WATCHDOG_BUFFER[i] & 0xFF] = 1;
      WATCHDOG_SCANCODE[WATCHDOG_BUFFER[i] >> 8] = 1;
    }
}

/*------------------------------------------------------------------*/
void
watchdog_reset ()
{
  WATCHDOG_KEYS_IN_BUFFER = 0;

  watchdog_update ();
}

/*------------------------------------------------------------------*/
int
watchdog_is_keyword (char *keyword)
{
  int i, j, l, n, found = 0;
  char pattern[WATCHDOG_MAX_WORD + 1];

  LW_MACRO_SPRINTF2 (pattern, "%s%s", WATCHDOG_KEYWORD, keyword);

  l = strlen (pattern);
  n = WATCHDOG_KEYS_IN_BUFFER - l + 1;
  for (i = 0; i < n && !found; ++i)
    {
      j = 0;
      while (pattern[j] &&
	     pattern[j] == ((char) WATCHDOG_BUFFER[i + j] & 0xFF))
	{
	  j++;
	}
      found = !pattern[j];
    }

  if (found)
    WATCHDOG_BUFFER[i] |= 0xFF;

  return found;
}
