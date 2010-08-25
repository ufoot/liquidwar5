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
/* Copyright (C) 1998-2010 Christian Mauduit                                 */
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
/* name          : server.c                                         */
/* content       : main source for the liquidwar-server executable  */
/* last update   : April 14th 2001                                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "server.h"
#include "netmess.h"
#include "sockgen.h"
#include "parser.h"
#include "basicopt.h"
#include "srvcont.h"
#include "log.h"
#include "wwwsrv.h"
#include "thrdgen.h"
#include "macro.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_SERVER_IDENT_LAG        "lag"
#define LW_SERVER_IDENT_PORT       "port"
#define LW_SERVER_IDENT_METASERVER "metaserver"
#define LW_SERVER_IDENT_PUBLIC     "public"
#define LW_SERVER_IDENT_PRIVATE    "private"
#define LW_SERVER_IDENT_PASSWORD   "password"
#define LW_SERVER_IDENT_COMMENT    "comment"
#define LW_SERVER_IDENT_LOG        "log"
#define LW_SERVER_IDENT_CALLBACK   "callback"

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

static int get_nb_teams ();
static int get_lag ();
static int get_port ();
static int get_privacy (char *metaserver);
static char *get_metaserver ();
static char *get_password ();
static char *get_comment ();
static char *get_callback ();

/*------------------------------------------------------------------*/
/*
 * This function returns how many teams there must be. Basically,
 * it checks the command line parameters, and if the number
 * of teams has not be told, then it prompts the user for it.
 */
static int
get_nb_teams ()
{
  int i;
  char str_i[33];
  int result = 0;
  char c;

  for (i = 1; i <= NB_TEAMS; ++i)
    {
      LW_MACRO_SPRINTF1 (str_i, "%d", i);
      if (exist_argument (str_i))
	{
	  result = i;
	}
    }

  if (result < 2 || result > NB_TEAMS)
    {
      log_print_str ("How many teams will connect to this server? ");

      while ((c = getchar ()) == '\n');

      if (c >= '2' && c <= '0' + NB_TEAMS)
	{
	  result = c - '0';

	  log_print_str ("Use \"-");
	  log_print_int (result);
	  log_println_str ("\" to get rid of this question.");
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * This function returns the server lag.
 * This parameter is quite important, since it's the number of rounds
 * which will elapse between the moment when the client sends his
 * key strokes and the moment when he will receive them back from
 * the server, along with the information from other players.
 */
static int
get_lag ()
{
  int result = LW_SERVER_DEFAULT_LAG;

  if (exist_argument (LW_SERVER_IDENT_LAG))
    {
      result = get_argument_int (LW_SERVER_IDENT_LAG);
      if (result < 0)
	{
	  result = 0;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns the port which should be used to communicate with clients
 */
static int
get_port ()
{
  int result = LW_SERVER_DEFAULT_PORT;

  if (exist_argument (LW_SERVER_IDENT_PORT))
    {
      result = get_argument_int (LW_SERVER_IDENT_PORT);
      if (result <= 0)
	{
	  result = LW_SERVER_DEFAULT_PORT;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * This function returns 1 if the server should register on
 * the meta server and 0 if it should not do it
 */
static int
get_privacy (char *metaserver)
{
  int result = 0;
  int chosen = 0;
  char c;

  if (exist_argument (LW_SERVER_IDENT_PUBLIC))
    {
      /*
       * The command line explicitely says -public,
       * we register on the metaserver
       */
      chosen = 1;
      result = 1;
    }

  if (exist_argument (LW_SERVER_IDENT_PRIVATE))
    {
      /*
       * The command line explicitely says -private,
       * we do not register on the metaserver
       */
      chosen = 1;
    }

  if (!chosen)
    {
      /*
       * no command line option, we ask the question
       * interactively
       */
      log_print_str ("Register on \"");
      log_print_str (metaserver);
      log_print_str ("\" (y/n)? ");

      while ((c = getchar ()) == '\n');

      if (c == 'y' || c == 'Y')
	{
	  result = 1;
	}

      if (result)
	{
	  log_println_str ("Use \"-public\" to get rid of this question.");
	}
      else
	{
	  log_println_str ("Use \"-private\" to get rid of this question.");
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns the metaserver name
 */
static char *
get_metaserver ()
{
  char *result = LW_SERVER_DEFAULT_METASERVER;

  if (exist_argument (LW_SERVER_IDENT_METASERVER))
    {
      result = get_argument_str (LW_SERVER_IDENT_METASERVER);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns the password associated to the server
 */
static char *
get_password ()
{
  char *result = "";

  result = get_argument_str (LW_SERVER_IDENT_PASSWORD);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns the comment associated to the server
 */
static char *
get_comment ()
{
  char *result = "";

  result = get_argument_str (LW_SERVER_IDENT_COMMENT);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns the log file name
 */
char *
lw_server_get_log ()
{
  char *result = "";

  result = get_argument_str (LW_SERVER_IDENT_LOG);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns the callback script/program called when a client connects
 * on the server.
 */
static char *
get_callback ()
{
  char *result = "";

  result = get_argument_str (LW_SERVER_IDENT_CALLBACK);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Exits the program and cleans up a few things
 */
void
lw_server_exit (int code)
{
  lw_sock_exit ();

  log_exit ();

  exit (code);
}

/*------------------------------------------------------------------*/
/*
 * Main function, launches the server.
 * Here we don't need to use the END_OF_MAIN() hack of Allegro, since
 * under windows the game will be compiled as a console App
 * (no WinMain needed) and we don't use Allegro at all here anyway...
 */
int
main (int argc, char **argv)
{
  int retcode = 0;
  LW_SRVCONT cont;
  LW_WWWSRV_DATA www_data;
  char *metaserver;
  int nb_teams;
  int port;
  int res = 1;

  lw_sock_init ();

  read_command_line (argc, argv);

  /*
   * we initialize the log _after_ read_command_line for
   * a special log file might have been specified
   */
  log_init ();

  if (lw_basicopt_process ("Liquid War Server"))
    {
      nb_teams = get_nb_teams ();

      if (nb_teams >= 2 && nb_teams <= NB_TEAMS)
	{
	  port = get_port ();
	  metaserver = get_metaserver ();
	  /* 
	   * we initialize the www_data struct which will contain
	   * all the informations to be transmitted to the metaserver
	   */
	  lw_wwwsrv_init (&www_data,
			  get_privacy (metaserver),
			  0,
			  metaserver,
			  port, 0, 0, get_password (), get_comment ());

	  log_flush ();
	  log_set_server_mode (1);

	  /*
	   * a good old infinite loop, the server has to be stopped
	   * with CTRL-C
	   */
	  while (1)
	    {
	      if (lw_srvcont_wait_teams (&cont,
					 &www_data,
					 nb_teams,
					 port,
					 get_password (), get_callback ()))
		{
		  if (lw_srvcont_tell_who (&cont))
		    {
		      if (lw_srvcont_final_ok (&cont))
			{
			  res = lw_srvcont_replicate_keys
			    (&cont, &www_data, get_lag (), get_password ());
			}
		    }
		}
	      lw_srvcont_close (&cont);

	      log_flush ();
	    }
	}
      else
	{
	  log_println_str ("Not enough or too many teams!");
	}
    }
  else
    {
      /*
       * we exit with a normal code (--help or --version)
       */

      lw_server_exit (0);
    }

  /*
   * If we get here, there necessarily an error
   */
  lw_server_exit (1);

  /*
   * Non-reachable code, but GCC would complain if we did not
   * return something...
   */

  return retcode;
}
