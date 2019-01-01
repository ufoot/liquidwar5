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
/* nom           : distor.c                                         */
/* contenu       : affichage avec des effets de vague               */
/* date de modif : 31 Juillet 99                                    */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "alleg2.h"
#include "area.h"
#include "config.h"
#include "distor.h"
#include "gfxmode.h"
#include "glouglou.h"
#include "profile.h"
#include "startup.h"
#include "time.h"
#include "viewport.h"
#include "log.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define PI_1024 3217

#define DISTORSION_PRECISION 4096
#define WAVE_SIZE_SCALE 4
#define DISTORSION_MAX 10
#define WAVE_MIN_LENGTH 64

static int DISTORSION_W;
static int DISTORSION_H;

static ALLEGRO_BITMAP *DISTORSION_TARGET = NULL;

static int WAVE_SHAPE_WX[MAX_W_DISPLAY];
static int WAVE_SHAPE_WY[DISTORSION_MAX + 2][MAX_W_DISPLAY];

static int *WAVE_SHAPE_X_CORRES[MAX_W_DISPLAY];

static int WAVE_SHAPE_HY[MAX_H_DISPLAY];
static int WAVE_SHAPE_HX[DISTORSION_MAX + 2][MAX_H_DISPLAY];

static int *WAVE_SHAPE_Y_CORRES[MAX_H_DISPLAY];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
init_distorsion_target (void)
{
  if (DISTORSION_TARGET == NULL)
    {
      DISTORSION_TARGET = my_create_bitmap (NEXT_SCREEN->w, NEXT_SCREEN->h);
    }
  else
    {
      if (DISTORSION_TARGET->w != NEXT_SCREEN->w ||
          DISTORSION_TARGET->h != NEXT_SCREEN->h)
        {
          destroy_bitmap (DISTORSION_TARGET);
          DISTORSION_TARGET = my_create_bitmap (NEXT_SCREEN->w,
                                                NEXT_SCREEN->h);
        }
    }
}

/*------------------------------------------------------------------*/
void
init_distorsion_displayer (void)
{
  int x, y, w, h;

  init_distorsion_target ();

  w = DISTORSION_TARGET->w;
  h = DISTORSION_TARGET->h;

  DISTORSION_W = (CONFIG_WAVE_AMPLI[0] + CONFIG_WAVE_AMPLI[3])
    / WAVE_SIZE_SCALE + 1;
  DISTORSION_H = (CONFIG_WAVE_AMPLI[1] + CONFIG_WAVE_AMPLI[2])
    / WAVE_SIZE_SCALE + 1;

  for (x = 0; x < DISTORSION_W + 1; ++x)
    WAVE_SHAPE_X_CORRES[x] = WAVE_SHAPE_HX[x];

  for (x = DISTORSION_W; x < w - DISTORSION_W - 2; ++x)
    WAVE_SHAPE_X_CORRES[x] = WAVE_SHAPE_HX[DISTORSION_W + 1];

  for (x = w - DISTORSION_W - 2; x < w - 1; ++x)
    WAVE_SHAPE_X_CORRES[x] = WAVE_SHAPE_HX[w - x - 2];

  WAVE_SHAPE_X_CORRES[w - 1] = WAVE_SHAPE_HX[0];

  for (y = 0; y < DISTORSION_H + 1; ++y)
    WAVE_SHAPE_Y_CORRES[y] = WAVE_SHAPE_WY[y];

  for (y = DISTORSION_H; y < h - DISTORSION_H - 2; ++y)
    WAVE_SHAPE_Y_CORRES[y] = WAVE_SHAPE_WY[DISTORSION_H + 1];

  for (y = h - DISTORSION_H - 2; y < h - 1; ++y)
    WAVE_SHAPE_Y_CORRES[y] = WAVE_SHAPE_WY[h - y - 2];

  WAVE_SHAPE_Y_CORRES[h - 1] = WAVE_SHAPE_WY[0];
}


