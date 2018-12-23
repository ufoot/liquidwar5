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
/* nom           : palette.c                                        */
/* contenu       : gestion de la palette de couleurs                */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "config.h"
#include "decal.h"
#include "disk.h"
#include "palette.h"
#include "network.h"
#include "mouse.h"
#include "texture.h"

/*==================================================================*/
/* constantes                                                       */
/*==================================================================*/

#define BG_TEXTURE_DEFAULT_COLOR 120
#define FG_TEXTURE_DEFAULT_COLOR 127

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int COLORS_PER_TEAM = 0;
PALETTE GLOBAL_PALETTE;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* mise en place de la palette, si necessaire                       */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static int
sqrt255 (int i)
{
  int result;

  result = fixsqrt (i * 256) / 256;
  if (result > 255)
    result = 255;

  return result;
}

/*------------------------------------------------------------------*/
static int
sqr255 (int i)
{
  int result;

  result = (i * i) / 256;
  if (result > 255)
    result = 255;

  return result;
}

/*------------------------------------------------------------------*/
static void
change_palette_brightness (PALETTE old_pal, PALETTE new_pal)
{
  int r, g, b;
  int i;
  int brightness;
  RGB *rgb;

  brightness = CONFIG_BRIGHTNESS - 8;
  if (brightness != 0)
    for (i = 0; i < 256; ++i)
      {
        rgb = old_pal + i;
        r = rgb->r * 4;
        g = rgb->g * 4;
        b = rgb->b * 4;

        if (brightness > 0)
          {
            r = (brightness * sqrt255 (r) + (8 - brightness) * r) / 8;
            g = (brightness * sqrt255 (g) + (8 - brightness) * g) / 8;
            b = (brightness * sqrt255 (b) + (8 - brightness) * b) / 8;
          }
        if (brightness < 0)
          {
            r = (-brightness * sqr255 (r) + (8 + brightness) * r) / 8;
            g = (-brightness * sqr255 (g) + (8 + brightness) * g) / 8;
            b = (-brightness * sqr255 (b) + (8 + brightness) * b) / 8;
          }

        rgb = new_pal + i;
        rgb->r = r / 4;
        rgb->g = g / 4;
        rgb->b = b / 4;
      }
  else
    for (i = 0; i < 256; ++i)
      new_pal[i] = old_pal[i];
}

/*------------------------------------------------------------------*/
void
my_set_palette (void)
{
  PALETTE old_pal, corrected_pal;
  int i, equal = 1;

  change_palette_brightness (GLOBAL_PALETTE, corrected_pal);

  get_palette (old_pal);
  for (i = 0; i < 256 && equal; ++i)
    {
      equal = equal && (corrected_pal[i].r == old_pal[i].r);
      equal = equal && (corrected_pal[i].g == old_pal[i].g);
      equal = equal && (corrected_pal[i].b == old_pal[i].b);
    }
  if (!equal)
    set_palette (corrected_pal);
}

/*------------------------------------------------------------------*/
static void
set_raw_texture_palette (void *ptr, int first)
{
  int i;
  char *data;

  data = ptr;
  data += 4 + LW_TEXTURE_SYSTEM_NAME_SIZE;
  for (i = first; i < first + 32; ++i)
    {
      GLOBAL_PALETTE[i].r = *(data++);
      GLOBAL_PALETTE[i].g = *(data++);
      GLOBAL_PALETTE[i].b = *(data++);
    }
}

/*------------------------------------------------------------------*/
static void
get_mono_texture_color (RGB * rgb, int num)
{
  int h1, s1, v1, r, g, b;
  float h2, s2, v2;
  float h[] = { 0, 20, 40, 70, 110, 150, 180, 210, 240, 270, 300, 330 };
  float s[] = { 1, 0.7, 0.4, 1, 0.5, 1, 0.5, 1, 0.5, 1 };
  float v[] = { 1, 1, 1, 0.8, 0.8, 0.6, 0.6, 0.3, 0.3, 0.1 };

  h1 = num / 10;
  s1 = num % 10;
  v1 = s1;

  if (h1 < 12)
    {
      h2 = h[h1];
      s2 = s[s1];
      v2 = v[v1];
    }
  else
    {
      h2 = 0;
      s2 = 0;
      v2 = 127 - num;
      v2 /= 7.;
    }

  hsv_to_rgb (h2, s2, v2, &r, &g, &b);

  rgb->r = r / 4;
  rgb->g = g / 4;
  rgb->b = b / 4;
  /* *rgb=CHOOSE_PALETTE [32+num]; */
}

/*------------------------------------------------------------------*/
static void
set_mono_texture_palette (int num, int first)
{
  get_mono_texture_color (GLOBAL_PALETTE + first, num);
}


