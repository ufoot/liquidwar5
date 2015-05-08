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
/* name          : sockdos.c                                        */
/* content       : fake DOS network support to avoid link failure   */
/* last update   : April 13th 2001                                  */
/********************************************************************/

/*
 * This file contains empty functions even if DOS releases of LW
 * have no network support. It permits safe linking.
 */

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

int LW_SOCK_LOG = 0;

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
lw_sock_init ()
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_exit ()
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_listen (int *sock, char *ip, int port)
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_accept (int *new_sock, char *ip, int *port, int listening_sock)
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_connect (int *sock, char *ip, int port)
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_peek_ex (int sock, int len)
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_send_str_ex (int sock, char *str)
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_recv_str_ex (int sock, char *str)
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_send_buffer_ex (int sock, char *buffer, int len)
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_recv_buffer_ex (int sock, char *buffer, int len)
{
  return 0;
}

/*------------------------------------------------------------------*/
int
lw_sock_close (int *sock)
{
  return 0;
}
