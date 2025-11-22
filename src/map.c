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
/* nom           : map.c                                            */
/* contenu       : decryptage des tableaux en memoire               */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <string.h>

#include "alleg2.h"
#include "bigdata.h"
#include "disk.h"
#include "map.h"
#include "texture.h"
#include "network.h"
#include "log.h"
#include "macro.h"
#include "serial.h"
#include "path.h"
#include "random.h"

/*==================================================================*/
/* constantes                                                       */
/*==================================================================*/

#define LIGHT_OR_DARK_TRESHOLD 315
// Color constants converted to ALLEGRO_COLOR - these are now created inline where needed
#define CONSIDERED_AS_DARK_RGB 0x000000   /* Black RGB */
#define CONSIDERED_AS_LIGHT_RGB 0xFFFFFF  /* White RGB */
#define PLAYABLE_AREA_RGB 0x010101        /* Very dark gray RGB for playable */
#define MINI_SIDE_SIZE 4
#define MINI_PLAYABLE_AREA 1024

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* Helper function to compare ALLEGRO_COLOR values */
static int
colors_equal(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2)
{
  return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a);
}

/*------------------------------------------------------------------*/
static int
calc_zoom_factor (int w, int h, int min_w, int min_h)
{
  int factor_w, factor_h, factor;

  if (w <= 0 || h <= 0)
    {
      log_println_str ("Error: map size is null or negative!");
      w = h = 1;
    }

  factor_w = ((min_w - 1) / w) + 1;
  factor_h = ((min_h - 1) / h) + 1;

  factor = (factor_w > factor_h) ? factor_w : factor_h;

  return factor;
}

/*------------------------------------------------------------------*/
static void
sort_light_and_dark (ALLEGRO_BITMAP * bmp)
{
  int x, y;
  ALLEGRO_COLOR pixel_color;
  float r, g, b, a;
  int brightness;
  ALLEGRO_COLOR light_color, dark_color;

  /* Create actual RGB colors from constants */
  light_color = al_map_rgb((CONSIDERED_AS_LIGHT_RGB >> 16) & 0xFF,
                          (CONSIDERED_AS_LIGHT_RGB >> 8) & 0xFF,
                          CONSIDERED_AS_LIGHT_RGB & 0xFF);
  dark_color = al_map_rgb((CONSIDERED_AS_DARK_RGB >> 16) & 0xFF,
                         (CONSIDERED_AS_DARK_RGB >> 8) & 0xFF,
                         CONSIDERED_AS_DARK_RGB & 0xFF);

  /* Set target bitmap once for efficiency */
  al_set_target_bitmap (bmp);

  for (y = 0; y < al_get_bitmap_height (bmp); ++y)
    for (x = 0; x < al_get_bitmap_width (bmp); ++x)
      {
        pixel_color = al_get_pixel (bmp, x, y);
        al_unmap_rgba_f (pixel_color, &r, &g, &b, &a);

        /* Apply weighted brightness formula with 0-255 scaling */
        brightness = (int)(6 * (r * 255) + 3 * (g * 255) + (b * 255));

        if (brightness > LIGHT_OR_DARK_TRESHOLD)
          al_put_pixel (x, y, light_color);
        else
          al_put_pixel (x, y, dark_color);
      }
}

/*------------------------------------------------------------------*/
static ALLEGRO_BITMAP *
extract_significant_part (ALLEGRO_BITMAP * src)
{
  int min_x = al_get_bitmap_width (src);
  int min_y = al_get_bitmap_height (src);
  int max_x = -1;
  int max_y = -1;
  int dst_x, dst_y, dst_w, dst_h;
  int x, y;
  ALLEGRO_BITMAP *result;

  for (y = 0; y < al_get_bitmap_height (src); ++y)
    for (x = 0; x < al_get_bitmap_width (src); ++x)
      if (is_color_dark(getpixel (src, x, y)))
        {
          if (min_x > x)
            min_x = x;
          if (min_y > y)
            min_y = y;
          if (max_x < x)
            max_x = x;
          if (max_y < y)
            max_y = y;
        }
  dst_x = min_x;
  dst_y = min_y;
  dst_w = max_x - min_x + 1;
  dst_h = max_y - min_y + 1;

  if (dst_w < MINI_SIDE_SIZE || dst_h < MINI_SIDE_SIZE)
    result = NULL;
  else
    {
      result = my_create_memory_bitmap (dst_w, dst_h);
      if (result)
        blit (src, result, dst_x, dst_y, 0, 0, dst_w, dst_h);
    }

  return result;
}

