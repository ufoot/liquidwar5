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
/* Copyright (C) 1998-2020 Christian Mauduit                                 */
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
/* name          : connect.c                                        */
/* contenu       : connect menu, the list of connected players      */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <string.h>

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
#include "error.h"
#include "internet.h"
#include "connect.h"
#include "lang.h"
#include "mouse.h"
#include "log.h"
#include "thrdgen.h"
#include "chat.h"
#include "netmess.h"
#include "sound.h"
#include "macro.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define CONNECT_BUF_SIZE 200

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/

typedef struct
{
  int sock;
  LW_TEAMSTARTINFO team_start_info[NB_TEAMS];
  int waited_teams;
  int start;
  int force_start;
  int thread_running;
  int result;
  int finished;
  char chat_buffer[LW_CHAT_MESSAGE_SIZE + 1];
  LW_CHAT_HISTORY chat_history;
  int chat_send;
}
LW_CONNECT_DATA;

LW_CONNECT_DATA GLOBAL_CONNECT_DATA;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

static void connect_on_server_keepalive (void *args);
static int update_connect_menu (DIALOG * d,
                                int waited_teams,
                                LW_TEAMSTARTINFO * team_start_info,
                                LW_CHAT_HISTORY * chat_history,
                                int *connected_teams_prev);
static void position_players_list (DIALOG * d);
static void position_chat_history (DIALOG * d);

/*------------------------------------------------------------------*/
int
lw_connect_menu (int sock)
{
  DIALOG d[12];
  DIALOG_PLAYER *dp;
  int choix = 4, retour = 0;
  char buf_waited_teams[CONNECT_BUF_SIZE];
  char buf_connected_players[NB_TEAMS * CONNECT_BUF_SIZE];
  char buf_chat_history[LW_CHAT_HISTORY_SIZE * CONNECT_BUF_SIZE];
  char buf_chat_message[LW_CHAT_MESSAGE_SIZE + 1];
  int first_display = 1;
  int first_getinfo = 1;
  LW_CONNECT_DATA *data;
  int network_error = 0;
  int hide_start_button = 0;
  int connected_teams_prev = -1;

  memset (d, 0, sizeof (d));
  buf_waited_teams[0] = '\0';
  buf_connected_players[0] = '\0';
  buf_chat_history[0] = '\0';
  buf_chat_message[0] = '\0';

  data = &GLOBAL_CONNECT_DATA;
  memset (data, 0, sizeof (LW_CONNECT_DATA));
  data->sock = sock;
  data->waited_teams = NB_TEAMS;

  LW_NETWORK_ON = 1;

  while (retour == 0)
    {
      standard_button (d + 4, 1, 0, 2, 10);
      standard_button (d + 6, 1, 5, 2, 10);
      standard_button (d + 8, 1, 7, 2, 10);
      standard_button (d + 9, 1, 7, 2, 10);
      standard_button (d + 10, 1, 8, 2, 10);

      quick_buttons (d);

      d[4].proc = my_textbox_proc;
      d[5].proc = my_textbox_proc;
      d[7].proc = my_textbox_proc;
      d[8].proc = d_box_proc;
      d[9].proc = my_edit_proc;

      position_players_list (d + 5);
      position_chat_history (d + 7);
      d[4].w = d[7].w;
      d[8].w = d[7].w;
      d[9].w = d[7].w;
      d[9].x++;
      d[9].y++;
      d[9].w -= 2;
      d[9].h -= 2;

      d[4].dp = buf_waited_teams;
      d[5].dp = buf_connected_players;
      d[6].dp = lw_lang_string (LW_LANG_STRING_CONNECT_STARTNOW);
      d[7].dp = buf_chat_history;
      d[9].dp = buf_chat_message;
      d[9].d1 = LW_CHAT_MESSAGE_SIZE;
      d[9].flags = D_EXIT;
      d[10].dp = lw_lang_string (LW_LANG_STRING_CONNECT_SENDMESSAGE);

      if (hide_start_button)
        {
          /*
           * We disactivate these buttons. If we did not do it
           * it would lead to network errors if someone would
           * press them twice.
           */
          d[6].proc = my_textbox_proc;
          d[6].dp = lw_lang_string (LW_LANG_STRING_CONNECT_WAITING);
          d[MENU_QUICK_PLAY].flags = D_HIDDEN;
        }

      d[11].proc = 0;

      if (first_display)
        {
          display_back_image ();
          first_display = 0;
        }
      dp = my_init_dialog (d, choix);
      while (my_update_dialog (dp) && !data->finished)
        {
          show_mouse (screen);
          if (!data->thread_running && !data->finished)
            {
              if (!first_getinfo)
                {
                  if (data->result == 0)
                    {
                      network_error = 1;
                    }
                  else
                    {
                      update_connect_menu (d,
                                           data->waited_teams,
                                           data->team_start_info,
                                           &(data->chat_history),
                                           &connected_teams_prev);
                    }
                }
              data->thread_running = 1;
              if (lw_thread_start (connect_on_server_keepalive, data))
                {
                  first_getinfo = 0;
                }
              else
                {
                  data->thread_running = 0;
                }
            }
          rest (10);
        }
      choix = shutdown_dialog (dp);

      if (network_error)
        {
          error_network_problem ();
          lw_sock_close (&(data->sock));
          retour = 1;
        }
      else
        {
          /*
           * If 0 teams are waited for, then the game must start, so
           * we simulate a key press on the "start" button.
           */
          if (data->waited_teams == 0)
            {
              retour = MENU_OK;
            }
          else
            {
              switch (choix)
                {
                case -1:
                case MENU_QUICK_BACK:
                  lw_sock_close (&(data->sock));
                  retour = 1;
                  break;
                case MENU_QUICK_MAIN:
                  lw_sock_close (&(data->sock));
                  retour = MENU_TOP;
                  break;
                case MENU_QUICK_QUIT:
                  lw_sock_close (&(data->sock));
                  if (confirm_quit ())
                    retour = MENU_EXIT;
                  else
                    display_back_image ();
                  break;
                case MENU_QUICK_PLAY:
                  /*
                   * Pressing Play or Start has the same effect here
                   * so there's no "break;"
                   */
                case 6:
                  if (data->waited_teams > 0)
                    {
                      data->force_start = 1;
                      hide_start_button = 1;
                      display_back_image ();
                    }
                  break;
                case 9:
                case 10:
                  if (strlen (d[9].dp) && !data->chat_send)
                    {
                      data->chat_send = 1;

                      strncpy (data->chat_buffer, d[9].dp,
                               LW_CHAT_MESSAGE_SIZE);
                      data->chat_buffer[LW_CHAT_MESSAGE_SIZE] = '\0';
                      ((char *) (d[9].dp))[0] = '\0';
                      d[9].d2 = 0;
                      d[9].proc (MSG_DRAW, d + 9, 0);
                    }
                  break;
                }
            }
        }
    }

  while (data->thread_running)
    {
      /*
       * Here, we wait until the thread is over, it shouldn't be very 
       * long since we just wait for the server's answer
       */
      rest (100);
    }

  if (retour > 0)
    retour--;

  return retour;
}

