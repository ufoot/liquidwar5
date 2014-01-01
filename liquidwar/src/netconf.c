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
/* name          : netconf.c                                        */
/* content       : functions to transmit the conf over the network  */
/* last update   : June 30th 2001                                   */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "netconf.h"
#include "sock2gen.h"
#include "log.h"
#include "macro.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

/*==================================================================*/
/* macros                                                           */
/*==================================================================*/

#define LW_NETCONF_CHECK_RANGE(VAL,MAX) \
if (config->VAL < 0) \
{ \
result=0; \
log_print_str("Error: " #VAL " is "); \
log_print_int(config->VAL); \
log_println_str(", and should be >= 0"); \
config->VAL = 0; \
} \
if (config->VAL > MAX) \
{ \
result=0; \
log_print_str("Error: " #VAL " is > "); \
log_print_int(config->VAL); \
log_print_str(", and should be <= "); \
log_println_int(MAX); \
config->VAL = MAX; \
}

#define LW_NETCONF_PRINT_VALUE(VAL) \
log_print_str(#VAL " = "); \
log_println_int(config->VAL);

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

/*==================================================================*/
/* static functions declaration                                     */
/*==================================================================*/

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Sends a config over the network
 */
int
lw_netconf_send (int *sock, LW_NETCONF * config)
{
  int result = 0;
  char buffer[LW_SOCK_MESSAGE_SIZE];

  LW_MACRO_SPRINTF1 (buffer, "%d", config->cursor_increase_speed);
  if (lw_sock_send_str (sock, buffer))
    {
      LW_MACRO_SPRINTF1 (buffer, "%d", config->fighter_attack);
      if (lw_sock_send_str (sock, buffer))
	{
	  LW_MACRO_SPRINTF1 (buffer, "%d", config->fighter_defense);
	  if (lw_sock_send_str (sock, buffer))
	    {
	      LW_MACRO_SPRINTF1 (buffer, "%d", config->fighter_new_health);
	      if (lw_sock_send_str (sock, buffer))
		{
		  LW_MACRO_SPRINTF1 (buffer, "%d", config->number_influence);
		  if (lw_sock_send_str (sock, buffer))
		    {
		      LW_MACRO_SPRINTF1 (buffer, "%d",
					 config->fighter_number);
		      if (lw_sock_send_str (sock, buffer))
			{
			  LW_MACRO_SPRINTF1 (buffer, "%d", config->game_time);
			  if (lw_sock_send_str (sock, buffer))
			    {
			      LW_MACRO_SPRINTF1 (buffer, "%d",
						 config->cpu_advantage);
			      if (lw_sock_send_str (sock, buffer))
				{
				  LW_MACRO_SPRINTF1 (buffer, "%d",
						     config->cpu_vs_human);
				  if (lw_sock_send_str (sock, buffer))
				    {
				      LW_MACRO_SPRINTF1 (buffer, "%d",
							 config->
							 asm_algorithm);
				      if (lw_sock_send_str (sock, buffer))
					{
					  LW_MACRO_SPRINTF1 (buffer, "%d",
							     config->
							     min_map_res);
					  if (lw_sock_send_str (sock, buffer))
					    {
					      result = 1;
					    }
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Receives a config from the network
 */
int
lw_netconf_recv (int *sock, LW_NETCONF * config)
{
  int result = 0;
  char buffer[LW_SOCK_MESSAGE_SIZE];

  memset (config, 0, sizeof (LW_NETCONF));

  if (lw_sock_recv_str (sock, buffer))
    {
      config->cursor_increase_speed = atoi (buffer);
      if (lw_sock_recv_str (sock, buffer))
	{
	  config->fighter_attack = atoi (buffer);
	  if (lw_sock_recv_str (sock, buffer))
	    {
	      config->fighter_defense = atoi (buffer);
	      if (lw_sock_recv_str (sock, buffer))
		{
		  config->fighter_new_health = atoi (buffer);
		  if (lw_sock_recv_str (sock, buffer))
		    {
		      config->number_influence = atoi (buffer);
		      if (lw_sock_recv_str (sock, buffer))
			{
			  config->fighter_number = atoi (buffer);
			  if (lw_sock_recv_str (sock, buffer))
			    {
			      config->game_time = atoi (buffer);
			      if (lw_sock_recv_str (sock, buffer))
				{
				  config->cpu_advantage = atoi (buffer);
				  if (lw_sock_recv_str (sock, buffer))
				    {
				      config->cpu_vs_human = atoi (buffer);
				      if (lw_sock_recv_str (sock, buffer))
					{
					  config->asm_algorithm =
					    atoi (buffer);
					  if (lw_sock_recv_str (sock, buffer))
					    {
					      config->min_map_res =
						atoi (buffer);

					      result = 1;
					    }
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Checks if all the values are within an acceptable range
 */
int
lw_netconf_check (LW_NETCONF * config)
{
  int result = 1;

  LW_NETCONF_CHECK_RANGE (cursor_increase_speed, 2);
  LW_NETCONF_CHECK_RANGE (fighter_attack, 16);
  LW_NETCONF_CHECK_RANGE (fighter_defense, 16);
  LW_NETCONF_CHECK_RANGE (fighter_new_health, 16);
  LW_NETCONF_CHECK_RANGE (number_influence, 16);
  LW_NETCONF_CHECK_RANGE (fighter_number, 32);
  LW_NETCONF_CHECK_RANGE (game_time, 16);
  LW_NETCONF_CHECK_RANGE (cpu_advantage, 4);
  LW_NETCONF_CHECK_RANGE (cpu_vs_human, 2);
  LW_NETCONF_CHECK_RANGE (asm_algorithm, 1);
  LW_NETCONF_CHECK_RANGE (min_map_res, 8);

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Prints the contents of a NETCONF object
 */
void
lw_netconf_print (LW_NETCONF * config)
{
  LW_NETCONF_PRINT_VALUE (cursor_increase_speed);
  LW_NETCONF_PRINT_VALUE (fighter_attack);
  LW_NETCONF_PRINT_VALUE (fighter_defense);
  LW_NETCONF_PRINT_VALUE (fighter_new_health);
  LW_NETCONF_PRINT_VALUE (number_influence);
  LW_NETCONF_PRINT_VALUE (fighter_number);
  LW_NETCONF_PRINT_VALUE (game_time);
  LW_NETCONF_PRINT_VALUE (cpu_advantage);
  LW_NETCONF_PRINT_VALUE (cpu_vs_human);
  LW_NETCONF_PRINT_VALUE (asm_algorithm);
  LW_NETCONF_PRINT_VALUE (min_map_res);
}
