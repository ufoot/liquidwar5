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
/* Copyright (C) 1998-2013 Christian Mauduit                                 */
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
/* name          : keyexch.c                                        */
/* content       : code to exchange key presses with the server     */
/* last update   : June 24th  2001                                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "netkey.h"
#include "keyexch.h"
#include "log.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#define LW_KEYEXCH_LAG_LOWER_LIMIT 2
#define LW_KEYEXCH_LAG_UPPER_LIMIT 5

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int LW_KEYEXCH_SOCK = -1;

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static int send_keys (int *sock, CURSOR * cursors, unsigned int checksum);
static int recv_keys (int *sock, CURSOR * cursors);

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static int
send_keys (int *sock, CURSOR * cursors, unsigned int checksum)
{
  int result = 0;
  int i;
  LW_NETKEY netkey;

  lw_netkey_reset (&netkey);

  for (i = 0; i < NB_TEAMS; ++i)
    {
      /*
       * We send informations about cursors who are not network
       * controlled only.
       */
      if (!cursors[i].from_network)
	{
	  netkey.key_states[i] = cursors[i].key_state;
	}
    }

  /*
   * If there are not enough keys in the buffer, we ask the server
   * to increase the lag
   */
  if (!lw_netkey_peek (sock, LW_KEYEXCH_LAG_LOWER_LIMIT))
    {
      netkey.cmd = LW_NETKEY_CMD_INCREASE_LAG;
    }

  /*
   * If there are not enough keys in the buffer, we ask the server
   * to increase the lag
   */
  if (lw_netkey_peek (sock, LW_KEYEXCH_LAG_UPPER_LIMIT))
    {
      netkey.cmd = LW_NETKEY_CMD_DECREASE_LAG;
    }

  /*
   * Checksum which will help the server to control things and
   * detect consistency problems.
   */
  netkey.checksum = checksum;

  result = lw_netkey_send (sock, &netkey);

  return result;
}

/*------------------------------------------------------------------*/
static int
recv_keys (int *sock, CURSOR * cursors)
{
  int result = 0;
  int i;
  LW_NETKEY netkey;

  result = lw_netkey_recv (sock, &netkey);

  for (i = 0; i < NB_TEAMS; ++i)
    {
      /*
       * The informations is retrieved from the network for
       * _every_ cursor_ , including those who are not network
       * managed. In fact, we just wait for the server to send
       * back the information we have sent to him a few game
       * cycles before.
       */
      cursors[i].key_state = netkey.key_states[i];
    }

  if (result)
    {
      /*
       * Now we check for checksum errors
       */
      if (!netkey.checksum)
	{
	  log_println_str
	    ("Error: incorrect checksum, the game is broken, or someone is cheating");
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
int
lw_keyexch_flush (CURSOR * cursors, unsigned int checksum)
{
  int result = 1;

  /*
   * It's important to send keys before we receive them
   */
  result = result && send_keys (&LW_KEYEXCH_SOCK, cursors, checksum);
  result = result && recv_keys (&LW_KEYEXCH_SOCK, cursors);

  return result;
}
