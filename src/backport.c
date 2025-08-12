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
/* name          : backport.c                                       */
/* content       : missing allegro 4 stuff                          */
/* last update   : Dec 24th 2018                                    */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <limits.h>

#include "backport.h"
#include "palette.h"
#include "macro.h"
#include "mutxgen.h"
#include "thrdgen.h"
#include "log.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#define _NB_TIMERS 16
#define _ITOA_BUF_SIZE 30

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

typedef struct
{
  int delay_ms;
  void (*proc) ();
} _backport_timer_data;

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

static int _allegro_errno = 0;
static unsigned char *_rgb_to_palette_map = NULL;  // 24-bit RGB to palette index map
static int _rgb_map_initialized = 0;
static int _dummy = 0;          // stupid dummy to get rid of unused param warning
static _backport_timer_data _backport_timers[_NB_TIMERS];
static LW_MUTEX_DATA _backport_timer_mutex = { NULL };
static char *_config_file = NULL;
static ALLEGRO_CONFIG *_config = NULL;

ALLEGRO_BITMAP *screen = NULL;
ALLEGRO_DISPLAY *allegro_display = NULL;
int SCREEN_W = 0;
int SCREEN_H = 0;
int VIRTUAL_H = 0;
ALLEGRO_FONT *font = NULL;
volatile int mouse_x = 0;
volatile int mouse_y = 0;
volatile int mouse_z = 0;
volatile int mouse_b = 0;
int *allegro_errno = &_allegro_errno;
volatile JOYSTICK_INFO joy[MAX_JOYSTICKS];
int num_joysticks = 0;
char empty_string[] = { 0, 0, 0, 0 };
volatile char key[KEY_MAX];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
scare_mouse ()
{
  /*
   * Using a higher level API now, looks like those disappeared.
   * Keeping them so that whatever workaround those calls were
   * achieving, it's easier to backport it afterwards.
   */
}

/*------------------------------------------------------------------*/
void
scare_mouse_area (int x, int y, int w, int h)
{
  /*
   * Using a higher level API now, looks like those disappeared.
   * Keeping them so that whatever workaround those calls were
   * achieving, it's easier to backport it afterwards.
   */
  _dummy = x;
  _dummy = y;
  _dummy = w;
  _dummy = h;
}

/*------------------------------------------------------------------*/
void
unscare_mouse ()
{
  /*
   * Using a higher level API now, looks like those disappeared.
   * Keeping them so that whatever workaround those calls were
   * achieving, it's easier to backport it afterwards.
   */
}

/*------------------------------------------------------------------*/
int
poll_mouse (void)
{
  // https://liballeg.org/stabledocs/en/alleg004.html#poll_mouse
  ALLEGRO_MOUSE_STATE mouse_state;

  memset (&mouse_state, 0, sizeof (mouse_state));
  al_get_mouse_state (&mouse_state);
  int num_axes = num_axes = al_get_mouse_num_axes ();
  if (num_axes >= 1)
    {
      mouse_x = al_get_mouse_state_axis (&mouse_state, 0);
    }
  if (num_axes >= 2)
    {
      mouse_y = al_get_mouse_state_axis (&mouse_state, 1);
    }
  if (num_axes >= 3)
    {
      mouse_z = al_get_mouse_state_axis (&mouse_state, 2);
    }
  mouse_b = mouse_state.buttons;

  return 0;
}

/*------------------------------------------------------------------*/
int
mouse_needs_poll (void)
{
  // https://liballeg.org/stabledocs/en/alleg004.html#mouse_needs_poll
  /*
   * Stupid answer which means "poll all the time" but this is (hopefully)
   * transition code, if this is a perf bottleneck it should be easy to
   * remove or change all that polling logic.
   */
  return 1;
}

/*------------------------------------------------------------------*/
void
position_mouse (int x, int y)
{
  // https://liballeg.org/stabledocs/en/alleg004.html#position_mouse
  al_set_mouse_xy (allegro_display, x, y);
  mouse_x = x;
  mouse_y = y;
}

/*------------------------------------------------------------------*/
void
set_mouse_sprite (ALLEGRO_BITMAP *sprite)
{
  // https://liballeg.org/stabledocs/en/alleg004.html#set_mouse_sprite
  if (sprite)
    {
      al_set_mouse_cursor (allegro_display, al_create_mouse_cursor (sprite, 0, 0));
    }
}

/*------------------------------------------------------------------*/
void
show_mouse (ALLEGRO_BITMAP *bmp)
{
  // https://liballeg.org/stabledocs/en/alleg004.html#show_mouse
  (void) bmp; // Ignore bitmap parameter since Allegro 5 handles cursor display differently
  al_show_mouse_cursor (allegro_display);
}

/*------------------------------------------------------------------*/
void
clear_to_color (ALLEGRO_BITMAP * bitmap, int color)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#clear_to_color
  al_set_target_bitmap (bitmap);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);
  al_clear_to_color (al_color);
}

/*------------------------------------------------------------------*/
void
putpixel (ALLEGRO_BITMAP * bitmap, int x, int y, int color)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#putpixel
  /*
   * Very likely, this is totally sub-efficient as we call set_target_bitmap
   * on *EVERY* putpixel call. Also, we emulate PALETTE code, which is ugly.
   * However, at least, it's safe transition code.
   * [TODO:ufoot] optimize that crap.
   */
  al_set_target_bitmap (bitmap);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);
  al_put_pixel (x, y, al_color);
}

/*------------------------------------------------------------------*/
static void
_init_rgb_to_palette_map (void)
{
  // Initialize the 24-bit RGB to palette index lookup table
  if (_rgb_map_initialized) 
    {
      return;
    }
  
  // Allocate 16MB for 24-bit RGB space (2^24 = 16,777,216 bytes)
  _rgb_to_palette_map = malloc(16777216);
  if (_rgb_to_palette_map == NULL)
    {
      return;  // Fall back to slow lookup if allocation fails
    }
  
  // Initialize all entries to 255 (invalid palette index)
  memset(_rgb_to_palette_map, 255, 16777216);
  
  // Fill the map with palette colors
  for (int i = 0; i < PALETTE_SIZE; i++)
    {
      float r, g, b, a;
      al_unmap_rgba_f (rgb_to_allegro_color(GLOBAL_PALETTE[i]), &r, &g, &b, &a);
      
      // Convert to 8-bit RGB
      unsigned char r8 = (unsigned char)(r * 255.0f);
      unsigned char g8 = (unsigned char)(g * 255.0f);
      unsigned char b8 = (unsigned char)(b * 255.0f);
      
      // Calculate 24-bit RGB index
      int rgb_index = (r8 << 16) | (g8 << 8) | b8;
      _rgb_to_palette_map[rgb_index] = (unsigned char)i;
    }
  
  _rgb_map_initialized = 1;
}

