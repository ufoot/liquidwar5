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
/* name          : srvcont.c                                        */
/* content       : global controler of the network channels         */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "srvcont.h"
#include "srvchan.h"
#include "sockgen.h"
#include "sock2gen.h"
#include "log.h"
#include "server.h"
#include "netkey.h"
#include "srvtime.h"
#include "startinf.h"
#include "thrdgen.h"
#include "macro.h"
#include "execgen.h"

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/
typedef struct
{
  int sock;
  char ip[LW_SOCK_IP_SIZE];
  int port;
  int *thread_running;
  char password[PASSWORD_SIZE + 1];
}
LW_SRVCONT_REJECT_DATA;

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_SRVCONT_INCREASE_LAG_LIMIT 10
#define LW_SRVCONT_DECREASE_LAG_LIMIT 20

#define LW_SRVCONT_DISPLAY_STATE_DELAY 60
#define LW_SRVCONT_REGISTER_DELAY      90

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static void reset (LW_SRVCONT * cont);
static int distribute_teams (LW_SRVCONT * cont);
static void recalc_nb_teams (LW_SRVCONT * cont);
static void find_chan_and_team_by_server_id (LW_SRVCONT * cont,
					     int server_id,
					     int *chan, int *team);
static int send_lag_key_presses (LW_SRVCONT * cont, int lag);
static int reject_new_clients (LW_SRVCONT * cont, char *password);
static void reject_new_clients_callback (void *args);
static void disconnect_client (LW_SRVCONT * cont, int i, char *reason);

/*------------------------------------------------------------------*/
/*
 * Resets the LW_SRVCONT struct
 */
static void
reset (LW_SRVCONT * cont)
{
  memset (cont, 0, sizeof (LW_SRVCONT));

  cont->sock = -1;
  cont->nb_teams = 0;
  cont->nb_chans = 0;
}

/*------------------------------------------------------------------*/
/*
 * Affects consistent values to all the teams
 */
