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
/* Copyright (C) 1998-2013 Christian Mauduit                                 */
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
/* nom           : maptex.c                                         */
/* content       : used to associate maps with textures             */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "maptex.h"
#include "map.h"
#include "texture.h"
#include "palette.h"
#include "disk.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* gestion des bitmaps des textures                                 */
/*------------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
static int
calc_real_texture_number (int num,
			  int texture, int network, int random,
			  int use_default_texture, int *found)
{
  int i;
  char *map_name;
  int real_texture;

  *found = 0;
  real_texture = texture;

  if (use_default_texture)
    {
      /*
       * OK, the player has chosen the "use default texture" option,
       * now we check that such a default texture does exists
       */

      map_name = lw_map_get_system_name (num, network, random);
      for (i = CHOOSE_COLOR_NUMBER;
	   i < CHOOSE_COLOR_NUMBER + RAW_TEXTURE_NUMBER + RAW_MAPTEX_NUMBER
	   && !(*found); ++i)
	{
	  if (strcmp (map_name, lw_texture_get_system_name (i)) == 0)
	    {
	      *found = 1;
	      real_texture = i;
	    }
	}
    }

  return real_texture;
}

/*-----------------------------------------------------------------*/
int
lw_maptex_is_custom_texture_used (int num,
				  int texture,
				  int network, int random,
				  int use_default_texture)
{
  int result = 0;

  calc_real_texture_number (num, texture, network, random,
			    use_default_texture, &result);

  return result;
}

/*-----------------------------------------------------------------*/
BITMAP *
lw_maptex_create_map (int num, int fg, int bg,
		      int network, int random, int min_w, int min_h,
		      int use_default_texture)
{
  BITMAP *result;
  int found;

  result = lw_map_create_textured (num,
				   calc_real_texture_number (num, fg,
							     network, random,
							     use_default_texture,
							     &found),
				   calc_real_texture_number (num, bg,
							     network, random,
							     use_default_texture,
							     &found),
				   network, random, min_w, min_h);

  return result;
}

/*-----------------------------------------------------------------*/
BITMAP *
lw_maptex_create_fg (int num, int fg, int network, int random,
		     int use_default_texture)
{
  BITMAP *result;
  int found;

  result =
    lw_texture_create_fg (calc_real_texture_number
			  (num, fg, network, random, use_default_texture,
			   &found));

  return result;
}

/*-----------------------------------------------------------------*/
BITMAP *
lw_maptex_create_bg (int num, int bg, int network, int random,
		     int use_default_texture)
{
  BITMAP *result;
  int found;

  result =
    lw_texture_create_bg (calc_real_texture_number
			  (num, bg, network, random, use_default_texture,
			   &found));

  return result;
}

/*-----------------------------------------------------------------*/
void
lw_maptex_set_fg_palette (int num,
			  int fg, int network, int random,
			  int use_default_texture)
{
  int found;

  set_fg_texture_palette (calc_real_texture_number
			  (num, fg, network, random, use_default_texture,
			   &found));
}

/*-----------------------------------------------------------------*/
void
lw_maptex_set_bg_palette (int num,
			  int bg, int network, int random,
			  int use_default_texture)
{
  int found;

  set_bg_texture_palette (calc_real_texture_number
			  (num, bg, network, random, use_default_texture,
			   &found));
}
