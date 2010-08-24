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
/* Copyright (C) 1998-2008 Christian Mauduit                                 */
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
/* nom           : mouse.c                                          */
/* contenu       : gestion de la souris                             */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#include "config.h"
#include "mouse.h"
#include "disk.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int MOUSE_CONTROL_REF_X = 0;
int MOUSE_CONTROL_REF_Y = 0;
int MOUSE_CONTROL_UP = 0;
int MOUSE_CONTROL_RIGHT = 0;
int MOUSE_CONTROL_DOWN = 0;
int MOUSE_CONTROL_LEFT = 0;

static int MOUSE_CONTROL_GAP = 1000;
static int MOUSE_ON = 1;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
lw_mouse_update_control (void)
{
  if (mouse_y <= MOUSE_CONTROL_REF_Y - MOUSE_CONTROL_GAP)
    {
      MOUSE_CONTROL_UP = 1;
      MOUSE_CONTROL_REF_Y = mouse_y + MOUSE_CONTROL_GAP;
    }
  else
    MOUSE_CONTROL_UP = 0;

  if (mouse_x >= MOUSE_CONTROL_REF_X + MOUSE_CONTROL_GAP)
    {
      MOUSE_CONTROL_RIGHT = 1;
      MOUSE_CONTROL_REF_X = mouse_x - MOUSE_CONTROL_GAP;
    }
  else
    MOUSE_CONTROL_RIGHT = 0;

  if (mouse_y >= MOUSE_CONTROL_REF_Y + MOUSE_CONTROL_GAP)
    {
      MOUSE_CONTROL_DOWN = 1;
      MOUSE_CONTROL_REF_Y = mouse_y - MOUSE_CONTROL_GAP;
    }
  else
    MOUSE_CONTROL_DOWN = 0;

  if (mouse_x <= MOUSE_CONTROL_REF_X - MOUSE_CONTROL_GAP)
    {
      MOUSE_CONTROL_LEFT = 1;
      MOUSE_CONTROL_REF_X = mouse_x + MOUSE_CONTROL_GAP;
    }
  else
    MOUSE_CONTROL_LEFT = 0;
}

/*------------------------------------------------------------------*/
void
lw_mouse_reset_control (void)
{
  MOUSE_CONTROL_REF_X = mouse_x;
  MOUSE_CONTROL_REF_Y = mouse_y;
  MOUSE_CONTROL_GAP = 2 + (8 - CONFIG_MOUSE_GAP) * (8 - CONFIG_MOUSE_GAP);

  if (MOUSE_CONTROL_REF_X < MOUSE_CONTROL_GAP)
    MOUSE_CONTROL_REF_X = MOUSE_CONTROL_GAP;
  if (MOUSE_CONTROL_REF_Y < MOUSE_CONTROL_GAP)
    MOUSE_CONTROL_REF_Y = MOUSE_CONTROL_GAP;
  if (MOUSE_CONTROL_REF_X > SCREEN_W - MOUSE_CONTROL_GAP - 1)
    MOUSE_CONTROL_REF_X = SCREEN_W - MOUSE_CONTROL_GAP - 1;
  if (MOUSE_CONTROL_REF_Y > SCREEN_H - MOUSE_CONTROL_GAP - 1)
    MOUSE_CONTROL_REF_Y = SCREEN_H - MOUSE_CONTROL_GAP - 1;

  position_mouse (MOUSE_CONTROL_REF_X, MOUSE_CONTROL_REF_Y);
  lw_mouse_update_control ();
}

/*------------------------------------------------------------------*/
void
lw_mouse_update_show_hide (void)
{
  if (MOUSE_ON)
    {
      if (font == BIG_FONT)
	{
	  set_mouse_sprite (BIG_MOUSE_CURSOR);
	}
      if (font == SMALL_FONT)
	{
	  set_mouse_sprite (SMALL_MOUSE_CURSOR);
	}
      show_mouse (screen);
    }
  else
    {
      set_mouse_sprite (INVISIBLE_MOUSE_CURSOR);
    }
}

/*------------------------------------------------------------------*/
void
lw_mouse_show (void)
{
  MOUSE_ON = 1;
  lw_mouse_update_show_hide ();
}

/*------------------------------------------------------------------*/
void
lw_mouse_hide (void)
{
  MOUSE_ON = 0;
  lw_mouse_update_show_hide ();
}