/*------------------------------------------------------------------*/
static void
create_wave_line (int *buffer,
                  int length, int number, int ampli, int speed, int sens)
{
  int i, period, period2, cycle_tmp;
  int ampli_cst, ampli_cos, freq_coef, cycle_coef;
  int f0, fp0x0, alphax03, betax02;
  int temp;
  int first_entry, last_entry, real_length;
  int *buffer2;

  length--;
  do
    {
      period = length / (number + 1);
      number--;
    }
  while (number >= 0 && period < WAVE_MIN_LENGTH);

  period2 = period / 2;
  if (period2 <= 0)
    {
      period2 = 1;
    }
  period = period2 * 2;
  real_length = period * (number + 1);
  first_entry = (length - real_length) / 2;
  last_entry = first_entry + real_length;
  buffer2 = buffer + first_entry;

  freq_coef = 16777216 / period;
  ampli_cst = (ampli * DISTORSION_PRECISION) / period2;
  ampli_cos = (ampli_cst * PI_1024) / 1024;
  cycle_tmp = 65536;
  while (speed > 0)
    {
      cycle_tmp /= 2;
      speed--;
    }
  cycle_tmp = fixsqrt (fixsqrt (cycle_tmp)) / 2;
  cycle_tmp /= SCREEN_W;
  cycle_tmp *= period;
  if (cycle_tmp <= 0)
    {
      cycle_tmp = 1;
    }
  cycle_coef = sens * 256 * (itofix (GLOBAL_TICKER % cycle_tmp) / cycle_tmp);

  f0 = fixsin (cycle_coef + freq_coef * period2);
  fp0x0 = (PI_1024 * fixcos (cycle_coef + freq_coef * period2)) / 1024;

  alphax03 = (ampli_cst / 256) * ((fp0x0 - 2 * f0) / 256);
  betax02 = (ampli_cst / 256) * ((-fp0x0 + 3 * f0) / 256);

  for (i = 0; i < period2; ++i)
    {
      temp = (3 * alphax03 * i) / period2 + 2 * betax02;
      temp = (temp * i) / period2;
      buffer2[i] = temp / WAVE_SIZE_SCALE;
    }
  for (i = period2; i < real_length - period2; ++i)
    {
      temp =
        ((ampli_cos) / 256) * (fixcos (cycle_coef + freq_coef * i) / 256);
      buffer2[i] = temp / WAVE_SIZE_SCALE;
    }

  alphax03 = (ampli_cst / 256) * ((-fp0x0 - 2 * f0) / 256);
  betax02 = (ampli_cst / 256) * ((fp0x0 + 3 * f0) / 256);

  for (i = 0; i < period2; ++i)
    {
      temp = (3 * alphax03 * i) / period2 + 2 * betax02;
      temp = (-temp * i) / period2;
      buffer2[real_length - 1 - i] = temp / WAVE_SIZE_SCALE;
    }

  for (i = 0; i < first_entry; ++i)
    buffer[i] = 0;
  for (i = last_entry; i < length; ++i)
    buffer[i] = 0;
  buffer[length] = 0;
}

/*------------------------------------------------------------------*/
void
prepare_wave_shapes (void)
{
  int w, h, x, y, i;

  w = DISTORSION_TARGET->w;
  h = DISTORSION_TARGET->h;

  create_wave_line (WAVE_SHAPE_WX, w,
                    CONFIG_WAVE_NUMBER[0],
                    CONFIG_WAVE_AMPLI[0] * CURRENT_AREA_W,
                    CONFIG_WAVE_SPEED[0], +1);
  create_wave_line (WAVE_SHAPE_HY, h,
                    CONFIG_WAVE_NUMBER[1],
                    CONFIG_WAVE_AMPLI[1] * CURRENT_AREA_H,
                    CONFIG_WAVE_SPEED[1], +1);

  create_wave_line (WAVE_SHAPE_WY[DISTORSION_H + 1], w,
                    CONFIG_WAVE_NUMBER[2],
                    CONFIG_WAVE_AMPLI[2] * CURRENT_AREA_H,
                    CONFIG_WAVE_SPEED[2], +1);
  create_wave_line (WAVE_SHAPE_HX[DISTORSION_W + 1], h,
                    CONFIG_WAVE_NUMBER[3],
                    CONFIG_WAVE_AMPLI[3] * CURRENT_AREA_W,
                    CONFIG_WAVE_SPEED[3], +1);

  for (x = 0; x < w; ++x)
    {
      WAVE_SHAPE_WX[x] += CURRENT_AREA_W * DISTORSION_PRECISION;
      WAVE_SHAPE_WY[0][x] = 0;
      WAVE_SHAPE_WY[1][x] = WAVE_SHAPE_WY[DISTORSION_H + 1][x] / DISTORSION_H;
      for (i = 1; i < DISTORSION_H; ++i)
        WAVE_SHAPE_WY[i + 1][x] = WAVE_SHAPE_WY[i][x] + WAVE_SHAPE_WY[1][x];
    }

  for (y = 0; y < h; ++y)
    {
      WAVE_SHAPE_HY[y] += CURRENT_AREA_H * DISTORSION_PRECISION;
      WAVE_SHAPE_HX[0][y] = 0;
      WAVE_SHAPE_HX[1][y] = WAVE_SHAPE_HX[DISTORSION_W + 1][y] / DISTORSION_W;
      for (i = 1; i < DISTORSION_W; ++i)
        WAVE_SHAPE_HX[i + 1][y] = WAVE_SHAPE_HX[i][y] + WAVE_SHAPE_HX[1][y];
    }
}

