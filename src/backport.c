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
/* name          : backport.c                                       */
/* content       : missing allegro 4 stuff                          */
/* last update   : Dec 24th 2018                                    */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "backport.h"
#include "palette.h"
#include "macro.h"
#include "mutxgen.h"
#include "thrdgen.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#define _NB_TIMERS 16

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

typedef struct {
  int delay_ms;
  void (*proc)();
} _backport_timer_data;

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

static int _allegro_errno = 0;
static int _dummy = 0;          // stupid dummy to get rid of unused param warning
static _backport_timer_data _backport_timers[_NB_TIMERS];
static LW_MUTEX_DATA _backport_timer_mutex={NULL};

ALLEGRO_BITMAP *screen = NULL;
int SCREEN_W = 0;
int SCREEN_H = 0;
ALLEGRO_FONT *font = NULL;
volatile int mouse_x = 0;
volatile int mouse_y = 0;
volatile int mouse_z = 0;
volatile int mouse_b = 0;
int *allegro_errno = &_allegro_errno;

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
  al_color = GLOBAL_PALETTE[color];
  al_put_pixel (x, y, al_color);
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
  al_color = GLOBAL_PALETTE[color];
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
  al_color = GLOBAL_PALETTE[color];
  // +1 on second coord because floating point vs integer
  al_draw_filled_rectangle (x1, y1, x2 + 1, y2 + 1, al_color);
}

/*------------------------------------------------------------------*/
void
vline (ALLEGRO_BITMAP * bitmap, int x, int y1, int y2, int color)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#rect
  if (y2 < y1)
    {
      return;
    }
  al_set_target_bitmap (bitmap);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = GLOBAL_PALETTE[color];
  al_draw_filled_rectangle (x, y1, x + 1, y2 + 1, al_color);
}

/*------------------------------------------------------------------*/
void
hline (ALLEGRO_BITMAP * bitmap, int x1, int y, int x2, int color)
{
  // https://liballeg.org/stabledocs/en/alleg013.html#rect
  if (x2 < x1)
    {
      return;
    }
  al_set_target_bitmap (bitmap);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = GLOBAL_PALETTE[color];
  al_draw_filled_rectangle (x1, y, x2 + 1, y + 1, al_color);
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
  al_color = GLOBAL_PALETTE[color];

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
int install_timer(){
  // https://liballeg.org/stabledocs/en/alleg005.html#install_timer
  memset(_backport_timers,0,sizeof(_backport_timers));
  memset(&_backport_timer_mutex,0,sizeof(_backport_timer_mutex));

  return (lw_mutex_init(&_backport_timer_mutex)==0) ? 0:-1;
}

/*------------------------------------------------------------------*/
void remove_timer(){
  // https://liballeg.org/stabledocs/en/alleg005.html#remove_timer
  memset(_backport_timers,0,sizeof(_backport_timers));
  memset(&_backport_timer_mutex,0,sizeof(_backport_timer_mutex));
}


/*------------------------------------------------------------------*/
void _backport_timer_callback(void *ptr) {
  _backport_timer_data *data=NULL;

  data=(_backport_timer_data *) ptr;
  while (data->proc) {
    data->proc();
    rest(data->delay_ms);
  }
}

/*------------------------------------------------------------------*/
int install_int(void (*proc)(), int speed) {
  // https://liballeg.org/stabledocs/en/alleg005.html#install_int
  int i=0;
  bool found=false;
  int ret=0;

  if (speed<=0) {
    speed=1;
  }

  lw_mutex_lock(&_backport_timer_mutex);
  for (i=0;i<_NB_TIMERS && !found;i++) {
    if (_backport_timers[i].proc==proc) {
      _backport_timers[i].delay_ms=speed;
      found=true;
    }
  }
  for (i=0;i<_NB_TIMERS && !found;i++) {
    if (_backport_timers[i].proc==NULL) {
      _backport_timers[i].proc=proc;
      _backport_timers[i].delay_ms=speed;
      if (!lw_thread_start(_backport_timer_callback,&(_backport_timers[i]))) {
        ret=-2;
      }
      found=true;
    }
  }
  lw_mutex_unlock(&_backport_timer_mutex);

  return found ? ret : -1;
}

/*------------------------------------------------------------------*/
void remove_int(void (*proc)()) {
  // https://liballeg.org/stabledocs/en/alleg005.html#remove_int
  int i=0;
  bool found=false;

  lw_mutex_lock(&_backport_timer_mutex);
  for (i=0;i<_NB_TIMERS && !found;i++) {
    if (_backport_timers[i].proc==proc) {
      _backport_timers[i].proc=NULL;
      found=true;
    }
  }
  lw_mutex_unlock(&_backport_timer_mutex);
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
