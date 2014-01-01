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
/* name          : srvchan.h                                        */
/* content       : handles a server network channel                 */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_SRVCHAN
#define LIQUID_WAR_INCLUDE_SRVCHAN

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "base.h"
#include "sockgen.h"
#include "srvteam.h"
#include "netkey.h"
#include "netconf.h"
#include "startinf.h"
#include "netmess.h"
#include "chat.h"

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/

typedef struct
{
  int sock;
  char ip[LW_SOCK_IP_SIZE];
  int port;
  int program_ok;
  int version_ok;
  int password_ok;
  int ready_ok;
  int nb_local_teams;
  int start;
  void *map;
  LW_NETCONF config;
  LW_SRVTEAM team[NB_TEAMS];
  LW_CHAT_HISTORY chat;
  unsigned int checksum_recv;
  unsigned int checksum_send;
  int normal_quit;
}
LW_SRVCHAN;

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

int lw_srvchan_wait_teams (LW_SRVCHAN * chan, int *free_teams,
			   int sock, void **map, LW_NETCONF * config,
			   char *password);
int lw_srvchan_keepalive (LW_SRVCHAN * chan, int waited_teams,
			  LW_TEAMSTARTINFO * team_start_info,
			  LW_CHAT_HISTORY * chat);
int lw_srvchan_tell_who (LW_SRVCHAN * chan, void *map, LW_NETCONF * config);
int lw_srvchan_final_ok (LW_SRVCHAN * chan);
int lw_srvchan_find_team_by_server_id (LW_SRVCHAN * chan, int server_id);
int lw_srvchan_find_first_free_team (LW_SRVCHAN * chan);
void lw_srvchan_close (LW_SRVCHAN * chan);
int lw_srvchan_recv_keys (LW_SRVCHAN * chan, LW_NETKEY * netkey);
int lw_srvchan_send_keys (LW_SRVCHAN * chan, LW_NETKEY * netkey);

int lw_srvchan_handle_ping (int sock, LW_NETMESS * mess);
int lw_srvchan_handle_echo (int sock, LW_NETMESS * mess);
int lw_srvchan_handle_program (int sock, LW_NETMESS * mess, int *ok);
int lw_srvchan_handle_version (int sock, LW_NETMESS * mess, int *ok);
int lw_srvchan_handle_password (int sock, LW_NETMESS * mess,
				int *ok, char *password);
int lw_srvchan_handle_free (int sock, LW_NETMESS * mess,
			    int free_teams, int *ok);
int lw_srvchan_handle_unknown (int sock, LW_NETMESS * mess);
int lw_srvchan_handle_quit (int sock, LW_NETMESS * mess, int *normal_quit);

#endif
