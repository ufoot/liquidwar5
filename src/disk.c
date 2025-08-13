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
/* nom           : disk.c                                           */
/* contenu       : lecture des donnees du fichier .dat              */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_physfs.h>

#include "alleg2.h"
#include "init.h"
#include "disk.h"
#include "log.h"
#include "map.h"
#include "startup.h"
#include "texture.h"
#include "macro.h"
#include "path.h"

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

int LOADED_FONT = 0;
int LOADED_MAP = 0;
int LOADED_BACK = 0;
int LOADED_TEXTURE = 0;
int LOADED_MAPTEX = 0;
int LOADED_SFX = 0;
int LOADED_WATER = 0;
int LOADED_MUSIC = 0;

ALLEGRO_SAMPLE *SAMPLE_SFX_TIME = NULL;
ALLEGRO_SAMPLE *SAMPLE_SFX_WIN = NULL;
ALLEGRO_SAMPLE *SAMPLE_SFX_GO = NULL;
ALLEGRO_SAMPLE *SAMPLE_SFX_CLICK = NULL;
ALLEGRO_SAMPLE *SAMPLE_SFX_LOOSE = NULL;
ALLEGRO_SAMPLE *SAMPLE_SFX_CONNECT = NULL;

ALLEGRO_SAMPLE *SAMPLE_WATER[SAMPLE_WATER_MAX_NUMBER];
void *RAW_MAP[RAW_MAP_MAX_NUMBER];
void *RAW_MAP_ORDERED[RAW_MAP_MAX_NUMBER];
void *RAW_TEXTURE[RAW_TEXTURE_MAX_NUMBER];
void *RAW_MAPTEX[RAW_TEXTURE_MAX_NUMBER];
ALLEGRO_SAMPLE *MIDI_MUSIC[MIDI_MUSIC_MAX_NUMBER];

ALLEGRO_BITMAP *BACK_IMAGE = NULL;

ALLEGRO_FONT *BIG_FONT = NULL;
ALLEGRO_FONT *SMALL_FONT = NULL;
ALLEGRO_BITMAP *BIG_MOUSE_CURSOR = NULL;
ALLEGRO_BITMAP *SMALL_MOUSE_CURSOR = NULL;
ALLEGRO_BITMAP *INVISIBLE_MOUSE_CURSOR = NULL;

//static RGB *FONT_PALETTE = NULL;
//static RGB *BACK_PALETTE = NULL;

static int CUSTOM_TEXTURE_OK = 0;
static int CUSTOM_MAP_OK = 0;
static int CUSTOM_MUSIC_OK = 0;

/*------------------------------------------------------------------*/
/* chargement des effets sonores                                    */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static void
lock_sound (ALLEGRO_SAMPLE * smp)
{
  LOCK_VARIABLE (*smp);
#ifdef DOS
  _go32_dpmi_lock_data (smp->data, (smp->bits / 8) * smp->len);
#else
  LW_MACRO_NOP (smp);
#endif
}

/*------------------------------------------------------------------*/
static ALLEGRO_SAMPLE *read_sfx(const char *filename) {
  ALLEGRO_SAMPLE *ret = NULL;

  char * path = lw_path_join3(STARTUP_DAT_PATH, "sfx", filename);
  if (path == NULL) {
    return NULL;
  }
  ret = al_load_sample(path);
  free(path);
  return ret;
}

/*------------------------------------------------------------------*/
static bool
read_sfx_dat ()
{
  SAMPLE_SFX_TIME = read_sfx("clock1.wav");
  SAMPLE_SFX_WIN = read_sfx("crowd1.wav");
  SAMPLE_SFX_CONNECT = read_sfx("cuckoo.wav");
  SAMPLE_SFX_GO = read_sfx("foghorn.wav");
  SAMPLE_SFX_CLICK = read_sfx("spash1.wav");
  SAMPLE_SFX_LOOSE = read_sfx("war.wav");

  return SAMPLE_SFX_TIME != NULL &&
    SAMPLE_SFX_WIN != NULL &&
    SAMPLE_SFX_CONNECT != NULL &&
    SAMPLE_SFX_GO != NULL &&
    SAMPLE_SFX_CLICK != NULL &&
    SAMPLE_SFX_LOOSE != NULL;
}

