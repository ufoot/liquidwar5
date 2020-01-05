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
/* name          : netkey.c                                         */
/* content       : functions to manipulate network key messages     */
/* last update   : June 20th 2001                                   */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "netkey.h"
#include "cursor.h"
#include "sock2gen.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_NETKEY_SERIALIZED_SIZE (NB_TEAMS+2+4)

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

/*
 * Used to store the information of a LW_NETKEY struct once it has
 * been serialized to be sent over the network
 */
typedef unsigned char LW_NETKEY_SERIALIZED[LW_NETKEY_SERIALIZED_SIZE];

/*==================================================================*/
/* static functions declaration                                     */
/*==================================================================*/

static char int_to_ascii_hexa (int src);
static int ascii_hexa_to_int (char src);
static void encode (LW_NETKEY_SERIALIZED dst, LW_NETKEY * src);
static void decode (LW_NETKEY * dst, LW_NETKEY_SERIALIZED src);

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Creates a blank netkey struct
 */
void
lw_netkey_reset (LW_NETKEY * netkey)
{
  int i;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      netkey->key_states[i] = 0;
    }

  netkey->cmd = LW_NETKEY_CMD_NONE;
  netkey->arg = 0;
}

/*------------------------------------------------------------------*/
/*
 * Transforms an integer into a readable hexadecimal ascii character
 */
static char
int_to_ascii_hexa (int src)
{
  char dst;

  dst = '0';
  if (src >= 1 && src <= 9)
    {
      dst = '0' + src;
    }
  if (src >= 10 && src <= 15)
    {
      dst = 'A' - 10 + src;
    }

  return dst;
}

/*------------------------------------------------------------------*/
/*
 * Transforms a readable hexadecimal ascii character into an integer
 */
static int
ascii_hexa_to_int (char src)
{
  int dst;

  dst = 0;
  if (src >= '0' && src <= '9')
    {
      dst = src - '0';
    }
  if (src >= 'A' && src <= 'F')
    {
      dst = src - 'A' + 10;
    }

  return dst;
}

/*------------------------------------------------------------------*/
/*
 * Serializes a LW_NETKEY struct to a buffer which is sendable over
 * the network.
 */
static void
encode (LW_NETKEY_SERIALIZED dst, LW_NETKEY * src)
{
  int i;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      /*
       * we translate each key press (stored as a logical or between
       * 1,2,4 and 8) into a readable hexadecimal character. This
       * makes debugging easier.
       */
      dst[i] = int_to_ascii_hexa (src->key_states[i]);
    }

  /*
   * We store the command as is since it's already a character
   */
  dst[NB_TEAMS] = src->cmd;
  /*
   * We transform the argument since we assume it's a numeric arg
   */
  dst[NB_TEAMS + 1] = int_to_ascii_hexa (src->arg);

  /*
   * Now we serialize the "map checksum"
   */
  dst[NB_TEAMS + 2 + 0] = (src->checksum) & 0xFF;
  dst[NB_TEAMS + 2 + 1] = (src->checksum >> 8) & 0xFF;
  dst[NB_TEAMS + 2 + 2] = (src->checksum >> 16) & 0xFF;
  dst[NB_TEAMS + 2 + 3] = (src->checksum >> 24) & 0xFF;
}

/*------------------------------------------------------------------*/
/*
 * Un-serializes a LW_NETKEY struct from a buffer which has been
 * received from the network.
 */
static void
decode (LW_NETKEY * dst, LW_NETKEY_SERIALIZED src)
{
  int i;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      /*
       * we translate each key press (stored as a logical or between
       * 1,2,4 and 8) from a readable hexadecimal character. This
       * makes debugging easier.
       */
      dst->key_states[i] = ascii_hexa_to_int (src[i]);
    }

  /*
   * We store the command as is since it's already a character
   */
  dst->cmd = src[NB_TEAMS];
  /*
   * We transform the argument since we assume it's a numeric arg
   */
  dst->arg = ascii_hexa_to_int (src[NB_TEAMS + 1]);

  /*
   * Now we serialize the "map checksum"
   */
  dst->checksum = ((unsigned int) src[NB_TEAMS + 2 + 3]) << 24
    | ((unsigned int) src[NB_TEAMS + 2 + 2]) << 16
    | ((unsigned int) src[NB_TEAMS + 2 + 1]) << 8
    | ((unsigned int) src[NB_TEAMS + 2 + 0]);
}

/*------------------------------------------------------------------*/
/*
 * Sends a netkey information over the network
 */
int
lw_netkey_send (int *sock, LW_NETKEY * netkey)
{
  int result;
  LW_NETKEY_SERIALIZED netkey_serialized;

  encode (netkey_serialized, netkey);

  result = lw_sock_send_buffer (sock,
                                (char *) netkey_serialized,
                                LW_NETKEY_SERIALIZED_SIZE);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Receives a netkey information from the network
 */
int
lw_netkey_recv (int *sock, LW_NETKEY * netkey)
{
  int result;
  LW_NETKEY_SERIALIZED netkey_serialized;

  result = lw_sock_recv_buffer (sock,
                                (char *) netkey_serialized,
                                LW_NETKEY_SERIALIZED_SIZE);

  decode (netkey, netkey_serialized);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns true if at leat "num" keys are available on the socket
 */
int
lw_netkey_peek (int *sock, int num)
{
  int result;

  result = lw_sock_peek (sock, num * LW_NETKEY_SERIALIZED_SIZE);

  return result;
}
