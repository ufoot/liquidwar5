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
/* Copyright (C) 1998-2025 Christian Mauduit                                 */
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
/* nom           : mesh.c                                           */
/* contenu       : calcul du maillage                               */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "backport.h"
#include "bigdata.h"
#include "config.h"
#include "map.h"
#include "maptex.h"
#include "mesh.h"
#include "network.h"
#include "log.h"
#include "random.h"

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

typedef struct
{
  char used;
  short size;
  void *link[NB_DIRS];
  int corres;
}
MESHER;

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define MESH_FG 1
#define MESH_BG 2

#define MESH_MAX_ELEM_SIZE 16

MESH *CURRENT_MESH = NULL;
int CURRENT_MESH_SIZE = 0;
int CURRENT_AREA_W = 0;
int CURRENT_AREA_H = 0;
ALLEGRO_BITMAP *CURRENT_AREA_DISP = NULL;
ALLEGRO_BITMAP *CURRENT_AREA_BACK = NULL;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* fonction creant le premier mesher, base de tous les calculs      */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static MESHER *
create_first_mesher (ALLEGRO_BITMAP * map)
{
  MESHER *result;
  int x, y, h, w, size, i, j;

  w = al_get_bitmap_width (map);
  h = al_get_bitmap_height (map);
  size = h * w;
  result = malloc_in_big_data_top (size * sizeof (MESHER));
  if (result != NULL)
    {
      for (i = 0; i < size; ++i)
        {
          result[i].size = 1;
          result[i].corres = 3;
          for (j = 0; j < NB_DIRS; ++j)
            result[i].link[j] = NULL;
        }

      for (y = 0; y < h; ++y)
        for (x = 0; x < w; ++x)
          result[y * w + x].used = (getpixel (map, x, y) == MESH_FG) ? 0 : 1;

      for (y = 1; y < h - 1; ++y)
        for (x = 1; x < w - 1; ++x)
          {
            i = y * w + x;

            if (result[i].used)
              {
                if (result[i - w].used)
                  result[i].link[DIR_NNW] = result[i].link[DIR_NNE]
                    = result + i - w;
                if (result[i - w + 1].used)
                  result[i].link[DIR_NE] = result + i - w + 1;
                if (result[i + 1].used)
                  result[i].link[DIR_ENE] = result[i].link[DIR_ESE]
                    = result + i + 1;
                if (result[i + w + 1].used)
                  result[i].link[DIR_SE] = result + i + w + 1;
                if (result[i + w].used)
                  result[i].link[DIR_SSE] = result[i].link[DIR_SSW]
                    = result + i + w;
                if (result[i + w - 1].used)
                  result[i].link[DIR_SW] = result + i + w - 1;
                if (result[i - 1].used)
                  result[i].link[DIR_WSW] = result[i].link[DIR_WNW]
                    = result + i - 1;
                if (result[i - w - 1].used)
                  result[i].link[DIR_NW] = result + i - w - 1;
              }
          }
    }

  return result;
}

/*------------------------------------------------------------------*/
/* fonction regroupant tous les carres d'1 certaine taille          */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static int
group_mesher (MESHER * mesher, ALLEGRO_BITMAP * map, int step)
{
  int found = 0;
  int x, y, w, h, i, j, k;
  MESHER *ne, *se, *sw, *nw, *test;

  w = al_get_bitmap_width (map);
  h = al_get_bitmap_height (map);

  for (y = 0; y < h - step; y += step * 2)
    for (x = 0; x < w - step; x += step * 2)
      {
        i = y * w + x;
        ne = mesher + i + step;
        se = mesher + i + (w + 1) * step;
        sw = mesher + i + w * step;
        nw = mesher + i;

        if (ne->used && ne->size == step
            && se->used && se->size == step
            && sw->used && sw->size == step
            && nw->used && nw->size == step
            && ne->link[DIR_NNW] == ne->link[DIR_NNE]
            && ne->link[DIR_ENE] == ne->link[DIR_ESE]
            && se->link[DIR_ENE] == se->link[DIR_ESE]
            && se->link[DIR_SSE] == se->link[DIR_SSW]
            && sw->link[DIR_SSE] == sw->link[DIR_SSW]
            && sw->link[DIR_WSW] == sw->link[DIR_WNW]
            && nw->link[DIR_WSW] == nw->link[DIR_WNW]
            && nw->link[DIR_NNW] == nw->link[DIR_NNE]
            && ne->link[DIR_NE] != NULL
            && se->link[DIR_SE] != NULL
            && sw->link[DIR_SW] != NULL && nw->link[DIR_NW] != NULL)
          {
            ne->used = 0;
            se->used = 0;
            sw->used = 0;

            nw->size = step * 2;
            nw->link[DIR_NNE] = ne->link[DIR_NNE];
            nw->link[DIR_NE] = ne->link[DIR_NE];
            nw->link[DIR_ENE] = ne->link[DIR_ENE];
            nw->link[DIR_ESE] = se->link[DIR_ESE];
            nw->link[DIR_SE] = se->link[DIR_SE];
            nw->link[DIR_SSE] = se->link[DIR_SSE];
            nw->link[DIR_SSW] = sw->link[DIR_SSW];
            nw->link[DIR_SW] = sw->link[DIR_SW];
            nw->link[DIR_WSW] = sw->link[DIR_WSW];

            for (j = 0; j < NB_DIRS; ++j)
              for (k = 0; k < NB_DIRS; ++k)
                {
                  if (nw->link[j])
                    {
                      test = ((MESHER *) (nw->link[j]))->link[k];
                      if (test == ne || test == se || test == sw)
                        ((MESHER *) (nw->link[j]))->link[k] = nw;
                    }
                }

            ++found;
          }
      }

  return found;
}