/*------------------------------------------------------------------*/
static bool
read_water_dat ()
{
  char water_files[][16] = {
    "amb3.wav", "amb4.wav", "bath1.wav", "bath2.wav",
    "bubble.wav", "flush.wav", "forest1.wav", "kitch4.wav",
    "lavaflow.wav", "niagara.wav", "shower1.wav", "sodapor.wav",
    "thundr2.wav", "thundr3.wav", "tidal.wav", "water.wav"
  };
  int num_files = sizeof(water_files) / sizeof(water_files[0]);
  int i;

  SAMPLE_WATER_NUMBER = 0;
  for (i = 0; i < num_files && i < SAMPLE_WATER_MAX_NUMBER; ++i)
    {
      char * path = lw_path_join3(STARTUP_DAT_PATH, "water", water_files[i]);
      if (path != NULL) {
        SAMPLE_WATER[i] = al_load_sample(path);
        if (SAMPLE_WATER[i] != NULL) {
          lock_sound (SAMPLE_WATER[i]);
          SAMPLE_WATER_NUMBER++;
        }
        free(path);
      }
    }
  return SAMPLE_WATER_NUMBER > 0;
}

/*------------------------------------------------------------------*/
/* chargement des autres donnees                                    */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static bool
read_texture_dat ()
{
  char texture_files[][32] = {
    "amethyst.gimp.pcx", "bricks.gimp.pcx", "crash1.gimp.pcx",
    "electricblue.gimp.pcx", "granite2.gimp.pcx", "greenmess.gimp.pcx",
    "lumps.gimp.pcx", "marble3.gimp.pcx", "pebbles.gimp.pcx",
    "pine.gimp.pcx", "poolbottom.gimp.pcx", "qbert.gimp.pcx",
    "redcubes.gimp.pcx", "smallsquares.gimp.pcx", "terra.gimp.pcx",
    "wood2.gimp.pcx"
  };
  int num_files = sizeof(texture_files) / sizeof(texture_files[0]);
  int i;

  RAW_TEXTURE_NUMBER = 0;
  for (i = 0; i < num_files && i < RAW_TEXTURE_MAX_NUMBER; ++i)
    {
      char * path = lw_path_join3(STARTUP_DAT_PATH, "texture", texture_files[i]);
      if (path != NULL) {
        void *texture = lw_texture_archive_raw(path);
        if (texture != NULL) {
          RAW_TEXTURE[i] = texture;
          RAW_TEXTURE_NUMBER++;
        }
        free(path);
      }
    }
  return RAW_TEXTURE_NUMBER > 0;
}

/*------------------------------------------------------------------*/
static bool
read_maptex_dat ()
{
  RAW_MAPTEX_NUMBER = 0;

  char * maptex_path = lw_path_join2(STARTUP_DAT_PATH, "maptex");
  if (maptex_path == NULL) {
    return false;
  }

  ALLEGRO_FS_ENTRY *dir = al_create_fs_entry(maptex_path);
  free(maptex_path);

  if (!dir || !al_open_directory(dir)) {
    if (dir) al_destroy_fs_entry(dir);
    return false;
  }

  ALLEGRO_FS_ENTRY *entry;
  while ((entry = al_read_directory(dir)) != NULL && RAW_MAPTEX_NUMBER < RAW_TEXTURE_MAX_NUMBER) {
    if (al_get_fs_entry_mode(entry) & ALLEGRO_FILEMODE_ISFILE) {
      const char *filename = al_get_fs_entry_name(entry);
      void *maptex = lw_texture_archive_raw(filename);
      if (maptex != NULL) {
        RAW_MAPTEX[RAW_MAPTEX_NUMBER] = maptex;
        RAW_MAPTEX_NUMBER++;
      }
    }
    al_destroy_fs_entry(entry);
  }

  al_close_directory(dir);
  al_destroy_fs_entry(dir);
  return RAW_MAPTEX_NUMBER > 0;
}

/*------------------------------------------------------------------*/
static bool
read_map_dat ()
{
  RAW_MAP_NUMBER = 0;

  char * map_path = lw_path_join2(STARTUP_DAT_PATH, "map");
  if (map_path == NULL) {
    return false;
  }

  ALLEGRO_FS_ENTRY *dir = al_create_fs_entry(map_path);
  free(map_path);

  if (!dir || !al_open_directory(dir)) {
    if (dir) al_destroy_fs_entry(dir);
    return false;
  }

  ALLEGRO_FS_ENTRY *entry;
  while ((entry = al_read_directory(dir)) != NULL && RAW_MAP_NUMBER < RAW_MAP_MAX_NUMBER) {
    if (al_get_fs_entry_mode(entry) & ALLEGRO_FILEMODE_ISFILE) {
      const char *filename = al_get_fs_entry_name(entry);
      const char *ext = strrchr(filename, '.');
      if (ext && (strcmp(ext, ".pcx") == 0 || strcmp(ext, ".bmp") == 0)) {
        void *map = lw_map_archive_raw(filename);
        if (map != NULL) {
          RAW_MAP[RAW_MAP_NUMBER] = map;
          RAW_MAP_NUMBER++;
        }
      }
    }
    al_destroy_fs_entry(entry);
  }

  al_close_directory(dir);
  al_destroy_fs_entry(dir);
  return RAW_MAP_NUMBER > 0;
}

