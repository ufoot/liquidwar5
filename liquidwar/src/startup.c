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
/* Copyright (C) 1998-2011 Christian Mauduit                                 */
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
/* nom           : startup.c                                        */
/* contenu       : parametres de demarrage                          */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "startup.h"
#include "server.h"
#include "macro.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define IDENT_MEM                 "mem"
#define IDENT_NOBACK              "noback"
#define IDENT_NOTEXTURE           "notex"
#define IDENT_NOWATER             "nowater"
#define IDENT_NOSFX               "nosfx"
#define IDENT_NOMUSIC             "nomusic"
#define IDENT_NOCUSTOM            "nocustom"
#define IDENT_NOSOUND             "silent"
#define IDENT_NOSVGA              "vga"
#define IDENT_NOASM               "c"
#define IDENT_NOJOY               "nojoy"
#define IDENT_STOP                "stop"
#define IDENT_SAFE                "safe"
#define IDENT_NICE                "nice"
#define IDENT_AUTO                "auto"
#define IDENT_CHECK               "check"
#define IDENT_CFG                 "cfg"
#define IDENT_DAT                 "dat"
#define IDENT_MAP                 "map"
#define IDENT_TEX                 "tex"
#define IDENT_MID                 "mid"
#define IDENT_SRV                 "server"
#define IDENT_GEN                 "mapgen"
#define IDENT_METASERVER          "metaserver"
#define IDENT_CAPTURE             "capture"
#define IDENT_TOMBOLA             "tombola"

#define STARTUP_MAX_METASERVER_LENGTH      200

#ifdef DOS
int STARTUP_BIG_DATA = 8;
#else
int STARTUP_BIG_DATA = 16;
#endif
int STARTUP_DIGI_CARD = DIGI_AUTODETECT;
int STARTUP_MIDI_CARD = MIDI_AUTODETECT;
int STARTUP_JOYSTICK_ON = 1;
int STARTUP_BACK_STATE = 1;
int STARTUP_TEXTURE_STATE = 1;
int STARTUP_WATER_STATE = 1;
int STARTUP_SFX_STATE = 1;
int STARTUP_MUSIC_STATE = 1;
int STARTUP_CUSTOM_STATE = 1;
int STARTUP_ASM = 1;
int STARTUP_SVGA = 1;
int STARTUP_STOP = 0;
int STARTUP_AUTO = 0;
int STARTUP_CHECK = 0;
int STARTUP_CAPTURE = 0;
int STARTUP_TOMBOLA = 0;

#ifdef UNIX
#ifdef ALLEGRO_MACOSX
/*
 * We test with ALLEGRO_MACOSX and no custom OSX define
 * for this is still a variant of the UNIX port, only
 * some paths differ (because of the .app bundle layout)
 */
static char *DEFAULT_CFG_PATH = CONFIG_UNIX_CFG;
static char *DEFAULT_LOG_PATH = "";
static char *DEFAULT_SRV_PATH = "./liquidwar-server";
static char *DEFAULT_GEN_PATH = "./liquidwar-mapgen";
static char *DEFAULT_DAT_PATH = "../Resources/data/liquidwar.dat";
static char *DEFAULT_MAP_PATH = "../Resources/custom/map/";
static char *DEFAULT_TEX_PATH = "../Resources/custom/texture/";
static char *DEFAULT_MID_PATH = "../Resources/custom/music/";
#else
static char *DEFAULT_CFG_PATH = CONFIG_UNIX_CFG;
static char *DEFAULT_LOG_PATH = "";
static char *DEFAULT_SRV_PATH = CONFIG_UNIX_SRV;
static char *DEFAULT_GEN_PATH = CONFIG_UNIX_GEN;
static char *DEFAULT_DAT_PATH = CONFIG_UNIX_DAT;
static char *DEFAULT_MAP_PATH = CONFIG_UNIX_MAP;
static char *DEFAULT_TEX_PATH = CONFIG_UNIX_TEX;
static char *DEFAULT_MID_PATH = CONFIG_UNIX_MID;
#endif
#else
#ifdef WIN32
static char *DEFAULT_CFG_PATH = "data/lwwin.cfg";
static char *DEFAULT_LOG_PATH = "data/lwwin.log";
static char *DEFAULT_SRV_PATH = "lwwinsrv.exe";
static char *DEFAULT_GEN_PATH = "lwwinmap.exe";
#endif
#ifdef DOS
static char *DEFAULT_CFG_PATH = "data/lwdos.cfg";
static char *DEFAULT_LOG_PATH = "data/lwdos.log";
static char *DEFAULT_SRV_PATH = "";
static char *DEFAULT_GEN_PATH = "";
#endif
static char *DEFAULT_DAT_PATH = "data/lw.dat";
static char *DEFAULT_MAP_PATH = "custom/map/";
static char *DEFAULT_TEX_PATH = "custom/texture/";
static char *DEFAULT_MID_PATH = "custom/music/";
#endif

