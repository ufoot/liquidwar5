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

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

#define PALETTE_SIZE 256
typedef struct
{
  ALLEGRO_COLOR colors[PALETTE_SIZE];
}
PALETTE;

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

extern ALLEGRO_BITMAP *screen;
extern ALLEGRO_FONT *font;

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void scare_mouse ();
void unscare_mouse ();

void putpixel (ALLEGRO_BITMAP * bitmap, int x, int y, int color);
void rectfill (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2, int color);

int usetc (char *s, int c);
int ugetc (const char *s);
int uwidth (const char *s);
int ustrlen (const char *s);
int uisspace (int c);
int ugetat(char *s, int index);
int usetat(char *s, int index, int c);
int uinsert(char *s, int index, int c);
int uremove(char *s, int index);
int uisok(int c);

int text_length(ALLEGRO_FONT *f, const char *s);
int text_height(ALLEGRO_FONT *f);
void textout_ex(ALLEGRO_BITMAP *bmp, const ALLEGRO_FONT *f, const char *s, int x, int y, int color, int bg);

void rest_callback(unsigned int time, void (*callback)(void));
void rest(unsigned int time);

#endif // LIQUID_WAR_INCLUDE_BACKPORT
