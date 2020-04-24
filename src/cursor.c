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
/* nom           : cursor.c                                         */
/* contenu       : placement et initialisation des curseurs         */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdlib.h>

#include "army.h"
#include "grad.h"
#include "cursor.h"
#include "mesh.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*
 * globals holding all the cursors of the game
 * I first wanted to have several cursors per team, for LW's
 * core algorithm can handle that easily, but I gave up since
 * it would have represented too much work with cursor & teams
 * associations and also with the GUI
 */
CURSOR CURRENT_CURSOR[NB_TEAMS];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * inactivate all cursors
 */
void
reset_all_cursor (void)
{
  int i;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      memset (&(CURRENT_CURSOR[i]), 0, sizeof (CURSOR));
    }
}

/*------------------------------------------------------------------*/
/*
 * initialization of a cursor, done at each new game
 */
static void
init_cursor (int index, int team, int x, int y, char *name)
{
  /*
   * we activate the cursor and set up user parameters
   */
  CURRENT_CURSOR[index].active = 1;
  CURRENT_CURSOR[index].team = team;
  CURRENT_CURSOR[index].x = x;
  CURRENT_CURSOR[index].y = y;
  /*
   * the following line reveals an important point of LW's algorithm
   * at game start, one has to set up the value of the cursor with
   * a great value, since it will be decremented at each game cycle.
   * the reason is the following:
   * on one hand, since I move my cursor of let's say
   * 1 pixel, I can not assume that the points on the map have kept
   * the same distance with the cursor. In the worst case, any point
   * on the map is 1 pixel farer from the cursor than before. I could
   * increment the distance of every single point of the map, but it
   * goes much faster just to decrement the value of the cursor 8-D
   * on the other hand, the value must be a lot smaller than the value
   * we have initialized the whole map with, since by default we have
   * no idea of how far the points are, so we must consider they are
   * "very" far
   * so that's why I use AREA_START_GRADIENT/2
   */
  CURRENT_CURSOR[index].val = AREA_START_GRADIENT / 2;
  /*
   * default values...
   */
  CURRENT_CURSOR[index].loose_time = -1;
  CURRENT_CURSOR[index].score_order = 0;
  /*
   * the colors allocated for fighters start at 128 in the palette
   * and the COLORS_PER_TEAM value gives how much color per team
   * are available
   */
  CURRENT_CURSOR[index].color_entry = 128 + team * COLORS_PER_TEAM;
  /*
   * Now we set up the cursor name
   */
  strncpy (CURRENT_CURSOR[index].name, name, NAME_SIZE);
  CURRENT_CURSOR[index].name[NAME_SIZE] = '\0';
}

/*------------------------------------------------------------------*/
/*
 * this is where the cursor is "blitted" on the map, algorythmically
 * speaking, ie the map acknowledges the position of the cursor
 */
void
apply_all_cursor (void)
{
  int i, x, y;
  MESH *temp;

  /*
   * loop for all the active cursors
   */
  for (i = 0; i < NB_TEAMS; ++i)
    if (CURRENT_CURSOR[i].active)
      {
        /*
         * x and y hold the position of the cursor
         */
        x = CURRENT_CURSOR[i].x;
        y = CURRENT_CURSOR[i].y;
        /*
         * sanity check, in case the cursor is not on a valid part
         * of the map. It should not happen but I hate protection
         * faults!
         */
        if ((temp = CURRENT_AREA[y * CURRENT_AREA_W + x].mesh) != NULL)
          /*
           * we just poke the value of the cursor into its
           * corresponding position in the map
           * very simple, eh?
           */
          temp->info[CURRENT_CURSOR[i].team].state.grad
            = CURRENT_CURSOR[i].val;
      }
}

/*------------------------------------------------------------------*/
/*
 * the purpose of this function is to place the cursor automatically
 * on the map, knowing that we can give a fixed position for each
 * team since all maps are potentially different.
 */
void
auto_cursor (int index, int team, char *name)
{
  int i, x, y, x_min, x_max, y_min, y_max, ok, x0, y0, n;

  x = y = n = 0;
  /*
   * loop for all the fighters of all teams
   */
  for (i = 0; i < CURRENT_ARMY_SIZE; ++i)
    /*
     * we exclude fighters who do not belong to the team we want
     */
    if (CURRENT_ARMY[i].team == team)
      {
        /*
         * the following totals will be used to get the center
         * of all the fighters belonging to this army
         * of course it implies fighters have been placed
         */
        x += CURRENT_ARMY[i].x;
        y += CURRENT_ARMY[i].y;
        n++;
      }

  /*
   * x0 and y0 should now be right in the middle of our fichters
   */
  x0 = x_max = x_min = (x /= n);
  y0 = y_max = y_min = (y /= n);

  ok = 0;
  /*
   * loop as long as no correct position has been found
   * we can't admit a random placing for network consideration, so we follow
   * "spiral" arround a default position and chose the first working
   * position (ie without a wall)
   * the starting point of the spiral is x0,y0
   */
  while (!ok)
    {
      /*
       * try and find a good spot going right
       */
      for (x = x_min; x <= x_max && !ok; ++x)
        if (CURRENT_AREA[y * CURRENT_AREA_W + x].mesh != NULL)
          {
            ok = 1;
            x0 = x;
            y0 = y;
          }
      /*
       * if we did not touch the right side yet, let's increase the limit
       */
      if (x_max < CURRENT_AREA_W - 1)
        ++x_max;

      /*
       * try and find a good spot going down
       */
      for (y = y_min; y <= y_max && !ok; ++y)
        if (CURRENT_AREA[y * CURRENT_AREA_W + x].mesh != NULL)
          {
            ok = 1;
            x0 = x;
            y0 = y;
          }
      /*
       * if we did not touch the bottom side yet, let's increase the limit
       */
      if (y_max < CURRENT_AREA_H - 1)
        ++y_max;

      /*
       * try and find a good spot going left
       */
      for (x = x_max; x >= x_min && !ok; --x)
        if (CURRENT_AREA[y * CURRENT_AREA_W + x].mesh != NULL)
          {
            ok = 1;
            x0 = x;
            y0 = y;
          }
      /*
       * if we did not touch the left side yet, let's decrease the limit
       */
      if (x_min > 0)
        --x_min;

      /*
       * try and find a good spot going up
       */
      for (y = y_max; y >= y_min && !ok; --y)
        if (CURRENT_AREA[y * CURRENT_AREA_W + x].mesh != NULL)
          {
            ok = 1;
            x0 = x;
            y0 = y;
          }
      /*
       * if we did not touch the top side yet, let's decrease the limit
       */
      if (y_min > 0)
        --y_min;
    }

  init_cursor (index, team, x0, y0, name);
}
