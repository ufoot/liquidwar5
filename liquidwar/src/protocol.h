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
/* name          : protocol.h                                       */
/* content       : messages sent by the client when connecting      */
/* last update   : April 16th 2001                                  */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_PROTOCOL
#define LIQUID_WAR_INCLUDE_PROTOCOL

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "network.h"
#include "startinf.h"
#include "chat.h"

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

/*
 * IMPORTANT: there's one good reason sock is passed as a pointer
 * in all these functions. In fact, the code uses constructs like
 * *(data->sock) = 0 (mostly in connect.c) to intercept network
 * calls, stop them, and treat GUI stuff and network code in
 * separate threads. Previous releases of LW (< 5.6.3) didn't do
 * that, but the bug appeared when trying to release 5.6.3 with
 * Allegro 4.2 and GCC 4. This is an ugly hack, but hopefully works..
 */

int lw_protocol_do_ping (int *sock);
int lw_protocol_tell_version (int *sock, char *version);
int lw_protocol_tell_program (int *sock, char *program);
int lw_protocol_tell_password (int *sock, char *password);
int lw_protocol_ask_free_teams (int *sock, int *nb);
int lw_protocol_reserve_team (int *sock, int num,
                              int control_type, char *name);
int lw_protocol_ask_who (int *sock, int num, LW_WHO * who);
int lw_protocol_send_map (int *sock, void *map);
int lw_protocol_recv_map (int *sock, void **map);
int lw_protocol_send_config (int *sock);
int lw_protocol_recv_config (int *sock);
int lw_protocol_i_am_ready (int *sock);
int lw_protocol_waiting (int *sock, int *waited_teams);
int lw_protocol_force_start (int *sock);
int lw_protocol_next (int *sock);
int lw_protocol_ask_teamstartinfo (int *sock, int team,
                                   LW_TEAMSTARTINFO * team_start_info);
int lw_protocol_chat_talk (int *sock, char *text);
int lw_protocol_chat_listen (int *sock, LW_CHAT_HISTORY * chat_history);
int lw_protocol_quit (int *sock);

#endif
