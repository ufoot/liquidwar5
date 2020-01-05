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
/* name          : netkey.h                                         */
/* content       : functions to manipulate network key messages     */
/* last update   : June 20th 2001                                   */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_NETKEY
#define LIQUID_WAR_INCLUDE_NETKEY

#include "base.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_NETKEY_CMD_NONE          'n'
#define LW_NETKEY_CMD_QUIT          'q'
#define LW_NETKEY_CMD_INCREASE_LAG  '+'
#define LW_NETKEY_CMD_DECREASE_LAG  '-'

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/

/*
 * This structure is used to store the information about keys strokes
 * sent over the network. The structure contains the keys plus
 * 2 special fields: a command and a parameter.
 */
typedef struct
{
  int key_states[NB_TEAMS];     /* the keys states as in the cursor */
  int cmd;                      /* a special command                */
  int arg;                      /* an argument for the command      */
  unsigned int checksum;
}
LW_NETKEY;

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

void lw_netkey_reset (LW_NETKEY * netkey);
int lw_netkey_send (int *sock, LW_NETKEY * netkey);
int lw_netkey_recv (int *sock, LW_NETKEY * netkey);
int lw_netkey_peek (int *sock, int num);

#endif