static char *DEFAULT_METASERVER = LW_SERVER_DEFAULT_METASERVER;

char STARTUP_CFG_PATH[LW_STARTUP_MAX_PATH_LENGTH];
char STARTUP_DAT_PATH[LW_STARTUP_MAX_PATH_LENGTH];
char STARTUP_LOG_PATH[LW_STARTUP_MAX_PATH_LENGTH];
char STARTUP_MAP_PATH[LW_STARTUP_MAX_PATH_LENGTH];
char STARTUP_TEX_PATH[LW_STARTUP_MAX_PATH_LENGTH];
char STARTUP_MID_PATH[LW_STARTUP_MAX_PATH_LENGTH];
char STARTUP_SRV_PATH[LW_STARTUP_MAX_PATH_LENGTH];
char STARTUP_GEN_PATH[LW_STARTUP_MAX_PATH_LENGTH];

char STARTUP_METASERVER[STARTUP_MAX_METASERVER_LENGTH];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
set_bigdata_size (void)
{
  int bigd;

  if (exist_argument_value (IDENT_MEM))
    {
      bigd = get_argument_int (IDENT_MEM);
      if (bigd < 1)
	bigd = 1;
      if (bigd > LW_STARTUP_MEM_MAX)
	bigd = LW_STARTUP_MEM_MAX;
      STARTUP_BIG_DATA = bigd;
    }
}

/*------------------------------------------------------------------*/
static void
set_joystick (void)
{
  if (exist_argument (IDENT_NOJOY))
    STARTUP_JOYSTICK_ON = 0;
}

/*------------------------------------------------------------------*/
static void
set_load_states (void)
{
  if (exist_argument (IDENT_NOBACK))
    STARTUP_BACK_STATE = 0;
  if (exist_argument (IDENT_NOTEXTURE))
    STARTUP_TEXTURE_STATE = 0;

  if (exist_argument (IDENT_NOWATER) || exist_argument (IDENT_NOSOUND))
    STARTUP_WATER_STATE = 0;
  if (exist_argument (IDENT_NOSFX) || exist_argument (IDENT_NOSOUND))
    STARTUP_SFX_STATE = 0;

  if (exist_argument (IDENT_NOSOUND)
      || (exist_argument (IDENT_NOSFX) && exist_argument (IDENT_NOWATER)))
    STARTUP_DIGI_CARD = DIGI_NONE;

  if (exist_argument (IDENT_NOMUSIC) || (exist_argument (IDENT_NOSOUND)))
    {
      STARTUP_MUSIC_STATE = 0;
      STARTUP_MIDI_CARD = MIDI_NONE;
    }

  if (exist_argument (IDENT_NOCUSTOM))
    STARTUP_CUSTOM_STATE = 0;
}

/*------------------------------------------------------------------*/
static void
set_graphic_possibilities (void)
{
  if (exist_argument (IDENT_NOSVGA))
    STARTUP_SVGA = 0;
}

/*------------------------------------------------------------------*/
static void
set_c_or_asm (void)
{
  if (exist_argument (IDENT_NOASM))
    STARTUP_ASM = 0;
}

/*------------------------------------------------------------------*/
static void
set_stop (void)
{
  if (exist_argument (IDENT_STOP))
    STARTUP_STOP = 1;
}

/*------------------------------------------------------------------*/
static void
set_auto_check (void)
{
  if (exist_argument (IDENT_AUTO))
    {
      STARTUP_AUTO = 1;
      STARTUP_CHECK = 0;
    }
  if (exist_argument (IDENT_CHECK))
    {
      STARTUP_AUTO = 0;
      STARTUP_CHECK = 1;
      STARTUP_STOP = 1;
    }
}

/*------------------------------------------------------------------*/
static void
set_safe (void)
{
  if (exist_argument (IDENT_NICE))
    {
      STARTUP_BIG_DATA = 4096;
      STARTUP_SVGA = 0;
    }

  if (exist_argument (IDENT_SAFE))
    {
      STARTUP_JOYSTICK_ON = 0;
      STARTUP_BIG_DATA = 1024;
      STARTUP_DIGI_CARD = DIGI_NONE;
      STARTUP_BACK_STATE = 0;
      STARTUP_TEXTURE_STATE = 0;
      STARTUP_WATER_STATE = 0;
      STARTUP_SFX_STATE = 0;
      STARTUP_MUSIC_STATE = 0;
      STARTUP_CUSTOM_STATE = 0;
      STARTUP_SVGA = 0;
    }
}

/*------------------------------------------------------------------*/
static void
correct_path_dir (char *path)
{
  int l;
  char c;

  l = strlen (path);
  if (l > 0)
    {
      c = path[l - 1];
      if (c == '/' || c == '\\')
	{
	  path[--l] = 0;
	}
    }

  fix_filename_case (path);
  fix_filename_slashes (path);
}

