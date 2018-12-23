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
/* Liquid War homepage : https://ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* nom           : pion.c                                           */
/* contenu       : affichage des curseurs                           */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#include "area.h"
#include "base.h"
#include "config.h"
#include "cursor.h"
#include "decal.h"
#include "palette.h"
#include "viewport.h"
#include "pion.h"
#include "time.h"

/*==================================================================*/
/* definitions de types                                             */
/*==================================================================*/

typedef struct
{
  int x;
  int y;
  int color;
  int row;
}
CURSOR_POINT;

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define CURSOR_CYCLE  256
#define CURSOR_LIGHTS 2

#define CURSOR_COLOR_NUMBER 32
#define CURSOR_POINT_NUMBER 68

#define CL_NUMBER  3

#define CL_INSIDE  0
#define CL_MIDDLE  1
#define CL_MIDDLE2 2
#define CL_OUTSIDE 3

static CURSOR_POINT CURSOR_LAYOUT[CURSOR_POINT_NUMBER] =
  { {0, -3, 0, CL_INSIDE}, {0, -4, 0, CL_MIDDLE2}, {0, -5, 0, CL_OUTSIDE},
{1, -4, 1, CL_MIDDLE}, {1, -5, 1, CL_OUTSIDE},
{1, -3, 2, CL_INSIDE}, {2, -4, 2, CL_OUTSIDE},
{2, -3, 3, CL_MIDDLE}, {3, -4, 3, CL_OUTSIDE},
{2, -2, 4, CL_INSIDE}, {3, -3, 4, CL_MIDDLE2},
{3, -2, 5, CL_MIDDLE}, {4, -3, 5, CL_OUTSIDE},
{3, -1, 6, CL_INSIDE}, {4, -2, 6, CL_OUTSIDE},
{4, -1, 7, CL_MIDDLE}, {5, -1, 7, CL_OUTSIDE},

{3, 0, 8, CL_INSIDE}, {4, 0, 8, CL_MIDDLE2}, {5, 0, 8, CL_OUTSIDE},
{4, 1, 9, CL_MIDDLE}, {5, 1, 9, CL_OUTSIDE},
{3, 1, 10, CL_INSIDE}, {4, 2, 10, CL_OUTSIDE},
{3, 2, 11, CL_MIDDLE}, {4, 3, 11, CL_OUTSIDE},
{2, 2, 12, CL_INSIDE}, {3, 3, 12, CL_MIDDLE2},
{2, 3, 13, CL_MIDDLE}, {3, 4, 13, CL_OUTSIDE},
{1, 3, 14, CL_INSIDE}, {2, 4, 14, CL_OUTSIDE},
{1, 4, 15, CL_MIDDLE}, {1, 5, 15, CL_OUTSIDE},

{-0, 3, 16, CL_INSIDE}, {-0, 4, 16, CL_MIDDLE2}, {-0, 5, 16, CL_OUTSIDE},
{-1, 4, 17, CL_MIDDLE}, {-1, 5, 17, CL_OUTSIDE},
{-1, 3, 18, CL_INSIDE}, {-2, 4, 18, CL_OUTSIDE},
{-2, 3, 19, CL_MIDDLE}, {-3, 4, 19, CL_OUTSIDE},
{-2, 2, 20, CL_INSIDE}, {-3, 3, 20, CL_MIDDLE2},
{-3, 2, 21, CL_MIDDLE}, {-4, 3, 21, CL_OUTSIDE},
{-3, 1, 22, CL_INSIDE}, {-4, 2, 22, CL_OUTSIDE},
{-4, 1, 23, CL_MIDDLE}, {-5, 1, 23, CL_OUTSIDE},

{-3, -0, 8, CL_INSIDE}, {-4, -0, 24, CL_MIDDLE2}, {-5, -0, 25, CL_OUTSIDE},
{-4, -1, 25, CL_MIDDLE}, {-5, -1, 26, CL_OUTSIDE},
{-3, -1, 10, CL_INSIDE}, {-4, -2, 27, CL_OUTSIDE},
{-3, -2, 27, CL_MIDDLE}, {-4, -3, 28, CL_OUTSIDE},
{-2, -2, 12, CL_INSIDE}, {-3, -3, 28, CL_MIDDLE2},
{-2, -3, 29, CL_MIDDLE}, {-3, -4, 29, CL_OUTSIDE},
{-1, -3, 14, CL_INSIDE}, {-2, -4, 30, CL_OUTSIDE},
{-1, -4, 31, CL_MIDDLE}, {-1, -5, 31, CL_OUTSIDE}
};

