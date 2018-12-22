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
/* Copyright (C) 1998-2016 Christian Mauduit                                 */
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
/* nom           : info.c                                           */
/* contenu       : gestion de la barre d'information                */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#include "alleg2.h"
#include "army.h"
#include "back.h"
#include "config.h"
#include "info.h"
#include "decal.h"
#include "palette.h"
#include "maptex.h"
#include "texture.h"
#include "time.h"
#include "viewport.h"
#include "watchdog.h"
#include "macro.h"
#include "network.h"
#include "random.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define INFO_BAR_W 50
#define INFO_BAR_H 15

static BITMAP *INFO_BAR_BACK = NULL;
static BITMAP *INFO_BAR = NULL;
static int INFO_BAR_POS_X[NB_TEAMS];
static int INFO_BAR_POS_Y[NB_TEAMS];
static int INFO_BAR_POS_W;
static int INFO_BAR_POS_H;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* on verifie si il y a des changements sur la barre d'etat         */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
int
init_info_bar (int w, int h, int epaisseur)
{
  int i, x, y;
  BITMAP *front, *back;

  if (INFO_BAR)
    destroy_bitmap (INFO_BAR);
  INFO_BAR = my_create_bitmap (w, h);
  INFO_BAR_BACK = my_create_bitmap (w, h);

  if (INFO_BAR && INFO_BAR_BACK)
    {
      if (w > h)
        {
          rectfill (INFO_BAR, 0, 0, epaisseur - 1, h - 1, MENU_BG);
          vline (INFO_BAR, epaisseur, 0, h - 1, MENU_FG);

          front =
            lw_maptex_create_fg (CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG,
                                 LW_NETWORK_ON, LW_RANDOM_ON,
                                 CONFIG_USE_DEFAULT_TEXTURE);
          back =
            lw_maptex_create_bg (CONFIG_LEVEL_MAP, CONFIG_LEVEL_BG,
                                 LW_NETWORK_ON, LW_RANDOM_ON,
                                 CONFIG_USE_DEFAULT_TEXTURE);

          for (y = 0; y < h; y += front->h)
            for (x = epaisseur + 1; x < w; x += front->w)
              draw_sprite (INFO_BAR, front, x, y);

          INFO_BAR_POS_W = w - epaisseur - 5;
          INFO_BAR_POS_H = (h - 1) / PLAYING_TEAMS - 1;
          for (i = 0; i < PLAYING_TEAMS; ++i)
            {
              INFO_BAR_POS_X[i] = epaisseur + 3;
              INFO_BAR_POS_Y[i] = (h + 1 - PLAYING_TEAMS
                                   * (INFO_BAR_POS_H + 1)) / 2
                + i * (INFO_BAR_POS_H + 1);

              for (y = INFO_BAR_POS_Y[i];
                   y < INFO_BAR_POS_Y[i] + INFO_BAR_POS_H; y++)
                for (x = INFO_BAR_POS_X[i];
                     x < INFO_BAR_POS_X[i] + INFO_BAR_POS_W; x++)
                  putpixel (INFO_BAR, x, y,
                            getpixel (back,
                                      (x - epaisseur - 1) % back->w,
                                      y % back->h));
            }

          destroy_bitmap (back);
          destroy_bitmap (front);
        }
      else
        {
          rectfill (INFO_BAR, 0, 0, w - 1, epaisseur - 1, MENU_BG);
          hline (INFO_BAR, 0, epaisseur, w - 1, MENU_FG);

          front =
            lw_maptex_create_fg (CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG,
                                 LW_NETWORK_ON, LW_RANDOM_ON,
                                 CONFIG_USE_DEFAULT_TEXTURE);
          back =
            lw_maptex_create_bg (CONFIG_LEVEL_MAP, CONFIG_LEVEL_BG,
                                 LW_NETWORK_ON, LW_RANDOM_ON,
                                 CONFIG_USE_DEFAULT_TEXTURE);

          for (y = epaisseur + 1; y < h; y += front->h)
            for (x = 0; x < w; x += front->w)
              draw_sprite (INFO_BAR, front, x, y);

          INFO_BAR_POS_W = (w - 1) / PLAYING_TEAMS - 1;
          INFO_BAR_POS_H = h - epaisseur - 5;
          for (i = 0; i < PLAYING_TEAMS; ++i)
            {
              INFO_BAR_POS_X[i] = (w + 1 - PLAYING_TEAMS
                                   * (INFO_BAR_POS_W + 1)) / 2
                + i * (INFO_BAR_POS_W + 1);
              INFO_BAR_POS_Y[i] = epaisseur + 3;

              for (y = INFO_BAR_POS_Y[i];
                   y < INFO_BAR_POS_Y[i] + INFO_BAR_POS_H; y++)
                for (x = INFO_BAR_POS_X[i];
                     x < INFO_BAR_POS_X[i] + INFO_BAR_POS_W; x++)
                  putpixel (INFO_BAR, x, y,
                            getpixel (back,
                                      x % back->w,
                                      (y - epaisseur - 1) % back->h));
            }

          destroy_bitmap (back);
          destroy_bitmap (front);
        }
      draw_sprite (INFO_BAR_BACK, INFO_BAR, 0, 0);
    }
  return (INFO_BAR && INFO_BAR_BACK);
}