/*------------------------------------------------------------------*/
int
getpixel (ALLEGRO_BITMAP * bitmap, int x, int y)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#getpixel
  /*
   * Fast palette lookup using 24-bit RGB to palette index map.
   * Falls back to distance calculation if color not found in map.
   */
  al_set_target_bitmap (bitmap);
  ALLEGRO_COLOR pixel_color = al_get_pixel (bitmap, x, y);
  
  // Convert to RGB
  float r, g, b, a;
  al_unmap_rgba_f (pixel_color, &r, &g, &b, &a);
  unsigned char r8 = (unsigned char)(r * 255.0f);
  unsigned char g8 = (unsigned char)(g * 255.0f);
  unsigned char b8 = (unsigned char)(b * 255.0f);
  
  // Initialize lookup table if needed
  if (!_rgb_map_initialized)
    {
      _init_rgb_to_palette_map();
    }
  
  // Fast lookup if map is available
  if (_rgb_to_palette_map != NULL)
    {
      int rgb_index = (r8 << 16) | (g8 << 8) | b8;
      unsigned char palette_index = _rgb_to_palette_map[rgb_index];
      
      if (palette_index != 255)  // Found exact match
        {
          return (int)palette_index;
        }
    }
  
  // Fallback: find closest palette color by distance
  int best_match = 0;
  float best_distance = 9999999.0f;
  
  for (int i = 0; i < PALETTE_SIZE; i++)
    {
      float pr, pg, pb, pa;
      al_unmap_rgba_f (rgb_to_allegro_color(GLOBAL_PALETTE[i]), &pr, &pg, &pb, &pa);
      
      // Calculate distance (simple RGB distance)
      float dr = r - pr;
      float dg = g - pg;
      float db = b - pb;
      float distance = dr * dr + dg * dg + db * db;
      
      if (distance < best_distance)
        {
          best_distance = distance;
          best_match = i;
        }
    }
  
  return best_match;
}

/*------------------------------------------------------------------*/
void
rect (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2, int color)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#rect
  if (x2 < x1 || y2 < y1)
    {
      return;
    }
  al_set_target_bitmap (bitmap);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);
  al_draw_filled_rectangle (x1, y1, x2, y1 + 1, al_color);
  al_draw_filled_rectangle (x2, y1, x2 + 1, y2, al_color);
  al_draw_filled_rectangle (x1 + 1, y2, x2 + 1, y2 + 1, al_color);
  al_draw_filled_rectangle (x1, y1 + 1, x1 + 1, y2 + 1, al_color);
}

/*------------------------------------------------------------------*/
void
rectfill (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2, int color)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#rectfill
  al_set_target_bitmap (bitmap);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);
  // +1 on second coord because floating point vs integer
  al_draw_filled_rectangle (x1, y1, x2 + 1, y2 + 1, al_color);
}

/*------------------------------------------------------------------*/
void
rectfill_dotted (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2,
                 int fg, int bg)
{
  /*
   * This is not a genuine Allegro function, but used to backport the behavior
   * of filling zones with a pattern which is:
   * +--+--+--+--+
   * +fg+bg+fg+bg+
   * +--+--+--+--+
   * +bg+fg+bg+fg+
   * +--+--+--+--+
   * +fg+bg+fg+bg+
   * +--+--+--+--+
   * +bg+fg+bg+fg+
   * +--+--+--+--+
   * etc.
   * Rather than implemeting generic pattern handling, this is quicker to implement.
   * and suits our limited need.
   */
  al_set_target_bitmap (bitmap);

  if (fg < 0 || fg >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_fg;
  al_fg = rgb_to_allegro_color(GLOBAL_PALETTE[fg]);

  if (bg < 0 || bg >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_bg;
  al_bg = rgb_to_allegro_color(GLOBAL_PALETTE[bg]);

  ALLEGRO_COLOR al_color = al_bg;
  int x = 0;
  int y = 0;

  for (y = y1; y <= y2; y++)
    {
      for (x = x1; x <= x2; x++)
        {
          al_color = ((x + y) % 2) != 0 ? al_fg : al_bg;
          al_put_pixel (x, y, al_color);
        }
    }
}

/*------------------------------------------------------------------*/
void
vline (ALLEGRO_BITMAP * bitmap, int x, int y1, int y2, int color)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#vline
  if (y2 < y1)
    {
      int y = y2;
      y2 = y1;
      y1 = y;
    }
  al_set_target_bitmap (bitmap);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);
  al_draw_filled_rectangle (x, y1, x + 1, y2 + 1, al_color);
}

/*------------------------------------------------------------------*/
void
hline (ALLEGRO_BITMAP * bitmap, int x1, int y, int x2, int color)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#hline
  if (x2 < x1)
    {
      int x = x2;
      x2 = x1;
      x1 = x;
    }
  al_set_target_bitmap (bitmap);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);
  al_draw_filled_rectangle (x1, y, x2 + 1, y + 1, al_color);
}

void
line (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2, int color)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#line
  if (x1 == x2)
    {
      vline (bitmap, x1, y1, y2, color);
      return;
    }
  if (y1 == y2)
    {
      hline (bitmap, x1, y1, x2, color);
      return;
    }

  al_set_target_bitmap (bitmap);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);

  /* ugliest line drawing ever, but we don't care, this is never called */
  int w = x2 - x1;
  int h = y2 - y1;
  int d = ABS (w) + ABS (h);
  int i = 0;
  for (i = 0; i <= d; i++)
    {
      int x = ((i * x2) + ((d - i) * x1)) / d;
      int y = ((i * y2) + ((d - i) * y1)) / d;
      al_put_pixel (x, y, al_color);
    }
}

/*------------------------------------------------------------------*/
int
usetc (char *s, int c)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#usetc
  ALLEGRO_USTR *us = NULL;
  int l = 0;
  const char *s2 = NULL;

  us = al_ustr_new (s);
  al_ustr_set_chr (us, 0, c);
  s2 = al_cstr (us);
  l = strlen (s);
  LW_MACRO_STRNCPY (s, s2, l + 1);
  al_ustr_free (us);

  return l;
}

