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
/* name          : sock2cli.c                                       */
/* content       : wrapper functions used on the client             */
/* last update   : July  2md 2001                                   */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "sock2gen.h"
#include "ticker.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#ifdef WIN32
/*
 * There are some problems with network under Windows, I suspect this
 * is due to the fact W98 has some poor support network, and for some
 * unexplained reasons map send/recv goes very slow, even on a LAN.
 * So I set a higher value to the client timeout, hoping it will solve
 * the problem.
 */
#define LW_SOCK2CLI_TIMEOUT_MSEC        30000
#else
/*
 * there's a 12 sec timeout by default on the client
 */
#define LW_SOCK2CLI_TIMEOUT_MSEC        15000
#endif

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
lw_sock_peek (int *sock, int len)
{
  int result = 0;

  if (*sock >= 0)
    {
      result = lw_sock_peek_ex (sock, len);
    }

  return result;
}

/*------------------------------------------------------------------*/
int
lw_sock_send_str (int *sock, char *str)
{
  int result = 0;
  int limit_ticker;

  /*
   * Now when allegro is running all standard sleep() or select()
   * UNIX functions do not work because allegro's timer messes
   * things up. Therefore we do our own dirty ugly select...
   */
  limit_ticker = get_ticker () + LW_SOCK2CLI_TIMEOUT_MSEC;
  while (limit_ticker > get_ticker () && !result && *sock >= 0)
    {
      if (lw_sock_send_str_ex (sock, str))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
int
lw_sock_recv_str (int *sock, char *str)
{
  int result = 0;
  int limit_ticker;

  /*
   * Now when allegro is running all standard sleep() or select()
   * UNIX functions do not work because allegro's timer messes
   * things up. Therefore we do our own dirty ugly select...
   */
  limit_ticker = get_ticker () + LW_SOCK2CLI_TIMEOUT_MSEC;
  while (limit_ticker > get_ticker () && !result && *sock >= 0)
    {
      if (lw_sock_recv_str_ex (sock, str))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
int
lw_sock_send_buffer (int *sock, char *buffer, int len)
{
  int result = 0;
  int limit_ticker;

  /*
   * Now when allegro is running all standard sleep() or select()
   * UNIX functions do not work because allegro's timer messes
   * things up. Therefore we do our own dirty ugly select...
   */
  limit_ticker = get_ticker () + LW_SOCK2CLI_TIMEOUT_MSEC;
  while (limit_ticker > get_ticker () && !result && *sock >= 0)
    {
      if (lw_sock_send_buffer_ex (sock, buffer, len))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
int
lw_sock_recv_buffer (int *sock, char *buffer, int len)
{
  int result = 0;
  int limit_ticker;

  /*
   * Now when allegro is running all standard sleep() or select()
   * UNIX functions do not work because allegro's timer messes
   * things up. Therefore we do our own dirty ugly select...
   */
  limit_ticker = get_ticker () + LW_SOCK2CLI_TIMEOUT_MSEC;
  while (limit_ticker > get_ticker () && !result && *sock >= 0)
    {
      if (lw_sock_peek_ex (sock, len))
	{
	  if (lw_sock_recv_buffer_ex (sock, buffer, len))
	    {
	      result = 1;
	    }
	}
    }

  return result;
}