/*------------------------------------------------------------------*/
void
free_info_bar (void)
{
  if (INFO_BAR)
    {
      destroy_bitmap (INFO_BAR);
      INFO_BAR = NULL;
    }
  if (INFO_BAR_BACK)
    {
      destroy_bitmap (INFO_BAR_BACK);
      INFO_BAR_BACK = NULL;
    }
}

/*------------------------------------------------------------------*/
static void
display_time (int x, int y, int w, int h)
{
  char buffer[6];
  int min, sec, time;

  /*
   * w and h args are ignored for now
   */
  LW_MACRO_NOP (w);
  LW_MACRO_NOP (h);

  time = TIME_LEFT;
  if (time < 0)
    time = 0;
  min = time / 60;
  sec = time % 60;

  buffer[0] = '0' + min / 10;
  buffer[1] = '0' + min % 10;
  buffer[2] = ':';
  buffer[3] = '0' + sec / 10;
  buffer[4] = '0' + sec % 10;
  buffer[5] = 0;

  textout_ex (INFO_BAR, font, buffer, x + 1, y + 1, -1, -1);
}

/*------------------------------------------------------------------*/
static void
display_horizontal_info (int w, int h, int epaisseur)
{
  int i, barre_w, barre_x, barre_y;

  /*
   * w arg is ignored for now
   */
  LW_MACRO_NOP (w);

  display_time (0, 0, epaisseur, h);

  for (i = 0; i < PLAYING_TEAMS; ++i)
    {
      barre_w = (ACTIVE_FIGHTERS[i] * (INFO_BAR_POS_W - 1))
        / CURRENT_ARMY_SIZE + 1;
      barre_x = INFO_BAR_POS_X[i] + INFO_BAR_POS_W - barre_w;
      barre_y = INFO_BAR_POS_Y[i];
      rectfill (INFO_BAR,
                barre_x,
                barre_y,
                barre_x + barre_w - 1,
                barre_y + INFO_BAR_POS_H - 1,
                COLOR_FIRST_ENTRY[i] + COLORS_PER_TEAM - 1);
    }
}

/*------------------------------------------------------------------*/
static void
display_vertical_info (int w, int h, int epaisseur)
{
  int i, barre_h, barre_x, barre_y;

  /*
   * h arg is ignored for now
   */
  LW_MACRO_NOP (h);

  display_time (0, 0, w, epaisseur);

  for (i = 0; i < PLAYING_TEAMS; ++i)
    {
      barre_h = (ACTIVE_FIGHTERS[i] * (INFO_BAR_POS_H - 1))
        / CURRENT_ARMY_SIZE + 1;
      barre_y = INFO_BAR_POS_Y[i] + INFO_BAR_POS_H - barre_h;
      barre_x = INFO_BAR_POS_X[i];
      rectfill (INFO_BAR,
                barre_x,
                barre_y,
                barre_x + INFO_BAR_POS_W - 1,
                barre_y + barre_h - 1,
                COLOR_FIRST_ENTRY[i] + COLORS_PER_TEAM - 1);
    }
}

