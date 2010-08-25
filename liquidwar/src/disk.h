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
/* Copyright (C) 1998-2010 Christian Mauduit                                 */
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
/* nom           : disk.h                                           */
/* contenu       : chargement des donnees du fichier .dat           */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_DISK
#define LIQUID_WAR_INCLUDE_DISK

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

/*==================================================================*/
/* constantes                                                       */
/*==================================================================*/

#define SAMPLE_WATER_MAX_NUMBER  32
#define SAMPLE_WATER_DAT_NUMBER  32

#define RAW_MAP_MAX_NUMBER     1024
#define RAW_MAP_DAT_NUMBER      512

#define RAW_TEXTURE_MAX_NUMBER 1024
#define RAW_TEXTURE_DAT_NUMBER  512

#define MIDI_MUSIC_MAX_NUMBER   256
#define MIDI_MUSIC_DAT_NUMBER   128

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

extern int SAMPLE_WATER_NUMBER;
extern int RAW_TEXTURE_NUMBER;
extern int RAW_MAPTEX_NUMBER;
extern int RAW_MAP_NUMBER;
extern int MIDI_MUSIC_NUMBER;

extern int LOADED_BACK;
extern int LOADED_TEXTURE;
extern int LOADED_MAPTEX;
extern int LOADED_SFX;
extern int LOADED_WATER;

extern SAMPLE *SAMPLE_SFX_TIME;
extern SAMPLE *SAMPLE_SFX_WIN;
extern SAMPLE *SAMPLE_SFX_GO;
extern SAMPLE *SAMPLE_SFX_CLICK;
extern SAMPLE *SAMPLE_SFX_LOOSE;
extern SAMPLE *SAMPLE_SFX_CONNECT;

extern SAMPLE *SAMPLE_WATER[SAMPLE_WATER_MAX_NUMBER];
extern void *RAW_MAP[RAW_MAP_MAX_NUMBER];
extern void *RAW_MAP_ORDERED[RAW_MAP_MAX_NUMBER];
extern void *RAW_TEXTURE[RAW_TEXTURE_MAX_NUMBER];
extern void *RAW_MAPTEX[RAW_TEXTURE_MAX_NUMBER];
extern MIDI *MIDI_MUSIC[MIDI_MUSIC_MAX_NUMBER];

extern BITMAP *BACK_IMAGE;

extern FONT *BIG_FONT;
extern FONT *SMALL_FONT;
extern BITMAP *BIG_MOUSE_CURSOR;
extern BITMAP *SMALL_MOUSE_CURSOR;
extern BITMAP *INVISIBLE_MOUSE_CURSOR;

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

int load_dat (void);
int load_custom (void);
void order_map (void);

#endif