/*------------------------------------------------------------------*/
void
set_bg_texture_palette (int num)
{
  if (num < CHOOSE_COLOR_NUMBER)
    {
      set_mono_texture_palette (num, BG_TEXTURE_FIRST_COLOR);
    }
  else
    {
      if (num >= CHOOSE_COLOR_NUMBER
          && num < CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER)
        {
          if (LOADED_TEXTURE)
            {
              set_raw_texture_palette (RAW_TEXTURE
                                       [num - CHOOSE_COLOR_NUMBER],
                                       BG_TEXTURE_FIRST_COLOR);
            }
          else
            {
              set_mono_texture_palette (BG_TEXTURE_DEFAULT_COLOR,
                                        BG_TEXTURE_FIRST_COLOR);
            }
        }

      if (num >= CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER
          && num <
          CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER + RAW_MAPTEX_NUMBER)
        {
          if (LOADED_MAPTEX)
            {
              set_raw_texture_palette (RAW_MAPTEX
                                       [num -
                                        (CHOOSE_COLOR_NUMBER +
                                         RAW_TEXTURE_NUMBER)],
                                       BG_TEXTURE_FIRST_COLOR);
            }
          else
            {
              set_mono_texture_palette (BG_TEXTURE_DEFAULT_COLOR,
                                        BG_TEXTURE_FIRST_COLOR);
            }
        }
    }
}

/*------------------------------------------------------------------*/
void
set_fg_texture_palette (int num)
{
  if (num < CHOOSE_COLOR_NUMBER)
    {
      set_mono_texture_palette (num, FG_TEXTURE_FIRST_COLOR);
    }
  else
    {
      if (num >= CHOOSE_COLOR_NUMBER
          && num < CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER)
        {
          if (LOADED_TEXTURE)
            {
              set_raw_texture_palette (RAW_TEXTURE
                                       [num - CHOOSE_COLOR_NUMBER],
                                       FG_TEXTURE_FIRST_COLOR);
            }
          else
            {
              set_mono_texture_palette (FG_TEXTURE_DEFAULT_COLOR,
                                        FG_TEXTURE_FIRST_COLOR);
            }
        }

      if (num >= CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER
          && num <
          CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER + RAW_MAPTEX_NUMBER)
        {
          if (LOADED_MAPTEX)
            {
              set_raw_texture_palette (RAW_MAPTEX
                                       [num -
                                        (CHOOSE_COLOR_NUMBER +
                                         RAW_TEXTURE_NUMBER)],
                                       FG_TEXTURE_FIRST_COLOR);
            }
          else
            {
              set_mono_texture_palette (FG_TEXTURE_DEFAULT_COLOR,
                                        FG_TEXTURE_FIRST_COLOR);
            }
        }
    }
}

/*------------------------------------------------------------------*/
void
set_palette_for_choose_color (void)
{
  int i;

  for (i = 0; i < 128; ++i)
    get_mono_texture_color (GLOBAL_PALETTE + 128 + i, i);
}

/*------------------------------------------------------------------*/
static void
set_team_color (int teinte, int first, int num)
{
  int i, col, col_r, col_g, col_b, coeff, tot, last;

  last = first + num - 1;

  get_mono_texture_color (GLOBAL_PALETTE + last, teinte * 10);

  col_r = GLOBAL_PALETTE[last].r;
  col_g = GLOBAL_PALETTE[last].g;
  col_b = GLOBAL_PALETTE[last].b;
  tot = 8 * num - 7;
  for (i = 0; i < num - 1; ++i)
    {
      coeff = num + 7 * i;
      col = (col_r * coeff) / tot;
      GLOBAL_PALETTE[first + i].r = col;
      col = (col_g * coeff) / tot;
      GLOBAL_PALETTE[first + i].g = col;
      col = (col_b * coeff) / tot;
      GLOBAL_PALETTE[first + i].b = col;
    }
}

/*------------------------------------------------------------------*/
void
set_playing_teams_palette (void)
{
  int i, j, n, color, active;

  COLORS_PER_TEAM = 128 / PLAYING_TEAMS;
  for (i = 0, n = 0; i < NB_TEAMS; ++i)
    {
      color = 0;
      active = 0;
      if (LW_NETWORK_ON)
        {
          for (j = 0; j < NB_TEAMS; ++j)
            {
              if (LW_NETWORK_INFO[j].active &&
                  LW_NETWORK_INFO[j].server_id == i)
                {
                  active = 1;
                  color = LW_NETWORK_INFO[j].color;
                }
            }
        }
      else
        {
          if (CONFIG_CONTROL_TYPE[i] != CONFIG_CONTROL_TYPE_OFF)
            {
              active = 1;
              color = CONFIG_TEAM_COLOR[i];
            }
        }

      if (active)
        {
          COLOR_FIRST_ENTRY[n] = 128 + n * COLORS_PER_TEAM;
          set_team_color (color, COLOR_FIRST_ENTRY[n], COLORS_PER_TEAM);
          n++;
        }
    }
}

/*------------------------------------------------------------------*/
void
my_fade_out (void)
{
  fade_out (48);
}

/*------------------------------------------------------------------*/
void
my_fade_in (void)
{
  PALETTE pal;

  change_palette_brightness (GLOBAL_PALETTE, pal);
  fade_in (pal, 48);

  lw_mouse_update_show_hide ();
}
