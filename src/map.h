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
/* nom           : map.h                                            */
/* contenu       : decryptage des tableaux en memoire               */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_MAP
#define LIQUID_WAR_INCLUDE_MAP

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <<allegro5/allegro.h>>

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_MAP_SYSTEM_NAME_SIZE 16
#define LW_MAP_READABLE_NAME_SIZE 32

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void *lw_map_archive_raw (const char *filename);
void *lw_map_archive_raw_bmp (ALLEGRO_BITMAP * bmp, PALETTE pal,
                              const char *filename);
ALLEGRO_BITMAP *lw_map_create_bicolor (int num, int fg, int bg,
                               int network, int random, int min_w, int min_h,
                               int *zoom_factor);
ALLEGRO_BITMAP *lw_map_create_textured (int num, int fg, int bg,
                                int network, int random, int min_w,
                                int min_h);
char *lw_map_get_system_name (int num, int network, int random);
char *lw_map_get_readable_name (int num, int network, int random);
void lw_map_get_res (int num, int *w, int *h,
                     int network, int random, int min_w, int min_h);

#endif