/*-----------------------------------------------------------------*/
static int
spread_color_down (ALLEGRO_BITMAP * bmp, ALLEGRO_COLOR color1, ALLEGRO_COLOR color2)
{
  int x, y, x1, y1, x2, y2, found = 0;

  al_set_target_bitmap (bmp);
  for (y = 0; y < al_get_bitmap_height (bmp); ++y)
    for (x = 0; x < al_get_bitmap_width (bmp); ++x)
      if (colors_equal(getpixel (bmp, x, y), color2))
        {
          x1 = x - 1;
          if (x1 < 0)
            x1 = 0;
          x2 = x + 1;
          if (x2 > al_get_bitmap_width (bmp) - 1)
            x2 = al_get_bitmap_width (bmp) - 1;
          y1 = y - 1;
          if (y1 < 0)
            y1 = 0;
          y2 = y + 1;
          if (y2 > al_get_bitmap_height (bmp) - 1)
            y2 = al_get_bitmap_height (bmp) - 1;

          if (colors_equal(getpixel (bmp, x2, y), color1))
            {
              putpixel_fast (x2, y, color2);
              found++;
            }
          if (colors_equal(getpixel (bmp, x2, y2), color1))
            {
              putpixel_fast (x2, y2, color2);
              found++;
            }
          if (colors_equal(getpixel (bmp, x, y2), color1))
            {
              putpixel_fast (x, y2, color2);
              found++;
            }
          if (colors_equal(getpixel (bmp, x1, y2), color1))
            {
              putpixel_fast (x1, y2, color2);
              found++;
            }
        }

  return found;
}

/*-----------------------------------------------------------------*/
static int
spread_color_up (ALLEGRO_BITMAP * bmp, ALLEGRO_COLOR color1, ALLEGRO_COLOR color2)
{
  int x, y, x1, y1, x2, y2, found = 0;

  al_set_target_bitmap (bmp);
  for (y = al_get_bitmap_height (bmp) - 1; y >= 0; --y)
    for (x = al_get_bitmap_width (bmp) - 1; x >= 0; --x)
      if (colors_equal(getpixel (bmp, x, y), color2))
        {
          x1 = x - 1;
          if (x1 < 0)
            x1 = 0;
          x2 = x + 1;
          if (x2 > al_get_bitmap_width (bmp) - 1)
            x2 = al_get_bitmap_width (bmp) - 1;
          y1 = y - 1;
          if (y1 < 0)
            y1 = 0;
          y2 = y + 1;
          if (y2 > al_get_bitmap_height (bmp) - 1)
            y2 = al_get_bitmap_height (bmp) - 1;

          if (colors_equal(getpixel (bmp, x1, y), color1))
            {
              putpixel_fast (x1, y, color2);
              found++;
            }
          if (colors_equal(getpixel (bmp, x1, y1), color1))
            {
              putpixel_fast (x1, y1, color2);
              found++;
            }
          if (colors_equal(getpixel (bmp, x, y1), color1))
            {
              putpixel_fast (x, y1, color2);
              found++;
            }
          if (colors_equal(getpixel (bmp, x2, y1), color1))
            {
              putpixel_fast (x2, y1, color2);
              found++;
            }
        }

  return found;
}