/*------------------------------------------------------------------*/
void
disp_distorted_area (void)
{
  char *src;
  int x, y, w, h, lim_w, lim_h, init_w, init_h;
  int *y_corres;
  int fp_x, fp_y, fp_y0, ip_y0;
  int fp_x0[MAX_W_DISPLAY];
  int reste_x;
#ifdef ASM
  int ip_x, ip_y;
  int temp = 0;
  int bmp_color_depth;
  int bmp_linear;
  int bmp_memory;

  bmp_color_depth = bitmap_color_depth (DISTORSION_TARGET);
  bmp_linear = is_linear_bitmap (DISTORSION_TARGET);
  bmp_memory = is_memory_bitmap (DISTORSION_TARGET);
#endif

  w = DISTORSION_TARGET->w;
  h = DISTORSION_TARGET->h;
  lim_w = DISTORSION_PRECISION * w;
  lim_h = DISTORSION_PRECISION * h;
  init_w = (CURRENT_AREA_W * DISTORSION_PRECISION) / 2;
  init_h = (CURRENT_AREA_H * DISTORSION_PRECISION) / 2;

  prepare_wave_shapes ();
  for (x = 0; x < w; ++x)
    fp_x0[x] = 0;

  fp_y0 = init_h;
  ip_y0 = 0;
  for (y = 0; y < h; ++y)
    {
      fp_y = fp_y0;
      fp_x = init_w;
      src = (char *) CURRENT_AREA_DISP->dat + ip_y0 * CURRENT_AREA_W;
      y_corres = WAVE_SHAPE_Y_CORRES[y];

#ifdef ASM
      ip_x = 0;
      ip_y = ip_y0;
      /*
       * draw_distor_line works on memory 8-bit bitmaps only
       */
      if (STARTUP_ASM && bmp_memory && bmp_linear && bmp_color_depth == 8)
        {
          draw_distor_line (DISTORSION_TARGET,
                            CURRENT_AREA_W,
                            w,
                            y,
                            fp_x,
                            ip_x,
                            fp_y,
                            ip_y,
                            lim_w,
                            lim_h,
                            fp_x0,
                            y_corres,
                            WAVE_SHAPE_WX,
                            WAVE_SHAPE_X_CORRES,
                            src, temp, temp, temp, temp, temp);
        }
      else
#endif
        for (x = 0; x < w; ++x)
          {
            putpixel (DISTORSION_TARGET, x, y, *src);

            reste_x = fp_x0[x] += WAVE_SHAPE_X_CORRES[x][y];
            fp_x += WAVE_SHAPE_WX[x];
            while (reste_x < -fp_x)
              {
                fp_x += lim_w;
                src--;
              }
            while (reste_x + fp_x >= lim_w)
              {
                fp_x -= lim_w;
                src++;
              }

            fp_y += y_corres[x];
            while (fp_y < 0)
              {
                fp_y += lim_h;
                src -= CURRENT_AREA_W;
              }
            while (fp_y >= lim_h)
              {
                fp_y -= lim_h;
                src += CURRENT_AREA_W;
              }
          }

      fp_y0 += WAVE_SHAPE_HY[y];
      while (fp_y0 >= lim_h)
        {
          fp_y0 -= lim_h;
          ip_y0++;
        }
    }

  blit (DISTORSION_TARGET, NEXT_SCREEN, 0, 0, 0, 0,
        DISTORSION_TARGET->w, DISTORSION_TARGET->h);
}
