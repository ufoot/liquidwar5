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

#include "backport.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

ALLEGRO_BITMAP *screen=NULL;
ALLEGRO_FONT *font=NULL;

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
unscare_mouse ()
{
  /*
   * Using a higher level API now, looks like those disappeared.
   * Keeping them so that whatever workaround those calls were
   * achieving, it's easier to backport it afterwards.
   */
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
  void al_set_target_bitmap (bitmap);
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
  if (x2<x1 || y2<y1) {
    return;
  }
  void al_set_target_bitmap (bitmap);
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
  void al_set_target_bitmap (bitmap);
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
  if (y2<y1) {
    return;
  }
  void al_set_target_bitmap (bitmap);
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
  if (x2<x1) {
    return;
  }
  void al_set_target_bitmap (bitmap);
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
  const char *s = NULL;

  us = al_ustr_new ("");
  al_ustr_append_chr (us, c);
  s = al_cstr (us);
  l = strlen (s);
  LW_MACRO_STRNCPY (s, al_cstr (us));
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
  c= al_ustr_get(us, 0);
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
  bool found=false;

  us = al_ustr_new (s);
  found=al_ustr_next(us,&pos);
  al_ustr_free (us);

  if (!found) {
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
  int l=0;

  us = al_ustr_new (s);
  l=al_ustr_length(s);
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
  return c<=32;
}

/*------------------------------------------------------------------*/
int ugetat(char *s, int index) {
  // https://liballeg.org/stabledocs/en/alleg002.html#ugetat
  ALLEGRO_USTR *us = NULL;
  int c=0;

  us = al_ustr_new (s);
  c=al_ustr_get(us,index);
  al_ustr_free (us);

  return c;
}

/*------------------------------------------------------------------*/
int
usetat(char *s, int index, int c,int max_size){
  // https://liballeg.org/stabledocs/en/alleg002.html#usetat
  /*
   * Altered the protoype of that one, not giving max_size here
   * is just totally super-dangerous IMHO.
   */
  ALLEGRO_USTR *us = NULL;
  int l=0;
  int max_len=max_size-1;

  l=strlen(s);
  if (index<0) {
    index=l-index;
  }
  if (index>= max_len || index<0){
    return;
  }

  us = al_ustr_new (s);
  if (c==0) {
    al_ustr_truncate(us,index);
  } else {
    al_ustr_remove_chr(us,index);
    al_ustr_insert_chr(us,index,c);
  }
  LW_MACRO_STRNCPY(s,al_cstr(us),max_size);
  al_ustr_free (us);

  return (strlen(s)-l);
}

/*------------------------------------------------------------------*/
int uinsert(char *s, int index, int c,int max_size){
  // https://liballeg.org/stabledocs/en/alleg002.html#uinsert
  /*
   * Altered the protoype of that one, not giving max_size here
   * is just totally super-dangerous IMHO.
   */
  ALLEGRO_USTR *us = NULL;
  int l=0;
  int max_len=max_size-1;

  l=strlen(s);
  if (index<0) {
    index=l-index;
  }
  if (index>= max_len || index<0){
    return;
  }

  us = al_ustr_new (s);
  if (c==0) {
    al_ustr_truncate(us,index);
  } else {
    al_ustr_insert_chr(us,index,c);
  }
  LW_MACRO_STRNCPY(s,al_cstr(us),max_size);
  al_ustr_free (us);

  return (strlen(s)-l);
}

/*------------------------------------------------------------------*/
int
uremove(char *s, int index){
  // https://liballeg.org/stabledocs/en/alleg002.html#uremove
  ALLEGRO_USTR *us = NULL;
  int l=0;

  l=strlen(s);
  if (index<0) {
    index=l-index;
  }
  if (index>= max_len || index<0){
    return;
  }

  us = al_ustr_new (s);
  al_ustr_remove_chr(us,index);
  LW_MACRO_STRNCPY(s,al_cstr(us),max_size);
  al_ustr_free (us);

  return (strlen(s)-l);
}

/*------------------------------------------------------------------*/
int
uisok(int c){
  // https://liballeg.org/stabledocs/en/alleg002.html#uiosok
  /*
   * [FIXME:ufoot] right now does nothing real, but game handles
   * UTF-8 very poorly, TBH.
   */
  return ((c >= ' ') && (c <= 127));
}

/*------------------------------------------------------------------*/
int text_length(ALLEGRO_FONT *f, const char *s) {
  // https://liballeg.org/stabledocs/en/alleg018.html#text_length
  return al_get_text_width(f, s);
}

/*------------------------------------------------------------------*/
int text_height(ALLEGRO_FONT *f) {
  // https://liballeg.org/stabledocs/en/alleg018.html#text_height
  return al_get_font_line_height(f);
}

/*------------------------------------------------------------------*/
void textout_ex(ALLEGRO_BITMAP *bmp, const ALLEGRO_FONT *f, const char *s, int x, int y, int color, int bg){
  // https://liballeg.org/stabledocs/en/alleg018.html#textout_ex

  al_set_target_bitmap (bmp);
  if (color < 0 || color >= PALETTE_SIZE)
    {
      return;
    }
  ALLEGRO_COLOR al_color;
  al_color = GLOBAL_PALETTE[color];
  al_draw_text(f,al_color,x,y,0,s);
}

/*------------------------------------------------------------------*/
void rest_callback(unsigned int time, void (*callback)(void)) {
  // https://liballeg.org/stabledocs/en/alleg005.html#rest_callback
  double delay;

  delay=double(time)/1000.0;
  if (callback==NULL) {
    al_rest(delay);
  } else {
    int i;

    for (i=0; i<time; i++) {
      al_rest(0.001);
      callback();
    }
  }
}

/*------------------------------------------------------------------*/
void rest(unsigned int time) {
  // https://liballeg.org/stabledocs/en/alleg005.html#rest
  rest_callback(int,NULL);
}

/*------------------------------------------------------------------*/
void draw_sprite(ALLEGRO_BITMAP *bmp, ALLEGRO_BITMAP *sprite, int x, int y) {
  // https://liballeg.org/stabledocs/en/alleg014.html#draw_sprite
  void al_set_target_bitmap (bmp);
  al_draw_bitmap(bmp,sprite,x,y,0);
}