/*-----------------------------------------------------------------*/
static int
check_if_playable (ALLEGRO_BITMAP * bmp)
{
  int x = 0, y = 0, x0, y0;
  int unplayable = 0;
  int playable_place = 0;
  ALLEGRO_COLOR light_color, dark_color, playable_color;

  /* Create ALLEGRO_COLOR from RGB constants */
  light_color = al_map_rgb((CONSIDERED_AS_LIGHT_RGB >> 16) & 0xFF,
                          (CONSIDERED_AS_LIGHT_RGB >> 8) & 0xFF,
                          CONSIDERED_AS_LIGHT_RGB & 0xFF);
  dark_color = al_map_rgb((CONSIDERED_AS_DARK_RGB >> 16) & 0xFF,
                         (CONSIDERED_AS_DARK_RGB >> 8) & 0xFF,
                         CONSIDERED_AS_DARK_RGB & 0xFF);
  playable_color = al_map_rgb((PLAYABLE_AREA_RGB >> 16) & 0xFF,
                             (PLAYABLE_AREA_RGB >> 8) & 0xFF,
                             PLAYABLE_AREA_RGB & 0xFF);

  /*
   * Now we draw the "outside" of the map. This is usefull
   * to avoid bugs (core dumps) if the map is not correctly drawn.
   */
  hline (bmp, 0, 0, al_get_bitmap_width (bmp) - 1, dark_color);
  hline (bmp, 0, al_get_bitmap_height (bmp) - 1, al_get_bitmap_width (bmp) - 1, dark_color);
  vline (bmp, 0, 0, al_get_bitmap_height (bmp) - 1, dark_color);
  vline (bmp, al_get_bitmap_width (bmp) - 1, 0, al_get_bitmap_height (bmp) - 1, dark_color);

  x0 = y0 = -1;
  for (y = 0; y < al_get_bitmap_height (bmp) && y0 < 0; ++y)
    for (x = 0; x < al_get_bitmap_width (bmp) && x0 < 0; ++x)
      if (is_color_light(getpixel (bmp, x, y)))
        {
          x0 = x;
          y0 = y;
        }

  if (x0 > 0 && y0 > 0)
    {
      putpixel (bmp, x, y, playable_color);
      while (spread_color_down (bmp, light_color, playable_color)
             + spread_color_up (bmp, light_color, playable_color));
    }
  else {
    log_print_str("L");
    unplayable |= 1;
  }

  for (y = 0; y < al_get_bitmap_height (bmp); ++y)
    for (x = 0; x < al_get_bitmap_width (bmp); ++x)
      if (is_color_light(getpixel (bmp, x, y)))
        ++playable_place;

  if (playable_place < MINI_PLAYABLE_AREA) {
    log_print_str("S");
  }
  unplayable |= (playable_place < MINI_PLAYABLE_AREA);

  return (!unplayable);
}

/*-----------------------------------------------------------------*/
static void
fill_with_fg_and_bg (ALLEGRO_BITMAP * bmp, ALLEGRO_COLOR fg, ALLEGRO_COLOR bg)
{
  int x, y;

  /*
   * We set up the real fg and bg color instead of the
   * CONSIDERED_AS_... constants.
   * Light pixels become bg (playable area), dark pixels become fg (walls).
   */
  al_set_target_bitmap (bmp);
  for (y = 0; y < al_get_bitmap_height (bmp); ++y)
    {
      for (x = 0; x < al_get_bitmap_width (bmp); ++x)
        {
          putpixel_fast (x, y,
                    is_color_light(getpixel (bmp, x, y)) ? bg : fg);
        }
    }
}

/*-----------------------------------------------------------------*/
static void
convert_to_buffer (ALLEGRO_BITMAP * bmp, char *buffer, int *size,
                   int *bg_size)
{
  int x, y, l, w, h;
  int is_light;

  w = al_get_bitmap_width (bmp);
  h = al_get_bitmap_height (bmp);
  (*size) = 0;
  (*bg_size) = 0;

  for (y = 0; y < h; ++y)
    {
      for (x = 0; x < w; )
        {
          l = 0;
          is_light = is_color_light(getpixel(bmp, x, y));

          if (is_light)
            {
              // Count consecutive light (playable) pixels
              while (x < w && is_color_light(getpixel(bmp, x, y)) && l < 127)
                {
                  l++;
                  x++;
                }
              buffer[(*size)++] = -l;
              (*bg_size) += l;
            }
          else
            {
              // Count consecutive dark (wall) pixels
              while (x < w && is_color_dark(getpixel(bmp, x, y)) && l < 127)
                {
                  l++;
                  x++;
                }
              buffer[(*size)++] = l;
            }
        }
    }
  buffer[(*size)++] = 0;
}