static char CURSOR_GRAPHIC_MEMORY[NB_TEAMS][CURSOR_POINT_NUMBER];

static char CURSOR_COLOR_MAP[CURSOR_COLOR_NUMBER];
static char CURSOR_COLOR_BACK[CURSOR_COLOR_NUMBER];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
init_disp_cursor (void)
{
  int i, x, y, color_back;

  for (i = 0; i < CURSOR_COLOR_NUMBER; ++i)
    {
      color_back = 0;
      while (color_back < FG_TEXTURE_FIRST_COLOR
             || color_back >= FG_TEXTURE_FIRST_COLOR + 32)
        {
          x = random () % CURRENT_AREA_W;
          y = random () % CURRENT_AREA_H;
          color_back = getpixel (CURRENT_AREA_BACK, x, y);
        }
      CURSOR_COLOR_BACK[i] = color_back;
    }
}

/*------------------------------------------------------------------*/
static void
disp_cursor (int number)
{
  int i, x, y, x0, y0, color_offset;

  x0 = CURRENT_CURSOR[number].x;
  y0 = CURRENT_CURSOR[number].y;
  color_offset = COLOR_FIRST_ENTRY[CURRENT_CURSOR[number].team];


  for (i = 0; i < CURSOR_POINT_NUMBER; ++i)
    {
      x = x0 + CURSOR_LAYOUT[i].x;
      y = y0 + CURSOR_LAYOUT[i].y;
      CURSOR_GRAPHIC_MEMORY[number][i] = getpixel (CURRENT_AREA_DISP, x, y);
      switch (CURSOR_LAYOUT[i].row)
        {
        case CL_OUTSIDE:
        case CL_INSIDE:
        case CL_MIDDLE:
          putpixel (CURRENT_AREA_DISP, x, y,
                    CURSOR_COLOR_MAP[CURSOR_LAYOUT[i].color] + color_offset);
          break;
        case CL_MIDDLE2:
          putpixel (CURRENT_AREA_DISP, x, y,
                    CURSOR_COLOR_BACK[CURSOR_LAYOUT[i].color]);
          break;
        }
    }
}

/*------------------------------------------------------------------*/
static void
undisp_cursor (int number)
{
  int i, x0, y0;

  x0 = CURRENT_CURSOR[number].x;
  y0 = CURRENT_CURSOR[number].y;

  for (i = 0; i < CURSOR_POINT_NUMBER; ++i)
    {
      putpixel (CURRENT_AREA_DISP,
                x0 + CURSOR_LAYOUT[i].x,
                y0 + CURSOR_LAYOUT[i].y, CURSOR_GRAPHIC_MEMORY[number][i]);
    }
}

/*------------------------------------------------------------------*/
void
disp_all_cursors (void)
{
  int i, degrad_size, fp, ip;

  degrad_size = CURSOR_COLOR_NUMBER / (2 * CURSOR_LIGHTS);
  ip = ((GLOBAL_TICKER * COLORS_PER_TEAM) / CURSOR_CYCLE)
    % (COLORS_PER_TEAM * 2);
  fp = 0;

  for (i = 0; i < CURSOR_COLOR_NUMBER; ++i)
    {
      if (ip < COLORS_PER_TEAM)
        CURSOR_COLOR_MAP[i] = ip;
      else
        CURSOR_COLOR_MAP[i] = 2 * COLORS_PER_TEAM - 1 - ip;

      fp += COLORS_PER_TEAM;
      while (fp >= degrad_size)
        {
          fp -= degrad_size;
          ip++;
        }
      while (ip >= 2 * COLORS_PER_TEAM)
        ip -= 2 * COLORS_PER_TEAM;
    }

  for (i = 0; i < NB_TEAMS; ++i)
    if (CURRENT_CURSOR[i].active)
      disp_cursor (i);
}

/*------------------------------------------------------------------*/
void
undisp_all_cursors (void)
{
  int i;

  for (i = NB_TEAMS - 1; i >= 0; --i)
    if (CURRENT_CURSOR[i].active)
      undisp_cursor (i);
}