/*------------------------------------------------------------------*/
int
ugetc (const char *s)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#ugetc
  ALLEGRO_USTR *us = NULL;
  int c = 0;

  us = al_ustr_new (s);
  c = al_ustr_get (us, 0);
  al_ustr_free (us);

  return c;
}

/*------------------------------------------------------------------*/
int
uwidth (const char *s)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#uwidth
  ALLEGRO_USTR *us = NULL;
  int pos = 0;
  bool found = false;

  us = al_ustr_new (s);
  found = al_ustr_next (us, &pos);
  al_ustr_free (us);

  if (!found)
    {
      return 0;
    }
  return pos;
}

/*------------------------------------------------------------------*/
int
ustrlen (const char *s)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#ustrlen
  ALLEGRO_USTR *us = NULL;
  int l = 0;

  us = al_ustr_new (s);
  l = al_ustr_length (us);
  al_ustr_free (us);

  return l;
}

/*------------------------------------------------------------------*/
int
uisspace (int c)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#uisspace
  /*
   * Super naive impl, should do the job for the LW use-case.
   */
  return c <= 32;
}

/*------------------------------------------------------------------*/
int
ucwidth (int c)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#ucwidth
  _dummy = c;
  return 1;
}

/*------------------------------------------------------------------*/
int
ugetat (char *s, int index)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#ugetat
  ALLEGRO_USTR *us = NULL;
  int c = 0;

  us = al_ustr_new (s);
  c = al_ustr_get (us, index);
  al_ustr_free (us);

  return c;
}

/*------------------------------------------------------------------*/
int
usetat (char *s, int index, int c, int max_size)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#usetat
  /*
   * Altered the protoype of that one, not giving max_size here
   * is just totally super-dangerous IMHO.
   */
  ALLEGRO_USTR *us = NULL;
  int l = 0;
  int max_len = max_size - 1;

  l = strlen (s);
  if (index < 0)
    {
      index = l - index;
    }
  if (index >= max_len || index < 0)
    {
      return 0;
    }

  us = al_ustr_new (s);
  if (c == 0)
    {
      al_ustr_truncate (us, index);
    }
  else
    {
      al_ustr_remove_chr (us, index);
      al_ustr_insert_chr (us, index, c);
    }
  LW_MACRO_STRNCPY (s, al_cstr (us), max_size);
  al_ustr_free (us);

  return (strlen (s) - l);
}

/*------------------------------------------------------------------*/
int
uinsert (char *s, int index, int c, int max_size)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#uinsert
  /*
   * Altered the protoype of that one, not giving max_size here
   * is just totally super-dangerous IMHO.
   */
  ALLEGRO_USTR *us = NULL;
  int l = 0;
  int max_len = max_size - 1;

  l = strlen (s);
  if (index < 0)
    {
      index = l - index;
    }
  if (index >= max_len || index < 0)
    {
      return 0;
    }

  us = al_ustr_new (s);
  if (c == 0)
    {
      al_ustr_truncate (us, index);
    }
  else
    {
      al_ustr_insert_chr (us, index, c);
    }
  LW_MACRO_STRNCPY (s, al_cstr (us), max_size);
  al_ustr_free (us);

  return (strlen (s) - l);
}

/*------------------------------------------------------------------*/
int
uremove (char *s, int index)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#uremove
  ALLEGRO_USTR *us = NULL;
  int l = 0;

  l = strlen (s);
  if (index < 0)
    {
      index = l - index;
    }
  if (index < 0)
    {
      return 0;
    }

  us = al_ustr_new (s);
  al_ustr_remove_chr (us, index);
  LW_MACRO_STRNCPY (s, al_cstr (us), l);
  al_ustr_free (us);

  return (strlen (s) - l);
}

/*------------------------------------------------------------------*/
int
uisok (int c)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#uiosok
  /*
   * [FIXME:ufoot] right now does nothing real, but game handles
   * UTF-8 very poorly, TBH.
   */
  return ((c >= ' ') && (c <= 127));
}

/*------------------------------------------------------------------*/
int
utolower (int c)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#utolower
  /*
   * [FIXME:ufoot] right now does nothing real, but game handles
   * UTF-8 very poorly, TBH.
   */
  if ((c >= 'A') && (c <= 'Z'))
    {
      c += 'a' - 'A';
    }
  return c;
}

/*------------------------------------------------------------------*/
char *
ustrdup (const char *src)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#ustrdup
  /*
   * [TODO:ufoot] find out why this was redefined in the first place.
   */
  return strdup (src);
}

/*------------------------------------------------------------------*/
int
ugetx (char **s)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#ugetx
  return ugetxc ((const char **) s);
}

/*------------------------------------------------------------------*/
int
ugetxc (const char **s)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#ugetxc
  int c = *((unsigned char *) ((*s)++));
  int n, t;

  if (c & 0x80)
    {
      n = 1;
      while (c & (0x80 >> n))
        n++;

      c &= (1 << (8 - n)) - 1;

      while (--n > 0)
        {
          t = *((unsigned char *) ((*s)++));

          if ((!(t & 0x80)) || (t & 0x40))
            {
              (*s)--;
              return '^';
            }

          c = (c << 6) | (t & 0x3F);
        }
    }

  return c;
}

/*------------------------------------------------------------------*/
char *
ustrtok_r (char *s, AL_CONST char *set, char **last)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#ustrtok_r
  char *prev_str, *tok;
  AL_CONST char *setp;
  int c, sc;

  ALLEGRO_ASSERT (last);

  if (!s)
    {
      s = *last;

      if (!s)
        return NULL;
    }

skip_leading_delimiters:

  prev_str = s;
  c = ugetx (&s);

  setp = set;

  while ((sc = ugetxc (&setp)) != 0)
    {
      if (c == sc)
        goto skip_leading_delimiters;
    }

  if (!c)
    {
      *last = NULL;
      return NULL;
    }

  tok = prev_str;

  for (;;)
    {
      prev_str = s;
      c = ugetx (&s);

      setp = set;

      do
        {
          sc = ugetxc (&setp);
          if (sc == c)
            {
              if (!c)
                {
                  *last = NULL;
                  return tok;
                }
              else
                {
                  int l = 0;
                  l = strlen (prev_str);
                  s += usetat (prev_str, 0, 0, l + 1);
                  *last = s;
                  return tok;
                }
            }
        }
      while (sc);
    }
}

