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
/* nom           : gfxmode.c                                        */
/* contenu       : gestion des modes graphiques                     */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#include "base.h"
#include "disk.h"
#include "log.h"
#include "gfxmode.h"
#include "startup.h"
#include "init.h"
#include "mouse.h"
#include "exit.h"
#include "lang.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#ifdef DOS
int GFX_MODE_W[5] = { 320, 360, 640, 800, 1024 };
int GFX_MODE_H[5] = { 240, 240, 480, 600, 768 };
#endif

#ifdef WIN32
int GFX_MODE_W[5] = { 320, 512, 640, 800, 1024 };
int GFX_MODE_H[5] = { 240, 384, 480, 600, 768 };
#endif

#ifdef UNIX
#ifdef GP2X
int GFX_MODE_W[5] = { 320, 320, 320, 320, 320 };
int GFX_MODE_H[5] = { 240, 240, 240, 240, 240 };
#else
int GFX_MODE_W[5] = { 320, 512, 640, 800, 1024 };
int GFX_MODE_H[5] = { 240, 384, 480, 600, 768 };
#endif
#endif

#ifdef DOS
static int GFX_MODE_TYPE_FULLSCREEN[] = { GFX_MODEX,
  GFX_MODEX,
  GFX_VESA2L,
  GFX_VESA2L,
  GFX_VESA2L
};

static int GFX_MODE_TYPE_WINDOWED[] = { GFX_MODEX,
  GFX_MODEX,
  GFX_VESA2L,
  GFX_VESA2L,
  GFX_VESA2L
};
#endif

#ifdef WIN32
static int GFX_MODE_TYPE_FULLSCREEN[] = { GFX_DIRECTX,
  GFX_DIRECTX,
  GFX_DIRECTX,
  GFX_DIRECTX,
  GFX_DIRECTX
};

static int GFX_MODE_TYPE_WINDOWED[] = { GFX_DIRECTX_WIN,
  GFX_DIRECTX_WIN,
  GFX_DIRECTX_WIN,
  GFX_DIRECTX_WIN,
  GFX_DIRECTX_WIN
};
#endif

#ifdef UNIX
#ifdef GP2X
static int GFX_MODE_TYPE_FULLSCREEN[] = { GFX_GP2X,
  GFX_GP2X,
  GFX_GP2X,
  GFX_GP2X,
  GFX_GP2X
};

static int GFX_MODE_TYPE_WINDOWED[] = { GFX_GP2X,
  GFX_GP2X,
  GFX_GP2X,
  GFX_GP2X,
  GFX_GP2X
};
#else
static int GFX_MODE_TYPE_FULLSCREEN[] = { GFX_AUTODETECT_FULLSCREEN,
  GFX_AUTODETECT_FULLSCREEN,
  GFX_AUTODETECT_FULLSCREEN,
  GFX_AUTODETECT_FULLSCREEN,
  GFX_AUTODETECT_FULLSCREEN
};

static int GFX_MODE_TYPE_WINDOWED[] = { GFX_AUTODETECT_WINDOWED,
  GFX_AUTODETECT_WINDOWED,
  GFX_AUTODETECT_WINDOWED,
  GFX_AUTODETECT_WINDOWED,
  GFX_AUTODETECT_WINDOWED
};
#endif
#endif

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
need_to_change_mode (int res1, int res2, int flip)
{
  if (res1 == res2 && !flip)
    return 0;
  else
    return !0;
}

/*------------------------------------------------------------------*/
static int
try_one_mode (int res, int flip, int fullscreen)
{
  int ret;
  int w1, h1, w2, h2;

  w1 = GFX_MODE_W[res];
  h1 = GFX_MODE_H[res];
  w2 = GFX_MODE_W[res];
  h2 = flip ? GFX_MODE_H[res] * 2 : GFX_MODE_H[res];

  log_print_str ("Changing video mode to ");
  log_print_int (w1);
  log_print_str ("x");
  log_print_int (h1);
  if (fullscreen)
    log_print_str (", fullscreen");
  else
    log_print_str (", windowed");
  if (flip)
    log_print_str (", page flipping enabled");

  if (fullscreen)
    ret = set_gfx_mode (GFX_MODE_TYPE_FULLSCREEN[res], w1, h1, w2, h2);
  else
    ret = set_gfx_mode (GFX_MODE_TYPE_WINDOWED[res], w1, h1, w2, h2);

  if (!ret)
    {
      display_success_driver ((char *) gfx_driver->ascii_name);
    }
  else
    {
      display_success (0);
    }

  return ret;
}

/*------------------------------------------------------------------*/
int
set_resolution (int res, int flip, int fullscreen, int *flip_enabled)
{
  int ret = -1;
  int toggle;
  int i;

#ifdef GP2X
  fullscreen = 1;
#endif

  if (flip_enabled)
    {
      *flip_enabled = 0;
    }

  for (toggle = 0; toggle < 2; ++toggle, fullscreen = !fullscreen)
    {
      for (i = res; i >= 0 && ret; --i)
	{
	  if (flip)
	    {
	      ret = try_one_mode (i, 1, fullscreen);
	      if (flip_enabled && !ret)
		{
		  *flip_enabled = 1;
		}
	    }
	  if (ret)
	    {
	      ret = try_one_mode (i, 0, fullscreen);
	    }
	}
      for (i = res + 1; i < 5 && ret; ++i)
	{
	  if (flip)
	    {
	      ret = try_one_mode (i, 1, fullscreen);
	      if (flip_enabled && !ret)
		{
		  *flip_enabled = 1;
		}
	    }
	  if (ret)
	    {
	      ret = try_one_mode (i, 0, fullscreen);
	    }
	}
    }

  if (!ret)
    {
      set_palette (black_palette);

      if (SCREEN_W >= 640)
	{
	  font = BIG_FONT;
	}
      else
	{
	  font = SMALL_FONT;
	}

      lw_mouse_show ();

      set_window_title (lw_lang_string (LW_LANG_STRING_APPNAME));
    }
  else
    {
      my_exit (EXIT_CODE_GRAPHICS_FAIL);
    }

  return (ret);
}
