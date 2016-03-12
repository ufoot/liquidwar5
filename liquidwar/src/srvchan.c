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
/* name          : srvchan.c                                        */
/* content       : handles a server network channel                 */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "srvchan.h"
#include "netmess.h"
#include "log.h"
#include "netmap.h"
#include "netconf.h"
#include "sock2gen.h"
#include "macro.h"

/*==================================================================*/
/* Constants                                                        */
/*==================================================================*/

/*
 * Following NULL-terminated list contains all versions which
 * are compatible with current.
 */
static char *ACCEPTED_VERSIONS[] = { "5.6.3",
  "5.6.4",
  "5.6.5",
  LW_VERSION,
  NULL
};

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static void reset (LW_SRVCHAN * chan);
static int handle_team (LW_SRVCHAN * chan, LW_NETMESS * mess,
			int *free_teams);
static int handle_ready (LW_SRVCHAN * chan, LW_NETMESS * mess);
static int handle_start (LW_SRVCHAN * chan, LW_NETMESS * mess);
static int handle_waiting (LW_SRVCHAN * chan,
			   LW_NETMESS * mess, int free_teams);
static int handle_next (LW_SRVCHAN * chan, LW_NETMESS * mess, int *next);
static int handle_teamstartinfo (LW_SRVCHAN * chan, LW_NETMESS * mess,
				 LW_TEAMSTARTINFO * team_start_info);
static int handle_chattalk (LW_SRVCHAN * chan, LW_NETMESS * mess,
			    LW_CHAT_HISTORY * chat);
static int handle_chatlisten (LW_SRVCHAN * chan, LW_NETMESS * mess);
static int handle_who (LW_SRVCHAN * chan, LW_NETMESS * mess);
static int handle_sendmap (LW_SRVCHAN * chan, LW_NETMESS * mess, void **map);
static int handle_recvmap (LW_SRVCHAN * chan, LW_NETMESS * mess, void *map);
static int handle_sendconfig (LW_SRVCHAN * chan, LW_NETMESS * mess,
			      LW_NETCONF * config);
static int handle_recvconfig (LW_SRVCHAN * chan, LW_NETMESS * mess,
			      LW_NETCONF * config);

/*------------------------------------------------------------------*/
/*
 * Resets the LW_SRVCHAN struct
 */
static void
reset (LW_SRVCHAN * chan)
{
  int i;

  memset (chan, 0, sizeof (LW_SRVCHAN));

  chan->sock = -1;
  chan->ip[0] = '\0';
  chan->port = 0;
  chan->program_ok = 0;
  chan->version_ok = 0;
  chan->password_ok = 0;
  chan->ready_ok = 0;
  chan->start = 0;
  chan->map = NULL;
  memset (&(chan->config), 0, sizeof (LW_NETCONF));
  for (i = 0; i < NB_TEAMS; ++i)
    {
      lw_srvteam_reset (&(chan->team[i]));
    }
  lw_chat_reset (&(chan->chat));
  chan->checksum_recv = 0;
  chan->checksum_send = 0;
  chan->normal_quit = 0;
}

/*------------------------------------------------------------------*/
/*
 * handles the "PING" message
 */
