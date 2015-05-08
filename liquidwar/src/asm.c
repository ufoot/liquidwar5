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
/* Copyright (C) 1998-2015 Christian Mauduit                                 */
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
/* nom           : asm.h                                            */
/* contenu       : assembly language related stuff                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#include "asm.h"
#include "area.h"
#include "log.h"
#include "startup.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_ASM_OFFSET_BITMAP_LINE   64

typedef void *ptr;
#define LW_ASM_SIZEOF_int            4
#define LW_ASM_SIZEOF_short          2
#define LW_ASM_SIZEOF_char           1
#define LW_ASM_SIZEOF_ptr            4

#define LW_ASM_SIZEOF_MESH_UPDATE    4
#define LW_ASM_SIZEOF_MESH_STATE     4
#define LW_ASM_SIZEOF_MESH_INFO      8
#define LW_ASM_SIZEOF_MESH_SIDE      4
#define LW_ASM_SIZEOF_MESH         104
#define LW_ASM_SIZEOF_FIGHTER        8
#define LW_ASM_SIZEOF_PLACE          8

#define LW_ASM_CHECK_SIZEOF(S) \
if (sizeof(S)!=LW_ASM_SIZEOF_ ## S) \
{ \
result=0; \
log_print_str("Error: " #S " size is "); \
log_print_int(sizeof(S)); \
log_print_str(" and should be "); \
log_println_int(LW_ASM_SIZEOF_ ## S); \
}

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
lw_asm_check_struct_align (void)
{
  int result = 1;

#ifdef ASM
  {
    BITMAP *bmp = NULL;
    int offset_bitmap_line;

    LW_ASM_CHECK_SIZEOF (int);
    LW_ASM_CHECK_SIZEOF (short);
    LW_ASM_CHECK_SIZEOF (char);
    LW_ASM_CHECK_SIZEOF (ptr);

    LW_ASM_CHECK_SIZEOF (MESH_UPDATE);
    LW_ASM_CHECK_SIZEOF (MESH_STATE);
    LW_ASM_CHECK_SIZEOF (MESH_INFO);
    LW_ASM_CHECK_SIZEOF (MESH_SIDE);
    LW_ASM_CHECK_SIZEOF (MESH);
    LW_ASM_CHECK_SIZEOF (FIGHTER);
    LW_ASM_CHECK_SIZEOF (PLACE);

    offset_bitmap_line = ((char *) &(bmp->line)) - ((char *) bmp);
    if (offset_bitmap_line != LW_ASM_OFFSET_BITMAP_LINE)
      {
	result = 0;

	log_print_str ("Error: offset for BITMAP->line is ");
	log_print_int (offset_bitmap_line);
	log_print_str (" and should be ");
	log_println_int (LW_ASM_OFFSET_BITMAP_LINE);

	STARTUP_ASM = 0;
      }
  }
#endif

  if (!result)
    {
      log_println_str ("WARNING!!!");
      log_println_str
	("Liquid War might crash unexpectedly... Please read previous error messages.");
      log_println ();
    }

  return result;
}
