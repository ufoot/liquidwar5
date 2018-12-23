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
/* Copyright (C) 1998-2018 Christian Mauduit                                 */
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
/* name          : wwwsrv.h                                         */
/* content       : server code used to contact www.ufoot.org        */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_WWWSRV
#define LIQUID_WAR_INCLUDE_WWWSRV

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "base.h"

/*==================================================================*/
/* structs                                                          */
/*==================================================================*/

typedef struct
{
  int privacy;
  int log;
  char metaserver[URL_SIZE + 1];
  int port;
  int busy_players;
  int max_players;
  char password[PASSWORD_SIZE + 1];
  char comment[COMMENT_SIZE + 1];
}
LW_WWWSRV_DATA;

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void lw_wwwsrv_init (LW_WWWSRV_DATA * data,
                     int privacy,
                     int log,
                     char *metaserver,
                     int port,
                     int busy_players,
                     int max_players, char *password, char *comment);
int lw_wwwsrv_register (LW_WWWSRV_DATA * data);

#endif
