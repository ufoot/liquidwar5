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
/* Copyright (C) 1998-2016 Christian Mauduit                                 */
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
/* name          : wwwsrv.c                                         */
/* content       : server code used to contact www.ufoot.org        */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <string.h>

#include "wwwsrv.h"
#include "log.h"
#include "httputil.h"
#include "thrdgen.h"
#include "server.h"
#include "macro.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_WWWSRV_HTTP_CONTENT_SIZE 1000
#define LW_WWWSRV_HTTP_URL_SIZE (URL_SIZE + 200 + COMMENT_SIZE * 3)

#define LW_WWWSRV_URL_REGISTER "register.txt.php"
#define LW_WWWSRV_DELAY 5

#define LW_WWWSRV_OK           "OK"

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static void lw_wwwsrv_callback (void *args);

/*==================================================================*/
/* global functions                                                 */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Initializes the data structure
 */
void
lw_wwwsrv_init (LW_WWWSRV_DATA * data,
		int privacy,
		int log,
		char *metaserver,
		int port,
		int busy_players,
		int max_players, char *password, char *comment)
{
  memset (data, 0, sizeof (LW_WWWSRV_DATA));

  data->privacy = privacy;
  data->log = log;
  strncpy (data->metaserver, metaserver, URL_SIZE);
  data->port = port;
  data->busy_players = busy_players;
  data->max_players = max_players;
  strncpy (data->password, password, PASSWORD_SIZE);
  strncpy (data->comment, comment, COMMENT_SIZE);
}

/*------------------------------------------------------------------*/
/*
 * Launches a thread which will register itself on the main server
 * Note that the return value just tells if the thread has been
 * successfully launched, but one can not use it to know if the
 * meta server actually registered us.
 */
int
lw_wwwsrv_register (LW_WWWSRV_DATA * data)
{
  int result = 0;

  if (data->privacy == 1)
    {
      /*
       * privacy is set to 1, so we try and contact the metaserver
       */
      result = lw_thread_start (lw_wwwsrv_callback, (void *) data);
    }
  else
    {
      /*
       * privacy is set to 0, we do not tell anyone about this server
       */
      result = 1;
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Main function of the server thread which tries to connect
 * on the metaserver
 */
static void
lw_wwwsrv_callback (void *args)
{
  LW_WWWSRV_DATA data;
  char buffer_content[LW_WWWSRV_HTTP_CONTENT_SIZE];
  char buffer_url[LW_WWWSRV_HTTP_URL_SIZE];
  char buffer_num[33];
  char buffer_comment[COMMENT_SIZE * 3 + 1];
  int ok = 0;

  memcpy (&data, args, sizeof (LW_WWWSRV_DATA));

  if (data.log)
    {
      log_print_str ("Trying to register on \"");
      log_print_str (data.metaserver);
      log_println_str ("\"...");
    }

  memset (&buffer_url, 0, sizeof (buffer_url));

  LW_MACRO_SPRINTF3 (buffer_url,
		     "%s%s?protocol=%s&game=" LW_PROGRAM "&version="
		     LW_VERSION, data.metaserver, LW_WWWSRV_URL_REGISTER,
		     LW_SERVER_METASERVER_PROTOCOL);

  LW_MACRO_SPRINTF1 (buffer_num, "%d", data.port);
  LW_MACRO_STRCAT (buffer_url, "&port=");
  LW_MACRO_STRCAT (buffer_url, buffer_num);

  LW_MACRO_SPRINTF1 (buffer_num, "%d", data.busy_players);
  LW_MACRO_STRCAT (buffer_url, "&busy_players=");
  LW_MACRO_STRCAT (buffer_url, buffer_num);

  LW_MACRO_SPRINTF1 (buffer_num, "%d", data.max_players);
  LW_MACRO_STRCAT (buffer_url, "&max_players=");
  LW_MACRO_STRCAT (buffer_url, buffer_num);

  LW_MACRO_STRCAT (buffer_url, "&password=");
  if (strlen (data.password) > 0)
    {
      LW_MACRO_STRCAT (buffer_url, "1");
    }
  else
    {
      LW_MACRO_STRCAT (buffer_url, "0");
    }

  LW_MACRO_STRCAT (buffer_url, "&comment=");
  lw_httputil_text_to_urlparam (buffer_comment,
				data.comment, sizeof (buffer_comment) - 1);
  LW_MACRO_STRCAT (buffer_url, buffer_comment);

  if (lw_httputil_get_page
      (buffer_content, buffer_url, sizeof (buffer_content)))
    {
      if (strstr (buffer_content, LW_WWWSRV_OK))
	{
	  ok = 1;
	}
      else
	{
	  log_print_str ("Incorrect answer from \"");
	  log_print_str (data.metaserver);
	  log_println_str ("\"!");
	}
    }
  else
    {
      log_println_str (buffer_content);
    }

  if (data.log)
    {
      if (ok)
	{
	  log_print_str ("Successfully registered on \"");
	  log_print_str (data.metaserver);
	  log_println_str ("\"");
	  log_flush ();
	}
      else
	{
	  log_print_str ("Unable to register on \"");
	  log_print_str (data.metaserver);
	  log_println_str ("\"");
	  log_flush ();
	}
    }
}