/*------------------------------------------------------------------*/
char *
uconvert_ascii (const char *s, char buf[], int size)
{
  // https://liballeg.org/stabledocs/en/alleg002.html#uconvert_ascii
  /*
   * Slightly altered prototype to make sure we use explicit size
   */
  LW_MACRO_STRNCPY (buf, s, size);
  return (char *) buf;
}

/*------------------------------------------------------------------*/
int
text_length (const ALLEGRO_FONT * f, const char *s)
{
  // https://liballeg.org/stabledocs/en/alleg018.html#text_length
  return al_get_text_width (f, s);
}

/*------------------------------------------------------------------*/
int
text_height (const ALLEGRO_FONT * f)
{
  // https://liballeg.org/stabledocs/en/alleg018.html#text_height
  return al_get_font_line_height (f);
}

/*------------------------------------------------------------------*/
void
textout_ex (ALLEGRO_BITMAP * bmp, const ALLEGRO_FONT * f, const char *s,
            int x, int y, int color, int bg)
{
  // https://liballeg.org/stabledocs/en/alleg018.html#textout_ex
  int w = 0;
  int h = 0;

  w = text_length (f, s);
  h = text_height (f);
  rectfill (bmp, x, y, x + w - 1, y + h - 1, bg);

  al_set_target_bitmap (bmp);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);

  al_draw_text (f, al_color, x, y, 0, s);
}

/*------------------------------------------------------------------*/
void
rest_callback (unsigned int time, void (*callback) (void))
{
  // https://liballeg.org/stabledocs/en/alleg005.html#rest_callback
  double delay;

  delay = ((double) time) / 1000.0;
  if (callback == NULL)
    {
      al_rest (delay);
    }
  else
    {
      unsigned int i;

      for (i = 0; i < time; i++)
        {
          al_rest (0.001);
          callback ();
        }
    }
}

/*------------------------------------------------------------------*/
void
rest (unsigned int time)
{
  // https://liballeg.org/stabledocs/en/alleg005.html#rest
  rest_callback (time, NULL);
}

/*------------------------------------------------------------------*/
int
install_timer ()
{
  // https://liballeg.org/stabledocs/en/alleg005.html#install_timer
  memset (_backport_timers, 0, sizeof (_backport_timers));
  memset (&_backport_timer_mutex, 0, sizeof (_backport_timer_mutex));

  return (lw_mutex_init (&_backport_timer_mutex) == 0) ? 0 : -1;
}

/*------------------------------------------------------------------*/
void
remove_timer ()
{
  // https://liballeg.org/stabledocs/en/alleg005.html#remove_timer
  memset (_backport_timers, 0, sizeof (_backport_timers));
  memset (&_backport_timer_mutex, 0, sizeof (_backport_timer_mutex));
}


/*------------------------------------------------------------------*/
void
_backport_timer_callback (void *ptr)
{
  _backport_timer_data *data = NULL;

  data = (_backport_timer_data *) ptr;
  while (data->proc)
    {
      data->proc ();
      rest (data->delay_ms);
    }
}

/*------------------------------------------------------------------*/
int
install_int (void (*proc) (), int speed)
{
  // https://liballeg.org/stabledocs/en/alleg005.html#install_int
  int i = 0;
  bool found = false;
  int ret = 0;

  if (speed <= 0)
    {
      speed = 1;
    }

  lw_mutex_lock (&_backport_timer_mutex);
  for (i = 0; i < _NB_TIMERS && !found; i++)
    {
      if (_backport_timers[i].proc == proc)
        {
          _backport_timers[i].delay_ms = speed;
          found = true;
        }
    }
  for (i = 0; i < _NB_TIMERS && !found; i++)
    {
      if (_backport_timers[i].proc == NULL)
        {
          _backport_timers[i].proc = proc;
          _backport_timers[i].delay_ms = speed;
          if (!lw_thread_start
              (_backport_timer_callback, &(_backport_timers[i])))
            {
              ret = -2;
            }
          found = true;
        }
    }
  lw_mutex_unlock (&_backport_timer_mutex);

  return found ? ret : -1;
}

/*------------------------------------------------------------------*/
int
install_int_ex (void (*proc) (), int speed)
{
  // https://liballeg.org/stabledocs/en/alleg005.html#install_int_ex
  // This is a compatibility stub - for full implementation, files should
  // be migrated to use modern Allegro 5 timer APIs like al_get_time()
  return install_int (proc, speed);
}

/*------------------------------------------------------------------*/
void
remove_int (void (*proc) ())
{
  // https://liballeg.org/stabledocs/en/alleg005.html#remove_int
  int i = 0;
  bool found = false;

  lw_mutex_lock (&_backport_timer_mutex);
  for (i = 0; i < _NB_TIMERS && !found; i++)
    {
      if (_backport_timers[i].proc == proc)
        {
          _backport_timers[i].proc = NULL;
          found = true;
        }
    }
  lw_mutex_unlock (&_backport_timer_mutex);
}

/*------------------------------------------------------------------*/
void
draw_sprite (ALLEGRO_BITMAP * bmp, ALLEGRO_BITMAP * sprite, int x, int y)
{
  // https://liballeg.org/stabledocs/en/alleg014.html#draw_sprite
  al_set_target_bitmap (bmp);
  al_draw_bitmap (sprite, x, y, 0);
}

/*------------------------------------------------------------------*/
void
blit (ALLEGRO_BITMAP * source, ALLEGRO_BITMAP * dest,
      int source_x, int source_y,
      int dest_x, int dest_y, int dest_width, int dest_height)
{
  // https://liballeg.org/stabledocs/en/alleg014.html#blit
  al_set_target_bitmap (dest);
  int sw = MAX (MIN (al_get_bitmap_width (source) - source_x, dest_width), 0);
  int sh =
    MAX (MIN (al_get_bitmap_height (source) - source_y, dest_height), 0);
  al_draw_bitmap_region (source, source_x, source_y, sw, sh, dest_x, dest_y,
                         0);
}

