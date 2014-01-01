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
/* Copyright (C) 1998-2014 Christian Mauduit                                 */
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
/* name          : checksum.c                                       */
/* content       : utilities to calculate a checksum from a map     */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "mesh.h"
#include "area.h"
#include "time.h"
#include "log.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

/*
 * We calculate the checksum once every 100 moves, should be
 * largely enough though, since it's only for "bug-finding"
 */
#define LW_CHECKSUM_PERIOD 100

#define LW_CHECKSUM_CONST_RVAL 55665
#define LW_CHECKSUM_CONST_CST1 52845
#define LW_CHECKSUM_CONST_CST2 22719

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* initializes the string tables                                    */
/*------------------------------------------------------------------*/
int
lw_checksum_calc (FIGHTER * army, int army_size)
{
  unsigned int checksum = 0;
  int i;
  unsigned short rval = LW_CHECKSUM_CONST_RVAL;
  unsigned short cst1 = LW_CHECKSUM_CONST_CST1;
  unsigned short cst2 = LW_CHECKSUM_CONST_CST2;
  unsigned char cipher;
  unsigned char value;
  FIGHTER *f;

  /*
   * We do not _always_ calculate the checksum, it would be a CPU
   * waste...
   */
  if (!(GLOBAL_CLOCK % LW_CHECKSUM_PERIOD))
    {
      for (i = 0; i < army_size; ++i)
	{
	  f = army + i;

	  value = (0xFF &
		   (((unsigned int) f->x) +
		    ((unsigned int) f->y) +
		    ((unsigned int) f->health) +
		    ((unsigned int) f->team) + ((unsigned int) f->last_dir)));

	  /*
	   * Got this algorithm in old notes I had taken long time
	   * ago when I was at school.
	   */
	  cipher = (value ^ (rval >> 8));
	  rval = (cipher + rval) * cst1 + cst2;
	  checksum += cipher;
	}
    }

  return checksum;
}