/*------------------------------------------------------------------*/
/* fonction creant le maillage definitif a partir du temporaire     */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static MESH *
mesher_to_mesh (MESHER * mesher, ALLEGRO_BITMAP * map, int *size, int *w,
                int *h)
{
  MESH *result;
  MESHER *temp;
  int mesher_size, i, j, k;

  (*w) = al_get_bitmap_width (map);
  (*h) = al_get_bitmap_height (map);
  mesher_size = (*h) * (*w);

  *size = 0;
  for (i = 0; i < mesher_size; ++i)
    if (mesher[i].used)
      ++(*size);

  result = malloc_in_big_data_bottom ((*size) * sizeof (MESH));
  if (result != NULL)
    {
      j = 0;
      for (i = 0; i < mesher_size; ++i)
        {
          if (mesher[i].used)
            {
              result[j].x = i % (*w);
              result[j].y = i / (*w);
              result[j].side.decal_for_dir = 0;
              result[j].side.size = mesher[i].size;
              for (k = 0; k < NB_DIRS; ++k)
                result[j].link[k] = mesher[i].link[k];
              mesher[i].corres = j;

              j++;
            }
        }

      for (j = 0; j < (*size); ++j)
        for (k = 0; k < NB_DIRS; ++k)
          if ((temp = (result[j].link[k])) != NULL)
            result[j].link[k] = result + temp->corres;
    }

  return result;
}

/*------------------------------------------------------------------*/
/* fonction creant tout le maillage si necessaire                   */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
void
reset_mesh (void)
{
  int i, j;

  for (i = 0; i < CURRENT_MESH_SIZE; ++i)
    {
      for (j = 0; j < NB_TEAMS; ++j)
        {
          CURRENT_MESH[i].info[j].state.dir = (i + j) % NB_DIRS;
          CURRENT_MESH[i].info[j].update.time = -1;
        }
    }
}

/*------------------------------------------------------------------*/
static void
test_one_mesh (MESH * test)
{
  MESH m;

  m.x = test->x;
  m.y = test->y;
  m.side = test->side;
  m.info[NB_TEAMS - 1] = test->info[NB_TEAMS - 1];
  m.link[NB_DIRS - 1] = test->link[NB_DIRS - 1];

  if (m.x >= 0 && m.y >= 0)
    {
      /*
       * Test above is worth nothing, actually it's
       * to get rid of a computer warning, but to
       * be honest this function existence is just
       * to check memory handling is OK and show
       * up basic pointers errors and reveal bugs.
       */
    }
}

/*------------------------------------------------------------------*/
void
test_mesh (void)
{
  int i, k;

  for (i = 0; i < CURRENT_MESH_SIZE; ++i)
    {
      test_one_mesh (CURRENT_MESH + i);
      for (k = 0; k < NB_DIRS; ++k)
        if (CURRENT_MESH[i].link[k] != NULL)
          test_one_mesh (CURRENT_MESH[i].link[k]);
    }
}

/*------------------------------------------------------------------*/
int
create_mesh (void)
{
  int retour = 0, i;
  ALLEGRO_BITMAP *map;
  MESHER *mesher;
  int zoom_factor = 1;

  CURRENT_MESH = NULL;
  CURRENT_MESH_SIZE = 0;
  CURRENT_AREA_W = 0;
  CURRENT_AREA_H = 0;

  if ((map = lw_map_create_bicolor
       (CONFIG_LEVEL_MAP,
        MESH_FG,
        MESH_BG,
        LW_NETWORK_ON,
        LW_RANDOM_ON,
        MIN_MAP_RES_W[LW_CONFIG_CURRENT_RULES.min_map_res],
        MIN_MAP_RES_H[LW_CONFIG_CURRENT_RULES.min_map_res],
        &zoom_factor)) != NULL)
    {
      mesher = create_first_mesher (map);
      if (mesher != NULL)
        {
          i = 1;
          while (i <= MESH_MAX_ELEM_SIZE / 2 && group_mesher (mesher, map, i))
            i *= 2;
          CURRENT_MESH = mesher_to_mesh (mesher, map,
                                         &CURRENT_MESH_SIZE,
                                         &CURRENT_AREA_W, &CURRENT_AREA_H);
          free_last_big_data_top ();
        }
      al_destroy_bitmap (map);
    }
  if (CURRENT_MESH == NULL)
    retour = -1;                /* pas assez de memoire */

  return retour;
}

/*------------------------------------------------------------------*/
/* fonction creant une bitmap de demo du maillage si necessaire     */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
create_mesh_bitmap (int mode)
{
  ALLEGRO_BITMAP *result;
  int i, j, color;

  result = lw_maptex_create_map
    (CONFIG_LEVEL_MAP,
     CONFIG_LEVEL_FG,
     CONFIG_LEVEL_BG,
     LW_NETWORK_ON,
     LW_RANDOM_ON,
     MIN_MAP_RES_W[LW_CONFIG_CURRENT_RULES.min_map_res],
     MIN_MAP_RES_H[LW_CONFIG_CURRENT_RULES.min_map_res],
     CONFIG_USE_DEFAULT_TEXTURE);
  if (result)
    {
      for (i = 0; i < CURRENT_MESH_SIZE; ++i)
        {
          switch (mode)
            {
            case 0:
              color = 0;
              j = CURRENT_MESH[i].side.size;
              while (j > 1)
                {
                  j /= 2;
                  ++color;
                }
              break;
            default:
              color = i;
            }

          rectfill (result, CURRENT_MESH[i].x,
                    CURRENT_MESH[i].y,
                    CURRENT_MESH[i].x +
                    CURRENT_MESH[i].side.size - 1,
                    CURRENT_MESH[i].y +
                    CURRENT_MESH[i].side.size - 1, 96 + color % 32);
        }
    }

  return result;
}