/*------------------------------------------------------------------*/
static void
correct_path_file (char *path)
{
  fix_filename_case (path);
  fix_filename_slashes (path);
}

/*------------------------------------------------------------------*/
static void
set_path (void)
{
  char home_path[512];
  char *home_env = NULL;

  if (exist_argument_value (IDENT_CFG))
    {
      LW_MACRO_STRCPY (STARTUP_CFG_PATH, get_argument_str (IDENT_CFG));
    }
  else
    {
#ifdef UNIX
      home_env = getenv ("HOME");
      if (home_env != NULL)
	{
	  LW_MACRO_SPRINTF1 (home_path, "%s/", home_env);
	}
      else
	{
	  home_env = "";
	  LW_MACRO_STRCPY (home_path, home_env);
	}
#else
      home_env = "";
      LW_MACRO_STRCPY (home_path, home_env);
#endif
      LW_MACRO_SPRINTF2 (STARTUP_CFG_PATH, "%s%s", home_path,
			 DEFAULT_CFG_PATH);
    }

  LW_MACRO_STRCPY (STARTUP_LOG_PATH, DEFAULT_LOG_PATH);

  if (exist_argument_value (IDENT_DAT))
    {
      LW_MACRO_STRCPY (STARTUP_DAT_PATH, get_argument_str (IDENT_DAT));
    }
  else
    {
      LW_MACRO_STRCPY (STARTUP_DAT_PATH, DEFAULT_DAT_PATH);
    }

  if (exist_argument_value (IDENT_TEX))
    {
      LW_MACRO_STRCPY (STARTUP_TEX_PATH, get_argument_str (IDENT_TEX));
    }
  else
    {
      LW_MACRO_STRCPY (STARTUP_TEX_PATH, DEFAULT_TEX_PATH);
    }

  if (exist_argument_value (IDENT_MAP))
    {
      LW_MACRO_STRCPY (STARTUP_MAP_PATH, get_argument_str (IDENT_MAP));
    }
  else
    {
      LW_MACRO_STRCPY (STARTUP_MAP_PATH, DEFAULT_MAP_PATH);
    }

  if (exist_argument_value (IDENT_MID))
    {
      LW_MACRO_STRCPY (STARTUP_MID_PATH, get_argument_str (IDENT_MID));
    }
  else
    {
      LW_MACRO_STRCPY (STARTUP_MID_PATH, DEFAULT_MID_PATH);
    }
  if (exist_argument_value (IDENT_SRV))
    {
      LW_MACRO_STRCPY (STARTUP_SRV_PATH, get_argument_str (IDENT_SRV));
    }
  else
    {
      LW_MACRO_STRCPY (STARTUP_SRV_PATH, DEFAULT_SRV_PATH);
    }
  if (exist_argument_value (IDENT_GEN))
    {
      LW_MACRO_STRCPY (STARTUP_GEN_PATH, get_argument_str (IDENT_GEN));
    }
  else
    {
      LW_MACRO_STRCPY (STARTUP_GEN_PATH, DEFAULT_GEN_PATH);
    }


  correct_path_file (STARTUP_CFG_PATH);
  correct_path_file (STARTUP_LOG_PATH);
  correct_path_file (STARTUP_DAT_PATH);
  correct_path_dir (STARTUP_TEX_PATH);
  correct_path_dir (STARTUP_MAP_PATH);
  correct_path_dir (STARTUP_MID_PATH);
  correct_path_file (STARTUP_SRV_PATH);
  correct_path_file (STARTUP_GEN_PATH);
}

/*------------------------------------------------------------------*/
static void
set_metaserver (void)
{
  if (exist_argument_value (IDENT_METASERVER))
    {
      LW_MACRO_STRCPY (STARTUP_METASERVER,
		       get_argument_str (IDENT_METASERVER));
    }
  else
    {
      LW_MACRO_STRCPY (STARTUP_METASERVER, DEFAULT_METASERVER);
    }
}

/*------------------------------------------------------------------*/
static void
set_misc (void)
{
  if (exist_argument (IDENT_CAPTURE))
    {
      STARTUP_CAPTURE = 1;
    }
  if (exist_argument (IDENT_TOMBOLA))
    {
      STARTUP_TOMBOLA = 1;
    }
}

/*------------------------------------------------------------------*/
static void
set_various_hacks (void)
{
#ifndef ASM
  /*
   * important to set it to 0, even if assembly code is not compiled
   * in, for we want the program to be able to tell others that it
   * can't do any assembly, in a network game for instance.
   */
  STARTUP_ASM = 0;
#endif
}

/*------------------------------------------------------------------*/
void
set_startup (void)
{
  set_joystick ();
  set_load_states ();
  set_graphic_possibilities ();
  set_c_or_asm ();
  set_path ();
  set_stop ();
  set_safe ();
  set_auto_check ();
  set_bigdata_size ();
  set_metaserver ();
  set_misc ();
  set_various_hacks ();
}
