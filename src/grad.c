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
/* nom           : grad.c                                           */
/* contenu       : etalement des gradients                          */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "alleg2.h"
#include "area.h"
#include "decal.h"
#include "grad.h"
#include "mesh.h"
#include "palette.h"
#include "spread.h"
#include "startup.h"
#include "time.h"
#include "config.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define OFFSET_TO_FIRST_LINK 56

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
create_gradient_bitmap (int team)
{
  ALLEGRO_BITMAP *result;
  int x, y, i, color;
  MESH *temp;

  result = my_create_bitmap (CURRENT_AREA_W, CURRENT_AREA_H);

  if (result)
    {
      i = 0;
      for (y = 0; y < CURRENT_AREA_H; ++y)
        for (x = 0; x < CURRENT_AREA_W; ++x)
          {
            if ((temp = CURRENT_AREA[i++].mesh) != NULL)
              {
                color = (temp->info[team].state.grad
                         + AREA_START_GRADIENT) % COLORS_PER_TEAM;
                putpixel (result, x, y, color + COLOR_FIRST_ENTRY[team]);
              }
            else
              putpixel (result, x, y, MENU_BG);
          }
    }

  return result;
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
create_dir_bitmap (int team)
{
  ALLEGRO_BITMAP *result;
  int x, y, i;
  MESH *temp;

  result = my_create_bitmap (CURRENT_AREA_W, CURRENT_AREA_H);

  i = 0;
  for (y = 0; y < CURRENT_AREA_H; ++y)
    for (x = 0; x < CURRENT_AREA_W; ++x)
      {
        if ((temp = CURRENT_AREA[i++].mesh) != NULL)
          putpixel (result, x, y, 134
                    + (temp->info[team].state.dir / 4) * 42
                    + (temp->info[team].state.dir % 4) * 10);
      }

  return result;
}

/*------------------------------------------------------------------*/
/* etalement des gradients                                          */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
void
spread_single_gradient (void)
{
  int i, new_grad, dir;
  MESH *pos, *last;
  MESH *temp;

  dir = (GLOBAL_CLOCK * 7) % NB_DIRS;
  switch (dir)
    {
    case DIR_ENE:
    case DIR_ESE:
    case DIR_SE:
    case DIR_SSE:
    case DIR_SSW:
    case DIR_SW:
      pos = CURRENT_MESH;
      last = CURRENT_MESH + CURRENT_MESH_SIZE;
#ifdef ASM
      if (STARTUP_ASM && LW_CONFIG_CURRENT_RULES.asm_algorithm)
        switch (PLAYING_TEAMS)
          {
          case 6:
            boost_gradient_down_6
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          case 5:
            boost_gradient_down_5
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          case 4:
            boost_gradient_down_4
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          case 3:
            boost_gradient_down_3
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          default:
            boost_gradient_down_2
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          }
      else
#endif
        while (pos != last)
          {
            for (i = 0; i < PLAYING_TEAMS; ++i)
              if ((temp = (pos->link[dir]))
                  && (temp->info[i].state.grad
                      > (new_grad =
                         pos->info[i].state.grad + pos->side.size)))
                temp->info[i].state.grad = new_grad;
            pos++;
          }
      break;
    case DIR_WSW:
    case DIR_WNW:
    case DIR_NW:
    case DIR_NNW:
    case DIR_NNE:
    case DIR_NE:
      pos = CURRENT_MESH + CURRENT_MESH_SIZE - 1;
      last = CURRENT_MESH - 1;
#ifdef ASM
      if (STARTUP_ASM && LW_CONFIG_CURRENT_RULES.asm_algorithm)
        switch (PLAYING_TEAMS)
          {
          case 6:
            boost_gradient_up_6
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          case 5:
            boost_gradient_up_5
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          case 4:
            boost_gradient_up_4
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          case 3:
            boost_gradient_up_3
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          default:
            boost_gradient_up_2
              (pos, CURRENT_MESH_SIZE - 1, OFFSET_TO_FIRST_LINK + dir * 4);
            break;
          }
      else
#endif
        while (pos != last)
          {
            for (i = 0; i < PLAYING_TEAMS; ++i)
              if ((temp = (pos->link[dir]))
                  && (temp->info[i].state.grad
                      > (new_grad =
                         pos->info[i].state.grad + pos->side.size)))
                temp->info[i].state.grad = new_grad;
            pos--;
          }
      break;
    }
}
