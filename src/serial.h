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
/* name    : serial.h                                               */
/* content : routines used to (un)serialize values                  */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_SERIAL
#define LIQUID_WAR_INCLUDE_SERIAL

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <<allegro5/allegro.h>>

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_MAP_SYSTEM_NAME_SIZE 16
#define LW_MAP_READABLE_NAME_SIZE 32

#define LW_TEXTURE_SYSTEM_NAME_SIZE 16

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

int lw_serial_get_int (void *raw);
void lw_serial_set_int (void *raw, int i);
short lw_serial_get_short (void *raw);
void lw_serial_set_short (void *raw, int s);
void lw_serial_get_map_header (void *raw_map, int *size, short *w, short *h);
void lw_serial_set_map_header (void *raw_map, int size, short w, short h);
void lw_serial_get_texture_header (void *raw_texture, short *w, short *h);
void lw_serial_set_texture_header (void *raw_texture, short w, short h);


#endif