/*------------------------------------------------------------------*/
void *
lw_map_archive_raw_bmp (ALLEGRO_BITMAP * bmp, const char *filename)
{
  int w = 0, h = 0, size = 0, bg_size = 0;
  ALLEGRO_BITMAP *sub_bmp;
  char *temp = NULL;
  char *result = NULL;
  static int index = 1;
  char readable_name_buffer[LW_MAP_READABLE_NAME_SIZE + 1];
  char system_name_buffer[LW_MAP_SYSTEM_NAME_SIZE + 1];

  if (!bmp) {
    log_print_str("B");
    return NULL;
  }

  sort_light_and_dark (bmp);
  sub_bmp = extract_significant_part (bmp);
  if (!sub_bmp) {
    log_print_str("E");
    al_destroy_bitmap (bmp);
    return NULL;
  }

  w = al_get_bitmap_width (sub_bmp);
  h = al_get_bitmap_height (sub_bmp);
  if (!check_if_playable (sub_bmp)) {
    log_print_str("P");
    al_destroy_bitmap (sub_bmp);
    al_destroy_bitmap (bmp);
    return NULL;
  }

  temp = malloc_in_big_data_bottom (w * h + 1);
  if (!temp) {
    log_print_str("M");
    al_destroy_bitmap (sub_bmp);
    al_destroy_bitmap (bmp);
    return NULL;
  }

  convert_to_buffer (sub_bmp, temp, &size, &bg_size);
  al_destroy_bitmap (sub_bmp);
  al_destroy_bitmap (bmp);

  if (temp)
    {
      result =
        malloc (size + 8 + LW_MAP_SYSTEM_NAME_SIZE +
                LW_MAP_READABLE_NAME_SIZE);
      if (result)
        {
          /*
           * Write the header (size + geometry)
           */
          lw_serial_set_map_header ((void *) result, size, (short) w,
                                    (short) h);

          /*
           * Then we store its system name
           */
          memset (system_name_buffer, 0, sizeof (system_name_buffer));
          LW_MACRO_STRCPY (system_name_buffer,
                           lw_path_get_system_name (filename));
          memcpy (result + 8, system_name_buffer, LW_MAP_SYSTEM_NAME_SIZE);

          /*
           * Then we store its readable name
           */
          memset (readable_name_buffer, 0, sizeof (readable_name_buffer));
          if (strcmp (filename, "lwmapgen") == 0)
            {
              /* 
               * This is a map generated by lwmapgen, we label it as such.
               */
              LW_MACRO_SPRINTF0 (readable_name_buffer, "Random map");
            }
          else
            {
              LW_MACRO_SPRINTF1 (readable_name_buffer, "Custom map %d",
                                 index++);
            }
          memcpy (result + 8 + LW_MAP_SYSTEM_NAME_SIZE, readable_name_buffer,
                  LW_MAP_READABLE_NAME_SIZE);

          /*
           * And at last we write the map itself
           */
          memcpy (result + 8 + LW_MAP_SYSTEM_NAME_SIZE +
                  LW_MAP_READABLE_NAME_SIZE, temp, size);
        }
      free_last_big_data_bottom ();
    }

  return result;
}

/*------------------------------------------------------------------*/
void *
lw_map_archive_raw (const char *filename)
{
  ALLEGRO_BITMAP *bmp;
  void *result = NULL;

  bmp = al_load_bitmap_flags(filename, ALLEGRO_MEMORY_BITMAP);
  if (bmp)
    {
      result = lw_map_archive_raw_bmp (bmp, filename);
    }

  return result;
}

