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
/* name          : netconf.h                                        */
/* content       : functions to transmit the conf over the network  */
/* last update   : June 30th 2001                                   */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_NETCONF
#define LIQUID_WAR_INCLUDE_NETCONF

#include "base.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/

typedef struct
{
  int cursor_increase_speed;
  int fighter_attack;
  int fighter_defense;
  int fighter_new_health;
  int number_influence;
  int fighter_number;
  int game_time;
  int cpu_advantage;
  int cpu_vs_human;
  int asm_algorithm;
  int min_map_res;
}
LW_NETCONF;

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

int lw_netconf_send (int *sock, LW_NETCONF * config);
int lw_netconf_recv (int *sock, LW_NETCONF * config);
int lw_netconf_check (LW_NETCONF * config);
void lw_netconf_print (LW_NETCONF * config);

#endif
