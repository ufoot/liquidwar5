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
/* nom           : palette.h                                        */
/* contenu       : gestion des palettes de couleur                  */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_PALETTE
#define LIQUID_WAR_INCLUDE_PALETTE

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define CHOOSE_COLOR_NUMBER 128
#define CHOOSE_COLOR_WHITE 120
#define CHOOSE_COLOR_BLACK 127

#define TEAM_FIRST_COLOR 128

#define FG_TEXTURE_FIRST_COLOR 64
#define BG_TEXTURE_FIRST_COLOR 96

#define TEXTURE_COLOR_NUMBER 32

#define MENU_BG 16
#define MENU_FG 17

extern PALETTE GLOBAL_PALETTE;
extern int COLORS_PER_TEAM;

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void my_set_palette (void);
void set_bg_texture_palette (int num);
void set_fg_texture_palette (int num);
void set_palette_for_choose_color (void);
void set_playing_teams_palette (void);
void my_fade_out (void);
void my_fade_in (void);

#endif
