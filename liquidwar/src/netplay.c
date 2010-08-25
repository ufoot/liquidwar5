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
/* nom           : netplay.c                                        */
/* content       : function which launches a network game           */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdlib.h>

#include "back.h"
#include "base.h"
#include "config.h"
#include "decal.h"
#include "dialog.h"
#include "help.h"
#include "menu.h"
#include "play.h"
#include "disk.h"
#include "protocol.h"
#include "sockgen.h"
#include "server.h"
#include "network.h"
#include "keyexch.h"
#include "error.h"
#include "internet.h"
#include "netplay.h"
#include "lang.h"
#include "log.h"
#include "connect.h"
#include "thrdgen.h"
#include "exit.h"
#include "capture.h"
#include "random.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_NETPLAY_DELAY 10

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/

typedef struct
{
  int sock;
  char address[LW_SOCK_IP_SIZE];
  int port;
  int running;
  int allow_exit;
  int free_data;
  int result;
}
LW_NETPLAY_CONNECT_DATA;

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static int connect_on_server_start (int *sock, char *ip, int port);
static int connect_on_server_continue (int sock);
static void connect_on_server_start_callback (void *arg);
static int connect_on_server_finish (int sock);

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * The equivalent of play_sequence, but for a network game.
 * Basically, this is the function called when you press "play" 
 * to launch a network game.
 */