/*------------------------------------------------------------------*/
static bool
read_back_dat ()
{
  char * path = lw_path_join3(STARTUP_DAT_PATH, "back", "lw5back.pcx");
  if (path == NULL) {
    return false;
  }

  log_println();
  log_print_str(path);
  log_println();
  BACK_IMAGE = al_load_bitmap(path);
  free(path);

  if (BACK_IMAGE == NULL) {
    return false;
  }

  return true;
}

/*------------------------------------------------------------------*/
static bool
read_font_dat ()
{
  char * small_font_path = lw_path_join3(STARTUP_DAT_PATH, "font", "degrad10.pcx");
  char * big_font_path = lw_path_join3(STARTUP_DAT_PATH, "font", "degrad20.pcx");
  char * small_cursor_path = lw_path_join3(STARTUP_DAT_PATH, "font", "mouse20.pcx");
  char * big_cursor_path = lw_path_join3(STARTUP_DAT_PATH, "font", "mouse40.pcx");
  char * void_cursor_path = lw_path_join3(STARTUP_DAT_PATH, "font", "void1.pcx");

  bool success = true;

  if (small_font_path) {
    SMALL_FONT = al_load_bitmap_font(small_font_path);
    free(small_font_path);
    if (!SMALL_FONT) success = false;
  } else {
    success = false;
  }

  if (big_font_path) {
    BIG_FONT = al_load_bitmap_font(big_font_path);
    free(big_font_path);
    if (!BIG_FONT) success = false;
  } else {
    success = false;
  }

  if (small_cursor_path) {
    SMALL_MOUSE_CURSOR = al_load_bitmap(small_cursor_path);
    free(small_cursor_path);
    if (!SMALL_MOUSE_CURSOR) success = false;
  } else {
    success = false;
  }

  if (big_cursor_path) {
    BIG_MOUSE_CURSOR = al_load_bitmap(big_cursor_path);
    free(big_cursor_path);
    if (!BIG_MOUSE_CURSOR) success = false;
  } else {
    success = false;
  }

  if (void_cursor_path) {
    INVISIBLE_MOUSE_CURSOR = al_load_bitmap(void_cursor_path);
    free(void_cursor_path);
    if (!INVISIBLE_MOUSE_CURSOR) success = false;
  } else {
    success = false;
  }

  return success;
}

/*------------------------------------------------------------------*/
static bool
read_music_dat ()
{
  /* Music loading disabled for now */
  MIDI_MUSIC_NUMBER = 0;
  return true;
}


/*------------------------------------------------------------------*/
static int check_loadable() {
  int loadable = 0;

  loadable = exists (STARTUP_DAT_PATH);

  // Checking for the existence of this file, to quickly spot whether
  // this is a genuine data folder. If that text file is not there, we
  // can just leave and assume this is an unkown random place.
  char * path = lw_path_join2(STARTUP_DAT_PATH, "liquidwar-data.txt");
  if (path == NULL) {
    return 0;
  }
  loadable = exists(path);
  free(path);

  return loadable;
}

