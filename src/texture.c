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
/* nom           : texture.c                                        */
/* contenu       : lecture des textures depuis la memoire           */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "base.h"
#include "alleg2.h"
#include "bigdata.h"
#include "texture.h"
#include "disk.h"
#include "palette.h"
#include "startup.h"
#include "log.h"
#include "serial.h"
#include "macro.h"
#include "path.h"
#include "serial.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_TEXTURE_RANDOM_MAX_RETRIES 10000

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* gestion des bitmaps des textures                                 */
/*------------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
static int
find_color (PALETTE pal, RGB rgb)
{
  int i, found = -1;

  for (i = 0; i < 256 && found < 0; ++i)
    if (pal[i].r == rgb.r && pal[i].g == rgb.g && pal[i].b == rgb.b)
      found = i;

  return found;
}

/*-----------------------------------------------------------------*/
static int
exist_color (PALETTE pal, RGB rgb)
{
  return (find_color (pal, rgb) >= 0);
}

/*-----------------------------------------------------------------*/
static int
recalculate_number_of_colors (int max_number, ALLEGRO_BITMAP * bmp,
                              PALETTE pal)
{
  int i, x, y, n = 1;
  RGB color;
  PALETTE pal2;

  color = pal[getpixel (bmp, 0, 0)];

  for (i = 0; i < 256; ++i)
    pal2[i] = color;

  for (y = 0; y < bmp->h && n < max_number; ++y)
    for (x = 0; x < bmp->w && n < max_number; ++x)
      {
        color = pal[getpixel (bmp, x, y)];
        if (!exist_color (pal2, color))
          pal2[n++] = color;
      }
  return n;
}

/*-----------------------------------------------------------------*/
static void
create_new_palette (PALETTE dst,
                    PALETTE src,
                    ALLEGRO_BITMAP * bmp, int first_color,
                    int number_of_colors)
{
  int i, x, y, index;
  int nb_retries = 0;
  RGB color;

  x = random () % bmp->w;
  y = random () % bmp->h;
  color = src[getpixel (bmp, x, y)];

  for (i = 0; i < 256; ++i)
    dst[i] = color;

  for (i = 1; i < number_of_colors;)
    {
      x = random () % bmp->w;
      y = random () % bmp->h;
      index = getpixel (bmp, x, y);
      color = src[index];
      if ((!exist_color (dst, color)) ||
          (nb_retries > LW_TEXTURE_RANDOM_MAX_RETRIES))
        {
          dst[first_color + (i++)] = color;
          nb_retries = 0;
        }
      else
        {
          nb_retries++;
        }
    }
}

/*-----------------------------------------------------------------*/
static void
correct_palette (PALETTE pal, int first_color, int number_of_colors)
{
  int i;

  for (i = 0; i < first_color; ++i)
    {
      pal[i].r = 0;
      pal[i].g = 0;
      pal[i].b = 0;
    }
  for (i = first_color + number_of_colors; i < 256; ++i)
    {
      pal[i].r = 63;
      pal[i].g = 63;
      pal[i].b = 63;
    }
}

/*-----------------------------------------------------------------*/
static void
create_converted_bitmap (ALLEGRO_BITMAP * bmp,
                         PALETTE dst,
                         PALETTE src, int first_color, int number_of_colors)
{
  char corres[256];
  int i, x, y, index;

  for (i = 0; i < 256; ++i)
    corres[i] = bestfit_color (dst, src[i].r, src[i].g, src[i].b);
  for (y = 0; y < bmp->h; ++y)
    for (x = 0; x < bmp->w; ++x)
      {
        index = corres[getpixel (bmp, x, y)];
        index = (index < first_color ||
                 index >= first_color + number_of_colors) ?
          first_color : index;
        putpixel (bmp, x, y, index);
      }
}

/*------------------------------------------------------------------*/
static void
red8col (ALLEGRO_BITMAP * bmp, PALETTE pal, int first_color,
         int number_of_colors)
{
  PALETTE pal2;
  int i;

  for (i = 0; i < 256; ++i)
    pal2[i] = pal[i];

  number_of_colors = recalculate_number_of_colors
    (number_of_colors, bmp, pal);
  create_new_palette (pal, pal2, bmp, first_color, number_of_colors);
  create_converted_bitmap (bmp, pal, pal2, first_color, number_of_colors);
  correct_palette (pal, first_color, number_of_colors);
}

/*------------------------------------------------------------------*/
static void
texture_8to5 (ALLEGRO_BITMAP * bmp, PALETTE pal, void *result,
              int first_color, int number_of_colors, char *filename)
{
  char *buffer;
  int pos = 0, pos8 = 0, x, y, i;
  char octet[5], toadd;
  int coul;
  char system_name_buffer[LW_TEXTURE_SYSTEM_NAME_SIZE + 1];

  lw_serial_set_texture_header (result, (short) bmp->w, (short) bmp->h);

  buffer = ((char *) result) + 2 * sizeof (short);

  /*
   * We store the system name
   */
  memset (system_name_buffer, 0, sizeof (system_name_buffer));
  LW_MACRO_STRCPY (system_name_buffer, lw_path_get_system_name (filename));
  memcpy (buffer, system_name_buffer, LW_TEXTURE_SYSTEM_NAME_SIZE);
  buffer += LW_TEXTURE_SYSTEM_NAME_SIZE;

  for (i = 0; i < number_of_colors; ++i)
    {
      buffer[pos++] = pal[first_color + i].r;
      buffer[pos++] = pal[first_color + i].g;
      buffer[pos++] = pal[first_color + i].b;
    }

  for (i = 0; i < 5; ++i)
    octet[i] = 0;

  for (y = 0; y < bmp->h; ++y)
    for (x = 0; x < bmp->w; ++x)
      {
        coul = getpixel (bmp, x, y) - first_color;
        toadd = 1 << pos8;
        octet[0] |= (coul & 1) ? toadd : 0;
        octet[1] |= (coul & 2) ? toadd : 0;
        octet[2] |= (coul & 4) ? toadd : 0;
        octet[3] |= (coul & 8) ? toadd : 0;
        octet[4] |= (coul & 16) ? toadd : 0;

        if (pos8 == 7 || (y == bmp->h - 1 && x == bmp->w - 1))
          {
            for (i = 0; i < 5; ++i)
              {
                buffer[pos++] = octet[i];
                octet[i] = 0;
              }
            pos8 = 0;
          }
        else
          pos8++;
      }
}

