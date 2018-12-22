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
/* Liquid War homepage : http://www.ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* name          : netmess.c                                        */
/* content       : functions to create and parse network messages   */
/* last update   : April 14th 2001                                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "netmess.h"

/*==================================================================*/
/* static functions declaration                                     */
/*==================================================================*/

static int eat_space (char **message);
static int read_separator (char **message);
static char *read_word (char **message);
static int get_code (char **message);
static int get_argc (char *message);
static char **get_argv (char *message, int argc);

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * strcasecmp does not exists on win32, so we make it default to strcmp.
 * It's not a big problem since case insensitive tests are usefull in
 * developpement and network stuff is easier to test under UNIX anyway...
 */
#ifndef strcasecmp
#define strcasecmp(A,B) strcmp((A),(B))
#endif

/*------------------------------------------------------------------*/
/*
 * Skip spaces, tabs and the likes.
 */
static int
eat_space (char **message)
{
  int result = 0;

  while (isspace (**message))
    {
      result++;
      ++(*message);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Reads a separator token. The separator is ',' 
 */
static int
read_separator (char **message)
{
  int result = 0;

  eat_space (message);
  if ((**message) == ',')
    {
      result++;
      ++(*message);
    }
  eat_space (message);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Reads a word, a number, basically what's between separators.
 * The returned pointer must be disallocated with a "free".
 */
static char *
read_word (char **message)
{
  char *begin = NULL;
  char *result = NULL;
  int len;
  char quote_char;

  eat_space (message);

  quote_char = **message;

  if (quote_char == '\"' || quote_char == '\'')
    {
      /*
       * We handle strings quoted with ' or ".
       * No escape character is handled yet...
       */
      ++(*message);

      begin = *message;

      while ((**message) != quote_char && (**message) != '\0')
        {
          ++(*message);
        }

      len = (*message) - begin;

      ++(*message);
    }
  else
    {
      begin = *message;

      while ((!isspace (**message)) && (**message) != ','
             && (**message) != '\0')
        {
          ++(*message);
        }

      len = (*message) - begin;
    }

  if (len >= 0)
    {
      result = (char *) malloc (len + 1);
      if (result != NULL)
        {
          if (len > 0)
            {
              strncpy (result, begin, len);
            }
          result[len] = 0;

          lw_netmess_cleanup_arg (result);
        }
    }
  eat_space (message);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns the code of the command.
 */
static int
get_code (char **message)
{
  char *command;
  int result = LW_NETMESS_CODE_UNKNOWN;

  if (read_separator (message) == 0)
    {
      if ((command = read_word (message)) != NULL)
        {
          if (strcasecmp (command, LW_NETMESS_TEXT_OK) == 0)
            {
              result = LW_NETMESS_CODE_OK;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_ERR) == 0)
            {
              result = LW_NETMESS_CODE_ERR;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_PING) == 0)
            {
              result = LW_NETMESS_CODE_PING;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_ECHO) == 0)
            {
              result = LW_NETMESS_CODE_ECHO;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_PROGRAM) == 0)
            {
              result = LW_NETMESS_CODE_PROGRAM;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_VERSION) == 0)
            {
              result = LW_NETMESS_CODE_VERSION;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_PASSWORD) == 0)
            {
              result = LW_NETMESS_CODE_PASSWORD;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_FREE) == 0)
            {
              result = LW_NETMESS_CODE_FREE;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_TEAM) == 0)
            {
              result = LW_NETMESS_CODE_TEAM;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_READY) == 0)
            {
              result = LW_NETMESS_CODE_READY;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_START) == 0)
            {
              result = LW_NETMESS_CODE_START;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_WAITING) == 0)
            {
              result = LW_NETMESS_CODE_WAITING;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_NEXT) == 0)
            {
              result = LW_NETMESS_CODE_NEXT;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_TEAMSTARTINFO) == 0)
            {
              result = LW_NETMESS_CODE_TEAMSTARTINFO;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_WHO) == 0)
            {
              result = LW_NETMESS_CODE_WHO;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_LOCAL) == 0)
            {
              result = LW_NETMESS_CODE_LOCAL;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_NETWORK) == 0)
            {
              result = LW_NETMESS_CODE_NETWORK;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_NOBODY) == 0)
            {
              result = LW_NETMESS_CODE_NOBODY;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_SENDMAP) == 0)
            {
              result = LW_NETMESS_CODE_SENDMAP;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_RECVMAP) == 0)
            {
              result = LW_NETMESS_CODE_RECVMAP;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_SENDCONFIG) == 0)
            {
              result = LW_NETMESS_CODE_SENDCONFIG;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_RECVCONFIG) == 0)
            {
              result = LW_NETMESS_CODE_RECVCONFIG;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_CHATTALK) == 0)
            {
              result = LW_NETMESS_CODE_CHATTALK;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_CHATLISTEN) == 0)
            {
              result = LW_NETMESS_CODE_CHATLISTEN;
            }
          else if (strcasecmp (command, LW_NETMESS_TEXT_QUIT) == 0)
            {
              result = LW_NETMESS_CODE_QUIT;
            }

          free (command);
        }
    }

  return result;
}

