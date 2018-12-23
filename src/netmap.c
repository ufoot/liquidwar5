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
/* Copyright (C) 1998-2018 Christian Mauduit                                 */
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
/* name          : netmap.c                                         */
/* content       : functions to transmit maps over the network      */
/* last update   : June 30th 2001                                   */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <string.h>

#include "netmap.h"
#include "log.h"
#include "sock2gen.h"
#include "serial.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_NETMAP_CHUNK_SIZE 64

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

/*==================================================================*/
/* static functions declaration                                     */
/*==================================================================*/

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Sends a map over the network
 */
int
lw_netmap_send (int *sock, void *netmap)
{
  int result = 0;
  char *ptr;
  int size;
  int sent;
  int to_send;

  ptr = (char *) netmap;
  if (lw_sock_send_buffer (sock, ptr, 8))
    {
      if (lw_sock_send_buffer (sock, ptr + 8, LW_MAP_SYSTEM_NAME_SIZE))
        {
          if (lw_sock_send_buffer
              (sock, ptr + 8 + LW_MAP_SYSTEM_NAME_SIZE,
               LW_MAP_READABLE_NAME_SIZE))
            {
              lw_serial_get_map_header (netmap, &size, NULL, NULL);

              result = 1;
              sent = 0;
              while (sent < size && result)
                {
                  to_send = size - sent;
                  if (to_send > LW_NETMAP_CHUNK_SIZE)
                    {
                      to_send = LW_NETMAP_CHUNK_SIZE;
                    }
                  if (lw_sock_send_buffer
                      (sock,
                       ptr + 8 + LW_MAP_SYSTEM_NAME_SIZE +
                       LW_MAP_READABLE_NAME_SIZE + sent, to_send))
                    {
                      sent += to_send;
                    }
                  else
                    {
                      result = 0;
                    }
                }
            }
        }
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Receives a map from the network
 */
int
lw_netmap_recv (int *sock, void **netmap)
{
  int result = 0;
  char buffer[8 + LW_MAP_SYSTEM_NAME_SIZE + LW_MAP_READABLE_NAME_SIZE];
  int size;
  short w, h;
  char *ptr;
  int received;
  int to_receive;

  ptr = (char *) (*netmap);
  if (ptr != NULL)
    {
      free (ptr);
    }
  ptr = NULL;

  if (lw_sock_recv_buffer (sock, buffer, 8))
    {
      if (lw_sock_recv_buffer (sock, buffer + 8, LW_MAP_SYSTEM_NAME_SIZE))
        {
          if (lw_sock_recv_buffer
              (sock, buffer + 8 + LW_MAP_SYSTEM_NAME_SIZE,
               LW_MAP_READABLE_NAME_SIZE))
            {
              lw_serial_get_map_header ((void *) buffer, &size, &w, &h);

              /*
               * A map under 3 bytes is reasonably not a "real" map...
               */
              if (size >= 10)
                {
                  ptr =
                    malloc (size + 8 + LW_MAP_SYSTEM_NAME_SIZE +
                            LW_MAP_READABLE_NAME_SIZE);
                  if (ptr != NULL)
                    {
                      memcpy (ptr, buffer,
                              8 + LW_MAP_SYSTEM_NAME_SIZE +
                              LW_MAP_READABLE_NAME_SIZE);

                      result = 1;
                      received = 0;
                      while (received < size && result)
                        {
                          to_receive = size - received;
                          if (to_receive > LW_NETMAP_CHUNK_SIZE)
                            {
                              to_receive = LW_NETMAP_CHUNK_SIZE;
                            }

                          if (lw_sock_recv_buffer
                              (sock,
                               ptr + 8 + LW_MAP_SYSTEM_NAME_SIZE +
                               LW_MAP_READABLE_NAME_SIZE + received,
                               to_receive))
                            {
                              received += to_receive;
                            }
                          else
                            {
                              result = 0;
                            }
                        }
                    }
                }
            }
        }
    }

  (*netmap) = (void *) ptr;

  return result;
}