int
lw_srvchan_handle_ping (int sock, LW_NETMESS * mess)
{
  int result = 0;

  LW_MACRO_NOP (mess);

  /*
   * It's a ping, we answer OK everytime
   */
  lw_sock_send_str (&sock, LW_NETMESS_TEXT_OK);
  result = 1;

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "ECHO" message
 */
int
lw_srvchan_handle_echo (int sock, LW_NETMESS * mess)
{
  int result = 0;
  char answer[LW_SOCK_MESSAGE_SIZE];
  int i;

  /*
   * echo, we rebuild the original message
   * from the information we have in the LW_NETMESS
   * struct, and resend it to the client
   */
  LW_MACRO_STRCPY (answer, LW_NETMESS_TEXT_ECHO);
  i = 0;
  while (i < mess->argc)
    {
      if (i == 0)
	{
	  LW_MACRO_STRCAT (answer, " ");
	}
      else
	{
	  LW_MACRO_STRCAT (answer, ",");
	}
      LW_MACRO_STRCAT (answer, mess->argv[i]);
      i++;
    }
  lw_sock_send_str (&sock, answer);
  result = 1;

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "PROGRAM" message
 */
int
lw_srvchan_handle_program (int sock, LW_NETMESS * mess, int *ok)
{
  int result = 0;

  if (mess->argc == 1)
    {
      /*
       * OK, there's an argument, we check if it's the good prog name.
       * This prevents form accepting random connections: if the client
       * is not really a liquidwar client, it will be thrown away
       */
      if (strcmp (mess->argv[0], LW_PROGRAM) == 0)
	{
	  (*ok) = 1;
	  result = 1;
	}
    }

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&sock, LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&sock, LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "VERSION" message
 */
int
lw_srvchan_handle_version (int sock, LW_NETMESS * mess, int *ok)
{
  int result = 0;
  int i = 0;

  if (mess->argc == 1)
    {
      /*
       * We accept "some" versions... But cross-version connections are
       * not recommended...
       */
      for (i = 0; ACCEPTED_VERSIONS[i] != NULL; ++i)
	{
	  if (strcmp (mess->argv[0], ACCEPTED_VERSIONS[i]) == 0)
	    {
	      (*ok) = 1;
	      result = 1;
	    }
	}
    }

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&sock, LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&sock, LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "PASSWORD" message
 */
int
lw_srvchan_handle_password (int sock, LW_NETMESS * mess,
			    int *ok, char *password)
{
  int result = 0;

  switch (mess->argc)
    {
    case 1:
      /*
       * OK, there's an argument, we check if it's the good password.
       */
      if (strncmp (mess->argv[0], password, PASSWORD_SIZE) == 0)
	{
	  (*ok) = 1;
	  result = 1;
	}
      break;
    }

  if (!strcmp (password, ""))
    {
      /*
       * OK, now the server's password is "", so we consider any
       * that any password from the client is OK.
       */
      (*ok) = 1;
      result = 1;
    }

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&sock, LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&sock, LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "FREE" message
 */
int
lw_srvchan_handle_free (int sock, LW_NETMESS * mess, int free_teams, int *ok)
{
  int result = 0;
  char answer[LW_SOCK_MESSAGE_SIZE];

  LW_MACRO_NOP (mess);

  LW_MACRO_SPRINTF2 (answer, "%s %d", LW_NETMESS_TEXT_OK, free_teams);

  lw_sock_send_str (&sock, answer);

  (*ok) = 1;
  result = 1;

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "TEAM" message
 */
static int
handle_team (LW_SRVCHAN * chan, LW_NETMESS * mess, int *free_teams)
{
  int result = 0;
  int i;

  if (mess->argc == 3)
    {
      /*
       * OK, there are 3 arguments
       * - arg 1 is the team index
       * - arg 2 is the "control_type" parameter 
       * - arg 3 is the team name
       */
      i = atoi (mess->argv[0]);
      if (i >= 0 && i < NB_TEAMS)
	{
	  if (!chan->team[i].active)
	    {
	      /*
	       * The message from the client makes sense, we
	       * return OK
	       */
	      result = 1;

	      /*
	       * Now we activate the team only if there's still
	       * room. If not we still return OK but the "WHO"
	       * message will later inform the client that some
	       * of his requests have been cancelled...
	       */
	      if ((*free_teams) > 0)
		{
		  (*free_teams)--;
		  chan->team[i].active = 1;

		  chan->team[i].control_type = atoi (mess->argv[1]);
		  strncpy (chan->team[i].name, mess->argv[2], NAME_SIZE);
		  chan->team[i].name[NAME_SIZE] = '\0';

		  log_print_str ("Team \"");
		  log_print_str (mess->argv[2]);
		  log_print_str ("\" on client \"");
		  log_print_str (chan->ip);
		  log_print_str (":");
		  log_print_int (chan->port);
		  log_println_str ("\" accepted");
		}
	      else
		{
		  log_print_str ("Team \"");
		  log_print_str (mess->argv[2]);
		  log_print_str ("\" on client \"");
		  log_print_str (chan->ip);
		  log_print_str (":");
		  log_print_int (chan->port);
		  log_println_str ("\" refused!");
		}
	    }
	}
    }

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "WHO" message
 */
static int
handle_who (LW_SRVCHAN * chan, LW_NETMESS * mess)
{
  int result = 0;
  int i;
  char answer[LW_SOCK_MESSAGE_SIZE];

  if (mess->argc == 1)
    {
      /*
       * OK, there's an argument, we interpret it as a number
       */
      i = atoi (mess->argv[0]);
      if (i >= 0 && i < NB_TEAMS)
	{
	  LW_MACRO_SPRINTF1 (answer, "%s", LW_NETMESS_TEXT_ERR);

	  if (chan->team[i].active)
	    {
	      if (chan->team[i].network)
		{
		  LW_MACRO_SPRINTF4 (answer,
				     "%s %d,%d,'%s'",
				     LW_NETMESS_TEXT_NETWORK,
				     chan->team[i].server_id,
				     chan->team[i].control_type,
				     chan->team[i].name);
		}
	      else
		{
		  LW_MACRO_SPRINTF4 (answer,
				     "%s %d,%d,'%s'",
				     LW_NETMESS_TEXT_LOCAL,
				     chan->team[i].server_id,
				     chan->team[i].control_type,
				     chan->team[i].name);
		}
	    }
	  else
	    {
	      LW_MACRO_SPRINTF1 (answer, "%s", LW_NETMESS_TEXT_NOBODY);
	    }

	  lw_sock_send_str (&(chan->sock), answer);
	  result = 1;
	}
    }

  /*
   * Now we send feedback on the network
   */
  if (!result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "READY" message
 */
static int
handle_ready (LW_SRVCHAN * chan, LW_NETMESS * mess)
{
  int result = 0;

  LW_MACRO_NOP (mess);

  /*
   * Here we do not return "OK" yet. In fact we will return
   * "OK" on the network later, when all the clients will be
   * connected, this way the "OK" answer to the "READY" message
   * is the signal for game start
   */
  chan->ready_ok = 1;
  result = 1;

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "START" message
 */
static int
handle_start (LW_SRVCHAN * chan, LW_NETMESS * mess)
{
  int result = 0;

  LW_MACRO_NOP (mess);

  /*
   * Here we just acknowledge the fact that the client required
   * the game to start right away
   */
  chan->start = 1;
  result = 1;

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "WAITING" message
 */
static int
handle_waiting (LW_SRVCHAN * chan, LW_NETMESS * mess, int free_teams)
{
  int result = 0;
  char answer[LW_SOCK_MESSAGE_SIZE];

  LW_MACRO_NOP (mess);

  /*
   * Here we answer the client the number of free teams
   */
  result = 1;

  LW_MACRO_SPRINTF2 (answer, "%s %d", LW_NETMESS_TEXT_OK, free_teams);

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&(chan->sock), answer);
    }
  else
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "NEXT" message
 */
static int
handle_next (LW_SRVCHAN * chan, LW_NETMESS * mess, int *next)
{
  int result = 0;

  LW_MACRO_NOP (chan);
  LW_MACRO_NOP (mess);

  /*
   * Here we do not return "OK" yet.
   * The point is that the "NEXT" message is like the "READY"
   * message, it gets an answer when the server decides to
   * talk to the client again.
   */
  (*next) = 1;
  result = 1;

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "TEAMSTARTINFO" message
 */
static int
handle_teamstartinfo (LW_SRVCHAN * chan, LW_NETMESS * mess,
		      LW_TEAMSTARTINFO * team_start_info)
{
  int result = 0;
  int i;
  char answer[LW_SOCK_MESSAGE_SIZE];

  if (mess->argc == 1)
    {
      /*
       * OK, there's an argument, we interpret it as a number
       */
      i = atoi (mess->argv[0]);
      if (i >= 0 && i < NB_TEAMS)
	{
	  LW_MACRO_SPRINTF4 (answer,
			     "%s %d,%d,'%s'",
			     LW_NETMESS_TEXT_OK,
			     team_start_info[i].active,
			     team_start_info[i].start,
			     team_start_info[i].name);

	  lw_sock_send_str (&(chan->sock), answer);

	  result = 1;
	}
    }

  /*
   * Now we send feedback on the network
   */
  if (!result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "CHATTALK" message
 */
static int
handle_chattalk (LW_SRVCHAN * chan, LW_NETMESS * mess, LW_CHAT_HISTORY * chat)
{
  int result = 0;

  if (mess->argc == 1)
    {
      /*
       * OK, there's an argument, we interpret it as a message.
       * Note that we push the message in a common chat history
       * struct, to share it with others.
       */
      log_print_str ("Client \"");
      log_print_str (chan->ip);
      log_print_str (":");
      log_print_int (chan->port);
      log_print_str ("\" says \"");
      log_print_str (mess->argv[0]);
      log_println_str ("\"");

      lw_chat_push (chat, chan->team[0].name, mess->argv[0]);

      result = 1;
    }

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "CHATLISTEN" message
 */
static int
handle_chatlisten (LW_SRVCHAN * chan, LW_NETMESS * mess)
{
  int result = 0;
  char answer[LW_SOCK_MESSAGE_SIZE];
  LW_CHAT_MESSAGE message;

  if (mess->argc == 0)
    {
      if (lw_chat_pop (&(chan->chat), &message))
	{
	  /*
	   * OK, there's a message in the queue, we send it
	   */
	  LW_MACRO_SPRINTF3 (answer,
			     "%s '%s','%s'",
			     LW_NETMESS_TEXT_OK,
			     message.author, message.content);
	}
      else
	{
	  LW_MACRO_SPRINTF0 (answer, LW_NETMESS_TEXT_OK);
	}

      lw_sock_send_str (&(chan->sock), answer);

      result = 1;
    }

  /*
   * Send error message if needed
   */
  if (!result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "SENDMAP" message
 */
static int
handle_sendmap (LW_SRVCHAN * chan, LW_NETMESS * mess, void **map)
{
  int result = 0;

  LW_MACRO_NOP (mess);

  result = lw_netmap_recv (&(chan->sock), map);

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }


  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "RECVMAP" message
 */
static int
handle_recvmap (LW_SRVCHAN * chan, LW_NETMESS * mess, void *map)
{
  int result = 0;

  LW_MACRO_NOP (mess);

  result = lw_netmap_send (&(chan->sock), map);

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "SENDCONFIG" message
 */
static int
handle_sendconfig (LW_SRVCHAN * chan, LW_NETMESS * mess, LW_NETCONF * config)
{
  int result = 0;

  LW_MACRO_NOP (mess);

  result = lw_netconf_recv (&(chan->sock), config);

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }


  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "RECVCONFIG" message
 */
static int
handle_recvconfig (LW_SRVCHAN * chan, LW_NETMESS * mess, LW_NETCONF * config)
{
  int result = 0;

  LW_MACRO_NOP (mess);

  result = lw_netconf_send (&(chan->sock), config);

  /*
   * Now we send feedback on the network
   */
  if (result)
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK);
    }
  else
    {
      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles the "QUIT" message
 */
int
lw_srvchan_handle_quit (int sock, LW_NETMESS * mess, int *normal_quit)
{
  int result = 0;

  LW_MACRO_NOP (sock);
  LW_MACRO_NOP (mess);

  /*
   * We flag the client as having quitted the server "normally".
   * This allow calling functions to make the difference between
   * a client that decided to leave on purpose from a client
   * that was disconnected because of lag or something.
   */
  (*normal_quit) = 1;

  /*
   * Here we do nothing and simply return an error, this will
   * cause the system to close the socket automatically...
   */
  result = -1;

  return result;
}

/*------------------------------------------------------------------*/
/*
 * handles an unknown message
 */
int
lw_srvchan_handle_unknown (int sock, LW_NETMESS * mess)
{
  int result = 0;

  LW_MACRO_NOP (mess);

  /*
   * We answer ERR all the time
   */
  lw_sock_send_str (&sock, LW_NETMESS_TEXT_ERR);

  /*
   * Since we return 0, the connection will be closed...
   */
  return result;
}

/*==================================================================*/
/* global functions                                                 */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Waits for a client to connect, returns 1 if successfull, 0 if not
 */
int
lw_srvchan_wait_teams (LW_SRVCHAN * chan, int *free_teams,
		       int sock, void **map, LW_NETCONF * config,
		       char *password)
{
  int result = 0;
  int ret;
  int ret_mess = 1;
  char message[LW_SOCK_MESSAGE_SIZE];
  LW_NETMESS *mess;
  int free_received = 0;
  reset (chan);

  if (lw_sock_accept (&(chan->sock), chan->ip, &(chan->port), sock, 1))
    {
      log_print_str ("Connection from \"");
      log_print_str (chan->ip);
      log_print_str (":");
      log_print_int (chan->port);
      log_print_str ("\"");
      log_println ();
      log_flush ();

      while ((!chan->ready_ok) && ret_mess &&
	     ((ret = lw_sock_recv_str (&(chan->sock), message)) >= 0))
	{
	  if (ret > 0)
	    {
	      mess = lw_netmess_read (message);
	      if (mess != NULL)
		{
		  /*
		   * switch to handle the different possible messages
		   */
		  switch (mess->code)
		    {
		    case LW_NETMESS_CODE_PING:
		      ret_mess = lw_srvchan_handle_ping (chan->sock, mess);
		      break;
		    case LW_NETMESS_CODE_ECHO:
		      ret_mess = lw_srvchan_handle_echo (chan->sock, mess);
		      break;
		    case LW_NETMESS_CODE_PROGRAM:
		      ret_mess = lw_srvchan_handle_program (chan->sock, mess,
							    &
							    (chan->
							     program_ok));
		      break;
		    case LW_NETMESS_CODE_VERSION:
		      ret_mess = lw_srvchan_handle_version (chan->sock, mess,
							    &
							    (chan->
							     version_ok));
		      break;
		    case LW_NETMESS_CODE_PASSWORD:
		      ret_mess = lw_srvchan_handle_password (chan->sock, mess,
							     &
							     (chan->
							      password_ok),
							     password);
		      break;
		    case LW_NETMESS_CODE_FREE:
		      ret_mess = lw_srvchan_handle_free (chan->sock, mess,
							 *free_teams,
							 &free_received);
		      break;
		    case LW_NETMESS_CODE_TEAM:
		      ret_mess = handle_team (chan, mess, free_teams);
		      break;
		    case LW_NETMESS_CODE_READY:
		      ret_mess = handle_ready (chan, mess);
		      break;
		    case LW_NETMESS_CODE_SENDMAP:
		      ret_mess = handle_sendmap (chan, mess, map);
		      break;
		    case LW_NETMESS_CODE_SENDCONFIG:
		      ret_mess = handle_sendconfig (chan, mess, config);
		      break;
		    case LW_NETMESS_CODE_QUIT:
		      ret_mess = lw_srvchan_handle_quit (chan->sock, mess,
							 &
							 (chan->normal_quit));
		      break;
		    default:
		      ret_mess = lw_srvchan_handle_unknown (sock, mess);
		      break;
		    }

		  /*
		   * lw_netmess_free must *not* be forgetted 
		   */
		  lw_netmess_free (mess);

		  if (!ret_mess)
		    {
		      log_print_str ("Error processing \"");
		      log_print_str (message);
		      log_print_str ("\"");
		      log_println ();
		    }
		}
	      else
		{
		  log_println_str ("Serious memory problem 8-(");
		  ret_mess = 0;
		}
	    }
	}

      if (chan->program_ok &&
	  chan->version_ok && chan->password_ok && chan->ready_ok && ret_mess)
	{
	  result = 1;

	  log_print_str ("Client \"");
	  log_print_str (chan->ip);
	  log_print_str (":");
	  log_print_int (chan->port);
	  log_println_str ("\" accepted");

	  /* 
	   * We play a little sound for people who have the server
	   * opened on a console and want it to beep when someone
	   * connects
	   */
	  log_beep ();
	}
      else
	{
	  if (chan->ready_ok)
	    {
	      /*
	       * Now if we are here it means we have received a "READY"
	       * message, but still the client did not send the correct
	       * messages before => we send an "ERR" messageon the
	       * network for consistency, since the client did not receive
	       * any message to his "READY" query yet.
	       */
	      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
	    }
	  lw_sock_close (&(chan->sock));

	  log_print_str ("Client \"");
	  log_print_str (chan->ip);
	  log_print_str (":");
	  log_print_int (chan->port);
	  if (chan->normal_quit)
	    {
	      log_println_str ("\" disconnected (probably a ping)");
	    }
	  else
	    {
	      log_println_str ("\" rejected!");
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends informations to the client on a regular basis, once it's
 * connected and waiting for the game to start
 */
int
lw_srvchan_keepalive (LW_SRVCHAN * chan,
		      int waited_teams,
		      LW_TEAMSTARTINFO * team_start_info,
		      LW_CHAT_HISTORY * chat)
{
  int result = 0;
  int ret;
  int ret_mess = 1;
  char message[LW_SOCK_MESSAGE_SIZE];
  LW_NETMESS *mess;
  int next = 0;

  /*
   * we set ready_ok back to false since we will wait for
   * another ready message
   */
  chan->ready_ok = 0;

  /*
   * We first answer the "READY" message which had been sent by the
   * client a possibly long time ago.
   */
  if (lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK))
    {
      while ((!chan->ready_ok) && (!next) && ret_mess &&
	     ((ret = lw_sock_recv_str (&(chan->sock), message)) >= 0))
	{
	  if (ret > 0)
	    {
	      mess = lw_netmess_read (message);
	      if (mess != NULL)
		{
		  /*
		   * switch to handle the different possible messages
		   */
		  switch (mess->code)
		    {
		    case LW_NETMESS_CODE_PING:
		      ret_mess = lw_srvchan_handle_ping (chan->sock, mess);
		      break;
		    case LW_NETMESS_CODE_ECHO:
		      ret_mess = lw_srvchan_handle_echo (chan->sock, mess);
		      break;
		    case LW_NETMESS_CODE_START:
		      ret_mess = handle_start (chan, mess);
		      break;
		    case LW_NETMESS_CODE_WAITING:
		      ret_mess = handle_waiting (chan, mess, waited_teams);
		      break;
		    case LW_NETMESS_CODE_NEXT:
		      ret_mess = handle_next (chan, mess, &next);
		      break;
		    case LW_NETMESS_CODE_TEAMSTARTINFO:
		      ret_mess = handle_teamstartinfo (chan, mess,
						       team_start_info);
		      break;
		    case LW_NETMESS_CODE_CHATTALK:
		      ret_mess = handle_chattalk (chan, mess, chat);
		      break;
		    case LW_NETMESS_CODE_CHATLISTEN:
		      ret_mess = handle_chatlisten (chan, mess);
		      break;
		    case LW_NETMESS_CODE_READY:
		      ret_mess = handle_ready (chan, mess);
		      break;
		    case LW_NETMESS_CODE_QUIT:
		      ret_mess = lw_srvchan_handle_quit (chan->sock, mess,
							 &
							 (chan->normal_quit));
		      break;
		    default:
		      ret_mess = lw_srvchan_handle_unknown (chan->sock, mess);
		      break;
		    }

		  /*
		   * lw_netmess_free must *not* be forgetted 
		   */
		  lw_netmess_free (mess);

		  if (!ret_mess)
		    {
		      log_print_str ("Error processing \"");
		      log_print_str (message);
		      log_print_str ("\"");
		      log_println ();
		    }
		}
	      else
		{
		  log_println_str ("Serious memory problem 8-(");
		  ret_mess = 0;
		}
	    }
	}

      if ((chan->ready_ok || next) && ret_mess)
	{
	  result = 1;
	  if (chan->ready_ok)
	    {
	      log_print_str ("Client \"");
	      log_print_str (chan->ip);
	      log_print_str (":");
	      log_print_int (chan->port);
	      log_println_str ("\" ready");
	    }
	}
      else
	{
	  if (chan->ready_ok || next)
	    {
	      /*
	       * Now if we are here it means we have received a "READY"
	       * message, but still the client did not send the correct
	       * messages before => we send an "ERR" messageon the
	       * network for consistency, since the client did not receive
	       * any message to his "READY" query yet.
	       */
	      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
	    }
	  lw_sock_close (&(chan->sock));

	  //log_println_str("Player KO");
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Answers OK to a client, and waits for the "WHO" message 
 */
int
lw_srvchan_tell_who (LW_SRVCHAN * chan, void *map, LW_NETCONF * config)
{
  int result = 0;
  int ret;
  int ret_mess = 1;
  char message[LW_SOCK_MESSAGE_SIZE];
  LW_NETMESS *mess;

  log_print_str ("Sending info to \"");
  log_print_str (chan->ip);
  log_print_str (":");
  log_print_int (chan->port);
  log_print_str ("\"");
  log_println ();

  /*
   * we set ready_ok back to false since we will wait for
   * another ready message
   */
  chan->ready_ok = 0;

  /*
   * We first answer the "READY" message which had been sent by the
   * client a possibly long time ago.
   */

  if (lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK))
    {
      while ((!chan->ready_ok) && ret_mess &&
	     ((ret = lw_sock_recv_str (&(chan->sock), message)) >= 0))
	{
	  if (ret > 0)
	    {
	      mess = lw_netmess_read (message);
	      if (mess != NULL)
		{
		  /*
		   * switch to handle the different possible messages
		   */
		  switch (mess->code)
		    {
		    case LW_NETMESS_CODE_PING:
		      ret_mess = lw_srvchan_handle_ping (chan->sock, mess);
		      break;
		    case LW_NETMESS_CODE_ECHO:
		      ret_mess = lw_srvchan_handle_echo (chan->sock, mess);
		      break;
		    case LW_NETMESS_CODE_WHO:
		      ret_mess = handle_who (chan, mess);
		      break;
		    case LW_NETMESS_CODE_READY:
		      ret_mess = handle_ready (chan, mess);
		      break;
		    case LW_NETMESS_CODE_RECVMAP:
		      ret_mess = handle_recvmap (chan, mess, map);
		      break;
		    case LW_NETMESS_CODE_RECVCONFIG:
		      ret_mess = handle_recvconfig (chan, mess, config);
		      break;
		    default:
		      ret_mess = lw_srvchan_handle_unknown (chan->sock, mess);
		      break;
		    }

		  /*
		   * lw_netmess_free must *not* be forgetted 
		   */
		  lw_netmess_free (mess);

		  if (!ret_mess)
		    {
		      log_print_str ("Error processing \"");
		      log_print_str (message);
		      log_print_str ("\"");
		      log_println ();
		    }
		}
	      else
		{
		  log_println_str ("Serious memory problem 8-(");
		  ret_mess = 0;
		}
	    }
	}

      if (chan->ready_ok && ret_mess)
	{
	  result = 1;
	  //      log_println_str("Player OK");
	}
      else
	{
	  if (chan->ready_ok)
	    {
	      /*
	       * Now if we are here it means we have received a "READY"
	       * message, but still the client did not send the correct
	       * messages before => we send an "ERR" messageon the
	       * network for consistency, since the client did not receive
	       * any message to his "READY" query yet.
	       */
	      lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_ERR);
	    }
	  lw_sock_close (&(chan->sock));

	  //log_println_str("Player KO");
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends the final "OK" message before streaming
 */
int
lw_srvchan_final_ok (LW_SRVCHAN * chan)
{
  int result = 0;

  if (lw_sock_send_str (&(chan->sock), LW_NETMESS_TEXT_OK))
    {
      result = 1;
    }
  else
    {
      lw_sock_close (&(chan->sock));
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns the index of the team associated to a given server team id
 */
int
lw_srvchan_find_team_by_server_id (LW_SRVCHAN * chan, int server_id)
{
  int result = -1;
  int i;

  for (i = 0; i < NB_TEAMS && result < 0; ++i)
    {
      if (chan->team[i].active && chan->team[i].server_id == server_id)
	{
	  result = i;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Returns the index of the first inactive team
 */
int
lw_srvchan_find_first_free_team (LW_SRVCHAN * chan)
{
  int result = -1;
  int i;

  for (i = 0; i < NB_TEAMS && result < 0; ++i)
    {
      if (!(chan->team[i].active))
	{
	  result = i;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Closes everything and frees the resources
 */
void
lw_srvchan_close (LW_SRVCHAN * chan)
{
  lw_sock_close (&(chan->sock));

  if (chan->map != NULL)
    {
      free (chan->map);
      chan->map = NULL;
    }

  reset (chan);
}

/*------------------------------------------------------------------*/
/*
 * Receives key presses from a computer.
 * The keys come in the "computer's order" ie one still has to
 * sort them by server id. 
 */
int
lw_srvchan_recv_keys (LW_SRVCHAN * chan, LW_NETKEY * netkey)
{
  int result = 0;
  int i;
  LW_NETKEY chan_netkey;

  lw_netkey_reset (&chan_netkey);

  result = lw_netkey_recv (&(chan->sock), &chan_netkey);

  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (chan->team[i].active && !chan->team[i].network)
	{
	  netkey->key_states[chan->team[i].server_id] =
	    chan_netkey.key_states[i];
	}
    }

  netkey->cmd = chan_netkey.cmd;
  netkey->arg = chan_netkey.arg;
  chan->checksum_recv = chan_netkey.checksum;

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends key presses to a computer.
 * The keys must be send in the "computer's order" ie they must
 * be sorted first. 
 */
int
lw_srvchan_send_keys (LW_SRVCHAN * chan, LW_NETKEY * netkey)
{
  int result = 0;
  int i;
  LW_NETKEY chan_netkey;

  lw_netkey_reset (&chan_netkey);

  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (chan->team[i].active)
	{
	  chan_netkey.key_states[i] =
	    netkey->key_states[chan->team[i].server_id];
	}
    }

  chan_netkey.cmd = netkey->cmd;
  chan_netkey.arg = netkey->arg;
  chan_netkey.checksum = chan->checksum_send;

  result = lw_netkey_send (&(chan->sock), &chan_netkey);

  return result;
}
