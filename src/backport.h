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
/* Liquid War homepage : http://www.ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* nom           : backport.h                                       */
/* content       : missing allegro 4 stuff                          */
/* last update   : Dec 24th 2018                                    */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_BACKPORT
#define LIQUID_WAR_INCLUDE_BACKPORT

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#ifndef AL_CONST
#define AL_CONST const
#endif // AL_CONST

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef MIN
#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#endif // MIN
#ifndef MAX
#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#endif // MAX
#ifndef MID
/* Returns the median of x, y, z */
#define MID(x,y,z)   ((x) > (y) ? ((y) > (z) ? (y) : ((x) > (z) ?    \
                       (z) : (x))) : ((y) > (z) ? ((z) > (x) ? (z) : \
                       (x)): (y)))
#endif // MID
#ifndef CLAMP
/* Optimized version of MID for when x <= z. */
#define CLAMP(x,y,z) MAX((x), MIN((y), (z)))
#endif // CLAMP
#ifndef ABS
#define ABS(x)       (((x) >= 0) ? (x) : (-(x)))
#endif // ABS
#ifndef SGN
#define SGN(x)       (((x) >= 0) ? 1 : -1)
#endif // SGN

#define _AL_MALLOC(size) al_malloc(size)
#define _AL_FREE(ptr) al_free(ptr)

#define LOCK_DATA(d, s)
#define LOCK_CODE(c, s)
#define UNLOCK_DATA(d, s)
#define LOCK_VARIABLE(x)
#define LOCK_FUNCTION(x)

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

#define PALETTE_SIZE 256
typedef ALLEGRO_COLOR PALETTE[PALETTE_SIZE];

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

extern ALLEGRO_BITMAP *screen;
extern int SCREEN_W;
extern int SCREEN_H;
extern ALLEGRO_FONT *font;
volatile int mouse_x;
volatile int mouse_y;
volatile int mouse_z;
volatile int mouse_b;
int *allegro_errno;

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void scare_mouse ();
void scare_mouse_area (int x, int y, int w, int h);
void unscare_mouse ();
int poll_mouse (void);
int mouse_needs_poll (void);

void putpixel (ALLEGRO_BITMAP * bitmap, int x, int y, int color);
void rect (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2,
           int color);
void rectfill (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2,
               int color);
void vline (ALLEGRO_BITMAP * bitmap, int x, int y1, int y2, int color);
void hline (ALLEGRO_BITMAP * bitmap, int x1, int y, int x2, int color);

int usetc (char *s, int c);
int ugetc (const char *s);
int uwidth (const char *s);
int ustrlen (const char *s);
int uisspace (int c);
int ugetat (char *s, int index);
int usetat (char *s, int index, int c, int max_size);
int uinsert (char *s, int index, int c, int max_size);
int uremove (char *s, int index);
int uisok (int c);

int text_length (const ALLEGRO_FONT * f, const char *s);
int text_height (const ALLEGRO_FONT * f);
void textout_ex (ALLEGRO_BITMAP * bmp, const ALLEGRO_FONT * f, const char *s,
                 int x, int y, int color, int bg);

void rest_callback (unsigned int time, void (*callback) (void));
void rest (unsigned int time);

void draw_sprite (ALLEGRO_BITMAP * bmp, ALLEGRO_BITMAP * sprite, int x,
                  int y);
void blit (ALLEGRO_BITMAP * source, ALLEGRO_BITMAP * dest,
           int source_x, int source_y,
           int dest_x, int dest_y, int dest_width, int dest_height);
void stretch_blit (ALLEGRO_BITMAP * source, ALLEGRO_BITMAP * dest,
                   int source_x, int source_y, int source_width,
                   int source_height, int dest_x, int dest_y, int dest_width,
                   int dest_height);

void acquire_bitmap (ALLEGRO_BITMAP * bmp);
void acquire_screen ();
void release_bitmap (ALLEGRO_BITMAP * bmp);
void release_screen ();

#endif // LIQUID_WAR_INCLUDE_BACKPORT
