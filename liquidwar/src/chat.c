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
/* Copyright (C) 1998-2016 Christian Mauduit                                 */
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
/* name          : chat.c                                           */
/* content       : functions to handle chat messages                */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "chat.h"

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Resets the LW_CHAT_HISTORY struct
 */
void
lw_chat_reset (LW_CHAT_HISTORY * history)
{
  memset (history, 0, sizeof (LW_CHAT_HISTORY));
}

/*------------------------------------------------------------------*/
/*
 * Pushes a message in the message list
 */
void
lw_chat_push (LW_CHAT_HISTORY * history, char *author, char *content)
{
  LW_CHAT_MESSAGE temp;

  /* 
   * We remove old messages if there's not enough room
   */
  while (history->n >= LW_CHAT_HISTORY_SIZE)
    {
      lw_chat_pop (history, &temp);
    }

  strncpy (history->messages[history->n].author, author, NAME_SIZE);
  history->messages[history->n].author[NAME_SIZE] = '\0';
  strncpy (history->messages[history->n].content, content,
           LW_CHAT_MESSAGE_SIZE);
  history->messages[history->n].content[LW_CHAT_MESSAGE_SIZE] = '\0';

  history->n++;
}

/*------------------------------------------------------------------*/
/*
 * Gets and removes a message from the message list (FIFO style)
 */
int
lw_chat_pop (LW_CHAT_HISTORY * history, LW_CHAT_MESSAGE * message)
{
  int result = 0;
  int i;

  memset (message, 0, sizeof (LW_CHAT_MESSAGE));

  if (history->n > 0)
    {
      memcpy (message, &(history->messages[0]), sizeof (LW_CHAT_MESSAGE));

      for (i = 1; i < history->n; ++i)
        {
          memcpy (&(history->messages[i - 1]),
                  &(history->messages[i]), sizeof (LW_CHAT_MESSAGE));
        }

      history->n--;

      result = 1;
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Gets a message from the message list without removing it
 */
int
lw_chat_get (LW_CHAT_HISTORY * history, LW_CHAT_MESSAGE * message, int i)
{
  int result = 0;

  memset (message, 0, sizeof (LW_CHAT_MESSAGE));

  if (i >= 0 && i < history->n)
    {
      memcpy (message, &(history->messages[i]), sizeof (LW_CHAT_MESSAGE));

      result = 1;
    }

  return result;
}