/*------------------------------------------------------------------*/
void
display_info (void)
{
  int x, y, w, h, epaisseur, temp_h, temp_w;

  if (!(CONFIG_INFO_BAR & 4))
    {
      temp_w = text_length (font, "00:00") + 2;
      temp_h = text_height (font) + 2;

      switch (CONFIG_INFO_BAR)
        {
        case 0:
        case 2:
          w = SCREEN_W;
          h = temp_h;
          if (h < 13)
            h = 13;
          epaisseur = temp_w;
          break;
        case 1:
        default:
          w = temp_w;
          if (w < 13)
            w = 13;
          h = SCREEN_H;
          epaisseur = temp_h;
          break;
        }

      if (!INFO_BAR)
        init_info_bar (w, h, epaisseur);

      draw_sprite (INFO_BAR, INFO_BAR_BACK, 0, 0);

      switch (CONFIG_INFO_BAR)
        {
        case 0:
          x = 0;
          y = PAGE_FLIP_H;
          hline (screen, x, y + h, x + w - 1, MENU_FG);
          display_horizontal_info (w, h, epaisseur);
          break;
        case 1:
          x = SCREEN_W - w;
          y = PAGE_FLIP_H;
          vline (screen, x - 1, y, y + h - 1, MENU_FG);
          display_vertical_info (w, h, epaisseur);
          break;
        case 2:
          x = 0;
          y = PAGE_FLIP_H + SCREEN_H - h;
          hline (screen, x, y - 1, x + w - 1, MENU_FG);
          display_horizontal_info (w, h, epaisseur);
          break;
        default:
          x = 0;
          y = PAGE_FLIP_H;
          vline (screen, x + w, y, y + h - 1, MENU_FG);
          display_vertical_info (w, h, epaisseur);
          break;
        }
      draw_sprite (screen, INFO_BAR, x, y);
    }
}

/*------------------------------------------------------------------*/
void
lw_info_get_room_for_viewport (int *x, int *y, int *w, int *h)
{
  int temp_h, temp_w;

  if (!(CONFIG_INFO_BAR & 4))
    {
      temp_w = text_length (font, "00:00") + 2;
      temp_h = text_height (font) + 2;

      if (temp_h < 13)
        {
          temp_h = 13;
        }

      if (temp_w < 13)
        {
          temp_w = 13;
        }

      switch (CONFIG_INFO_BAR)
        {
        case 0:
          *x = 0;
          *y = temp_h + 1;
          *w = SCREEN_W;
          *h = SCREEN_H - temp_h - 1;
          break;
        case 1:
          *x = 0;
          *y = 0;
          *w = SCREEN_W - temp_w - 1;
          *h = SCREEN_H;
          break;
        case 2:
          *x = 0;
          *y = 0;
          *w = SCREEN_W;
          *h = SCREEN_H - temp_h - 1;
          break;
        default:
          *x = temp_w + 1;
          *y = 0;
          *w = SCREEN_W - temp_w - 1;
          *h = SCREEN_H;
          break;
        }
    }
  else
    {
      *x = 0;
      *y = 0;
      *w = SCREEN_W;
      *h = SCREEN_H;
    }
}

/*------------------------------------------------------------------*/
void
check_info_state (void)
{
  static int changed;

  if (WATCHDOG_SCANCODE[KEY_F1])
    {
      if (CONFIG_INFO_BAR >= 4)
        CONFIG_INFO_BAR -= 4;
      else
        CONFIG_INFO_BAR += 4;
      free_info_bar ();
      changed = CONFIG_PAGE_FLIP ? 2 : 1;
      lw_viewport_register_change (changed);
    }
  if (WATCHDOG_SCANCODE[KEY_F2] && CONFIG_INFO_BAR < 4)
    {
      CONFIG_INFO_BAR++;
      if (CONFIG_INFO_BAR == 4)
        CONFIG_INFO_BAR = 0;
      if (CONFIG_INFO_BAR == 8)
        CONFIG_INFO_BAR = 4;
      free_info_bar ();
      changed = CONFIG_PAGE_FLIP ? 2 : 1;
      lw_viewport_register_change (changed);
    }

  if (changed > 0)
    {
      display_back_image ();
      changed--;
    }
}
