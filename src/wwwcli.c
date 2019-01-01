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
/* name          : wwwcli.c                                         */
/* content       : client code used to contact www.ufoot.org        */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "wwwcli.h"
#include "startup.h"
#include "log.h"
#include "httputil.h"
#include "init.h"
#include "server.h"
#include "macro.h"
#include "ping.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_WWWCLI_HTTP_CONTENT_SIZE 100000
#define LW_WWWCLI_HTTP_URL_SIZE (URL_SIZE + 200)

#define LW_WWWCLI_URL_LIST "list.txt.php"
#define LW_WWWCLI_TEMP_BUFFER_SIZE 100

/*
 * If a line is shorter than this, we consider it as an empty
 * line, this avoids errors with useless stupid malformatted lines
 */
#define LW_WWWCLI_MIN_LINE_SIZE 20

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static int parse_line (LW_WWWCLI_DATA * result, char **str);
static void skip_space (char **str);
static int read_string (char *result, char **str, int size);
static int read_int (int *result, char **str);
static int read_separator (char **str);
static int read_eol (char **str);
static int read_eol_n (char **str, int n);
static void next_line (char **str);

/*==================================================================*/
/* global functions                                                 */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Main function of the server thread which tries to connect
 * on the metaserver
 */
int
lw_wwwcli_get_games (LW_WWWCLI_DATA * buffer, int *size)
{
  int result = 0;
  char buffer_content[LW_WWWCLI_HTTP_CONTENT_SIZE];
  char buffer_url[LW_WWWCLI_HTTP_URL_SIZE];
  char *pos, *pos_tmp;
  int i = 0;

  log_print_str ("Getting server list from \"");
  log_print_str (STARTUP_METASERVER);
  log_print_str ("\" ");

  memset (&buffer_url, 0, sizeof (buffer_url));
  LW_MACRO_SPRINTF3 (buffer_url,
                     "%s%s?protocol=%s&game=" LW_PROGRAM "&version="
                     LW_VERSION, STARTUP_METASERVER, LW_WWWCLI_URL_LIST,
                     LW_SERVER_METASERVER_PROTOCOL);

  if (lw_httputil_get_page
      (buffer_content, buffer_url, sizeof (buffer_content)))
    {
      pos = buffer_content;

      while (read_eol (&pos));

      while (i < (*size) && strlen (pos) > 0)
        {
          pos_tmp = pos;
          if (read_eol_n (&pos_tmp, LW_WWWCLI_MIN_LINE_SIZE))
            {
              /*
               * This was just an empty or too short line...
               */
              pos = pos_tmp;
            }
          else
            {
              /*
               * We parse the line only if there's something on it...
               */
              if (parse_line (buffer + i, &pos))
                {
                  if (buffer[i].busy_players < buffer[i].max_players)
                    {
                      /*
                       * we increment i only if there's some room left on the
                       * server. If it's full of people playing on it, it's
                       * of absolutely no use to present it as a working
                       * server... And not incrementing i will make it be
                       * overwritten by the next server.
                       */
                      ++i;
                      log_print_str ("+");
                    }
                  else
                    {
                      log_print_str ("=");
                    }
                }
              else
                {
                  log_print_str ("-");
                }
            }
        }

      result = 1;
    }

  if (result == 1)
    {
      (*size) = i;
    }
  else
    {
      (*size) = 0;
    }

  display_success (result);
  if (!result)
    {
      log_print_str ("Error: ");
      log_println_str (buffer_content);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Parses a line returned by the meta-server
 */
static int
parse_line (LW_WWWCLI_DATA * result, char **str)
{
  int ok = 0;
  char temp[LW_WWWCLI_TEMP_BUFFER_SIZE];

  memset (result, 0, sizeof (LW_WWWCLI_DATA));


  if (read_string (result->ip, str, sizeof (result->ip)) &&
      read_separator (str) &&
      read_int (&(result->port), str) &&
      read_separator (str) &&
      read_string (temp, str, sizeof (temp)) &&
      read_separator (str) &&
      read_string (temp, str, sizeof (temp)) &&
      read_separator (str) &&
      read_int (&(result->uptime), str) &&
      read_separator (str) &&
      read_int (&(result->busy_players), str) &&
      read_separator (str) &&
      read_int (&(result->max_players), str) &&
      read_separator (str) &&
      read_int (&(result->password), str) &&
      read_separator (str) &&
      read_string (result->comment, str, sizeof (result->comment)))
    {
      LW_MACRO_SPRINTF3 (result->label,
                         "%s (%d/%d)",
                         result->ip,
                         result->busy_players, result->max_players);

      result->ping_delay = lw_ping_try_server (result->ip, result->port);

      ok = 1;
    }

  next_line (str);

  return ok;
}

/*------------------------------------------------------------------*/
/*
 * Skip spaces, tabs and the likes.
 */
static void
skip_space (char **str)
{
  while ((isspace (**str) || (**str) == '\r') && (**str) != '\n')
    {
      ++(*str);
    }
}

/*------------------------------------------------------------------*/
/*
 * Reads a string between ''. Double '' are interpreted as a single '
 * which is the standard SQL convention
 */
static int
read_string (char *result, char **str, int size)
{
  int i = 0;
  int done = 0;
  char c;

  skip_space (str);

  c = **str;
  if (c == '\'')
    {
      ++(*str);
      while ((c = **str) != '\0' && c != '\n' && !done)
        {
          ++(*str);
          if (c == '\'')
            {
              if ((**str) == '\'')
                {
                  ++(*str);
                  if (i < size - 1)
                    {
                      result[i++] = '\'';
                    }
                }
              else
                {
                  done = 1;
                }
            }
          else
            {
              if (i < size - 1)
                {
                  result[i++] = c;
                }
            }
        }
    }

  result[i++] = '\0';

  skip_space (str);

  return done;
}

/*------------------------------------------------------------------*/
/*
 * Reads an integer
 */
static int
read_int (int *result, char **str)
{
  int ok = 0;
  char c;

  (*result) = 0;

  skip_space (str);

  while (isdigit (c = (**str)))
    {
      ++(*str);

      (*result) *= 10;
      (*result) += c - '0';

      ok = 1;
    }

  skip_space (str);

  return ok;
}

/*------------------------------------------------------------------*/
/*
 * Reads a field separator
 */
static int
read_separator (char **str)
{
  int ok = 0;
  char c;

  skip_space (str);

  c = **str;

  if (c == ',')
    {
      ++(*str);
      ok = 1;
    }

  skip_space (str);

  return ok;
}

/*------------------------------------------------------------------*/
/*
 * Reads a new-line character
 */
static int
read_eol (char **str)
{
  int ok = 0;
  char c;

  skip_space (str);

  c = **str;

  if (c == '\n')
    {
      ++(*str);
      ok = 1;
    }

  skip_space (str);

  return ok;
}

/*------------------------------------------------------------------*/
/*
 * Reads "less than n chars", then a "\n"
 */
static int
read_eol_n (char **str, int n)
{
  int ok = 0;
  char *tmp;

  skip_space (str);

  tmp = strchr (*str, '\n');
  if (tmp != NULL)
    {
      if (tmp - (*str) < n)
        {
          ok = 1;
          (*str) = tmp + 1;
          skip_space (str);
        }
    }

  return ok;
}

/*------------------------------------------------------------------*/
/*
 * Reads an end of line
 */
static void
next_line (char **str)
{
  char *pos;
  int len;

  skip_space (str);

  pos = strchr (*str, '\n');
  if (pos)
    {
      (*str) = pos + 1;
    }
  else
    {
      /*
       * No '\n' found, we decided to go to the
       * very end of the string
       */
      len = strlen (*str);
      (*str) += len;
    }

  skip_space (str);
}
