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
/* name          : sockw32.c                                        */
/* content       : simple wrappers on the winsock API               */
/* last update   : April 13th 2001                                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#ifdef WIN32
#include <winsock.h>
#endif
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "sockgen.h"
#include "log.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#define LW_SOCK_NB_BACKLOG               10
#define LW_SOCK_ACCEPT_SEC                1
#define LW_SOCK_ACCEPT_USEC               0

#define LW_DESIRED_WINSOCK_VERSION 0x0101

/*==================================================================*/
/* macros                                                           */
/*==================================================================*/

#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

int LW_SOCK_LOG = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Useless on UNIX, this function is usefull on Microsh*t Windozs
 * where one has to "initialize" the winsock api 8-(
 */
int
lw_sock_init ()
{
  int result = 0;
  int err;
  WSADATA wsadata;

  err = WSAStartup (LW_DESIRED_WINSOCK_VERSION, &wsadata);

  if (err != 0)
    {
      if (LW_SOCK_LOG)
	{
	  log_println_str ("Unable to initialize winsock API");
	}
    }
  else
    {
      result = 1;
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Useless on UNIX.
 */
int
lw_sock_exit ()
{
  int result = 1;

  WSACleanup ();

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Creates, binds and prepares a TCP socket for listening.
 * This is usefull on the server.
 */
int
lw_sock_listen (int *sock, int port)
{
  int result = 0;
  struct sockaddr_in name;
  int enable = 1;

  *sock = socket (AF_INET, SOCK_STREAM, 0);
  if (*sock >= 0)
    {
      setsockopt (*sock, SOL_SOCKET, SO_REUSEADDR,
		  (char *) &enable, sizeof (int));

      name.sin_family = AF_INET;
      name.sin_addr.s_addr = INADDR_ANY;
      name.sin_port = htons ((short) port);
      if (bind (*sock, (struct sockaddr *) &name, sizeof name) >= 0)
	{
	  if (listen ((*sock), LW_SOCK_NB_BACKLOG) >= 0)
	    {
	      result = 1;
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Accepts an incoming connexion from a program which has issued a
 * connect. Usefull on the server.
 */
int
lw_sock_accept (int *new_sock, char *ip, int *port,
		int listening_sock, int wait)
{
  int result = 0;
  struct sockaddr_in name;
  int namelen = sizeof (struct sockaddr_in);
  fd_set read;
  struct timeval tv;
  int res;
  int enable = 1;
  int disable = 0;
  unsigned long disable_ul = 0;
  struct linger li;

  *new_sock = -1;
  ip[0] = '\0';
  *port = 0;

  if (listening_sock >= 0)
    {
      FD_ZERO (&read);
      FD_SET (listening_sock, &read);
      if (wait)
	{
	  tv.tv_sec = LW_SOCK_ACCEPT_SEC;
	  tv.tv_usec = LW_SOCK_ACCEPT_USEC;
	}
      else
	{
	  tv.tv_sec = 0;
	  tv.tv_usec = 0;
	}
      res = select (listening_sock + 1, &read, NULL, NULL, &tv);
      if (res >= 1)
	{
	  (*new_sock) =
	    accept (listening_sock, (struct sockaddr *) &name, &namelen);
	  if ((*new_sock) >= 0)
	    {
	      li.l_onoff = 0;
	      li.l_linger = 0;
	      setsockopt (*new_sock, SOL_SOCKET, SO_KEEPALIVE,
			  (char *) &enable, sizeof (int));
	      setsockopt (*new_sock, SOL_SOCKET, SO_OOBINLINE,
			  (char *) &disable, sizeof (int));
	      setsockopt (*new_sock, SOL_SOCKET, SO_LINGER,
			  (char *) &li, sizeof (struct linger));

	      ioctlsocket (*new_sock, FIONBIO, &disable_ul);

	      strncpy (ip, inet_ntoa (name.sin_addr), LW_SOCK_IP_SIZE - 1);
	      ip[LW_SOCK_IP_SIZE - 1] = 0;
	      (*port) = (int) ntohs (name.sin_port);

	      result = 1;
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Establishes a connection. The remote must have called sock_listen
 * first and must confirm the acceptation with a sock_accept.
 * Usefull on the client. 
 */
int
lw_sock_connect (int *sock, char *ip, int port)
{
  int result = 0;
  struct sockaddr_in name;
  int enable = 1;
  int disable = 0;
  unsigned long disable_ul = 0;
  struct linger li;

  *sock = socket (AF_INET, SOCK_STREAM, 0);
  if (*sock >= 0)
    {
      name.sin_family = AF_INET;
      name.sin_addr.s_addr = INADDR_ANY;
      name.sin_port = 0;
      if (bind (*sock, (struct sockaddr *) &name, sizeof name) >= 0)
	{
	  name.sin_family = AF_INET;
	  name.sin_addr.s_addr = inet_addr (ip);
	  name.sin_port = htons ((short) port);
	  if (connect ((*sock), (struct sockaddr *) &name, sizeof name) >= 0)
	    {
	      /*
	       * Added this code copied/paste from accept.
	       * don'tknow if it's usefull
	       */
	      li.l_onoff = 0;
	      li.l_linger = 0;
	      setsockopt (*sock, SOL_SOCKET, SO_KEEPALIVE,
			  (char *) &enable, sizeof (int));
	      setsockopt (*sock, SOL_SOCKET, SO_OOBINLINE,
			  (char *) &disable, sizeof (int));
	      setsockopt (*sock, SOL_SOCKET, SO_LINGER,
			  (char *) &li, sizeof (struct linger));

	      ioctlsocket (*sock, FIONBIO, &disable_ul);

	      result = 1;
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Closes a socket for good.
 */
int
lw_sock_close (int *sock)
{
  int result = 0;
  int sock_to_close;

  /*
   * We set *sock to 0 before we close it. This should reduce 
   * thread problems
   */
  sock_to_close = *sock;
  (*sock) = -1;

  if (shutdown (sock_to_close, 2) != 0)
    {
      /*
       * We could print a warning here.
       */
    }

  if (closesocket (sock_to_close) != 0)
    {
      /*
       * Strange error, was the socket really opened?
       */
    }
  else
    {
      result = 1;
    }

  return result;
}
