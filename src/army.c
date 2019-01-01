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
/* Copyright (C) 1998-2019 Christian Mauduit                                 */
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
/* nom           : army.c                                           */
/* contenu       : placement des armees                             */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "area.h"
#include "army.h"
#include "bigdata.h"
#include "config.h"
#include "cursor.h"
#include "mesh.h"
#include "decal.h"
#include "network.h"
#include "log.h"

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

FIGHTER *CURRENT_ARMY = NULL;
int CURRENT_ARMY_SIZE = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static int
get_battle_room (void)
{
  int i, n;

  n = 0;
  for (i = 0; i < CURRENT_MESH_SIZE; ++i)
    n += CURRENT_MESH[i].side.size * CURRENT_MESH[i].side.size;

  return n;
}

/*------------------------------------------------------------------*/
static int
add_fighter (FIGHTER * f, int team, int x, int y, int health)
{
  int j;

  j = y * CURRENT_AREA_W + x;

  if (CURRENT_AREA[j].mesh && (!CURRENT_AREA[j].fighter))
    {
      CURRENT_AREA[j].fighter = f;
      f->health = health;
      f->team = team;
      f->x = x;
      f->y = y;
      f->last_dir = j % NB_DIRS;
      return 1;
    }
  else
    return 0;
}

/*------------------------------------------------------------------*/
static void
place_team (int part, int team)
{
  int placed, x, y, x_min, y_min, x_max, y_max;
  int fighters;
  int health;
  FIGHTER *pos;

  fighters = CURRENT_ARMY_SIZE / PLAYING_TEAMS;
  ACTIVE_FIGHTERS[team] = 0;
  health = MAX_FIGHTER_HEALTH - 1;

  switch (part)
    {
    case 0:
      x = CURRENT_AREA_W / 6;
      y = CURRENT_AREA_H / 4;
      break;
    case 1:
      x = CURRENT_AREA_W / 2;
      y = CURRENT_AREA_H / 4;
      break;
    case 2:
      x = (5 * CURRENT_AREA_W) / 6;
      y = CURRENT_AREA_H / 4;
      break;
    case 3:
      x = CURRENT_AREA_W / 6;
      y = (3 * CURRENT_AREA_H) / 4;
      break;
    case 4:
      x = CURRENT_AREA_W / 2;
      y = (3 * CURRENT_AREA_H) / 4;
      break;
    default:
      x = (5 * CURRENT_AREA_W) / 6;
      y = (3 * CURRENT_AREA_H) / 4;
      break;
    }
  x_min = x_max = x;
  y_min = y_max = y;

  pos = CURRENT_ARMY + team;
  placed = 0;
  while (placed < fighters)
    {
      for (x = x_min; x <= x_max && placed < fighters; ++x)
        placed += add_fighter
          (pos + placed * PLAYING_TEAMS, team, x, y_min, health);
      if (x_max < CURRENT_AREA_W - 2)
        ++x_max;

      for (y = y_min; y <= y_max && placed < fighters; ++y)
        placed += add_fighter
          (pos + placed * PLAYING_TEAMS, team, x_max, y, health);
      if (y_max < CURRENT_AREA_H - 2)
        ++y_max;

      for (x = x_max; x >= x_min && placed < fighters; --x)
        placed += add_fighter
          (pos + placed * PLAYING_TEAMS, team, x, y_max, health);
      if (x_min > 1)
        --x_min;

      for (y = y_max; y >= y_min && placed < fighters; --y)
        placed += add_fighter
          (pos + placed * PLAYING_TEAMS, team, x_min, y, health);
      if (y_min > 1)
        --y_min;
    }
}

/*------------------------------------------------------------------*/
void
place_all_team (void)
{
  int i, j, n;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      ACTIVE_FIGHTERS[i] = 0;
    }

  n = 0;
  if (LW_NETWORK_ON)
    {
      /*
       * In this case, we *must* place the teams in the same order
       * or the behavior would not be the same on all the computers.
       */
      for (j = 0; j < NB_TEAMS && n < NB_TEAMS; ++j)
        {
          for (i = 0; i < NB_TEAMS && n < NB_TEAMS; ++i)
            {
              if (LW_NETWORK_INFO[i].active && LW_NETWORK_INFO[i].part == j)
                {
                  place_team (LW_NETWORK_INFO[i].part,
                              LW_NETWORK_INFO[i].server_id);
                  auto_cursor (i,
                               LW_NETWORK_INFO[i].server_id,
                               LW_NETWORK_INFO[i].name);
                  n++;
                }
            }
        }
    }
  else
    {
      for (i = 0; i < NB_TEAMS && n < NB_TEAMS; ++i)
        {
          if (CONFIG_CONTROL_TYPE[i] != CONFIG_CONTROL_TYPE_OFF)
            {
              place_team (i, n);
              auto_cursor (i, n, CONFIG_PLAYER_NAME[i]);
              n++;
            }
        }
    }
}

/*------------------------------------------------------------------*/
void
test_army (void)
{
  int i, k, x, y;
  FIGHTER f;
  MESH *m;

  for (i = 0; i < CURRENT_ARMY_SIZE; ++i)
    {
      f.team = CURRENT_ARMY[i].team;
      x = f.x = CURRENT_ARMY[i].x;
      y = f.y = CURRENT_ARMY[i].y;
      f.health = CURRENT_ARMY[i].health;
      k = y * CURRENT_AREA_W + x;
      m = CURRENT_AREA[k].mesh;
      if (m)
        if (m->info[0].state.grad)
          {
            /*
             * test above does nothing, but at least
             * it queries memory, should we have an
             * ugly bug, at least it would reveal
             * it now rather than show up later.
             */
          }
    }
}

/*------------------------------------------------------------------*/
int
create_army (void)
{
  int ret = -1, i;
  int fill_table[33] = { 1, 2, 3, 4, 5, 6, 8, 9,
    10, 12, 14, 16, 18, 20, 22, 24,
    25, 27, 29, 31, 33, 36, 40, 45,
    50, 55, 60, 65, 70, 75, 80, 90,
    99
  };

  CURRENT_ARMY_SIZE = (get_battle_room () *
                       fill_table[LW_CONFIG_CURRENT_RULES.fighter_number]) /
    100;

  CURRENT_ARMY_SIZE /= PLAYING_TEAMS;
  if (CURRENT_ARMY_SIZE < 1)
    CURRENT_ARMY_SIZE = 1;
  CURRENT_ARMY_SIZE *= PLAYING_TEAMS;
  if ((CURRENT_ARMY
       = malloc_in_big_data_bottom
       (CURRENT_ARMY_SIZE * sizeof (FIGHTER))) != 0)
    {
      for (i = 0; i < CURRENT_ARMY_SIZE; ++i)
        {
          CURRENT_ARMY[i].health = 0;
          CURRENT_ARMY[i].team = -1;
          CURRENT_ARMY[i].x = 0;
          CURRENT_ARMY[i].y = 0;
          CURRENT_ARMY[i].last_dir = 0;
        }
      ret = 0;
    }
  return ret;
}