static int
distribute_teams (LW_SRVCONT * cont)
{
  int result = 1;
  int i, j, k;
  int i_orig, j_orig;
  int asm_algorithm;

  /*
   * The first step is to attribute a unique number to each
   * local team for each channel.
   */
  k = 0;
  for (i = 0; i < cont->nb_chans; ++i)
    {
      for (j = 0; j < NB_TEAMS; ++j)
	{
	  if (cont->chan[i].team[j].active)
	    {
	      cont->chan[i].team[j].server_id = k++;
	    }
	}
    }

  /*
   * Now k should be equal to the number of available teams,
   * or it means there's a serious error...
   */
  if (k != cont->nb_teams)
    {
      log_println_str ("Unable to attribute teams!");
      result = 0;
    }

  /*
   * The second step is to define which teams in each channel
   * will be the "network" teams.
   */
  for (i = 0; i < cont->nb_chans; ++i)
    {
      for (k = 0; k < cont->nb_teams; ++k)
	{
	  if (lw_srvchan_find_team_by_server_id (&(cont->chan[i]), k) < 0)
	    {
	      j = lw_srvchan_find_first_free_team (&(cont->chan[i]));
	      if (j >= 0)
		{
		  cont->chan[i].team[j].active = 1;
		  cont->chan[i].team[j].network = 1;
		  cont->chan[i].team[j].server_id = k;
		}
	      else
		{
		  result = 0;
		  log_println_str ("Unable to find a free team!");
		}
	    }
	}
    }

  /*
   * Third step, we copy the team names and other parameters in all
   * the teams structures. This is redundant information but it
   * will save useless CPU waste later...
   */
  for (i = 0; i < cont->nb_chans; ++i)
    {
      for (j = 0; j < NB_TEAMS; ++j)
	{
	  if (cont->chan[i].team[j].active)
	    {
	      find_chan_and_team_by_server_id (cont,
					       cont->chan[i].team[j].
					       server_id, &i_orig, &j_orig);
	      if (i_orig >= 0 && j_orig >= 0)
		{
		  /*
		   * Now we check that we're not uselessely copying
		   * stuff from the same struct. It's a waste of time
		   * and can cause some bugs depending on the implementation
		   * of the LW_MACRO_STRCPY macro.
		   */
		  if (i_orig != i || j_orig != j)
		    {
		      LW_MACRO_STRCPY (cont->chan[i].team[j].name,
				       cont->chan[i_orig].team[j_orig].name);
		      cont->chan[i].team[j].control_type =
			cont->chan[i_orig].team[j_orig].control_type;
		    }
		}
	      else
		{
		  result = 0;
		  log_println_str ("Unable to find a free team!");
		}
	    }
	}
    }

  /*
   * Last step, we check if the "asm_algorithm" parameter should be
   * enabled. Indeed, it must be enabled on _all_ computers or on
   * none of them...
   */
  asm_algorithm = 1;
  for (i = 0; i < cont->nb_chans; ++i)
    {
      asm_algorithm = (cont->chan[i].config.asm_algorithm && asm_algorithm);
    }
  for (i = 0; i < cont->nb_chans; ++i)
    {
      cont->chan[i].config.asm_algorithm = asm_algorithm;
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Calculates the cont->nb_teams value
 */
static void
recalc_nb_teams (LW_SRVCONT * cont)
{
  int i, j;

  cont->nb_teams = 0;
  for (i = 0; i < cont->nb_chans; ++i)
    {
      for (j = 0; j < NB_TEAMS; ++j)
	{
	  if (cont->chan[i].team[j].active)
	    {
	      cont->nb_teams++;
	    }
	}
    }
}

/*------------------------------------------------------------------*/
/*
 * Gets the chan and team index for a given server id
 */
static void
find_chan_and_team_by_server_id (LW_SRVCONT * cont,
				 int server_id, int *chan, int *team)
{
  int i;
  int j;

  *chan = *team = -1;
  for (i = 0; i < cont->nb_chans; ++i)
    {
      for (j = 0; j < NB_TEAMS; ++j)
	{
	  if (cont->chan[i].team[j].active &&
	      (cont->chan[i].team[j].server_id == server_id) &&
	      !(cont->chan[i].team[j].network))
	    {
	      (*chan) = i;
	      (*team) = j;
	    }
	}
    }
}

/*------------------------------------------------------------------*/
/*
 * Prepares the LW_TEAMSTARTINFO structures
 */
static void
prepare_team_start_info (LW_TEAMSTARTINFO * team_start_info,
			 LW_SRVCONT * cont)
{
  int i, j, k;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      lw_teamstartinfo_reset (team_start_info + i);
    }

  k = 0;
  for (i = 0; i < cont->nb_chans; ++i)
    {
      for (j = 0; j < NB_TEAMS; ++j)
	{
	  if (cont->chan[i].team[j].active && k < NB_TEAMS)
	    {
	      team_start_info[k].active = 1;
	      team_start_info[k].start = cont->chan[i].start;
	      LW_MACRO_STRCPY (team_start_info[k].name,
			       cont->chan[i].team[j].name);
	      k++;
	    }
	}
    }
}

/*==================================================================*/
/* global functions                                                 */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* 
 * Waits for a given number of teams to connect
 */
int
lw_srvcont_wait_teams (LW_SRVCONT * cont,
		       LW_WWWSRV_DATA * www_data,
		       int nb_teams, int port, char *password, char *callback)
{
  int free_teams = 0;
  int result = 0;
  int i;
  int start;
  int started = 0;
  LW_TEAMSTARTINFO team_start_info[NB_TEAMS];
  int cur_time, last_time;
  int display_waiting_for = 1;
  LW_CHAT_HISTORY chat;
  LW_CHAT_MESSAGE chat_mess;

  reset (cont);

  cur_time = last_time = lw_srvtime_seconds ();

  lw_chat_reset (&chat);

  /*
   * We register the server on the metaserver for the
   * first time, with 0 active teams.
   */
  www_data->busy_players = cont->nb_teams;
  www_data->max_players = nb_teams;
  www_data->log = 1;
  lw_wwwsrv_register (www_data);

  if (lw_sock_listen (&(cont->sock), port))
    {
      log_print_str ("Listening on port ");
      log_print_int (port);
      log_println_str ("...");
      log_flush ();

      /*
       * Now we just wait until there are enough teams connected
       */
      while (!started)
	{
	  free_teams = nb_teams - cont->nb_teams;

	  if (display_waiting_for)
	    {
	      log_print_str ("Waiting for ");
	      log_print_int (free_teams);
	      if (free_teams > 1)
		{
		  log_print_str (" teams...");
		}
	      else
		{
		  log_print_str (" team...");
		}
	      log_println ();

	      display_waiting_for = 0;
	    }

	  if (lw_srvchan_wait_teams (&(cont->chan[cont->nb_chans]),
				     &free_teams,
				     cont->sock,
				     &(cont->chan[cont->nb_chans].map),
				     &(cont->chan[cont->nb_chans].config),
				     password))
	    {
	      if (cont->nb_teams == 0)
		{
		  /*
		   * OK, this guy is the first one to connect on
		   * the server, so we'll fire the "callback" script/program
		   */
		  if (callback && strlen (callback))
		    {
		      log_print_str ("Executing \"");
		      log_print_str (callback);
		      log_println_str ("\"");
		      lw_exec_cmd (callback);
		    }
		}
	      cont->nb_teams = nb_teams - free_teams;
	      ++(cont->nb_chans);
	      display_waiting_for = 1;
	    }

	  /*
	   * Now we check if me must force a game start
	   */
	  start = 1;
	  for (i = 0; i < cont->nb_chans; ++i)
	    {
	      start = start && cont->chan[i].start;
	    }
	  if (cont->nb_teams >= 2 && (start || cont->nb_teams >= nb_teams))
	    {
	      if (cont->nb_teams < nb_teams)
		{
		  /*
		   * OK, all the players have pressed the "start" button and there
		   * are at least 2 playing teams -> we start the game!
		   */
		  nb_teams = cont->nb_teams;
		  log_print_str ("Starting game with only ");
		  log_print_int (cont->nb_teams);
		  log_println_str (" teams");
		}
	      free_teams = 0;
	    }

	  /*
	   * The official start signal is: free_teams is set to 0.
	   * We use an extra flag "started" in case free_teams would
	   * be recalculated
	   */
	  if (free_teams <= 0)
	    {
	      free_teams = 0;
	      started = 1;
	    }


	  /*
	   * We replicate chat messages
	   */
	  while (lw_chat_pop (&chat, &chat_mess))
	    {
	      for (i = 0; i < cont->nb_chans; ++i)
		{
		  lw_chat_push (&(cont->chan[i].chat),
				chat_mess.author, chat_mess.content);
		}
	    }

	  /*
	   * now we send feedback to all the connected teams
	   */
	  prepare_team_start_info (team_start_info, cont);
	  /*
	   * loop with a step of -1 to handle correcty the
	   * possible disconnection of players
	   */
	  for (i = cont->nb_chans - 1; i >= 0; --i)
	    {
	      if (!lw_srvchan_keepalive (&(cont->chan[i]),
					 free_teams, team_start_info, &chat))
		{
		  /*
		   * There's an error with this client, so 
		   * we're simply going to disconnect it...
		   */
		  disconnect_client (cont, i, "keepalive failed");

		  display_waiting_for = 1;
		}
	    }
	  recalc_nb_teams (cont);

	  www_data->busy_players = cont->nb_teams;
	  www_data->max_players = nb_teams;

	  /*
	   * We register the server again since the number of 
	   * connected teams might have changed
	   */
	  cur_time = lw_srvtime_seconds ();
	  if (cur_time - last_time > LW_SRVCONT_REGISTER_DELAY
	      || display_waiting_for)
	    {
	      www_data->log = 0;
	      lw_wwwsrv_register (www_data);
	      last_time = cur_time;
	    }
	}

      /*
       * For now the server is a "blocking" server which is never statisfied
       * until the good number of players has connected itself, so if we
       * get here it means the operation has been successfull
       */
      result = 1;
    }
  else
    {
      log_print_str ("Unable to bind socket on port ");
      log_print_int (port);
      log_println ("!");

      /*
       * We consider this a fatal error and quit the program
       * right away.
       */
      lw_server_exit (1);
    }

  return result;
}

/*------------------------------------------------------------------*/
/* 
 * Tells the teams about what the other teams are
 */
int
lw_srvcont_tell_who (LW_SRVCONT * cont)
{
  int i;
  int result = 0;

  if (distribute_teams (cont))
    {
      result = 1;
      for (i = cont->nb_chans - 1; i >= 0; --i)
	{
	  if (!lw_srvchan_tell_who (&(cont->chan[i]),
				    cont->chan[0].map,
				    &(cont->chan[0].config)))
	    {
	      /*
	       * There's been an error exchanging data with this team
	       * so we decide to "close" it.
	       */
	      disconnect_client (cont, i, "can't send game info");
	    }
	}

      /*
       * We start the game only if there are more than 1 channels opened...
       */
      if (cont->nb_chans > 0)
	{
	  result = 1;
	}
    }

  return result;
}


/*------------------------------------------------------------------*/
/* 
 * Sends a final OK message to all the clients
 */
int
lw_srvcont_final_ok (LW_SRVCONT * cont)
{
  int i;
  int result = 0;

  for (i = cont->nb_chans - 1; i >= 0; --i)
    {
      if (!lw_srvchan_final_ok (&(cont->chan[i])))
	{
	  disconnect_client (cont, i, "can't send final ok");
	}
    }

  /*
   * We start the game only if there are more than 1 channels opened...
   */
  if (cont->nb_chans > 0)
    {
      result = 1;
    }

  if (result)
    {
      log_print_str ("Game start");
      log_println ();
    }
  else
    {
      log_print_str ("Unable to start game");
      log_println ();
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Sends a serie of blank key presses to create an artificial lag.
 * This lag is usefull, since this way, a few game cycles will 
 * have to be run before a real key press comes back to a given
 * player. This leaves times for other players to send their own
 * key presses and this way the game can run smoothly.
 */
static int
send_lag_key_presses (LW_SRVCONT * cont, int lag)
{
  int result = 1;
  int i, j;
  LW_NETKEY netkey;

  lw_netkey_reset (&netkey);

  for (j = 0; j < lag && result; ++j)
    {
      for (i = cont->nb_chans - 1; i >= 0; --i)
	{
	  /*
	   * We put the checksum to 1, to inform the clients that
	   * everything is OK
	   */
	  cont->chan[i].checksum_send = 1;
	  if (!lw_srvchan_send_keys (&(cont->chan[i]), &netkey))
	    {
	      disconnect_client (cont, i, "error sending lag keys");
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/* 
 * Checks if the checksums from the clients are all the same
 */
int
check_checksum (LW_SRVCONT * cont)
{
  int i, j;
  int ok = 1;
  int nb_diff;

  for (i = 0; i < cont->nb_chans; ++i)
    {
      cont->chan[i].checksum_send = 1;
      nb_diff = 0;

      /*
       * We do not check checksums if there's only 1 connected client.
       */
      if (cont->nb_chans >= 2)
	{
	  for (j = 0; j < cont->nb_chans; ++j)
	    {
	      if (cont->chan[i].checksum_recv != cont->chan[j].checksum_recv)
		{
		  ok = 0;
		  nb_diff++;
		}
	    }

	  /*
	   * Now if this channel has a different checksum
	   * than everyone else, we consider there's an error
	   * on it. But if there's at least one channel that
	   * has the same checksum, we consider it's OK
	   * (for this channel only of course...)
	   */
	  if (nb_diff >= cont->nb_chans - 1)
	    {
	      cont->chan[i].checksum_send = 0;

	      log_print_str ("Checksum error on client \"");
	      log_print_str (cont->chan[i].ip);
	      log_print_str (":");
	      log_print_int (cont->chan[i].port);
	      log_print_str ("\" (");
	      log_print_int (cont->chan[i].checksum_recv);
	      log_print_str (")");
	      log_println ();
	    }
	}
    }

  return ok;
}

/*------------------------------------------------------------------*/
/* 
 * Sends a final OK message to all the clients
 */
int
lw_srvcont_replicate_keys (LW_SRVCONT * cont,
			   LW_WWWSRV_DATA * www_data, int lag, char *password)
{
  int result = 1;
  LW_NETKEY netkey;
  int i;
  int increase_lag_counter = 0;
  int decrease_lag_counter = 0;
  int all_clients_request_increase_lag;
  int all_clients_request_decrease_lag;
  int a_client_requests_increase_lag;
  int a_client_requests_decrease_lag;
  int rounds = 0;
  int last_rounds = 0;
  int average_lag = 0;
  time_t start, start2, end;
  double elapsed;
  int lag_key_presses_to_send = 0;
  int skip_keys;

  www_data->log = 1;
  lw_wwwsrv_register (www_data);

  lag_key_presses_to_send = lag;

  start2 = start = lw_srvtime_seconds ();

  while (cont->nb_chans > 0)
    {
      lw_netkey_reset (&netkey);

      /*
       * We still listen on the listening socket, to reject
       * newcomers since the game already started
       */
      reject_new_clients (cont, password);

      all_clients_request_increase_lag = 1;
      all_clients_request_decrease_lag = 1;
      a_client_requests_increase_lag = 0;
      a_client_requests_decrease_lag = 0;

      /*
       * First we get the informations from all the players
       */
      for (i = cont->nb_chans - 1; i >= 0; --i)
	{
	  if (lw_srvchan_recv_keys (&(cont->chan[i]), &netkey))
	    {
	      /*
	       * if at least a client does not request a lag
	       * increase, then we don't do it
	       */
	      if (netkey.cmd == LW_NETKEY_CMD_INCREASE_LAG)
		{
		  a_client_requests_increase_lag = 1;
		}
	      else
		{
		  all_clients_request_increase_lag = 0;
		}

	      /*
	       * if at least a client does not request a lag
	       * decrease, then we don't do it
	       */
	      if (netkey.cmd == LW_NETKEY_CMD_DECREASE_LAG)
		{
		  a_client_requests_decrease_lag = 1;
		}
	      else
		{
		  all_clients_request_decrease_lag = 0;
		}
	    }
	  else
	    {
	      /*
	       * There's been an error receiving data from this team
	       * so we decide to "close" it.
	       */
	      disconnect_client (cont, i, "no more keys");
	    }
	}

      /*
       * we reset the cmd and arg fields
       */
      netkey.cmd = LW_NETKEY_CMD_NONE;
      netkey.arg = 0;

      /*
       * We check if the checksums are OK
       */
      check_checksum (cont);

      /*
       * we use a counter system, so that all client must answer
       * "yes we want a lag increase" several times in a row
       * before we actually update the lag
       *
       * Note that test is formulated using 2 values and a
       * logical and, this is just to get rid of warning
       * complaining a_client_requests_increase_lag is unused
       */
      if (all_clients_request_increase_lag && a_client_requests_increase_lag)
	{
	  increase_lag_counter++;
	}
      else
	{
	  increase_lag_counter = 0;
	}

      /*
       * we use a counter system, so that all client must answer
       * "yes we want a lag decrease" several times in a row
       * before we actually update the lag
       *
       * Note that test is formulated using 2 values and a
       * logical or, this is just to get rid of warning
       * complaining all_client_requests_decrease_lag is unused
       */
      if (a_client_requests_decrease_lag || all_clients_request_decrease_lag)
	{
	  decrease_lag_counter++;
	}
      else
	{
	  decrease_lag_counter = 0;
	}

      /*
       * If we need to increase the lag, we send blank key presses
       */
      if (increase_lag_counter >= LW_SRVCONT_INCREASE_LAG_LIMIT
	  && lag_key_presses_to_send == 0)
	{
	  increase_lag_counter = 0;
	  lag++;
	  lag_key_presses_to_send++;
	}


      /*
       * If we need to decrease the lag, we simply do not send
       * the key presses...
       */
      skip_keys = 0;
      if (decrease_lag_counter >= LW_SRVCONT_DECREASE_LAG_LIMIT)
	{
	  decrease_lag_counter = 0;
	  lag--;
	  if (lag_key_presses_to_send > 0)
	    {
	      /*
	       * We cancel the planified lag keys
	       */
	      lag_key_presses_to_send--;
	    }
	  else
	    {
	      /*
	       * No lag keys planned, we skip the current keys
	       */
	      skip_keys = 1;
	    }
	}

      if (!skip_keys)
	{
	  /*
	   * Now we send the information to all the players
	   */
	  for (i = cont->nb_chans - 1; i >= 0; --i)
	    {
	      if (!lw_srvchan_send_keys (&(cont->chan[i]), &netkey))
		{
		  /*
		   * There's been an error receiving data from this team
		   * so we decide to "close" it.
		   */
		  disconnect_client (cont, i, "error sending keys");
		}
	    }
	}

      /*
       * We send the lag key presses 1 by 1. This is a way
       * to synchronize correctly at the beginning of a game.
       */
      if (lag_key_presses_to_send > 0)
	{
	  lag_key_presses_to_send--;
	  send_lag_key_presses (cont, 1);
	}

      rounds++;
      last_rounds++;
      average_lag += lag;

      end = lw_srvtime_seconds ();
      elapsed = end - start;

      if (((int) elapsed) > LW_SRVCONT_DISPLAY_STATE_DELAY)
	{
	  if (rounds <= 0)
	    {
	      rounds = 1;
	    }
	  if (last_rounds <= 0)
	    {
	      last_rounds = 1;
	    }

	  log_print_int (rounds);
	  log_print_str (" rounds, ");
	  log_print_int (last_rounds / ((int) elapsed));
	  log_print_str (" rounds/sec, average lag is ");
	  log_print_int (average_lag / last_rounds);
	  log_println ();

	  start = end;
	  last_rounds = 0;
	  average_lag = 0;
	}

      if (end - start2 > LW_SRVCONT_REGISTER_DELAY)
	{
	  start2 += LW_SRVCONT_REGISTER_DELAY;

	  www_data->log = 0;
	  lw_wwwsrv_register (www_data);
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/* 
 * Closes everything and frees the resources
 */
void
lw_srvcont_close (LW_SRVCONT * cont)
{
  int i;

  lw_sock_close (&(cont->sock));

  log_print_str ("Game over");
  log_println ();

  for (i = 0; i < cont->nb_chans; ++i)
    {
      lw_srvchan_close (&(cont->chan[i]));
    }

  reset (cont);
}

/*------------------------------------------------------------------*/
/*
 * Launches a thread which will handle new connections while
 * other players are playing.
 */
static int
reject_new_clients (LW_SRVCONT * cont, char *password)
{
  int result = 0;
  static LW_SRVCONT_REJECT_DATA data;
  static int thread_running = 0;

  /*
   * If there's already a thread answering to a client, we do nothing,
   * for we can answer later, and it makes programming easier and
   * safer. In any case, the answer we're going to give to the client
   * is: "there's no room for you guy" so it's not that bad to be
   * a little late when answering.
   */
  if (!thread_running)
    {
      data.sock = -1;
      data.ip[0] = '\0';
      data.port = 0;
      data.thread_running = &thread_running;
      strncpy (data.password, password, PASSWORD_SIZE);
      data.password[PASSWORD_SIZE] = '\0';

      if (lw_sock_accept (&(data.sock), data.ip, &(data.port), cont->sock, 0))
	{
	  thread_running = 1;
	  result =
	    lw_thread_start (reject_new_clients_callback, (void *) &data);
	  if (!result)
	    {
	      lw_sock_close (&(data.sock));
	      thread_running = 0;
	    }
	}
      else
	{
	  /*
	   * privacy is set to 0, we do not tell anyone about this server
	   */
	  result = 1;
	}
    }
  else
    {
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
reject_new_clients_callback (void *args)
{
  LW_SRVCONT_REJECT_DATA data;
  int ret;
  int ret_mess = 1;
  char message[LW_SOCK_MESSAGE_SIZE];
  LW_NETMESS *mess;
  int program = 0;
  int version = 0;
  int password = 0;
  int free_received = 0;
  int normal_quit = 0;

  memcpy (&data, args, sizeof (LW_SRVCONT_REJECT_DATA));

  log_print_str ("Connection from \"");
  log_print_str (data.ip);
  log_print_str (":");
  log_print_int (data.port);
  log_print_str ("\"");
  log_println ();
  log_flush ();

  while ((!free_received) && ret_mess &&
	 ((ret = lw_sock_recv_str (&(data.sock), message)) >= 0))
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
		  ret_mess = lw_srvchan_handle_ping (data.sock, mess);
		  break;
		case LW_NETMESS_CODE_ECHO:
		  ret_mess = lw_srvchan_handle_echo (data.sock, mess);
		  break;
		case LW_NETMESS_CODE_PROGRAM:
		  ret_mess =
		    lw_srvchan_handle_program (data.sock, mess, &program);
		  break;
		case LW_NETMESS_CODE_VERSION:
		  ret_mess =
		    lw_srvchan_handle_version (data.sock, mess, &version);
		  break;
		case LW_NETMESS_CODE_PASSWORD:
		  ret_mess =
		    lw_srvchan_handle_password (data.sock, mess, &password,
						data.password);
		  break;
		case LW_NETMESS_CODE_FREE:
		  ret_mess =
		    lw_srvchan_handle_free (data.sock, mess, 0,
					    &free_received);
		  break;
		case LW_NETMESS_CODE_QUIT:
		  ret_mess =
		    lw_srvchan_handle_quit (data.sock, mess, &normal_quit);
		  break;
		default:
		  ret_mess = lw_srvchan_handle_unknown (data.sock, mess);
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

  log_print_str ("Refusing \"");
  log_print_str (data.ip);
  log_print_str (":");
  log_print_int (data.port);
  log_print_str ("\"");
  if (normal_quit)
    {
      /*
       * If the client quitted with a normal "QUIT" message, then
       * we mention in the log that it was probably simply pinging.
       */
      log_print_str (" (probably a ping)");
    }
  log_println ();

  lw_sock_close (&(data.sock));
  (*(data.thread_running)) = 0;
}

/*------------------------------------------------------------------*/
/*
 * Disconnects a client. This code supposes that the caller function
 * is executing a loop with a step of -1 (ie a "reverse" loop).
 */
static void
disconnect_client (LW_SRVCONT * cont, int i, char *reason)
{
  int j;

  log_print_str ("Disconnecting \"");
  log_print_str (cont->chan[i].ip);
  log_print_str (":");
  log_print_int (cont->chan[i].port);
  log_print_str ("\" (");
  if (cont->chan[i].normal_quit)
    {
      /*
       * The client decided to quit, therefore this is not
       * an error, we mark it as such
       */
      log_print_str ("decided to quit");
    }
  else
    {
      log_print_str (reason);
    }
  log_println_str (")");

  lw_srvchan_close (&(cont->chan[i]));
  for (j = i + 1; j < cont->nb_chans; ++j)
    {
      cont->chan[j - 1] = cont->chan[j];
    }
  cont->nb_chans--;
}