/*------------------------------------------------------------------*/
void *
lw_texture_archive_raw (const char *filename)
{
  int i, w, h, size = 0;
  ALLEGRO_BITMAP *bmp;
  PALETTE pal;
  char *result = NULL, *temp = NULL;
  char *f = (char *) filename;

  bmp = load_bitmap (filename, pal);
  if (bmp)
    {
      w = bmp->w;
      h = bmp->h;
      if (w > 0 && h > 0)
        {
          temp = malloc (size =
                         2 * sizeof (short)
                         + LW_TEXTURE_SYSTEM_NAME_SIZE
                         + 3 * TEXTURE_COLOR_NUMBER + ((w * h + 7) / 8) * 5);
          if (temp)
            {
              red8col (bmp, pal, 0, TEXTURE_COLOR_NUMBER);
              texture_8to5 (bmp, pal, temp, 0, TEXTURE_COLOR_NUMBER, f);
            }
        }
      destroy_bitmap (bmp);
    }
  if (temp)
    {
      result = malloc (size);
      if (result)
        {
          for (i = 0; i < size; ++i)
            {
              result[i] = temp[i];
            }
        }
      free (temp);
    }

  return result;
}

/*------------------------------------------------------------------*/
static ALLEGRO_BITMAP *
create_raw_texture (void *ptr, int first)
{
  int x, y, pos8 = 0, color;
  char totest, *data;
  ALLEGRO_BITMAP *result;
  short w, h;

  data = ptr;
  lw_serial_get_texture_header (data, &w, &h);
  data += 2 * sizeof (short) + LW_TEXTURE_SYSTEM_NAME_SIZE + 3 * 32;

  result = my_create_bitmap (w, h);
  if (result)
    for (y = 0; y < h; ++y)
      for (x = 0; x < w; ++x)
        {
          totest = 1 << pos8;
          color = first + ((data[0] & totest) ? 1 : 0)
            + ((data[1] & totest) ? 2 : 0)
            + ((data[2] & totest) ? 4 : 0)
            + ((data[3] & totest) ? 8 : 0) + ((data[4] & totest) ? 16 : 0);
          putpixel (result, x, y, color);
          if (pos8 == 7)
            {
              data += 5;
              pos8 = 0;
            }
          else
            pos8++;
        }
  return result;
}

/*------------------------------------------------------------------*/
static void *
get_raw_texture (int num)
{
  void *texture = NULL;

  if (num > RAW_TEXTURE_NUMBER + RAW_MAPTEX_NUMBER + CHOOSE_COLOR_NUMBER - 1)
    num = RAW_TEXTURE_NUMBER + RAW_MAPTEX_NUMBER + CHOOSE_COLOR_NUMBER - 1;

  if (num >= CHOOSE_COLOR_NUMBER
      && num < CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER && LOADED_TEXTURE)
    {
      texture = RAW_TEXTURE[num - CHOOSE_COLOR_NUMBER];
    }

  if (num >= CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER
      && num < CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER + RAW_MAPTEX_NUMBER
      && LOADED_MAPTEX)
    {
      texture = RAW_MAPTEX[num - (CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER)];
    }

  return texture;
}

/*------------------------------------------------------------------*/
static ALLEGRO_BITMAP *
create_mono_texture (int first)
{
  ALLEGRO_BITMAP *result;

  result = my_create_bitmap (1, 1);
  putpixel (result, 0, 0, first);
  return result;
}

/*------------------------------------------------------------------*/
static ALLEGRO_BITMAP *
create_texture (int num, int first)
{
  ALLEGRO_BITMAP *result;
  void *texture;

  texture = get_raw_texture (num);

  if (texture)
    {
      result = create_raw_texture (texture, first);
    }
  else
    {
      result = create_mono_texture (first);
    }

  return result;
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
lw_texture_create_bg (int num)
{
  return create_texture (num, BG_TEXTURE_FIRST_COLOR);
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
lw_texture_create_fg (int num)
{
  return create_texture (num, FG_TEXTURE_FIRST_COLOR);
}

/*------------------------------------------------------------------*/
char *
lw_texture_get_system_name (int num)
{
  static char result[LW_MAP_SYSTEM_NAME_SIZE + 1];
  void *raw_texture;
  char *char_ptr;

  raw_texture = get_raw_texture (num);

  LW_MACRO_MEMSET0 (result);
  if (raw_texture)
    {
      char_ptr = (char *) raw_texture;
      memcpy (result, char_ptr + 4, LW_TEXTURE_SYSTEM_NAME_SIZE);
    }
  else
    {
      LW_MACRO_SPRINTF1 (result, "Color %d", num);
    }
  result[sizeof (result) - 1] = '\0';

  return result;
}