/*------------------------------------------------------------------*/
void
stretch_blit (ALLEGRO_BITMAP * source, ALLEGRO_BITMAP * dest,
              int source_x, int source_y, int source_width, int source_height,
              int dest_x, int dest_y, int dest_width, int dest_height)
{
  // https://liballeg.org/stabledocs/en/alleg014.html#stretch_blit
  al_set_target_bitmap (dest);
  al_draw_scaled_bitmap (source, source_x, source_y, source_width,
                         source_height, dest_x, dest_y, dest_width,
                         dest_height, 0);
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *
load_bitmap (const char *filename, PALETTE pal)
{
  // https://liballeg.org/stabledocs/en/alleg014.html#load_bitmap
  ALLEGRO_BITMAP *bmp;
  (void) pal; // Ignore palette parameter since Allegro 5 handles palettes differently
  
  bmp = al_load_bitmap (filename);
  return bmp;
}

/*------------------------------------------------------------------*/
void
set_volume (int digi_volume, int midi_volume)
{
  // https://liballeg.org/stabledocs/en/alleg011.html#set_volume
  (void) digi_volume; // Digital volume not used in this implementation
  // In Allegro 5, we would set the mixer gain, but for now just ignore
  // since MIDI playback uses ALLEGRO_SAMPLE which has its own volume control
  (void) midi_volume; // MIDI volume handled per-sample in play_midi
}

/*------------------------------------------------------------------*/
int
play_midi (ALLEGRO_SAMPLE *midi, int loop)
{
  // https://liballeg.org/stabledocs/en/alleg011.html#play_midi
  static ALLEGRO_SAMPLE_INSTANCE *current_music = NULL;
  
  // Stop any currently playing music
  if (current_music)
    {
      al_stop_sample_instance (current_music);
      al_destroy_sample_instance (current_music);
      current_music = NULL;
    }
  
  if (midi)
    {
      current_music = al_create_sample_instance (midi);
      if (current_music)
        {
          al_set_sample_instance_playmode (current_music, 
            loop ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE);
          al_attach_sample_instance_to_mixer (current_music, al_get_default_mixer ());
          return al_play_sample_instance (current_music) ? 1 : 0;
        }
    }
  
  return midi ? 0 : 1; // Return 1 for stop (NULL), 0 for failed play
}

/*------------------------------------------------------------------*/
int
play_sample (ALLEGRO_SAMPLE *spl, int vol, int pan, int freq, int loop)
{
  // https://liballeg.org/stabledocs/en/alleg011.html#play_sample
  // Allegro 4: play_sample(sample, vol, pan, freq, loop)
  // vol: 0-255, pan: 0-255 (128=center), freq: 1000=normal, loop: 0=once, 1=loop
  
  if (!spl) {
    return -1;
  }
  
  ALLEGRO_SAMPLE_INSTANCE *instance = al_create_sample_instance(spl);
  if (!instance) {
    return -1;
  }
  
  // Set volume (0-255 -> 0.0-1.0)
  float al_vol = (float)vol / 255.0f;
  al_set_sample_instance_gain(instance, al_vol);
  
  // Set pan (0-255 -> -1.0 to 1.0, where 128 = 0.0 center)
  float al_pan = ((float)pan - 128.0f) / 128.0f;
  al_set_sample_instance_pan(instance, al_pan);
  
  // Set frequency/speed (1000 = normal speed)
  float speed = (float)freq / 1000.0f;
  al_set_sample_instance_speed(instance, speed);
  
  // Set looping
  ALLEGRO_PLAYMODE playmode = loop ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE;
  al_set_sample_instance_playmode(instance, playmode);
  
  // Attach to default mixer and play
  al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());
  
  if (al_play_sample_instance(instance)) {
    // In Allegro 4, play_sample returned the voice number (positive)
    // We'll return 1 to indicate success (can't return actual voice)
    return 1;
  } else {
    al_destroy_sample_instance(instance);
    return -1;
  }
}

/*------------------------------------------------------------------*/
void
acquire_bitmap (ALLEGRO_BITMAP * bmp)
{
  // https://liballeg.org/stabledocs/en/alleg009.html#acquire_bitmap
  al_lock_bitmap (bmp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_PIXEL_FORMAT_ANY);
}

/*------------------------------------------------------------------*/
void
acquire_screen ()
{
  // https://liballeg.org/stabledocs/en/alleg009.html#acquire_screen
  acquire_bitmap (screen);
}

/*------------------------------------------------------------------*/
void
release_bitmap (ALLEGRO_BITMAP * bmp)
{
  // https://liballeg.org/stabledocs/en/alleg009.html#release_bitmap
  al_unlock_bitmap (bmp);
}

/*------------------------------------------------------------------*/
void
release_screen ()
{
  // https://liballeg.org/stabledocs/en/alleg009.html#release_screen
  release_bitmap (screen);
}

/*------------------------------------------------------------------*/
void
set_clip_rect (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2)
{
  // https://liballeg.org/stabledocs/en/alleg009.html#set_clip_rect
  al_set_target_bitmap (bitmap);
  if (x1 < 0)
    {
      x1 = 0;
    }
  if (y1 < 0)
    {
      y1 = 0;
    }
  if (x1 > al_get_bitmap_width (bitmap))
    {
      x1 = al_get_bitmap_width (bitmap);
    }
  if (y1 > al_get_bitmap_height (bitmap))
    {
      y1 = al_get_bitmap_height (bitmap);
    }
  if (x2 > al_get_bitmap_width (bitmap))
    {
      x2 = al_get_bitmap_width (bitmap);
    }
  if (y2 > al_get_bitmap_height (bitmap))
    {
      y2 = al_get_bitmap_height (bitmap);
    }
  int w = x2 - x1 + 1;
  int h = y2 - y1 + 1;
  if (w < 0)
    {
      w = 0;
    }
  if (h < 0)
    {
      h = 0;
    }
  al_set_clipping_rectangle (x1, y1, w, h);
}

/*------------------------------------------------------------------*/
int
install_joystick (int type)
{
  // https://liballeg.org/stabledocs/en/alleg007.html#install_joystick
  
  (void) type; // Ignore type parameter since Allegro 5 auto-detects
  
  if (!al_install_joystick ())
    {
      return -1;
    }
  
  num_joysticks = al_get_num_joysticks ();
  return 0;
}

