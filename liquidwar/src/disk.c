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
/* nom           : disk.c                                           */
/* contenu       : lecture des donnees du fichier .dat              */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <allegro.h>

#include "alleg2.h"
#include "init.h"
#include "disk.h"
#include "log.h"
#include "map.h"
#include "palette.h"
#include "startup.h"
#include "texture.h"
#include "macro.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#define SAMPLE_SFX_NUMBER  6

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int SAMPLE_WATER_NUMBER = 0;
int RAW_TEXTURE_NUMBER = 0;
int RAW_MAPTEX_NUMBER = 0;
int RAW_MAP_NUMBER = 0;
int MIDI_MUSIC_NUMBER = 0;

int LOADED_BACK = 0;
int LOADED_TEXTURE = 0;
int LOADED_MAPTEX = 0;
int LOADED_SFX = 0;
int LOADED_WATER = 0;
int LOADED_MUSIC = 0;

SAMPLE *SAMPLE_SFX_TIME = NULL;
SAMPLE *SAMPLE_SFX_WIN = NULL;
SAMPLE *SAMPLE_SFX_GO = NULL;
SAMPLE *SAMPLE_SFX_CLICK = NULL;
SAMPLE *SAMPLE_SFX_LOOSE = NULL;
SAMPLE *SAMPLE_SFX_CONNECT = NULL;

SAMPLE *SAMPLE_WATER[SAMPLE_WATER_MAX_NUMBER];
void *RAW_MAP[RAW_MAP_MAX_NUMBER];
void *RAW_MAP_ORDERED[RAW_MAP_MAX_NUMBER];
void *RAW_TEXTURE[RAW_TEXTURE_MAX_NUMBER];
void *RAW_MAPTEX[RAW_TEXTURE_MAX_NUMBER];
MIDI *MIDI_MUSIC[MIDI_MUSIC_MAX_NUMBER];

BITMAP *BACK_IMAGE = NULL;

FONT *BIG_FONT = NULL;
FONT *SMALL_FONT = NULL;
BITMAP *BIG_MOUSE_CURSOR = NULL;
BITMAP *SMALL_MOUSE_CURSOR = NULL;
BITMAP *INVISIBLE_MOUSE_CURSOR = NULL;

static RGB *FONT_PALETTE = NULL;
static RGB *BACK_PALETTE = NULL;

static int CUSTOM_TEXTURE_OK = 0;
static int CUSTOM_MAP_OK = 0;
static int CUSTOM_MUSIC_OK = 0;

/*------------------------------------------------------------------*/
/* chargement des effets sonores                                    */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static void
lock_sound (SAMPLE * smp)
{
  LOCK_VARIABLE (*smp);
#ifdef DOS
  _go32_dpmi_lock_data (smp->data, (smp->bits / 8) * smp->len);
#else
  LW_MACRO_NOP (smp);
#endif
}

/*------------------------------------------------------------------*/
static void
read_sfx_dat (DATAFILE * df)
{
  SAMPLE *list[SAMPLE_SFX_NUMBER];
  int i;

  /*
   * First, we associate the _first_ sound of the sub datafile
   * to all sounds. This will operate as default value which
   * will prevent the game from segfaulting if we use it with
   * an outdated or too recent datafile
   */
  for (i = 0; i < SAMPLE_SFX_NUMBER && df[i].type != DAT_END; ++i)
    {
      list[i] = df[0].dat;
    }

  /*
   * Now we associate the real sounds, provided that they exist...
   */
  for (i = 0; i < 6 && df[i].type != DAT_END; ++i)
    {
      list[i] = df[i].dat;
      lock_sound (list[i]);
    }

  SAMPLE_SFX_TIME = list[0];
  SAMPLE_SFX_WIN = list[1];
  SAMPLE_SFX_CONNECT = list[2];
  SAMPLE_SFX_GO = list[3];
  SAMPLE_SFX_CLICK = list[4];
  SAMPLE_SFX_LOOSE = list[5];
}

/*------------------------------------------------------------------*/
static void
read_water_dat (DATAFILE * df)
{
  int i;

  for (i = 0; i < SAMPLE_WATER_DAT_NUMBER && df[i].type != DAT_END; ++i)
    {
      SAMPLE_WATER[i] = df[i].dat;
      lock_sound (SAMPLE_WATER[i]);
      SAMPLE_WATER_NUMBER++;
    }
}

