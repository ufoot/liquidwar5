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
/* nom           : viewport.c                                       */
/* contenu       : gestion de l'aire de jeu et du page flipping     */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>

#include "area.h"
#include "alleg2.h"
#include "config.h"
#include "gfxmode.h"
#include "viewport.h"
#include "info.h"
#include "distor.h"
#include "log.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int PAGE_FLIP_H = 0;
ALLEGRO_BITMAP *NEXT_SCREEN = NULL;

static int VIEWPORT_X = 0;
static int VIEWPORT_Y = 0;
static int VIEWPORT_W = 0;
static int VIEWPORT_H = 0;

static int VIEWPORT_CHANGE_REGISTERED = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
rect_for_viewport (void)
{
  rect (screen, VIEWPORT_X - 1, VIEWPORT_Y - 1 + PAGE_FLIP_H,
        VIEWPORT_X + VIEWPORT_W,
        VIEWPORT_Y + VIEWPORT_H + PAGE_FLIP_H, MENU_FG);
}

/*------------------------------------------------------------------*/
static void
init_viewport_size (int x, int y, int w, int h)
{
  int scale_w;
  int scale_h;

  static int viewport_sizes[7] = { 18, 24, 27, 30, 33, 35, 36 };

  scale_w = 10 * w;
  scale_h = 10 * h;

  if (CONFIG_VIEWPORT_SIZE >= 8)
    {
      VIEWPORT_W = scale_w;
      VIEWPORT_H = scale_h;
    }

  if (CONFIG_VIEWPORT_SIZE > 0 && CONFIG_VIEWPORT_SIZE < 8)
    {
      if (scale_w * CURRENT_AREA_H < scale_h * CURRENT_AREA_W)
        {
          VIEWPORT_W = (scale_w * (viewport_sizes[CONFIG_VIEWPORT_SIZE - 1]))
            / viewport_sizes[6];
          VIEWPORT_H = (VIEWPORT_W * CURRENT_AREA_H) / CURRENT_AREA_W;
        }
      else
        {
          VIEWPORT_H = (scale_h * (viewport_sizes[CONFIG_VIEWPORT_SIZE - 1]))
            / viewport_sizes[6];
          VIEWPORT_W = (VIEWPORT_H * CURRENT_AREA_W) / CURRENT_AREA_H;
        }
    }

  if (CONFIG_VIEWPORT_SIZE <= 0)
    {
      VIEWPORT_W = CURRENT_AREA_W;
      VIEWPORT_H = CURRENT_AREA_H;
      while (VIEWPORT_W > w || VIEWPORT_H > h)
        {
          VIEWPORT_W /= 2;
          VIEWPORT_H /= 2;
        }
    }
  else
    {
      VIEWPORT_W *= w;
      VIEWPORT_H *= h;

      VIEWPORT_W /= scale_w;
      VIEWPORT_H /= scale_h;
    }

  VIEWPORT_X = x + ((w - VIEWPORT_W) / 8) * 4;
  VIEWPORT_Y = y + (h - VIEWPORT_H) / 2;
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
page_flip (void)
{
  int old_h, new_h;
  int x, y, w, h;

  if (CONFIG_PAGE_FLIP && VIRTUAL_H >= (2 * SCREEN_H))
    {
      if (NEXT_SCREEN)
        {
          destroy_bitmap (NEXT_SCREEN);
        }

      lw_info_get_room_for_viewport (&x, &y, &w, &h);
      init_viewport_size (x, y, w, h);

      old_h = PAGE_FLIP_H;
      new_h = old_h ? 0 : SCREEN_H;

      scroll_screen (0, old_h);
      set_clip_rect (screen, 0, new_h, SCREEN_W - 1, new_h + SCREEN_H - 1);

      PAGE_FLIP_H = new_h;

      NEXT_SCREEN = create_sub_bitmap (screen, VIEWPORT_X,
                                       VIEWPORT_Y + new_h,
                                       VIEWPORT_W, VIEWPORT_H);
    }
  else
    {
      PAGE_FLIP_H = 0;
      if (VIEWPORT_CHANGE_REGISTERED || !NEXT_SCREEN)
        {
          scroll_screen (0, 0);
          set_clip_rect (screen, 0, 0, SCREEN_W - 1, SCREEN_H - 1);

          lw_info_get_room_for_viewport (&x, &y, &w, &h);
          init_viewport_size (x, y, w, h);

          NEXT_SCREEN = create_sub_bitmap (screen, VIEWPORT_X,
                                           VIEWPORT_Y,
                                           VIEWPORT_W, VIEWPORT_H);
        }
    }

  if (VIEWPORT_CHANGE_REGISTERED > 0)
    {
      rect_for_viewport ();
      init_distorsion_displayer ();
      VIEWPORT_CHANGE_REGISTERED--;
    }

  return (NEXT_SCREEN);
}

/*------------------------------------------------------------------*/
void
last_flip (void)
{
  if (NEXT_SCREEN)
    {
      if (CONFIG_PAGE_FLIP && VIRTUAL_H >= (2 * SCREEN_H))
        {
          set_clip_rect (screen, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
          if (!PAGE_FLIP_H)
            {
              blit (screen, screen, 0, SCREEN_H, 0, 0, SCREEN_W, SCREEN_H);
              scroll_screen (0, 0);
            }
          else
            PAGE_FLIP_H = 0;
        }

      destroy_bitmap (NEXT_SCREEN);
      NEXT_SCREEN = NULL;
    }
}

/*------------------------------------------------------------------*/
void
lw_viewport_register_change (int count)
{
  VIEWPORT_CHANGE_REGISTERED = count;
}
