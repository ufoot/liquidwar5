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
/* Copyright (C) 1998-2011 Christian Mauduit                                 */
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
/* name          : protocol.c                                       */
/* content       : messages sent by the client when connecting      */
/* last update   : April 16th 2001                                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "sock2gen.h"
#include "netmess.h"
#include "netmap.h"
#include "netconf.h"
#include "protocol.h"
#include "macro.h"

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static int recv_ok (int *sock);

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static int
recv_ok (int *sock)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];
  int ret = 0;
  LW_NETMESS *netmess;

  while (*sock >= 0 && (ret = lw_sock_recv_str (sock, message)) == 0);
  if (*sock >= 0 && ret > 0)
    {
      netmess = lw_netmess_read (message);
      if (netmess != NULL)
	{
	  if (netmess->code == LW_NETMESS_CODE_OK && netmess->argc == 0)
	    {
	      result = 1;
	    }
	  lw_netmess_free (netmess);
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends a dummy "PING" message to the server, to check that the
 * connection is working OK.
 */
int
lw_protocol_do_ping (int *sock)
{
  int result = 0;

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_PING))
    {
      if (recv_ok (sock))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends the name of the running client to the server. This is to
 * prevent a liquidwar client to connect itself on something else
 * than a real liquidwar server.
 */
int
lw_protocol_tell_program (int *sock, char *program)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];

  LW_MACRO_SPRINTF2 (message, "%s '%s'", LW_NETMESS_TEXT_PROGRAM, program);
  if (*sock >= 0 && lw_sock_send_str (sock, message))
    {
      if (recv_ok (sock))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends the version of the client to the server.
 */
int
lw_protocol_tell_version (int *sock, char *version)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];

  LW_MACRO_SPRINTF2 (message, "%s '%s'", LW_NETMESS_TEXT_VERSION, version);
  if (*sock >= 0 && lw_sock_send_str (sock, message))
    {
      if (recv_ok (sock))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends the game password to the server.
 */
int
lw_protocol_tell_password (int *sock, char *password)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];

  LW_MACRO_SPRINTF2 (message, "%s '%s'", LW_NETMESS_TEXT_PASSWORD, password);
  if (*sock >= 0 && lw_sock_send_str (sock, message))
    {
      if (recv_ok (sock))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Asks the server how many players can connect to it.
 */
int
lw_protocol_ask_free_teams (int *sock, int *nb)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];
  int ret = 0;
  LW_NETMESS *netmess;

  *nb = 0;
  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_FREE))
    {
      while (*sock >= 0 && (ret = lw_sock_recv_str (sock, message)) == 0);
      if (*sock >= 0 && ret > 0)
	{
	  netmess = lw_netmess_read (message);
	  if (netmess != NULL)
	    {
	      if (netmess->code == LW_NETMESS_CODE_OK && netmess->argc == 1)
		{
		  *nb = atoi (netmess->argv[0]);
		  result = 1;
		}
	      lw_netmess_free (netmess);
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Inform the server that this player will be playing.
 */
int
lw_protocol_reserve_team (int *sock, int num, int control_type, char *name)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];

  LW_MACRO_SPRINTF4 (message, "%s %d,%d,'%s'",
		     LW_NETMESS_TEXT_TEAM, num, control_type, name);
  if (*sock >= 0 && lw_sock_send_str (sock, message))
    {
      if (recv_ok (sock))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Ask the server who is playing on a given channel
 */
int
lw_protocol_ask_who (int *sock, int num, LW_WHO * who)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];
  int ret = 0;
  LW_NETMESS *netmess;

  who->active = 0;
  who->network = 0;
  who->server_id = -1;
  who->name[0] = '\0';

  LW_MACRO_SPRINTF2 (message, "%s %d", LW_NETMESS_TEXT_WHO, num);
  if (*sock >= 0 && lw_sock_send_str (sock, message))
    {
      while (*sock >= 0 && (ret = lw_sock_recv_str (sock, message)) == 0);
      if (*sock >= 0 && ret > 0)
	{
	  netmess = lw_netmess_read (message);
	  if (netmess != NULL)
	    {
	      switch (netmess->code)
		{
		case LW_NETMESS_CODE_NOBODY:
		  if (netmess->argc == 0)
		    {
		      /*
		       * We leave the server id to the default -1 value
		       */
		      result = 1;
		    }
		  break;
		case LW_NETMESS_CODE_NETWORK:
		  who->network = 1;
		  /*
		   * no "break" here since the only difference between
		   * the LOCAL and NETWORK messages is that the network
		   * flag is set to 1 in the "NETWORK" case.
		   */
		case LW_NETMESS_CODE_LOCAL:
		  if (netmess->argc == 3)
		    {
		      who->active = 1;
		      who->server_id = atoi (netmess->argv[0]);
		      who->control_type = atoi (netmess->argv[1]);
		      strncpy (who->name, netmess->argv[2], NAME_SIZE);
		      who->name[NAME_SIZE] = '\0';
		      result = 1;
		    }
		  break;
		}
	      lw_netmess_free (netmess);
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends the selected map to the server
 */
int
lw_protocol_send_map (int *sock, void *map)
{
  int result = 0;

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_SENDMAP))
    {
      if (lw_netmap_send (sock, map))
	{
	  if (recv_ok (sock))
	    {
	      result = 1;
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Receives the map from the server
 */
int
lw_protocol_recv_map (int *sock, void **map)
{
  int result = 0;

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_RECVMAP))
    {
      if (lw_netmap_recv (sock, map))
	{
	  if (recv_ok (sock))
	    {
	      result = 1;
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends the config to the server
 */
int
lw_protocol_send_config (int *sock)
{
  int result = 0;
  LW_NETCONF config;

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_SENDCONFIG))
    {

      memcpy (&config, &LW_CONFIG_CURRENT_RULES, sizeof (LW_NETCONF));

      if (lw_netconf_send (sock, &config))
	{
	  if (recv_ok (sock))
	    {
	      result = 1;
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Receives the config from the server
 */
int
lw_protocol_recv_config (int *sock)
{
  int result = 0;
  LW_NETCONF config;

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_RECVCONFIG))
    {
      if (lw_netconf_recv (sock, &config))
	{
	  if (recv_ok (sock))
	    {
	      memcpy (&LW_CONFIG_CURRENT_RULES, &config, sizeof (LW_NETCONF));

	      result = 1;
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Tell the server that the client is ready to play
 */
int
lw_protocol_i_am_ready (int *sock)
{
  int result = 0;

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_READY))
    {
      if (recv_ok (sock))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Tells the server one is waiting for the game to start
 */
int
lw_protocol_waiting (int *sock, int *waited_teams)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];
  int ret = 0;
  LW_NETMESS *netmess;

  /*
   * Previous versions of LW used to set "waited teams" to
   * 0 here. This caused weird bugs since this affectation
   * is done in a separate thread, so the main thread would
   * see "waited_teams==0" and then would try to start a game...
   * All this ended in an "Error processing WHO 0" on the server.
   * So it's very important *not* to set it to 0. We do not need
   * to set it to any value in fact...
   */
  /*
   * (*waited_teams)=NB_TEAMS;
   */

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_WAITING))
    {
      while (*sock >= 0 && (ret = lw_sock_recv_str (sock, message)) == 0);
      if (*sock >= 0 && ret > 0)
	{
	  netmess = lw_netmess_read (message);
	  if (netmess != NULL)
	    {
	      if (netmess->code == LW_NETMESS_CODE_OK && netmess->argc == 1)
		{
		  *waited_teams = atoi (netmess->argv[0]);
		  result = 1;
		}
	      lw_netmess_free (netmess);
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Forces the server to start the game
 */
int
lw_protocol_force_start (int *sock)
{
  int result = 0;

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_START))
    {
      if (recv_ok (sock))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends a "NEXT" message
 */
int
lw_protocol_next (int *sock)
{
  int result = 0;

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_NEXT))
    {
      if (recv_ok (sock))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends a "TEAMSTARTINFO" message
 */
int
lw_protocol_ask_teamstartinfo (int *sock, int team,
			       LW_TEAMSTARTINFO * team_start_info)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];
  int ret = 0;
  LW_NETMESS *netmess;

  lw_teamstartinfo_reset (team_start_info);

  LW_MACRO_SPRINTF2 (message, "%s %d", LW_NETMESS_TEXT_TEAMSTARTINFO, team);
  if (*sock >= 0 && lw_sock_send_str (sock, message))
    {
      while (*sock >= 0 && (ret = lw_sock_recv_str (sock, message)) == 0);
      if (*sock >= 0 && ret > 0)
	{
	  netmess = lw_netmess_read (message);
	  if (netmess != NULL)
	    {
	      switch (netmess->code)
		{
		case LW_NETMESS_CODE_OK:
		  if (netmess->argc == 3)
		    {
		      team_start_info->active = atoi (netmess->argv[0]);
		      team_start_info->start = atoi (netmess->argv[1]);
		      strncpy (team_start_info->name, netmess->argv[2],
			       NAME_SIZE);
		      team_start_info->name[NAME_SIZE] = '\0';

		      result = 1;
		    }
		  break;
		}
	      lw_netmess_free (netmess);
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends a "CHATTALK" message
 */
int
lw_protocol_chat_talk (int *sock, char *text)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];

  LW_MACRO_SPRINTF2 (message, "%s '%s'", LW_NETMESS_TEXT_CHATTALK, text);

  if (*sock >= 0 && lw_sock_send_str (sock, message))
    {
      if (recv_ok (sock))
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends a "CHATLISTEN" message
 */
int
lw_protocol_chat_listen (int *sock, LW_CHAT_HISTORY * chat_history)
{
  int result = 0;
  char message[LW_SOCK_MESSAGE_SIZE];
  int ret = 0;
  LW_NETMESS *netmess;

  if (*sock >= 0 && lw_sock_send_str (sock, LW_NETMESS_TEXT_CHATLISTEN))
    {
      while (*sock >= 0 && (ret = lw_sock_recv_str (sock, message)) == 0);
      if (*sock >= 0 && ret > 0)
	{
	  netmess = lw_netmess_read (message);
	  if (netmess != NULL)
	    {
	      switch (netmess->code)
		{
		case LW_NETMESS_CODE_OK:
		  switch (netmess->argc)
		    {
		    case 0:
		      /*
		       * No message, we do nothing
		       */
		      result = 1;
		      break;
		    case 2:
		      /*
		       * Message received, enqueue it
		       */
		      lw_chat_push (chat_history,
				    netmess->argv[0], netmess->argv[1]);
		      result = 1;
		      break;
		    }
		  break;
		}
	      lw_netmess_free (netmess);
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends a "QUIT" message to the server, in case it wants to make
 * the difference between a client that disconnects on purpose
 * and someones that has network problems.
 */
int
lw_protocol_quit (int *sock)
{
  int result = 0;

  lw_sock_send_str (sock, LW_NETMESS_TEXT_QUIT);

  /*
   * We leave result to 0, the sockets needs to be closed wether
   * the QUIT message was sent or not.
   */

  return result;
}
