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
/* Copyright (C) 1998-2014 Christian Mauduit                                 */
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
/* name          : httputil.c                                       */
/* content       : low level functions to handle http requests      */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "httputil.h"
#include "dnsutil.h"
#include "log.h"
#include "sockgen.h"
#include "sock2gen.h"
#include "macro.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_HTTPUTIL_PORT              80
#define LW_HTTPUTIL_HOST_SIZE         200
#define LW_HTTPUTIL_REQUEST_SIZE      1000
#define LW_HTTPUTIL_EMPTY_LINES_LIMIT 10
#define LW_HTTPUTIL_MIN_LEN           1
#define LW_HTTPUTIL_GET_CMD \
    "GET http://%s HTTP/1.1\x0d\x0a" \
    "Host: %s\x0d\x0a" \
    "Connection: close\x0d\x0a"
/* additional CR+LF supplied by lw_sock_send_str() */
/*
 * Note: I replaced the original "\x0d\x0a" that where in the
 * patch by the equivalent "\r\n". I find it more readable,
 * that's all 8-) But... ...Roderick Schertler told me that
 * it was safer to use "\x0d\x0a" for the values for "\r" or 
 * "\n" are up to the compiler, and might cause problems
 * especially on Mac platforms.
 */

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

/*==================================================================*/
/* global functions                                                 */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Sends a request and waits for a response.
 * If there's an error, the error message is written in "content".
 * If there's no error, "content" contains the web page.
 */
int
lw_httputil_get_page (char *content, char *url, int size)
{
  int result = 0;
  char ip[LW_SOCK_IP_SIZE];
  char host[LW_HTTPUTIL_HOST_SIZE + 1];
  char request[LW_HTTPUTIL_REQUEST_SIZE + 1];
  char data[LW_SOCK_MESSAGE_SIZE];
  int data_len;
  char *search;
  int sock;
  int empty_lines = 0;

  memset (content, 0, size);

  LW_MACRO_STRCPY (host, url);

  search = strchr (host, '/');
  if (search)
    {
      (*search) = '\0';
    }

  LW_MACRO_SPRINTF2 (request, LW_HTTPUTIL_GET_CMD, url, host);

  if (lw_dnsutil_name_to_ip (ip, host))
    {
      if (lw_sock_connect (&sock, ip, LW_HTTPUTIL_PORT))
	{
	  if (lw_sock_send_str (&sock, request))
	    {
	      result = 1;
	      /* read status line */
	      if (result && lw_sock_recv_str (&sock, data) != 1)
		{
		  result = 0;
		  LW_MACRO_SNPRINTF0 (content, size,
				      "Failed to read status line");
		}
	      /* make sure status code starts with 2 */
	      if (result &&
		  !((search = strchr (data, ' ')) && search[1] == '2'))
		{
		  result = 0;
		  LW_MACRO_SNPRINTF2 (content, size,
				      "Failure from web server \"%s\": \"%s\"",
				      host, data);
		}
	      if (result)
		{
		  /* skip past headers */
		  while ((result =
			  (result && (lw_sock_recv_str (&sock, data) == 1)))
			 && data[0])
		    {
		      /* nothing to do here */
		    }

		  if (result)
		    {
		      /* finally, we read the list of servers */
		      while (lw_sock_recv_str (&sock, data) == 1 &&
			     empty_lines < LW_HTTPUTIL_EMPTY_LINES_LIMIT)
			{
			  data_len = strlen (data);
			  /*
			   * If the line is too short, we ignore it,
			   * in fact, we get many "too short" lines
			   * such as the size of the file for instance,
			   * which is shown as an hex number in HTTP 1.1
			   *
			   * No "usefull" line should be smaller than 
			   * LW_HTTPUTIL_MIN_LEN however.
			   */
			  if (data_len > LW_HTTPUTIL_MIN_LEN)
			    {
			      if ((int) (strlen (content) + data_len) <
				  size - 2)
				{
				  LW_MACRO_STRNCAT (content, data, size);
				  LW_MACRO_STRNCAT (content, "\n", size);
				}
			    }
			  if (data_len == 0)
			    {
			      empty_lines++;
			    }
			}
		    }
		  else
		    {
		      LW_MACRO_SNPRINTF0 (content, size,
					  "Failed to read headers");
		    }
		}
	    }
	  else
	    {
	      LW_MACRO_SNPRINTF1 (content, size, "Request \"%s\" failed!",
				  request);
	    }
	  lw_sock_close (&sock);
	}
      else
	{
	  LW_MACRO_SNPRINTF1 (content, size, "Can't connect to \"%s\"!", ip);
	}
    }
  else
    {
      LW_MACRO_SNPRINTF1 (content, size, "Unable to find host \"%s\"!", host);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Transforms a text into an URL compatible parameter, ie
 * " " becomes "%20" for instance.
 * urlparam must be a buffer of size+1 characters
 * Note that "_" is transformed into " ", this is to allow more
 * easily people to submit spaces while using command line arguments
 * with the server.
 */
void
lw_httputil_text_to_urlparam (char *urlparam, char *text, int size)
{
  int i, j;
  char buffer[33];
  unsigned char c;

  memset (urlparam, 0, size + 1);

  j = 0;
  for (i = 0; text[i] && j < size; ++i)
    {
      if (text[i] == '_')
	{
	  c = ' ';
	}
      else
	{
	  c = text[i];
	}

      if (isalnum (c))
	{
	  urlparam[j] = c;
	  j++;
	}
      else
	{
	  if (j < size - 2)
	    {
	      LW_MACRO_SPRINTF1 (buffer, "%X", c);
	      urlparam[j] = '%';
	      urlparam[j + 1] = buffer[0];
	      urlparam[j + 2] = buffer[1];
	    }
	  j += 3;
	}
    }
}
