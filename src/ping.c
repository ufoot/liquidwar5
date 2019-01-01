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
/* name    : ping.c                                                 */
/* content : utility to calculate an approximate ping time          */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>

#include "ping.h"
#include "thrdgen.h"
#include "ticker.h"
#include "macro.h"
#include "sockgen.h"
#include "protocol.h"
#include "log.h"
#include "sockgen.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

/*
 * Delay in msec used for polling loops
 */
#define LW_PING_DELAY      10

/*
 * Maximum delay for a ping test, here it's of 750 msec for the
 * whole thing, ie connect + ping
 */
#define LW_PING_TEST_MAX 750

/*
 * Number of pings we'll use to calibrate
 */
#define LW_PING_NB_RETRIES 3

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/

typedef struct
{
  int sock;
  char address[LW_SOCK_IP_SIZE];
  int port;
  int allow_exit;
  int done;
  int result;
}
LW_PING_TRY_DATA;

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static void try_server_callback (void *arg);

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Tries to connect on a server and get an approximate ping time.
 * The answer is given in msec. If -1, means that the server could
 * not be reached.
 */
int
lw_ping_try_server (char *address, int port)
{
  int result = -1;
  LW_PING_TRY_DATA *try_data;
  int start_time = 0;

  try_data = (LW_PING_TRY_DATA *) malloc (sizeof (LW_PING_TRY_DATA));
  if (try_data != NULL)
    {
      memset (try_data, 0, sizeof (LW_PING_TRY_DATA));

      try_data->sock = -1;
      LW_MACRO_STRCPY (try_data->address, address);
      try_data->port = port;
      try_data->allow_exit = 0;
      try_data->result = -1;

      if (lw_thread_start (try_server_callback, try_data))
        {
          start_time = get_ticker ();
          while (try_data->done == 0 &&
                 get_ticker () < start_time + LW_PING_TEST_MAX)
            {
              rest (LW_PING_DELAY);
            }
          result = try_data->result;
          try_data->allow_exit = 1;
        }
      else
        {
          /*
           * The thread could not be launched...
           */
          free (try_data);
        }
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Callback used by the thread launched by lw_ping_try_server
 * to open the TCP/IP connection with the server and evaluate
 * an approximate ping time.
 */
static void
try_server_callback (void *arg)
{
  int result = 0;
  LW_PING_TRY_DATA *try_data;
  int start_time;
  int global_start_time;
  int delay[LW_PING_NB_RETRIES];
  int i, n, sum;

  try_data = (LW_PING_TRY_DATA *) arg;

  for (i = 0; i < LW_PING_NB_RETRIES; ++i)
    {
      delay[i] = 0;
    }

  result = lw_sock_connect (&(try_data->sock),
                            try_data->address, try_data->port);

  if (result == 1)
    {
      /*
       * OK we got connected on the server, now we time stuff.
       */
      n = 0;
      global_start_time = get_ticker ();
      for (i = 0; result == 1 &&
           i < LW_PING_NB_RETRIES &&
           get_ticker () < global_start_time + LW_PING_TEST_MAX; ++i)
        {
          start_time = get_ticker ();
          result = lw_protocol_do_ping (&try_data->sock);
          if (result == 1)
            {
              delay[i] = get_ticker () - start_time;
              n = i + 1;
            }

          /*
           * We calculate an average ping from all the values we have
           */
          sum = 0;
          for (i = 0; i < n; ++i)
            {
              sum += delay[i];
            }
          if (n > 0)
            {
              try_data->result = sum / n;
            }
        }

      /*
       * OK we're done the calling thread can abort
       */
      try_data->done = 1;

      /*
       * We're polite, we tell the server we quit.
       */
      if (result)
        {
          lw_protocol_quit (&try_data->sock);
        }
      lw_sock_close (&try_data->sock);
    }

  while (!try_data->allow_exit)
    {
      /*
       * We simply wait until the calling
       * thread has the results it's waiting for.
       */
      rest (LW_PING_DELAY);
    }

  free (try_data);
}
