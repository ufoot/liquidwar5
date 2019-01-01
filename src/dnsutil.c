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
/* Copyright (C) 1998-2019 Christian Mauduit                                 */
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
/* name          : dnsutil.c                                        */
/* content       : low level functions to handle dns requests       */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#ifdef WIN32
#include <winsock.h>
#endif
#ifdef UNIX
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <string.h>
#include <stdio.h>

#include "dnsutil.h"
#include "log.h"
#include "sockgen.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

/*==================================================================*/
/* global functions                                                 */
/*==================================================================*/

int
lw_dnsutil_name_to_ip (char *ip, char *name)
{
  int result = 0;
#ifdef DOS
  memset (ip, 0, LW_SOCK_IP_SIZE);
#else
  struct hostent *h;
  struct in_addr addr;

  memset (ip, 0, LW_SOCK_IP_SIZE);
  h = gethostbyname (name);
  if (h && h->h_addrtype == AF_INET && h->h_length >= 4 && h->h_addr_list[0])
    {
#ifdef UNIX
      addr = *((struct in_addr *) (h->h_addr_list[0]));
#else
      addr.s_addr = *((u_long *) (h->h_addr_list[0]));
#endif

      strncpy (ip, inet_ntoa (addr), LW_SOCK_IP_SIZE - 1);
      ip[LW_SOCK_IP_SIZE - 1] = '\0';

      result = 1;
    }
#endif
  return result;
}
