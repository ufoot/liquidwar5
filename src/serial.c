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
/* Copyright (C) 1998-2020 Christian Mauduit                                 */
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
/* name    : serial.c                                               */
/* content : routines used to (un)serialize values                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "serial.h"

/*==================================================================*/
/* constantes                                                       */
/*==================================================================*/

/*
 * If you define LW_SERIAL_WRONG_BYTE_ORDER, the game will
 * be compiledd with the "other" byte order. This is for testing
 * only, do *not* used for release build, _even_ when compiling on
 * a Mac. The purpose of this constant is testing, to check that
 * there are no bugs. Should you compile a release with this flag
 * set to 1, you would not be able to play in networked mode with
 * other players using the "standard" byte order.
 */
//#define LW_SERIAL_WRONG_ENDIANESS

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*
 * Here's an ugly serialization of ints and shorts.
 * Usefull to make the game work on Mac OS X and possible
 * any platform that does not have PC-like endianess
 */

/*------------------------------------------------------------------*/
int
lw_serial_get_int (void *raw)
{
  int i = 0;
  unsigned char *ptr;

  ptr = (unsigned char *) raw;

#ifndef LW_SERIAL_WRONG_ENDIANESS
  i = ((unsigned int) ptr[3]) << 24
    | ((unsigned int) ptr[2]) << 16
    | ((unsigned int) ptr[1]) << 8 | ((unsigned int) ptr[0]);
#else
  i = ((unsigned int) ptr[0]) << 24
    | ((unsigned int) ptr[1]) << 16
    | ((unsigned int) ptr[2]) << 8 | ((unsigned int) ptr[3]);
#endif

  return i;
}

/*------------------------------------------------------------------*/
void
lw_serial_set_int (void *raw, int i)
{
  unsigned char *ptr;

  ptr = (unsigned char *) raw;

#ifndef LW_SERIAL_WRONG_ENDIANESS
  ptr[0] = 0xFF & i;
  ptr[1] = 0xFF & i >> 8;
  ptr[2] = 0xFF & i >> 16;
  ptr[3] = i >> 24;
#else
  ptr[3] = 0xFF & i;
  ptr[2] = 0xFF & i >> 8;
  ptr[1] = 0xFF & i >> 16;
  ptr[0] = i >> 24;
#endif
}

/*------------------------------------------------------------------*/
short
lw_serial_get_short (void *raw)
{
  short s = 0;
  unsigned char *ptr;

  ptr = (unsigned char *) raw;

#ifndef LW_SERIAL_WRONG_ENDIANESS
  s = ((unsigned int) ptr[1]) << 8 | ((unsigned int) ptr[0]);
#else
  s = ((unsigned int) ptr[0]) << 8 | ((unsigned int) ptr[1]);
#endif

  return s;
}

/*------------------------------------------------------------------*/
void
lw_serial_set_short (void *raw, int s)
{
  unsigned char *ptr;

  ptr = (unsigned char *) raw;

#ifndef LW_SERIAL_WRONG_ENDIANESS
  ptr[0] = 0xFF & s;
  ptr[1] = 0xFF & s >> 8;
#else
  ptr[1] = 0xFF & s;
  ptr[0] = 0xFF & s >> 8;
#endif
}

/*------------------------------------------------------------------*/
void
lw_serial_get_map_header (void *raw_map, int *size, short *w, short *h)
{
  char *header;

  header = (char *) raw_map;

  if (size != NULL)
    {
      (*size) = lw_serial_get_int (header);
    }
  if (w != NULL)
    {
      (*w) = lw_serial_get_short (header + 4);
    }
  if (h != NULL)
    {
      (*h) = lw_serial_get_short (header + 6);
    }
}

/*------------------------------------------------------------------*/
void
lw_serial_set_map_header (void *raw_map, int size, short w, short h)
{
  char *header;

  header = (char *) raw_map;

  lw_serial_set_int (header, size);
  lw_serial_set_short (header + 4, w);
  lw_serial_set_short (header + 6, h);
}

/*------------------------------------------------------------------*/
void
lw_serial_get_texture_header (void *raw_texture, short *w, short *h)
{
  char *header;

  header = (char *) raw_texture;

  if (w != NULL)
    {
      (*w) = lw_serial_get_short (header + 0);
    }
  if (h != NULL)
    {
      (*h) = lw_serial_get_short (header + 2);
    }
}

/*------------------------------------------------------------------*/
void
lw_serial_set_texture_header (void *raw_texture, short w, short h)
{
  char *header;

  header = (char *) raw_texture;

  lw_serial_set_short (header + 0, w);
  lw_serial_set_short (header + 2, h);
}
