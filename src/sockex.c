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
/* name          : sockw32.c                                        */
/* content       : simple wrappers on the winsock API               */
/* last update   : April 13th 2001                                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#ifdef WIN32
#include <winsock.h>
#else
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#endif

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "macro.h"
#include "sockgen.h"
#include "log.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#define LW_SOCK_RECV_SEC                  8
#define LW_SOCK_RECV_USEC                 0
#define LW_SOCK_SEND_SEC                  8
#define LW_SOCK_SEND_USEC                 0

#define LW_SOCK_SEND_BUFFER_SIZE        100
#define LW_SOCK_RECV_BUFFER_SIZE        200

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

/*==================================================================*/
/* static fonctions                                                 */
/*==================================================================*/

static void clean_buffer (char *buffer, int len);

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Cleans up a message buffer from all characters which are not
 * plain standard ASCII. Used for logging information.
 */
static void
clean_buffer (char *buffer, int len)
{
  int i;

  for (i = 0; i < len; ++i)
    {
      if (!isalnum (buffer[i]) && buffer[i] != '-' && buffer[i] != '+')
        {
          buffer[i] = '.';
        }
    }
}

/*------------------------------------------------------------------*/
/*
 * Returns true if the given number bytes is available on the socket
 */
int
lw_sock_peek_ex (int *sock, int len)
{
  int result = 0;
  int res;
  fd_set read;
  struct timeval tv;
  char buffer[LW_SOCK_MESSAGE_SIZE];
  int sock_copy;

  if ((sock_copy = *sock) >= 0)
    {
      if (len <= LW_SOCK_MESSAGE_SIZE - 1)
        {
          FD_ZERO (&read);
          FD_SET (sock_copy, &read);
          tv.tv_sec = 0;
          tv.tv_usec = 0;
          res = select (sock_copy + 1, &read, NULL, NULL, &tv);
          if (res > 0)
            {
              if (FD_ISSET (sock_copy, &read))
                {
                  /*
                   * First we test if there's enough data available
                   */
                  if (recv (sock_copy, buffer, len, MSG_PEEK) == len)
                    {
                      result = 1;
                    }
                }
            }
        }
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends a string on the network.
 * The advantage of this function over a raw "send" is that it does
 * a "strlen" automatically to know the length of the string, and
 * adds a tailing CR+LF so that the message is "telnet/http compliant"
 */
int
lw_sock_send_str_ex (int *sock, char *str)
{
  int result = 0;
  int len;
  char buffer[LW_SOCK_MESSAGE_SIZE];
  int sock_copy;
  fd_set write;
  struct timeval tv;
  int res;
  int total_sent;
  int sent;
#ifdef WIN32
  int winerr;
#endif

  if ((sock_copy = *sock) >= 0)
    {
      result = 1;

      /*
       * We put the string in a buffer, since we'll probably have
       * to modify it (cut if it's too long, add a tailing CR+LF).
       */
      len = LW_SOCK_MESSAGE_SIZE - 3;
      LW_MACRO_STRNCPY (buffer, str, len);
      buffer[len] = '\x0d';
      ++len;
      buffer[len] = '\x0a';
      ++len;
      buffer[len] = '\0';

      total_sent = 0;
      while (total_sent < len && result && (sock_copy = *sock) >= 0)
        {
          FD_ZERO (&write);
          FD_SET (sock_copy, &write);
          tv.tv_sec = LW_SOCK_SEND_SEC;
          tv.tv_usec = LW_SOCK_SEND_USEC;

          errno = 0;
          res = select (sock_copy + 1, NULL, &write, NULL, &tv);

          switch (res)
            {
            case -1:
#ifdef WIN32
              winerr = WSAGetLastError ();
              if (winerr != WSAEINTR && winerr != WSAENOBUFS)
                {
                  result = 0;
                }
#else
              if (errno != EINTR && errno != ENOBUFS)
                {
                  result = 0;
                }
#endif
              break;
            case 1:
              if (FD_ISSET (sock_copy, &write))
                {
                  sent =
                    send (sock_copy, buffer + total_sent, len - total_sent,
                          0);
                  if (sent > 0 && sent <= len - total_sent)
                    {
                      total_sent += sent;
                    }
                  else
                    {
                      result = 0;
                    }
                }
              break;
            default:
              result = 0;
            }
        }

      if (LW_SOCK_LOG)
        {
          switch (result)
            {
            case 1:
              log_print_int (sock_copy);
              log_print_str (" > \"");
              log_print_str (str);
              log_print_str ("\"");
              log_println ();
              log_flush ();
              break;
            case 0:
              log_print_int (sock_copy);
              log_print_str (" > timeout!");
              log_println ();
              break;
            default:
              log_print_int (sock_copy);
              log_print_str (" > unexpected error!");
              log_println ();
              break;
            }
        }
    }
  return result;
}

/*------------------------------------------------------------------*/
/*
 * Receives some data from the network.
 * A tailing "\n" is expected. The routine handles both chr(10) and chr(13)
 * correctly so that the program can be used with telnet under UNIX or
 * windows without much trouble.
 * This tailing "\n" is removed, so the return string is just the exact
 * message which has been send with sock_send.
 * Buffer must be able to accept at least LW_SOCK_MESSAGE_SIZE chars.
 * Note that the function will not block forever, if there's no incoming
 * for more than a #define specified amount of time, it will return
 * an error.
 */
int
lw_sock_recv_str_ex (int *sock, char *str)
{
  int result = 0;
  int pos, l;
  int cr_found;
  char *cr;
  fd_set read;
  int sock_copy;
  struct timeval tv;
  int res;
#ifdef WIN32
  int winerr;
#endif

  str[0] = '\0';

  if ((sock_copy = *sock) >= 0)
    {
      result = 1;

      /*
       * We keep on trying to get data until
       * - we get a '\n' character
       * - the size of incoming data exceeds LW_SOCK_MESSAGE_SIZE
       * - there's a reception low level error
       */
      cr_found = 0;
      pos = 0;
      while (!cr_found && pos < LW_SOCK_MESSAGE_SIZE - 1 && result > 0
             && (sock_copy = *sock) >= 0)
        {
          FD_ZERO (&read);
          FD_SET (sock_copy, &read);
          tv.tv_sec = LW_SOCK_RECV_SEC;
          tv.tv_usec = LW_SOCK_RECV_USEC;

          errno = 0;
          res = select (sock_copy + 1, &read, NULL, NULL, &tv);

          switch (res)
            {
            case -1:
#ifdef WIN32
              winerr = WSAGetLastError ();
              if (winerr != WSAEINTR)
                {
                  result = 0;
                }
#else
              if (errno != EINTR)
                {
                  result = 0;
                }
#endif
              break;
            case 1:
              /*
               * We check that the event we just received concerns the socket
               * we are polling. If the event is not for us, let's consider
               * everything is fine...
               */
              if (!FD_ISSET (sock_copy, &read))
                {
                  result = 1;
                }
              else
                {
                  /*
                   * We get the caracters one by one. This is a performance
                   * killer but we don't care since this routine is only
                   * used at the very beginning of the game, when the players
                   * are connecting themselves. And it has the *big* advantage
                   * that "netkeys" are not eaten up by this routine. More
                   * precisely, there's used to be a bug because when reading
                   * the final "OK" message, the clients read some netkeys
                   * along with it, which caused some network inconsistency
                   * since the messages had "disappeared".
                   */
                  if ((l = recv (sock_copy, str + pos, 1, 0)) <= 0)
                    {
                      /*
                       * OK, now we have received a message on this socket
                       * but there's no data. In most of the cases this means
                       * the socket is dead, so we return -1 instead of 0
                       * so that the caller can make the difference between
                       * a timeout (0) and a dead socket (-1).
                       */
                      result = -1;
                    }
                  else
                    {
                      /*
                       * pos is an offset in the buffer. It is used to keep a
                       * trace of where new data should be appended in case we
                       * have to call recv several times to retrieve the whole
                       * message.
                       */
                      pos += l;
                      /*
                       * We add a tailing '\0' so that the string is "C compliant"
                       */
                      str[pos] = 0;
                      /*
                       * We seek for character 10, which should be '\n'
                       */
                      if ((cr = strchr (str, '\x0a')) != NULL)
                        {
                          cr_found = 1;
                          /*
                           * We handle the special character '13' for very often,
                           * especially when using telnet, the strings come
                           * with char(13)chr(10) at the end. So if it happens
                           * that after removing the 10 there's still a 13, then
                           * we remove the 13 as well.
                           */
                          if ((cr - str) >= 1 && (*(cr - 1)) == '\x0d')
                            {
                              /*
                               * Let's cut this ugly "ascii 13" character
                               * along with "ascii 10"
                               */
                              (*(cr - 1)) = 0;
                            }
                          else
                            {
                              /*
                               * No "ascii 13" in sight, simply remove "ascii 10"
                               */
                              (*cr) = 0;
                            }
                        }
                      result = 1;
                    }
                }
              break;
            default:
              result = 0;
            }
        }

      if (LW_SOCK_LOG)
        {
          switch (result)
            {
            case 1:
              log_print_int (sock_copy);
              log_print_str (" < \"");
              log_print_str (str);
              log_print_str ("\"");
              log_println ();
              log_flush ();
              break;
            case 0:
              log_print_int (sock_copy);
              log_print_str (" < timeout!");
              log_println ();
              break;
            case -1:
              log_print_int (sock_copy);
              log_print_str (" < closed!");
              log_println ();
              break;
            default:
              log_print_int (sock_copy);
              log_print_str (" < unexpected error!");
              log_println ();
              break;
            }
        }
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends a buffer on the network.
 * Only a standard send wrapper
 */
int
lw_sock_send_buffer_ex (int *sock, char *buffer, int len)
{
  int result = 0;
  char trace[LW_SOCK_MESSAGE_SIZE];
  fd_set write;
  struct timeval tv;
  int sock_copy;
  int res;
  int total_sent;
  int sent;
#ifdef WIN32
  int winerr;
#endif

  if ((sock_copy = *sock) >= 0)
    {
      result = 1;

      if (len <= LW_SOCK_MESSAGE_SIZE - 1)
        {
          total_sent = 0;
          while (total_sent < len && result && (sock_copy = *sock) >= 0)
            {
              FD_ZERO (&write);
              FD_SET (sock_copy, &write);
              tv.tv_sec = LW_SOCK_SEND_SEC;
              tv.tv_usec = LW_SOCK_SEND_USEC;

              errno = 0;
              res = select (sock_copy + 1, NULL, &write, NULL, &tv);

              switch (res)
                {
                case -1:
#ifdef WIN32
                  winerr = WSAGetLastError ();
                  if (winerr != WSAEINTR && winerr != WSAENOBUFS)
                    {
                      result = 0;
                    }
#else
                  if (errno != EINTR && errno != ENOBUFS)
                    {
                      result = 0;
                    }
#endif
                  break;
                case 1:
                  if (FD_ISSET (sock_copy, &write))
                    {
                      sent = send (sock_copy,
                                   buffer + total_sent,
                                   MIN (len - total_sent,
                                        LW_SOCK_SEND_BUFFER_SIZE), 0);
                      if (sent > 0 && sent <= len - total_sent)
                        {
                          total_sent += sent;
                        }
                      else
                        {
                          result = 0;
                        }
                    }
                  break;
                default:
                  result = 0;
                }
            }

          if (LW_SOCK_LOG)
            {
              if (result)
                {
                  LW_MACRO_STRNCPY (trace, buffer, len);
                  trace[len] = '\0';
                  log_print_int (sock_copy);
                  log_print_str (" > [");
                  clean_buffer (trace, len);
                  log_print_str (trace);
                  log_print_str ("]");
                  log_println ();
                  log_flush ();
                }
              else
                {
                  log_print_int (sock_copy);
                  log_print_str (" > timeout!");
                  log_println ();
                }
            }
        }
      else
        {
          result = 0;
          if (LW_SOCK_LOG)
            {
              log_print_int (sock_copy);
              log_print_str (" > message too large!");
              log_println ();
            }
        }
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Receives a buffer on the network.
 * Only a standard recv wrapper
 */
int
lw_sock_recv_buffer_ex (int *sock, char *buffer, int len)
{
  int result = 0;
  char trace[LW_SOCK_MESSAGE_SIZE];
  int res;
  fd_set read;
  struct timeval tv;
  int sock_copy;
  int total_received;
  int received;
#ifdef WIN32
  int winerr;
#endif

  memset (buffer, 0, len);

  if ((sock_copy = *sock) >= 0)
    {
      result = 1;

      if (len <= LW_SOCK_MESSAGE_SIZE - 1)
        {
          total_received = 0;
          while (total_received < len && result && (sock_copy = *sock) >= 0)
            {
              FD_ZERO (&read);
              FD_SET (sock_copy, &read);
              tv.tv_sec = LW_SOCK_RECV_SEC;
              tv.tv_usec = LW_SOCK_RECV_USEC;

              errno = 0;
              res = select (sock_copy + 1, &read, NULL, NULL, &tv);

              switch (res)
                {
                case -1:
#ifdef WIN32
                  winerr = WSAGetLastError ();
                  if (winerr != WSAEINTR)
                    {
                      result = 0;
                    }
#else
                  if (errno != EINTR)
                    {
                      result = 0;
                    }
#endif
                  break;
                case 1:
                  if (FD_ISSET (sock_copy, &read))
                    {
                      received = recv (sock_copy,
                                       buffer + total_received,
                                       MIN (len - total_received,
                                            LW_SOCK_RECV_BUFFER_SIZE), 0);
                      if (received > 0 && received <= len - total_received)
                        {
                          total_received += received;
                        }
                      else
                        {
                          result = 0;
                        }
                    }
                  break;
                default:
                  result = 0;
                }
            }

          if (LW_SOCK_LOG)
            {
              if (result)
                {
                  LW_MACRO_STRNCPY (trace, buffer, len);
                  trace[len] = '\0';
                  log_print_int (sock_copy);
                  log_print_str (" < [");
                  clean_buffer (trace, len);
                  log_print_str (trace);
                  log_print_str ("]");
                  log_println ();
                  log_flush ();
                }
              else
                {
                  log_print_int (sock_copy);
                  log_print_str (" < timeout!");
                  log_println ();
                }
            }
        }
      else
        {
          result = 0;
          if (LW_SOCK_LOG)
            {
              log_print_int (sock_copy);
              log_print_str (" < message too large!");
              log_println ();
            }
        }
    }

  return result;
}