/*------------------------------------------------------------------*/
int
poll_joystick ()
{
  // https://liballeg.org/stabledocs/en/alleg007.html#poll_joystick

  memset ((void *) &joy[0], 0, sizeof (joy));
  num_joysticks = al_get_num_joysticks ();
  int j = 0;
  for (j = 0; j < num_joysticks && j < MAX_JOYSTICKS; j++)
    {
      ALLEGRO_JOYSTICK *joystick = NULL;
      joystick = al_get_joystick (j);
      if (joystick == NULL)
        {
          continue;
        }
      joy[j].name = al_get_joystick_name (joystick);
      ALLEGRO_JOYSTICK_STATE joystick_state;
      memset (&joystick_state, 0, sizeof (joystick_state));
      al_get_joystick_state (joystick, &joystick_state);
      int num_sticks = al_get_joystick_num_sticks (joystick);
      int s = 0;
      for (s = 0; s < num_sticks && s < MAX_JOYSTICK_STICKS; s++)
        {
          joy[j].stick[s].name = al_get_joystick_stick_name (joystick, s);
          int num_axes = al_get_joystick_num_axes (joystick, s);
          int a = 0;
          for (a = 0; a < num_axes && a < MAX_JOYSTICK_AXIS; a++)
            {
              joy[j].stick[s].axis[a].name =
                al_get_joystick_axis_name (joystick, s, a);
              float p = joystick_state.stick[s].axis[a];
              joy[j].stick[s].axis[a].pos = (int) (p * 128.0);
              if (p < 0)
                {
                  joy[j].stick[s].axis[a].d1 = 1;
                }
              else if (p > 0)
                {
                  joy[j].stick[s].axis[a].d2 = 1;
                }
            }
          int num_buttons = al_get_joystick_num_buttons (joystick);
          int b = 0;
          for (b = 0; b < num_buttons && b < MAX_JOYSTICK_BUTTONS; b++)
            {
              joy[j].button[b].name =
                al_get_joystick_button_name (joystick, b);
              joy[j].button[b].b = joystick_state.button[b] ? 1 : 0;
            }
        }
    }

  return 0;
}

/*------------------------------------------------------------------*/
int
keypressed ()
{
  // https://liballeg.org/stabledocs/en/alleg006.html#keypressed
  /*
   * [FIXME:ufoot] implement this.
   */
  return 0;
}

/*------------------------------------------------------------------*/
int
readkey ()
{
  // https://liballeg.org/stabledocs/en/alleg006.html#readkey
  /*
   * [FIXME:ufoot] implement this.
   */
  return 0;
}

/*------------------------------------------------------------------*/
int
ureadkey (int *scancode)
{
  // https://liballeg.org/stabledocs/en/alleg006.html#ureadkey
  /*
   * [FIXME:ufoot] implement this.
   */
  if (scancode != NULL)
    {
      (*scancode) = 0;
    }
  return 0;
}

/*------------------------------------------------------------------*/
void
clear_keybuf ()
{
  // https://liballeg.org/stabledocs/en/alleg006.html#clear_keybuf
  /*
   * [FIXME:ufoot] implement this.
   */
}

/*------------------------------------------------------------------*/
void
simulate_keypress (int key)
{
  // https://liballeg.org/stabledocs/en/alleg006.html#simulate_keypress
  /*
   * [FIXME:ufoot] implement this.
   */
  _dummy = key;
}

/*------------------------------------------------------------------*/
int
scancode_to_ascii (int scancode)
{
  // https://liballeg.org/stabledocs/en/alleg006.html#scancode_to_ascii
  /*
   * Again, most unreliable and primitive impl, but should be enough
   * for the limited usage we make of that function.
   */
  int ascii = 0;
  char *name = ustrdup (al_keycode_to_name (scancode));

  // naive tolower
  char *pos = NULL;
  for (pos = name; *pos; pos++)
    {
      char c = *pos;
      if (c >= 'A' && c <= 'Z')
        {
          (*pos) = c + 'a' - 'A';
        }
    }
  // normally it's "space" but accept " " too.
  if (strcmp (name, " ") == 0 || strcmp (name, "space") == 0)
    {
      ascii = 0;
    }
  else if (strlen (name) == 1 && name[0] >= 'a' && name[0] <= 'z')
    {
      // standard letter, the only thing we really need to handle
      ascii = name[0];
    }
  free (name);

  return ascii;
}

/*------------------------------------------------------------------*/
void
remove_keyboard (void)
{
  // In Allegro 5, we don't need to explicitly remove keyboard
  // This function is provided for compatibility and does nothing
}

/*------------------------------------------------------------------*/
void set_config_file(char *filename) {
  _config_file = filename;
  _config = al_load_config_file(filename);
  if (_config != NULL) {
    log_println_str("loaded config");
  } else {
    log_println_str("could not load config");
  }
}

/*------------------------------------------------------------------*/
int get_config_int(const char *section, const char *key, int def) {
  const char *value = get_config_string(section, key, "");
  if (value == NULL || strlen(value) == 0) {
    return def;
  }
  return atoi(value);
}

/*------------------------------------------------------------------*/
const char *get_config_string(const char *section, const char *key, const char *def) {
  if (_config == NULL) {
    return def;
  }
  const char *value = al_get_config_value(_config, section, key);
  if (value == NULL) {
    return def;
  }
  return value;
}

/*------------------------------------------------------------------*/
void set_config_int(const char *section, const char *key, int value) {
  // The following leaks memory as this buf is never freed. However, in the
  // program, set_config_int is really rarely used, only at the end of the
  // program, so before it causes a critical visible leak, there is a long way.
  char *buf = malloc(_ITOA_BUF_SIZE);
  memset(buf, 0, _ITOA_BUF_SIZE);
  snprintf(buf, _ITOA_BUF_SIZE, "%d", value);
  set_config_string(section, key, buf);
}

/*------------------------------------------------------------------*/
void set_config_string(const char *section, const char *key, const char *value) {
  if (_config == NULL) {
    return;
  }
  al_set_config_value(_config, section, key, value);
}

/*------------------------------------------------------------------*/
void flush_config_file() {
  if (_config == NULL) {
    return ;
  }
  if (al_save_config_file(_config_file, _config)) {
    log_println_str("saved config");
  } else {
    log_println_str("could not save config");
  }
}

/*------------------------------------------------------------------*/
int exists(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (f != NULL) {
    fclose(f);
    return 1;
  }
  return 0;
}

/*------------------------------------------------------------------*/
// Graphics mode compatibility functions

// Dummy graphics driver for compatibility
static char dummy_driver_name[] = "Allegro 5 Display";
static LW_GFX_DRIVER_INFO dummy_gfx_driver = { dummy_driver_name };

LW_GFX_DRIVER_INFO *gfx_driver = &dummy_gfx_driver;

// Black palette for compatibility
static PALETTE black_palette_data;
void *black_palette = &black_palette_data;

