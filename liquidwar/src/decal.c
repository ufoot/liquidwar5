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
/* nom           : decal.c                                          */
/* contenu       : gestion du decalage equipe jouantes/curseurs     */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "cursor.h"
#include "mesh.h"
#include "army.h"
#include "decal.h"
#include "time.h"
#include "log.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int PLAYING_TEAMS = 0;
int ACTIVE_FIGHTERS[NB_TEAMS];
int COLOR_FIRST_ENTRY[NB_TEAMS];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
get_cursor_from_team (int team)
{
  int i, cursor = -1;

  for (i = 0; i < NB_TEAMS && cursor < 0; ++i)
    if (team == CURRENT_CURSOR[i].team)
      cursor = i;

  return i;
}

/*------------------------------------------------------------------*/
int
eliminate_team (int team)
{
  int i, j;

  if (PLAYING_TEAMS > 0)
    {
      PLAYING_TEAMS--;
      for (i = 0; i < NB_TEAMS; ++i)
	if (CURRENT_CURSOR[i].team == team && CURRENT_CURSOR[i].active)
	  {
	    CURRENT_CURSOR[i].active = 0;
	    CURRENT_CURSOR[i].loose_time = TIME_ELAPSED;
	    CURRENT_CURSOR[i].score_order = PLAYING_TEAMS + 1;
	  }
      for (i = 0; i < NB_TEAMS; ++i)
	if (CURRENT_CURSOR[i].team > team)
	  CURRENT_CURSOR[i].team--;
      for (i = 0; i < CURRENT_ARMY_SIZE; ++i)
	if (CURRENT_ARMY[i].team > team)
	  CURRENT_ARMY[i].team--;
      for (j = team; j < PLAYING_TEAMS; ++j)
	{
	  ACTIVE_FIGHTERS[j] = ACTIVE_FIGHTERS[j + 1];
	  COLOR_FIRST_ENTRY[j] = COLOR_FIRST_ENTRY[j + 1];

	  for (i = 0; i < CURRENT_MESH_SIZE; ++i)
	    {
	      CURRENT_MESH[i].info[j] = CURRENT_MESH[i].info[j + 1];
	    }
	}

      return 0;
    }
  else
    return 1;
}