/*------------------------------------------------------------------*/
static void *
lw_map_get_safe (int num, int network, int random)
{
  void *raw_map = NULL;

  if (num > RAW_MAP_NUMBER - 1)
    num = RAW_MAP_NUMBER - 1;

  if (num < 0)
    num = 0;

  if (network && LW_NETWORK_RAW_MAP != NULL)
    {
      raw_map = LW_NETWORK_RAW_MAP;
    }
  else
    {
      if (random && LW_RANDOM_RAW_MAP != NULL)
        {
          raw_map = LW_RANDOM_RAW_MAP;
        }
      else
        {
          raw_map = RAW_MAP_ORDERED[num];
        }
    }

  return raw_map;
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
lw_map_create_bicolor (int num, int fg, int bg,
                       int network, int random, int min_w, int min_h,
                       int *zoom_factor)
{
  int pos_dst, pos_src, i, j, l;
  ALLEGRO_COLOR color;
  short w, h;
  signed char *data_src;
  ALLEGRO_BITMAP *result = NULL;
  ALLEGRO_BITMAP *temp;
  void *raw_map;
  int size, surface;
  int x, y;
  ALLEGRO_COLOR fg_color, bg_color;

  raw_map = lw_map_get_safe (num, network, random);

  /* Convert int colors to ALLEGRO_COLOR - fg/bg are palette indices or RGB values */
  fg_color = al_map_rgb((fg >> 16) & 0xFF, (fg >> 8) & 0xFF, fg & 0xFF);
  bg_color = al_map_rgb((bg >> 16) & 0xFF, (bg >> 8) & 0xFF, bg & 0xFF);

  data_src = raw_map;
  data_src += 8 + LW_MAP_SYSTEM_NAME_SIZE + LW_MAP_READABLE_NAME_SIZE;

  /*
   * We read the header (size + geometry)
   */
  lw_serial_get_map_header (raw_map, &size, &w, &h);

  surface = ((int) w) * ((int) h);
  *zoom_factor = calc_zoom_factor (w, h, min_w, min_h);

  pos_src = pos_dst = 0;
  temp = my_create_memory_bitmap (w, h);

  if (temp)
    {
      ALLEGRO_COLOR dark_color, light_color, pixel_color;
      int px, py;

      dark_color = al_map_rgb((CONSIDERED_AS_DARK_RGB >> 16) & 0xFF,
                             (CONSIDERED_AS_DARK_RGB >> 8) & 0xFF,
                             CONSIDERED_AS_DARK_RGB & 0xFF);
      light_color = al_map_rgb((CONSIDERED_AS_LIGHT_RGB >> 16) & 0xFF,
                              (CONSIDERED_AS_LIGHT_RGB >> 8) & 0xFF,
                              CONSIDERED_AS_LIGHT_RGB & 0xFF);

      al_set_target_bitmap(temp);
      while (pos_src < size && (l = data_src[pos_src++]) != 0)
        {
          if (l > 0)
            pixel_color = dark_color;
          else
            {
              l = -l;
              pixel_color = light_color;
            }
          for (i = 0; i < l && pos_dst < surface; ++i)
            {
              px = pos_dst % w;
              py = pos_dst / w;
              putpixel_fast(px, py, pixel_color);
              pos_dst++;
            }
        }

      /*
       * We check if the map is "playable". Normally unplayable maps
       * should never make it there but...
       */
      if (check_if_playable (temp))
        {
          fill_with_fg_and_bg (temp, fg_color, bg_color);

          result = my_create_memory_bitmap (w * (*zoom_factor), h * (*zoom_factor));

          if (result)
            {
              /*
               * Not we magnify the map if needed, so that very small
               * maps are interesting to play on fast fancy computers
               */
              al_set_target_bitmap (result);
              for (y = 0; y < h; ++y)
                for (x = 0; x < w; ++x)
                  {
                    color = getpixel (temp, x, y);
                    for (i = 0; i < *zoom_factor; ++i)
                      for (j = 0; j < *zoom_factor; ++j)
                        {
                          putpixel_fast (x * *zoom_factor + j,
                                    y * *zoom_factor + i, color);
                        }
                  }
            }
        }
      else
        {
          log_println_str ("Error: unplayable map!");
        }

      al_destroy_bitmap (temp);
    }

  return result;
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
lw_map_create_textured (int num, int fg, int bg,
                        int network, int random, int min_w, int min_h)
{
  int x, y, w, h;
  ALLEGRO_COLOR color;
  ALLEGRO_BITMAP *result = NULL, *bg_tex, *fg_tex;
  int fg_w, fg_h;
  int bg_w, bg_h;
  int zoom_factor, zoom_factor_fg, zoom_factor_bg;

  bg_tex = lw_texture_create_bg (bg);
  fg_tex = lw_texture_create_fg (fg);

  if (bg_tex && fg_tex)
    {
      bg_w = al_get_bitmap_width (bg_tex);
      bg_h = al_get_bitmap_height (bg_tex);
      fg_w = al_get_bitmap_width (fg_tex);
      fg_h = al_get_bitmap_height (fg_tex);

      result =
        lw_map_create_bicolor (num, 1, 0, network, random,
                               min_w, min_h, &zoom_factor);
      if (result)
        {
          w = al_get_bitmap_width (result);
          h = al_get_bitmap_height (result);
          zoom_factor_fg = zoom_factor_bg = zoom_factor;
          if (w != zoom_factor * al_get_bitmap_width (fg_tex) || h != zoom_factor * al_get_bitmap_height (fg_tex))
            {
              zoom_factor_fg = 1;
            }
          if (w != zoom_factor * al_get_bitmap_width (bg_tex) || h != zoom_factor * al_get_bitmap_height (bg_tex))
            {
              zoom_factor_bg = 1;
            }
          al_set_target_bitmap (result);
          for (y = 0; y < h; ++y)
            for (x = 0; x < w; ++x)
              {
                if (is_color_light(getpixel (result, x, y)))
                  {
                    color =
                      getpixel (fg_tex, (x / zoom_factor_fg) % fg_w,
                                (y / zoom_factor_bg) % fg_h);
                  }
                else
                  {
                    color =
                      getpixel (bg_tex, (x / zoom_factor_bg) % bg_w,
                                (y / zoom_factor_bg) % bg_h);
                  }
                putpixel_fast (x, y, color);
              }
        }
    }

  if (bg_tex)
    al_destroy_bitmap (bg_tex);

  if (fg_tex)
    al_destroy_bitmap (fg_tex);

  return result;
}

/*------------------------------------------------------------------*/
char *
lw_map_get_system_name (int num, int network, int random)
{
  static char result[LW_MAP_SYSTEM_NAME_SIZE + 1];
  void *raw_map;
  char *char_ptr;

  raw_map = lw_map_get_safe (num, network, random);
  char_ptr = (char *) raw_map;

  LW_MACRO_MEMSET0 (result);
  memcpy (result, char_ptr + 8, LW_MAP_SYSTEM_NAME_SIZE);
  result[sizeof (result) - 1] = '\0';

  return result;
}

/*------------------------------------------------------------------*/
char *
lw_map_get_readable_name (int num, int network, int random)
{
  static char result[LW_MAP_READABLE_NAME_SIZE + 1];
  void *raw_map;
  char *char_ptr;

  raw_map = lw_map_get_safe (num, network, random);
  char_ptr = (char *) raw_map;

  LW_MACRO_MEMSET0 (result);
  memcpy (result, char_ptr + 8 + LW_MAP_SYSTEM_NAME_SIZE,
          LW_MAP_READABLE_NAME_SIZE);
  result[sizeof (result) - 1] = '\0';

  return result;
}

/*------------------------------------------------------------------*/
void
lw_map_get_res (int num, int *w, int *h, int network, int random, int min_w,
                int min_h)
{
  void *raw_map;
  short res_w, res_h;
  int zoom_factor;

  raw_map = lw_map_get_safe (num, network, random);

  lw_serial_get_map_header (raw_map, NULL, &res_w, &res_h);

  zoom_factor = calc_zoom_factor (res_w, res_h, min_w, min_h);

  *w = res_w * zoom_factor;
  *h = res_h * zoom_factor;
}
