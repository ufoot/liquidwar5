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
#include "backport.h"
#include "bigdata.h"
#include "texture.h"
#include "disk.h"
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

/* Palette-based texture functions removed - not needed for Allegro 5 RGB mode */

/*------------------------------------------------------------------*/
void *
lw_texture_archive_raw (const char *filename)
{
  /* Simplified for Allegro 5 - just store the bitmap pointer directly
   * No palette quantization or 5-bit packing needed with RGB bitmaps */
  ALLEGRO_BITMAP *bmp;

  bmp = al_load_bitmap_flags(filename, ALLEGRO_MEMORY_BITMAP);

  /* Return the bitmap as-is - callers will use it as an ALLEGRO_BITMAP* */
  return (void *)bmp;
}

/*------------------------------------------------------------------*/
static ALLEGRO_BITMAP *
create_raw_texture (void *ptr)
{
  /* Simplified for Allegro 5 - ptr is already an ALLEGRO_BITMAP*
   * Just return it directly */
  return (ALLEGRO_BITMAP *)ptr;
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
create_mono_texture (ALLEGRO_COLOR color)
{
  ALLEGRO_BITMAP *result;

  result = my_create_memory_bitmap (1, 1);
  putpixel (result, 0, 0, color);
  return result;
}

/*------------------------------------------------------------------*/
static ALLEGRO_BITMAP *
create_texture (int num, ALLEGRO_COLOR default_color)
{
  ALLEGRO_BITMAP *result;
  void *texture;

  texture = get_raw_texture (num);

  if (texture)
    {
      result = create_raw_texture (texture);
    }
  else
    {
      result = create_mono_texture (default_color);
    }

  return result;
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
lw_texture_create_bg (int num)
{
  // In true color mode, use a default gray for background textures
  return create_texture (num, al_map_rgb(64, 64, 64));
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
lw_texture_create_fg (int num)
{
  // In true color mode, use a default white for foreground textures
  return create_texture (num, al_map_rgb(192, 192, 192));
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
