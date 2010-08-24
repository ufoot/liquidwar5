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
/* name          : chat.h                                           */
/* content       : functions to handle chat messages                */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_CHAT
#define LIQUID_WAR_INCLUDE_CHAT

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "base.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#define LW_CHAT_MESSAGE_SIZE 80
#define LW_CHAT_HISTORY_SIZE 20

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/

typedef struct
{
  char author[NAME_SIZE + 1];
  char content[LW_CHAT_MESSAGE_SIZE + 1];
}
LW_CHAT_MESSAGE;

typedef struct
{
  int n;
  LW_CHAT_MESSAGE messages[LW_CHAT_HISTORY_SIZE];
}
LW_CHAT_HISTORY;

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void lw_chat_reset (LW_CHAT_HISTORY * history);
void lw_chat_push (LW_CHAT_HISTORY * history, char *author, char *content);
int lw_chat_pop (LW_CHAT_HISTORY * history, LW_CHAT_MESSAGE * message);
int lw_chat_get (LW_CHAT_HISTORY * history, LW_CHAT_MESSAGE * message, int i);


#endif