/*------------------------------------------------------------------*/
/* chargement des autres donnees                                    */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static void
read_texture_dat (DATAFILE * df)
{
  int i;

  RAW_TEXTURE_NUMBER = 0;
  for (i = 0; i < RAW_TEXTURE_DAT_NUMBER && df[i].type != DAT_END; ++i)
    {
      RAW_TEXTURE[i] = df[i].dat;
      RAW_TEXTURE_NUMBER++;
    }
}

/*------------------------------------------------------------------*/
static void
read_maptex_dat (DATAFILE * df)
{
  int i;

  RAW_MAPTEX_NUMBER = 0;
  for (i = 0; i < RAW_TEXTURE_DAT_NUMBER && df[i].type != DAT_END; ++i)
    {
      RAW_MAPTEX[i] = df[i].dat;
      RAW_MAPTEX_NUMBER++;
    }
}

/*------------------------------------------------------------------*/
static void
read_map_dat (DATAFILE * df)
{
  int i;

  RAW_MAP_NUMBER = 0;
  for (i = 0; i < RAW_MAP_DAT_NUMBER && df[i].type != DAT_END; ++i)
    {
      RAW_MAP[i] = df[i].dat;
      RAW_MAP_NUMBER++;
    }
}

/*------------------------------------------------------------------*/
static void
read_back_dat (DATAFILE * df)
{
  int i, x, y;

  BACK_PALETTE = df[1].dat;
  BACK_IMAGE = df[0].dat;

  /*
   * strange, with Allegro 4.0, the liquidwarcol utility
   * and the datafile compiler do not work so well together,
   * and so the palette stored in the datafile always
   * start at color 0, which explains the "18 shift"
   */

  for (i = 0; i <= 45; ++i)
    GLOBAL_PALETTE[i + 18] = BACK_PALETTE[i];

  for (x = 0; x < BACK_IMAGE->w; ++x)
    for (y = 0; y < BACK_IMAGE->w; ++y)
      {
	putpixel (BACK_IMAGE, x, y, getpixel (BACK_IMAGE, x, y) + 18);
      }
}

/*------------------------------------------------------------------*/
static void
create_default_back (void)
{
  static RGB back_coul;

  memset (&back_coul, 0, sizeof (RGB));
  back_coul.r = 1;
  back_coul.g = 1;
  back_coul.b = 8;

  BACK_IMAGE = my_create_bitmap (1, 1);
  putpixel (BACK_IMAGE, 0, 0, 18);
  GLOBAL_PALETTE[18] = back_coul;
}

/*------------------------------------------------------------------*/
static void
read_font_dat (DATAFILE * df)
{
  int i;

  FONT_PALETTE = df[4].dat;
  SMALL_FONT = df[0].dat;
  BIG_FONT = df[1].dat;
  SMALL_MOUSE_CURSOR = df[2].dat;
  BIG_MOUSE_CURSOR = df[3].dat;
  INVISIBLE_MOUSE_CURSOR = df[5].dat;

  for (i = 1; i <= 17; ++i)
    GLOBAL_PALETTE[i] = FONT_PALETTE[i];
}

/*------------------------------------------------------------------*/
static void
read_music_dat (DATAFILE * df)
{
  int i;

  MIDI_MUSIC_NUMBER = 0;
  for (i = 0; i < MIDI_MUSIC_DAT_NUMBER && df[i].type != DAT_END; ++i)
    {
      MIDI_MUSIC[i] = df[i].dat;
      MIDI_MUSIC_NUMBER++;
    }
}

