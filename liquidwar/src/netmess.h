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
/* Copyright (C) 1998-2015 Christian Mauduit                                 */
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
/* name          : netmess.h                                        */
/* content       : functions to create and parse network messages   */
/* last update   : April 14th 2001                                  */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_NETMESS
#define LIQUID_WAR_INCLUDE_NETMESS

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/

/*
 * This structure is used to retrieve the informations about a
 * network message. The advantage of using this is that it makes
 * various checks about the syntax of messages and one can use
 * C switches on the message code since it's an integer.
 * Without this one would spent one's time writting boring code
 * full of "strcmp" and the likes...
 */
typedef struct
{
  int argc;			/* the number of arguments                          */
  int code;			/* the code of the command (ex: LW_NETMESS_CODE_OK) */
  char **argv;			/* the parsed arguments, like in the main() func    */
}
LW_NETMESS;

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_NETMESS_CODE_UNKNOWN        0
#define LW_NETMESS_CODE_OK             1
#define LW_NETMESS_CODE_ERR            2
#define LW_NETMESS_CODE_PING           3
#define LW_NETMESS_CODE_ECHO           4
#define LW_NETMESS_CODE_PROGRAM        5
#define LW_NETMESS_CODE_VERSION        6
#define LW_NETMESS_CODE_PASSWORD       7
#define LW_NETMESS_CODE_FREE           8
#define LW_NETMESS_CODE_TEAM           9
#define LW_NETMESS_CODE_READY         10
#define LW_NETMESS_CODE_START         11
#define LW_NETMESS_CODE_WAITING       12
#define LW_NETMESS_CODE_NEXT          13
#define LW_NETMESS_CODE_TEAMSTARTINFO 14
#define LW_NETMESS_CODE_WHO           15
#define LW_NETMESS_CODE_LOCAL         16
#define LW_NETMESS_CODE_NETWORK       17
#define LW_NETMESS_CODE_NOBODY        18
#define LW_NETMESS_CODE_SENDMAP       19
#define LW_NETMESS_CODE_RECVMAP       20
#define LW_NETMESS_CODE_SENDCONFIG    21
#define LW_NETMESS_CODE_RECVCONFIG    22
#define LW_NETMESS_CODE_CHATTALK      23
#define LW_NETMESS_CODE_CHATLISTEN    24
#define LW_NETMESS_CODE_QUIT          25

#define LW_NETMESS_TEXT_OK            "OK"
#define LW_NETMESS_TEXT_ERR           "ERR"
#define LW_NETMESS_TEXT_PING          "PING"
#define LW_NETMESS_TEXT_ECHO          "ECHO"
#define LW_NETMESS_TEXT_PROGRAM       "PROGRAM"
#define LW_NETMESS_TEXT_VERSION       "VERSION"
#define LW_NETMESS_TEXT_PASSWORD      "PASSWORD"
#define LW_NETMESS_TEXT_FREE          "FREE"
#define LW_NETMESS_TEXT_TEAM          "TEAM"
#define LW_NETMESS_TEXT_READY         "READY"
#define LW_NETMESS_TEXT_START         "START"
#define LW_NETMESS_TEXT_WAITING       "WAITING"
#define LW_NETMESS_TEXT_NEXT          "NEXT"
#define LW_NETMESS_TEXT_TEAMSTARTINFO "TEAMSTARTINFO"
#define LW_NETMESS_TEXT_WHO           "WHO"
#define LW_NETMESS_TEXT_LOCAL         "LOCAL"
#define LW_NETMESS_TEXT_NETWORK       "NETWORK"
#define LW_NETMESS_TEXT_NOBODY        "NOBODY"
#define LW_NETMESS_TEXT_SENDMAP       "SENDMAP"
#define LW_NETMESS_TEXT_RECVMAP       "RECVMAP"
#define LW_NETMESS_TEXT_SENDCONFIG    "SENDCONFIG"
#define LW_NETMESS_TEXT_RECVCONFIG    "RECVCONFIG"
#define LW_NETMESS_TEXT_CHATTALK      "CHATTALK"
#define LW_NETMESS_TEXT_CHATLISTEN    "CHATLISTEN"
#define LW_NETMESS_TEXT_QUIT          "QUIT"

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

extern LW_NETMESS *lw_netmess_read (char *message);
extern void lw_netmess_free (LW_NETMESS * ptr);
extern void lw_netmess_cleanup_arg (char *string);

#endif