/*------------------------------------------------------------------*/
/*
 * Updates the fields of the menu
 */
static int
update_connect_menu (DIALOG * d,
                     int waited_teams,
                     LW_TEAMSTARTINFO * team_start_info,
                     LW_CHAT_HISTORY * chat_history,
                     int *connected_teams_prev)
{
  int result = 0;
  int i;
  char *sep;
  char buf[CONNECT_BUF_SIZE];
  LW_CHAT_MESSAGE chat_mess;
  static int old_chat_length = 0;
  int chat_length;
  int l;
  int connected_teams = 0;

  LW_MACRO_SNPRINTF3 ((char *) d[4].dp,
                      CONNECT_BUF_SIZE,
                      "%s%d%s",
                      lw_lang_string (LW_LANG_STRING_CONNECT_WAITINGFOR),
                      waited_teams,
                      lw_lang_string (LW_LANG_STRING_CONNECT_TEAMS));

  ((char *) (d[5].dp))[0] = '\0';
  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (team_start_info[i].active)
        {
          if (team_start_info[i].start)
            {
              sep = "*";
            }
          else
            {
              sep = "-";
            }

          LW_MACRO_SPRINTF3 (buf, "%d %s %s\n",
                             i + 1, sep, team_start_info[i].name);

          LW_MACRO_STRNCAT ((char *) d[5].dp, buf,
                            NB_TEAMS * CONNECT_BUF_SIZE);

          connected_teams++;
        }
    }

  ((char *) (d[7].dp))[0] = '\0';
  for (i = 0; lw_chat_get (chat_history, &chat_mess, i); ++i)
    {
      LW_MACRO_SPRINTF2 (buf, "%s: %s\n",
                         chat_mess.author, chat_mess.content);
      LW_MACRO_STRNCAT ((char *) d[7].dp, buf,
                        LW_CHAT_HISTORY_SIZE * CONNECT_BUF_SIZE);
    }
  /*
   * Now if the length of the string has changed, we
   * scroll to the end of the list. The "string length"
   * test is not 100% safe, but its purpose is just to
   * scroll through the list when a message is sent.
   * if it fails sometimes, it's not _that_ bad.
   */
  chat_length = strlen (d[7].dp);
  if (chat_length != old_chat_length)
    {
      old_chat_length = chat_length;

      l = (d[7].h - 8) / text_height (font);
      d[7].d2 = MAX (0, d[7].d1 - l);
    }

  scare_mouse ();
  d[4].proc (MSG_DRAW, d + 4, 0);
  d[5].proc (MSG_DRAW, d + 5, 0);
  d[7].proc (MSG_DRAW, d + 7, 0);
  unscare_mouse ();

  show_mouse (screen);

  /*
   * If there are more teams connected than last times, we're likely
   * to announce it with a sound
   */
  if (connected_teams > 0 && connected_teams > *connected_teams_prev)
    {
      /*
       * We check if this is not the first time 
       * (*connected_teams_prev==-1) and in this case we play 
       * nothing
       */
      if ((*connected_teams_prev) >= 0)
        {
          play_connect ();
        }
      (*connected_teams_prev) = connected_teams;
    }
  else
    {
      /*
       * We updated the numner of connected teams, but take care
       * note to overwrite it if it's -1
       */
      if ((*connected_teams_prev) >= 0)
        {
          (*connected_teams_prev) = connected_teams;
        }
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Retrieves informations from the server once all players are
 * connected. Also displays information in a dialog box.
 */
static void
connect_on_server_keepalive (void *arg)
{
  int result = 1;
  int i;
  LW_CONNECT_DATA *data;

  data = (LW_CONNECT_DATA *) arg;

  if (!data->finished)
    {
      if (result == 1)
        {
          if (!lw_protocol_waiting (&data->sock, &(data->waited_teams)))
            {
              result = 0;
            }
        }

      if (result == 1)
        {
          if (data->waited_teams > 0)
            {
              /*
               * We are still waiting for teams
               */
              for (i = 0; i < NB_TEAMS; ++i)
                {
                  if (result == 1)
                    {
                      if (!lw_protocol_ask_teamstartinfo (&data->sock, i,
                                                          &
                                                          (data->team_start_info
                                                           [i])))
                        {
                          result = 0;
                        }
                    }
                }

              if (result == 1 && data->chat_send)
                {
                  lw_netmess_cleanup_arg (data->chat_buffer);

                  if (!lw_protocol_chat_talk (&data->sock, data->chat_buffer))
                    {
                      result = 0;
                    }

                  data->chat_send = 0;
                }

              if (result == 1)
                {
                  if (!lw_protocol_chat_listen
                      (&data->sock, &(data->chat_history)))
                    {
                      result = 0;
                    }
                }

              if (result == 1 && data->force_start)
                {
                  if (!lw_protocol_force_start (&data->sock))
                    {
                      result = 0;
                    }
                  /*
                   * It's useless to keep on sending the START message, once is
                   * just enough, let's save bandwidth!
                   */
                  data->force_start = 0;
                }

              if (result == 1)
                {
                  if (!lw_protocol_next (&data->sock))
                    {
                      result = 0;
                    }
                }
            }
          else
            {
              /*
               * No more teams waited, we say we're ready
               */
              if (result == 1)
                {
                  if (!lw_protocol_i_am_ready (&data->sock))
                    {
                      result = 0;
                    }
                  data->finished = 1;
                }
            }
        }
    }

  data->result = result;
  data->thread_running = 0;
}

/*------------------------------------------------------------------*/
static void
position_players_list (DIALOG * d)
{
  d->x = (MENU_VIRTUAL_W + MENU_W_SPACE) / 2;
  d->y =
    (MENU_VIRTUAL_H - MENU_H_INTERNET_LIST) / 2 + MENU_H_STANDARD +
    MENU_H_SPACE;
  d->w = (MENU_W_STANDARD + MENU_W_INTERNET_LIST) / 2;
  d->h = 4 * MENU_H_STANDARD + 3 * MENU_H_SPACE;

  color_one_dialog (d);
  menu_real_coord (d);
}

/*------------------------------------------------------------------*/
static void
position_chat_history (DIALOG * d)
{
  d->x = (MENU_VIRTUAL_W - MENU_W_SPACE) / 2 - MENU_W_INTERNET_LIST;
  d->y = (MENU_VIRTUAL_H - MENU_H_INTERNET_LIST) / 2;
  d->w = MENU_W_INTERNET_LIST;
  d->h = MENU_H_INTERNET_LIST;

  color_one_dialog (d);
  menu_real_coord (d);
  d->key = 0;
  d->dp = "";
  d->flags = D_EXIT;
}