/*------------------------------------------------------------------*/
int
load_dat (void)
{
  int result = 1;
  int loadable;
  DATAFILE *df;

  log_print_str ("Loading data from \"");
  log_print_str (STARTUP_DAT_PATH);
  log_print_str ("\"");

#ifdef DOS
  loadable = 1;
#else
  loadable = exists (STARTUP_DAT_PATH);
#endif

  display_success (loadable);

  if (loadable)
    {
      log_print_str ("Loading fonts");
      log_flush ();
      df = load_datafile_object (STARTUP_DAT_PATH, "font_dat");
      if (result &= (df != NULL))
	read_font_dat (df[0].dat);
      display_success (df != NULL);
    }
  if (loadable)
    {
      log_print_str ("Loading maps");
      log_flush ();
      df = load_datafile_object (STARTUP_DAT_PATH, "map_dat");
      if (result &= (df != NULL))
	read_map_dat (df[0].dat);
      display_success (df != NULL);
    }

  if (loadable && STARTUP_BACK_STATE)
    {
      log_print_str ("Loading background bitmap");
      log_flush ();
      df = load_datafile_object (STARTUP_DAT_PATH, "back_dat");
      if (df != NULL)
	{
	  read_back_dat (df[0].dat);
	  LOADED_BACK = 1;
	}
      else
	{
	  create_default_back ();
	  result &= !STARTUP_CHECK;
	}
      display_success (df != NULL);
    }
  else
    create_default_back ();
  if (loadable && STARTUP_SFX_STATE)
    {
      log_print_str ("Loading sound fx");
      log_flush ();
      df = load_datafile_object (STARTUP_DAT_PATH, "sfx_dat");
      if (df != NULL)
	{
	  read_sfx_dat (df[0].dat);
	  LOADED_SFX = 1;
	}
      else
	result &= !STARTUP_CHECK;
      display_success (df != NULL);
    }
  if (loadable && STARTUP_TEXTURE_STATE)
    {
      log_print_str ("Loading textures");
      log_flush ();
      df = load_datafile_object (STARTUP_DAT_PATH, "texture_dat");
      if (df != NULL)
	{
	  read_texture_dat (df[0].dat);
	  LOADED_TEXTURE = 1;
	}
      else
	result &= !STARTUP_CHECK;
      display_success (df != NULL);

      log_print_str ("Loading map textures");
      log_flush ();
      df = load_datafile_object (STARTUP_DAT_PATH, "maptex_dat");
      if (df != NULL)
	{
	  read_maptex_dat (df[0].dat);
	  LOADED_MAPTEX = 1;
	}
      else
	result &= !STARTUP_CHECK;
      display_success (df != NULL);
    }

  if (loadable && STARTUP_WATER_STATE)
    {
      log_print_str ("Loading water sounds");
      log_flush ();
      df = load_datafile_object (STARTUP_DAT_PATH, "water_dat");
      if (df != NULL)
	{
	  read_water_dat (df[0].dat);
	  LOADED_WATER = 1;
	}
      else
	result &= !STARTUP_CHECK;
      display_success (df != NULL);
    }

  if (loadable && STARTUP_MUSIC_STATE)
    {
      log_print_str ("Loading midi music");
      log_flush ();
      df = load_datafile_object (STARTUP_DAT_PATH, "music_dat");
      if (df != NULL)
	{
	  read_music_dat (df[0].dat);
	  LOADED_MUSIC = 1;
	}
      else
	result &= !STARTUP_CHECK;
      display_success (df != NULL);
    }

  return loadable && result;
}

/*------------------------------------------------------------------*/
static int
load_custom_texture_callback (const char *file, int mode, void *unused)
{
  void *pointeur;

  LW_MACRO_NOP (mode);
  LW_MACRO_NOP (unused);

  if ((pointeur = lw_texture_archive_raw (file)) != NULL)
    {
      RAW_TEXTURE[RAW_TEXTURE_NUMBER++] = pointeur;
      log_print_str ("+");
      CUSTOM_TEXTURE_OK = 1;
    }
  else
    {
      log_print_str ("-");
    }
  log_flush ();

  return 0;
}

/*------------------------------------------------------------------*/
static int
load_custom_texture (void)
{
  int result = 1;
  char buf[512];

  LW_MACRO_SPRINTF1 (buf, "%s\\*.*", STARTUP_TEX_PATH);

  fix_filename_case (buf);
  fix_filename_slashes (buf);

  CUSTOM_TEXTURE_OK = 0;
  for_each_file_ex (buf, 0, FA_DIREC, load_custom_texture_callback, NULL);
  result = CUSTOM_TEXTURE_OK;

  return result;
}