/*------------------------------------------------------------------*/
/* 
 * Gets the number of arguments in the message.
 * It's a little different from the C convention used in main(),
 * since 0 means "0 args" (whereas in main() 0 args corresponds to argc==1).
 */
static int
get_argc (char *message)
{
  int result = 0;
  char *word = NULL;
  int null_arg = 0;

  if (read_separator (&message) == 0)
    {
      if (message[0] == '\0')
        {
          null_arg = 1;
        }

      word = read_word (&message);
      if (word != NULL)
        {
          free (word);
          result = 1;
          while (read_separator (&message) == 1)
            {
              word = read_word (&message);
              if (word != NULL)
                {
                  free (word);
                  result++;
                }
            }
        }
    }

  if (result == 1 && null_arg)
    {
      result = 0;
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Parses all the arguments of the message.
 */
static char **
get_argv (char *message, int argc)
{
  char **result = NULL;
  char *word;
  int i = 0;

  if (argc > 0 && read_separator (&message) == 0)
    {
      result = (char **) malloc (argc * sizeof (char *));
      if (result != NULL)
        {
          word = read_word (&message);
          if (word != NULL)
            {
              result[i] = word;
              i = 1;
              while (i < argc && read_separator (&message) == 1)
                {
                  word = read_word (&message);
                  if (word != NULL)
                    {
                      result[i] = word;
                      i++;
                    }
                }
            }
        }
      if (i != argc)
        {
          free (result);
          result = NULL;
        }
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Parses and interprets a network message.
 * The syntaxe must be "COMMAND arg1,arg2,arg3,...".
 * This function should never fail, unless there are some problems
 * in memory allocation. Parsing an invalid message should simply
 * return a LW_NETMESS_CODE_UNKNOWN type with no args at all.
 * It's important to free the obtained pointer with lw_netmess_free.
 */
LW_NETMESS *
lw_netmess_read (char *message)
{
  LW_NETMESS *result = NULL;

  result = (LW_NETMESS *) malloc (sizeof (LW_NETMESS));
  if (result != NULL)
    {
      result->code = get_code (&message);
      result->argc = get_argc (message);
      result->argv = get_argv (message, result->argc);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * To be called after each successfull call to lw_netmess_read
 * This function calls free() internally.
 */
void
lw_netmess_free (LW_NETMESS * ptr)
{
  int i;

  if (ptr != NULL)
    {
      if (ptr->argv != NULL)
        {
          for (i = 0; i < ptr->argc; ++i)
            {
              free (ptr->argv[i]);
            }
          if (ptr->argc > 0)
            {
              free (ptr->argv);
            }
        }
    }
}

/*------------------------------------------------------------------*/
/*
 * Cleanups strings so that they can be used as arguments in
 * network messages
 */
void
lw_netmess_cleanup_arg (char *string)
{
  char *pos;
  unsigned char c;

  pos = string;
  while ((c = *pos) != '\0')
    {
      if (c < 32 || c > 127 || strchr ("'\",", c))
        {
          (*pos) = ' ';
        }
      pos++;
    }
}
