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
/* Copyright (C) 1998-2008 Christian Mauduit                                 */
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
/* nom           : area.c                                           */
/* contenu       : creation de l'aire de jeu                        */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "area.h"
#include "bigdata.h"
#include "grad.h"
#include "mesh.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

PLACE *CURRENT_AREA = NULL;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
reset_game_area (void)
{
  int i, k;

  for (i = 0; i < CURRENT_MESH_SIZE; ++i)
    for (k = 0; k < NB_TEAMS; ++k)
      CURRENT_MESH[i].info[k].state.grad = AREA_START_GRADIENT;

  for (i = 0; i < CURRENT_AREA_W * CURRENT_AREA_H; ++i)
    CURRENT_AREA[i].fighter = NULL;
}

/*------------------------------------------------------------------*/
int
create_game_area (void)
{
  int x, y, i, test;

  CURRENT_AREA = malloc_in_big_data_bottom
    (CURRENT_AREA_W * CURRENT_AREA_H * sizeof (PLACE));

  if (CURRENT_AREA && CURRENT_MESH)
    {
      i = 0;
      for (y = 0; y < CURRENT_AREA_H; ++y)
	for (x = 0; x < CURRENT_AREA_W; ++x)
	  {
	    CURRENT_AREA[i].mesh = NULL;
	    CURRENT_AREA[i].fighter = NULL;
	    i++;
	  }
      for (i = 0; i < CURRENT_MESH_SIZE; ++i)
	for (y = 0; y < CURRENT_MESH[i].side.size; ++y)
	  for (x = 0; x < CURRENT_MESH[i].side.size; ++x)
	    {
	      CURRENT_AREA[(CURRENT_MESH[i].y + y) * CURRENT_AREA_W
			   + CURRENT_MESH[i].x + x].mesh = CURRENT_MESH + i;
	      test = CURRENT_MESH[i].info[0].state.grad;
	    }
    }

  return (CURRENT_AREA ? 0 : -1);
}