/*------------------------------------------------------------------*/
int
load_dat (void)
{
  int result = 1;
  int loadable = 0;

  log_print_str ("Searching for data in \"");
  log_print_str (STARTUP_DAT_PATH);
  log_print_str ("\"");
  log_flush ();

  loadable = check_loadable();
  display_success (loadable);

  if (loadable && STARTUP_BACK_STATE)
    {
      log_print_str ("Loading background bitmap");
      log_flush ();
      LOADED_BACK = read_back_dat();
      display_success(LOADED_BACK);
      result &= LOADED_BACK;
    }
  if (loadable)
    {
      log_print_str ("Loading fonts");
      log_flush ();
      LOADED_FONT = read_font_dat ();
      display_success (LOADED_FONT);
      result &= LOADED_FONT;
    }
  if (loadable)
    {
      log_print_str ("Loading maps");
      log_flush ();
      LOADED_MAP = read_map_dat ();
      display_success (LOADED_MAP);
      result &= LOADED_MAP;
    }

  if (loadable && STARTUP_SFX_STATE)
    {
      log_print_str ("Loading sound fx");
      log_flush ();
      if (read_sfx_dat())
        {
          LOADED_SFX = 1;
          display_success(1);
        }
      else
        {
          result &= !STARTUP_CHECK;
          display_success(0);
        }
    }
  if (loadable && STARTUP_TEXTURE_STATE)
    {
      log_print_str ("Loading textures");
      log_flush ();
      LOADED_TEXTURE = read_texture_dat();
      display_success(LOADED_TEXTURE);
      if (!LOADED_TEXTURE) {
        result &= !STARTUP_CHECK;
      }

      log_print_str ("Loading map textures");
      log_flush ();
      LOADED_MAPTEX = read_maptex_dat();
      display_success(LOADED_MAPTEX);
      if (!LOADED_MAPTEX) {
        result &= !STARTUP_CHECK;
      }
    }

  if (loadable && STARTUP_WATER_STATE)
    {
      log_print_str ("Loading water sounds");
      log_flush ();
      LOADED_WATER = read_water_dat();
      display_success(LOADED_WATER);
      if (!LOADED_WATER) {
        result &= !STARTUP_CHECK;
      }
    }

  if (loadable && STARTUP_MUSIC_STATE)
    {
      log_print_str ("Loading midi music");
      log_flush ();
      if (read_music_dat())
        {
          LOADED_MUSIC = 1;
          display_success(1);
        }
      else
        {
          result &= !STARTUP_CHECK;
          display_success(0);
        }
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
  CUSTOM_TEXTURE_OK = 0;

  ALLEGRO_FS_ENTRY *dir = al_create_fs_entry(STARTUP_TEX_PATH);
  if (!dir || !al_open_directory(dir)) {
    if (dir) al_destroy_fs_entry(dir);
    return 0;
  }

  ALLEGRO_FS_ENTRY *entry;
  while ((entry = al_read_directory(dir)) != NULL && RAW_TEXTURE_NUMBER < RAW_TEXTURE_MAX_NUMBER) {
    if (al_get_fs_entry_mode(entry) & ALLEGRO_FILEMODE_ISFILE) {
      const char *filename = al_get_fs_entry_name(entry);
      load_custom_texture_callback(filename, 0, NULL);
    }
    al_destroy_fs_entry(entry);
  }

  al_close_directory(dir);
  al_destroy_fs_entry(dir);
  return CUSTOM_TEXTURE_OK;
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
  CUSTOM_MAP_OK = 0;

  ALLEGRO_FS_ENTRY *dir = al_create_fs_entry(STARTUP_MAP_PATH);
  if (!dir || !al_open_directory(dir)) {
    if (dir) al_destroy_fs_entry(dir);
    return 0;
  }

  ALLEGRO_FS_ENTRY *entry;
  while ((entry = al_read_directory(dir)) != NULL && RAW_MAP_NUMBER < RAW_MAP_MAX_NUMBER) {
    if (al_get_fs_entry_mode(entry) & ALLEGRO_FILEMODE_ISFILE) {
      const char *filename = al_get_fs_entry_name(entry);
      load_custom_map_callback(filename, 0, NULL);
    }
    al_destroy_fs_entry(entry);
  }

  al_close_directory(dir);
  al_destroy_fs_entry(dir);
  return CUSTOM_MAP_OK;
}

/*------------------------------------------------------------------*/
static int
load_custom_music_callback (const char *file, int mode, void *unused)
{
  LW_MACRO_NOP (mode);
  LW_MACRO_NOP (unused);
  LW_MACRO_NOP (file);

  /* Music loading disabled for now */
  log_print_str ("-");
  log_flush ();

  return 0;
}

/*------------------------------------------------------------------*/
static int
load_custom_music (void)
{
  CUSTOM_MUSIC_OK = 0;

  ALLEGRO_FS_ENTRY *dir = al_create_fs_entry(STARTUP_MID_PATH);
  if (!dir || !al_open_directory(dir)) {
    if (dir) al_destroy_fs_entry(dir);
    return 0;
  }

  ALLEGRO_FS_ENTRY *entry;
  while ((entry = al_read_directory(dir)) != NULL && MIDI_MUSIC_NUMBER < MIDI_MUSIC_MAX_NUMBER) {
    if (al_get_fs_entry_mode(entry) & ALLEGRO_FILEMODE_ISFILE) {
      const char *filename = al_get_fs_entry_name(entry);
      load_custom_music_callback(filename, 0, NULL);
    }
    al_destroy_fs_entry(entry);
  }

  al_close_directory(dir);
  al_destroy_fs_entry(dir);
  return CUSTOM_MUSIC_OK;
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
