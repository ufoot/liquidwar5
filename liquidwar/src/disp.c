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
/* nom           : disp.c                                           */
/* contenu       : affichage de l'aire de jeu                       */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "area.h"
#include "config.h"
#include "disp.h"
#include "grad.h"
#include "viewport.h"
#include "wave.h"
#include "distor.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
disp_stretch_area (void)
{
  stretch_blit (CURRENT_AREA_DISP, NEXT_SCREEN, 0, 0,
		CURRENT_AREA_W, CURRENT_AREA_H,
		0, 0, NEXT_SCREEN->w, NEXT_SCREEN->h);
}

/*------------------------------------------------------------------*/
void
display_area (void)
{
  if ((CONFIG_WAVE_AMPLI[0]
       || CONFIG_WAVE_AMPLI[1]
       || CONFIG_WAVE_AMPLI[2] || CONFIG_WAVE_AMPLI[3]) && CONFIG_WAVE_ON)
    disp_distorted_area ();
  else
    disp_stretch_area ();
}

/*------------------------------------------------------------------*/
void
display_gradient (int i)
{
  BITMAP *bmp;

  bmp = create_gradient_bitmap (i);
  if (bmp)
    {
      stretch_blit (bmp, NEXT_SCREEN, 0, 0, bmp->w, bmp->h,
		    0, 0, NEXT_SCREEN->w, NEXT_SCREEN->h);
      destroy_bitmap (bmp);
    }
}

/*------------------------------------------------------------------*/
void
display_mesh (int i)
{
  BITMAP *bmp;

  bmp = create_mesh_bitmap (i);
  if (bmp)
    {
      stretch_blit (bmp, NEXT_SCREEN, 0, 0, bmp->w, bmp->h,
		    0, 0, NEXT_SCREEN->w, NEXT_SCREEN->h);
      destroy_bitmap (bmp);
    }
}