/*------------------------------------------------------------------*/
static int
load_custom_map_callback (const char *file, int mode, void *unused)
{
  void *pointeur;

  LW_MACRO_NOP (mode);
  LW_MACRO_NOP (unused);

  if ((pointeur = lw_map_archive_raw (file)) != NULL)
    {
      RAW_MAP[RAW_MAP_NUMBER++] = pointeur;
      log_print_str ("+");
      CUSTOM_MAP_OK = 1;
    }
  else
    {
      log_print_str ("-");
    }

  return 0;
}

/*------------------------------------------------------------------*/
static int
load_custom_map (void)
{
  int result = 1;
  char buf[512];

  LW_MACRO_SPRINTF1 (buf, "%s\\*.*", STARTUP_MAP_PATH);

  fix_filename_case (buf);
  fix_filename_slashes (buf);

  CUSTOM_MAP_OK = 0;
  for_each_file_ex (buf, 0, FA_DIREC, load_custom_map_callback, NULL);
  result = CUSTOM_MAP_OK;

  return result;
}

/*------------------------------------------------------------------*/
static int
load_custom_music_callback (const char *file, int mode, void *unused)
{
  void *pointeur;

  LW_MACRO_NOP (mode);
  LW_MACRO_NOP (unused);

  if ((pointeur = load_midi (file)) != NULL)
    {
      MIDI_MUSIC[MIDI_MUSIC_NUMBER++] = pointeur;
      log_print_str ("+");
      CUSTOM_MUSIC_OK = 1;
    }
  else
    {
      log_print_str ("-");
    }
  log_flush ();

  return 0;
}

/*------------------------------------------------------------------*/
static int
load_custom_music (void)
{
  int result = 1;
  char buf[512];

  LW_MACRO_SPRINTF1 (buf, "%s\\*.*", STARTUP_MID_PATH);

  fix_filename_case (buf);
  fix_filename_slashes (buf);

  CUSTOM_MUSIC_OK = 0;
  for_each_file_ex (buf, 0, FA_DIREC, load_custom_music_callback, NULL);
  result = CUSTOM_MUSIC_OK;

  return result;
}

/*------------------------------------------------------------------*/
int
load_custom (void)
{
  int success, result = 1;

  if (STARTUP_TEXTURE_STATE && STARTUP_CUSTOM_STATE)
    {
      log_print_str ("Loading custom textures from \"");
      log_print_str (STARTUP_TEX_PATH);
      log_print_str ("\" ");
      log_flush ();
      success = load_custom_texture ();
      if (!success)
	result &= !STARTUP_CHECK;
      display_success (success);
    }

  if (STARTUP_CUSTOM_STATE)
    {
      log_print_str ("Loading custom maps from \"");
      log_print_str (STARTUP_MAP_PATH);
      log_print_str ("\" ");
      log_flush ();
      success = load_custom_map ();
      if (!success)
	result &= !STARTUP_CHECK;
      display_success (success);
    }

  if (STARTUP_CUSTOM_STATE && STARTUP_MUSIC_STATE)
    {
      log_print_str ("Loading custom musics from \"");
      log_print_str (STARTUP_MID_PATH);
      log_print_str ("\" ");
      log_flush ();
      success = load_custom_music ();
      if (!success)
	result &= !STARTUP_CHECK;
      display_success (success);
    }

  return result;
}

/*------------------------------------------------------------------*/
void
order_map (void)
{
  int incorrect_order = 1;
  int i;


  char name1[LW_MAP_READABLE_NAME_SIZE + 1];
  char name2[LW_MAP_READABLE_NAME_SIZE + 1];
  void *temp;

  for (i = 0; i < RAW_MAP_NUMBER; ++i)
    {
      RAW_MAP_ORDERED[i] = RAW_MAP[i];
    }

  while (incorrect_order)
    {
      incorrect_order = 0;

      for (i = 0; i < RAW_MAP_NUMBER - 1; ++i)
	{
	  LW_MACRO_STRCPY (name1, lw_map_get_readable_name (i, 0, 0));
	  LW_MACRO_STRCPY (name2, lw_map_get_readable_name (i + 1, 0, 0));
	  if (strcmp (name1, name2) > 0)
	    {
	      incorrect_order = 1;

	      temp = RAW_MAP_ORDERED[i];
	      RAW_MAP_ORDERED[i] = RAW_MAP_ORDERED[i + 1];
	      RAW_MAP_ORDERED[i + 1] = temp;
	    }
	}
    }
}