/*------------------------------------------------------------------*/
int set_gfx_mode(int card, int w, int h, int v_w, int v_h) {
  // In Allegro 5, graphics mode setup is handled differently
  // This is a compatibility stub that returns success
  // Real display creation should be handled at a higher level
  (void)card;
  (void)v_w; // Virtual width not used in current implementation
  
  // Set the global screen dimensions for compatibility
  SCREEN_W = w;
  SCREEN_H = h;
  VIRTUAL_H = v_h ? v_h : h * 2; // Set virtual height for page flipping
  
  return 0; // Success in Allegro 4 convention
}

/*------------------------------------------------------------------*/
void set_palette(void *palette) {
  // Update the global palette with the provided palette
  // The palette parameter is expected to be PALETTE (RGB array)
  if (palette) {
    PALETTE *pal = (PALETTE *)palette;
    for (int i = 0; i < PALETTE_SIZE; i++) {
      GLOBAL_PALETTE[i] = (*pal)[i];
    }
  }
}

/*------------------------------------------------------------------*/
void set_window_title(const char *title) {
  // Get the current display and set its title
  ALLEGRO_DISPLAY *display = al_get_current_display();
  if (display && title) {
    al_set_window_title(display, title);
  }
}

/*------------------------------------------------------------------*/
// Global variables for Allegro 4 compatibility
char *allegro_id = "Allegro 5.x";

// Driver info structures (stubs for compatibility)
static LW_GFX_DRIVER_INFO timer_driver_info = {"Allegro 5 Timer"};
static LW_GFX_DRIVER_INFO keyboard_driver_info = {"Allegro 5 Keyboard"};
static LW_GFX_DRIVER_INFO mouse_driver_info = {"Allegro 5 Mouse"};
static LW_GFX_DRIVER_INFO digi_driver_info = {"Allegro 5 Audio"};
static LW_GFX_DRIVER_INFO midi_driver_info = {"Allegro 5 MIDI"};
static LW_GFX_DRIVER_INFO joystick_driver_info = {"Allegro 5 Joystick"};

LW_GFX_DRIVER_INFO *timer_driver = &timer_driver_info;
LW_GFX_DRIVER_INFO *keyboard_driver = &keyboard_driver_info;
LW_GFX_DRIVER_INFO *mouse_driver = &mouse_driver_info;
LW_GFX_DRIVER_INFO *digi_driver = &digi_driver_info;
LW_GFX_DRIVER_INFO *midi_driver = &midi_driver_info;
LW_GFX_DRIVER_INFO *joystick_driver = &joystick_driver_info;

/*------------------------------------------------------------------*/
int install_allegro(int system_id, int *errno_ptr, int (*atexit_ptr)()) {
  // Allegro 4 install_allegro function compatibility
  (void)system_id;
  (void)errno_ptr;
  (void)atexit_ptr;
  
  return allegro_init();
}

/*------------------------------------------------------------------*/
int allegro_init(void) {
  // Initialize Allegro 5 core system
  if (!al_install_system(ALLEGRO_VERSION_INT, NULL)) {
    return -1; // Allegro 4 returns non-zero on failure
  }

  // Initialize subsystems that the game expects to be available
  // These correspond to the various install_* functions in Allegro 4
  
  // Keyboard support
  if (!al_install_keyboard()) {
    return -1;
  }

  // Mouse support  
  if (!al_install_mouse()) {
    return -1;
  }

  // Joystick support (optional, will be initialized by install_joystick() when needed)
  // al_install_joystick(); - removed, now handled by install_joystick() function

  // Audio support (optional)
  al_install_audio();
  al_init_acodec_addon();

  // Image loading support
  if (!al_init_image_addon()) {
    return -1;
  }

  // Font support
  if (!al_init_font_addon()) {
    return -1;
  }

  // Primitives for drawing operations
  if (!al_init_primitives_addon()) {
    return -1;
  }

  return 0; // Success in Allegro 4 style (0 = success)
}

/*------------------------------------------------------------------*/
void set_uformat(int format) {
  // Allegro 5 doesn't require setting unicode format like Allegro 4
  // This is a stub for compatibility
  (void)format; // Suppress unused parameter warning
}

/*------------------------------------------------------------------*/
void set_color_depth(int depth) {
  // In Allegro 5, color depth is set when creating the display
  // This is stored for later use in graphics mode setup
  (void)depth; // Suppress unused parameter warning
}

/*------------------------------------------------------------------*/
void set_color_conversion(int flags) {
  // Allegro 5 handles color conversion automatically
  // This is a stub for compatibility
  (void)flags; // Suppress unused parameter warning
}

/*------------------------------------------------------------------*/
int install_keyboard(void) {
  // This is called from init.c but keyboard is already initialized in allegro_init
  // Return success (0 in Allegro 4 style)
  return al_is_keyboard_installed() ? 0 : -1;
}

/*------------------------------------------------------------------*/
int install_mouse(void) {
  // This is called from init.c but mouse is already initialized in allegro_init
  // Return success (not -1 in Allegro 4 style)
  return al_is_mouse_installed() ? 1 : -1;
}

/*------------------------------------------------------------------*/
int install_sound(int digi_card, int midi_card, const char *cfg_path) {
  // Audio is already initialized in allegro_init
  // These parameters are for Allegro 4 compatibility
  (void)digi_card;
  (void)midi_card;
  (void)cfg_path;
  
  return al_is_audio_installed() ? 0 : -1;
}

/*------------------------------------------------------------------*/
void set_close_button_callback(void (*callback)(void)) {
  // In Allegro 5, this would be handled through event handling
  // For now, store the callback for potential future use
  (void)callback; // Stub for compatibility
}

/*------------------------------------------------------------------*/
void get_palette (PALETTE pal) {
  // In Allegro 4, this would get the current hardware palette
  // In Allegro 5, we don't have hardware palettes, so we stub this
  // Initialize with a default grayscale palette using 8-bit values
  int i;
  for (i = 0; i < 256; i++) {
    pal[i].r = i;  // 8-bit grayscale (0-255)
    pal[i].g = i;
    pal[i].b = i;
  }
}

/*------------------------------------------------------------------*/
void fade_in (PALETTE pal, int speed) {
  // In Allegro 4, this would gradually fade the screen from black to the palette
  // In Allegro 5, we simulate this by just setting the palette (no actual fading)
  (void)pal;    // Palette parameter ignored in Allegro 5
  (void)speed;  // Speed parameter ignored
  // TODO: Could implement actual screen fading using al_draw_tinted_bitmap
}

