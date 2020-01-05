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
/* name          : path.c                                           */
/* content       : path and filenames manipulation                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "path.h"
#include "startup.h"
#include "macro.h"
#include "log.h"

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Fonction used to extract the "system name" for maps and textures.
 * System name is basically the filename without its path and
 * extension.
 */
char *
lw_path_get_system_name (const char *filename)
{
  char buf1[LW_STARTUP_MAX_PATH_LENGTH];
  static char buf2[LW_STARTUP_MAX_PATH_LENGTH];
  char *start;
  //char *end;
  char *search;

  LW_MACRO_STRCPY (buf1, filename);

  if ((search = strrchr (buf1, '/')) != NULL ||
      (search = strrchr (buf1, '\\')) != NULL)
    {
      start = search + 1;
    }
  else
    {
      start = buf1;
    }

  LW_MACRO_STRCPY (buf2, start);

  search = strchr (buf2, '.');
  /*
     if (search != NULL)
     {
     end = search;
     }
     else
     {
     end = buf2 + strlen (buf2);
     }
   */

  if (search == NULL)
    {
      search = buf2 + strlen (buf2);
    }

  memset (search, 0, sizeof (buf2) - (search - buf2));

  return buf2;
}