int
netplay_sequence ()
{
  int sock = -1;
  int result = 0;

  lw_config_set_current_rules_to_default ();

  if (connect_on_server_start (&sock,
			       CONFIG_SERVER_ADDRESS, CONFIG_SERVER_PORT))
    {
      if (connect_on_server_continue (sock))
	{
	  result = lw_connect_menu (sock);

	  if (result == MENU_OK)
	    {
	      if (connect_on_server_finish (sock))
		{
		  LW_NETWORK_ON = 1;
		  lw_network_attribute_colors ();
		  lw_network_attribute_parts ();
		  result = play_sequence_ex ();
		}
	    }
	}
    }

  LW_KEYEXCH_SOCK = -1;
  lw_sock_close (&sock);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Opens the socket on the server. This is a little complex, for we
 * fire an extra thread so that if things go slow, the player can
 * use ESCAPE to stop everything.
 */
static int
connect_on_server_start (int *sock, char *address, int port)
{
  int result = 1;
  LW_NETPLAY_CONNECT_DATA *connect_data;
  LW_NETPLAY_CONNECT_DATA connect_data_stack;
  int esc_pressed = 0;
  int dummy;

  connect_data = (LW_NETPLAY_CONNECT_DATA *)
    malloc (sizeof (LW_NETPLAY_CONNECT_DATA));
  if (!connect_data)
    {
      /*
       * Couldn't allocate, fallback on stack data
       */
      connect_data = &connect_data_stack;
    }

  connect_data->sock = -1;
  strncpy (connect_data->address, address, sizeof (connect_data->address));
  connect_data->address[sizeof (connect_data->address) - 1] = '\0';
  connect_data->port = port;
  connect_data->running = 0;
  connect_data->allow_exit = 0;
  connect_data->free_data = 1;
  connect_data->result = 0;

  display_center_message (lw_lang_string (LW_LANG_STRING_NETPLAY_CONNECT));

  connect_data->running = 1;
  if (connect_data != &connect_data_stack &&
      lw_thread_start (connect_on_server_start_callback, connect_data))
    {
      /*
       * Everything's OK, do nothing!
       */
    }
  else
    {
      /*
       * Memory or thread error, we're in trouble,
       * we fallback on calling the function in the main thread
       * with the data allocated in the stack.
       */
      connect_data->free_data = 0;
      connect_on_server_start_callback (connect_data);
    }

  /*
   * We wait until the thread is finished and/or the player
   * pressed ESC
   */
  while (connect_data->running && !esc_pressed)
    {
      if (key[KEY_ESC])
	{
	  esc_pressed = 1;
	}
      /*
       * We call keypressed() to force Allegro to call automatically
       * poll_keyboard() if needed
       */
      dummy = keypressed ();
      rest (LW_NETPLAY_DELAY);
      lw_capture_dump_menu ();
      my_exit_poll ();
    }

  result = connect_data->result;
  *sock = connect_data->sock;

  if (!result)
    {
      *sock = -1;
      if (!esc_pressed)
	{
	  error_unable_to_connect ();
	}
    }

  /*
   * We tell the thread that it can exit whenever it wants,
   * we don't care, it has its own data...
   */
  connect_data->allow_exit = 1;

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Callback used by the thread launched by connect_on_server_start
 * to open the TCP/IP connection with the server
 */
static void
connect_on_server_start_callback (void *arg)
{
  LW_NETPLAY_CONNECT_DATA *connect_data;

  connect_data = (LW_NETPLAY_CONNECT_DATA *) arg;

  connect_data->result = lw_sock_connect (&(connect_data->sock),
					  connect_data->address,
					  connect_data->port);
  connect_data->running = 0;

  /*
   * Now we wait until the main threads tells us we can stop.
   * It's important for we must not free the data too early...
   */
  while (!connect_data->allow_exit)
    {
      rest (LW_NETPLAY_DELAY);
    }

  if (connect_data->free_data)
    {
      free (connect_data);
    }
}

/*------------------------------------------------------------------*/
/*
 * Exchanges some messages with the server,
 * and displays message boxes to the player so that he has an idea
 * of what's going on.
 */
static int
connect_on_server_continue (int sock)
{
  int result = 1;
  int free_teams;
  int i;

  if (result == 1)
    {
      display_center_message (lw_lang_string (LW_LANG_STRING_NETPLAY_PING));
      if (!lw_protocol_do_ping (&sock))
	{
	  result = 0;
	  error_not_a_liquidwar_server ();
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_CHECKPROGRAM));
      if (!lw_protocol_tell_program (&sock, LW_PROGRAM))
	{
	  result = 0;
	  error_not_a_liquidwar_server ();
	}
    }
  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_CHECKVERSION));
      if (!lw_protocol_tell_version (&sock, LW_VERSION))
	{
	  result = 0;
	  error_wrong_version ();
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_SENDPASSWORD));
      if (!lw_protocol_tell_password (&sock, CONFIG_PASSWORD))
	{
	  result = 0;
	  error_bad_password ();
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_FREETEAMS));
      if (lw_protocol_ask_free_teams (&sock, &free_teams))
	{
	  if (free_teams <= 0)
	    {
	      result = 0;
	      error_server_full ();
	    }
	}
      else
	{
	  result = 0;
	  error_network_problem ();
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_RESERVETEAMS));
      for (i = 0; i < NB_TEAMS && free_teams > 0 && result == 1; ++i)
	{
	  if (CONFIG_CONTROL_TYPE[i] == CONFIG_CONTROL_TYPE_HUMAN ||
	      ((CONFIG_CONTROL_TYPE[i] == CONFIG_CONTROL_TYPE_CPU) &&
	       CONFIG_ALLOW_NETWORK_BOTS))
	    {
	      if (lw_protocol_reserve_team (&sock, i,
					    CONFIG_CONTROL_TYPE[i],
					    CONFIG_PLAYER_NAME[i]))
		{
		  free_teams--;
		}
	      else
		{
		  result = 0;
		  error_network_problem ();
		}
	    }
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_SENDCONFIG));
      if (!lw_protocol_send_config (&sock))
	{
	  result = 0;
	  error_network_problem ();
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_SENDMAP));
      if (!lw_protocol_send_map
	  (&sock,
	   LW_RANDOM_ON ? LW_RANDOM_RAW_MAP :
	   RAW_MAP_ORDERED[CONFIG_LEVEL_MAP]))
	{
	  result = 0;
	  error_cant_send_map ();
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_WAITING));
      if (!lw_protocol_i_am_ready (&sock))
	{
	  result = 0;
	  error_network_problem ();
	}
    }

  /*
   * if the operation succeded, we set the global value LW_KEYEXCH_SOCK
   * to the right socket, otherwise functions from lw_keyech won't work...
   */
  if (result)
    {
      LW_KEYEXCH_SOCK = sock;
    }
  else
    {
      LW_KEYEXCH_SOCK = -1;
      lw_sock_close (&sock);
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Retrieves informations from the server once all players are
 * connected. Also displays information in a dialog box.
 */
static int
connect_on_server_finish (int sock)
{
  int result = 1;
  int i;

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_WHOPLAYS));
      for (i = 0; i < NB_TEAMS && result == 1; ++i)
	{
	  if (!lw_protocol_ask_who (&sock, i, &(LW_NETWORK_INFO[i])))
	    {
	      result = 0;
	      error_network_problem ();
	    }
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_RECVCONFIG));
      if (!lw_protocol_recv_config (&sock))
	{
	  result = 0;
	  error_network_problem ();
	}
    }

  if (result == 1)
    {
      if (!lw_netconf_check (&LW_CONFIG_CURRENT_RULES))
	{
	  result = 0;
	  error_bad_config_range ();
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_RECVMAP));
      if (!lw_protocol_recv_map (&sock, &LW_NETWORK_RAW_MAP))
	{
	  result = 0;
	  error_cant_receive_map ();
	}
    }

  if (result == 1)
    {
      display_center_message (lw_lang_string
			      (LW_LANG_STRING_NETPLAY_WAITING));
      if (!lw_protocol_i_am_ready (&sock))
	{
	  result = 0;
	  error_network_problem ();
	}
    }

  /*
   * We close the socket if something failed
   */
  if (!result)
    {
      LW_KEYEXCH_SOCK = -1;
      lw_sock_close (&sock);
    }

  return result;
}