/*------------------------------------------------------------------*/
void fade_out (int speed) {
  // In Allegro 4, this would gradually fade the screen to black  
  // In Allegro 5, we simulate this (no actual fading for now)
  (void)speed;  // Speed parameter ignored
  // TODO: Could implement actual screen fading using al_draw_tinted_bitmap
}

/*------------------------------------------------------------------*/
void hsv_to_rgb (float h, float s, float v, int *r, int *g, int *b) {
  // Convert HSV color space to RGB
  // Based on standard HSV to RGB conversion algorithm
  int i;
  float f, p, q, t;
  
  if (s == 0) {
    // Achromatic (grey)
    *r = *g = *b = (int)(v * 255);
    return;
  }
  
  h /= 60; // sector 0 to 5
  i = (int)h;
  f = h - i; // fractional part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  
  switch (i) {
    case 0:
      *r = (int)(v * 255);
      *g = (int)(t * 255);
      *b = (int)(p * 255);
      break;
    case 1:
      *r = (int)(q * 255);
      *g = (int)(v * 255);
      *b = (int)(p * 255);
      break;
    case 2:
      *r = (int)(p * 255);
      *g = (int)(v * 255);
      *b = (int)(t * 255);
      break;
    case 3:
      *r = (int)(p * 255);
      *g = (int)(q * 255);
      *b = (int)(v * 255);
      break;
    case 4:
      *r = (int)(t * 255);
      *g = (int)(p * 255);
      *b = (int)(v * 255);
      break;
    default: // case 5:
      *r = (int)(v * 255);
      *g = (int)(p * 255);
      *b = (int)(q * 255);
      break;
  }
}

/*------------------------------------------------------------------*/
int fixsqrt (int x) {
  // Fixed-point square root (Allegro 4 compatibility)
  // Input and output are in 16.16 fixed-point format
  // Simple implementation using floating point
  if (x <= 0) return 0;
  
  // Convert from fixed-point to float, take sqrt, convert back
  float f = (float)x / 65536.0f;
  f = sqrtf(f);
  return (int)(f * 65536.0f);
}

/*------------------------------------------------------------------*/
ALLEGRO_COLOR rgb_to_allegro_color (RGB rgb) {
  // Convert RGB structure to ALLEGRO_COLOR
  // Both use 8-bit values (0-255)
  return al_map_rgb(rgb.r, rgb.g, rgb.b);
}

/*------------------------------------------------------------------*/
RGB allegro_color_to_rgb (ALLEGRO_COLOR color) {
  // Convert ALLEGRO_COLOR to RGB structure
  RGB rgb;
  unsigned char r, g, b;
  
  al_unmap_rgb(color, &r, &g, &b);
  // Both use 8-bit values (0-255)
  rgb.r = r;
  rgb.g = g; 
  rgb.b = b;
  
  return rgb;
}

/*------------------------------------------------------------------*/
void delete_file(const char *filename) {
  // Delete a file - Allegro 4 compatibility function
  // Maps directly to standard C remove() function
  if (filename) {
    remove(filename);
  }
}

/*------------------------------------------------------------------*/
void ellipse (ALLEGRO_BITMAP * bitmap, int x, int y, int rx, int ry, int color) {
  // Draw ellipse outline - Allegro 4 compatibility
  if (color < 0 || color >= PALETTE_SIZE) {
    return;
  }
  ALLEGRO_COLOR al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);
  
  ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
  al_set_target_bitmap(bitmap);
  al_draw_ellipse(x, y, rx, ry, al_color, 1.0);
  al_set_target_bitmap(old_target);
}

/*------------------------------------------------------------------*/
void ellipsefill (ALLEGRO_BITMAP * bitmap, int x, int y, int rx, int ry, int color) {
  // Draw filled ellipse - Allegro 4 compatibility
  if (color < 0 || color >= PALETTE_SIZE) {
    return;
  }
  ALLEGRO_COLOR al_color = rgb_to_allegro_color(GLOBAL_PALETTE[color]);
  
  ALLEGRO_BITMAP *old_target = al_get_target_bitmap();
  al_set_target_bitmap(bitmap);
  al_draw_filled_ellipse(x, y, rx, ry, al_color);
  al_set_target_bitmap(old_target);
}

/*------------------------------------------------------------------*/
void fix_filename_case(char *filename) {
  // Allegro 4 compatibility function - converts filename to correct case
  // On case-sensitive filesystems, this would try to find the correct case
  // For now, just leave the filename as-is since most modern systems handle this
  (void)filename; // Stub implementation - no changes needed on modern systems
}

/*------------------------------------------------------------------*/
void fix_filename_slashes(char *filename) {
  // Allegro 4 compatibility function - converts path separators
  // Convert all backslashes to forward slashes for cross-platform compatibility
  if (filename) {
    char *p = filename;
    while (*p) {
      if (*p == '\\') {
        *p = '/';
      }
      p++;
    }
  }
}

/*------------------------------------------------------------------*/
int bestfit_color(PALETTE pal, int r, int g, int b) {
  // Allegro 4 compatibility function - finds closest matching color in palette
  int best_index = 0;
  int best_distance = INT_MAX;
  int i;
  
  for (i = 0; i < 256; i++) {
    int dr = r - pal[i].r;
    int dg = g - pal[i].g;
    int db = b - pal[i].b;
    int distance = dr*dr + dg*dg + db*db;
    
    if (distance < best_distance) {
      best_distance = distance;
      best_index = i;
      if (distance == 0) break; // Exact match found
    }
  }
  
  return best_index;
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *my_create_bitmap(int w, int h) {
  // Allegro 4 compatibility function - creates a bitmap
  return al_create_bitmap(w, h);
}

/*------------------------------------------------------------------*/
ALLEGRO_BITMAP *create_sub_bitmap(ALLEGRO_BITMAP *parent, int x, int y, int w, int h) {
  // Allegro 4 compatibility function - creates a sub-bitmap
  return al_create_sub_bitmap(parent, x, y, w, h);
}

/*------------------------------------------------------------------*/
void scroll_screen(int x, int y) {
  // Allegro 4 compatibility function - scrolls the screen
  // In Allegro 5, this is mainly used for page flipping effects
  // For now, implement as a stub since modern page flipping works differently
  (void)x;
  (void)y;
  // Note: This function was used for hardware scrolling in Allegro 4
  // Modern systems handle this through different mechanisms
}
