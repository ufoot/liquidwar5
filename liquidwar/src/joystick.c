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
/* Copyright (C) 1998-2010 Christian Mauduit                                 */
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
/* nom           : joystick.c                                       */
/* contenu       : gestion du joystick                              */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#include "config.h"
#include "joystick.h"
#include "startup.h"
#include "log.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/
#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int JOYSTICK_CONTROL_JOY1_UP = 0;
int JOYSTICK_CONTROL_JOY1_RIGHT = 0;
int JOYSTICK_CONTROL_JOY1_DOWN = 0;
int JOYSTICK_CONTROL_JOY1_LEFT = 0;
int JOYSTICK_CONTROL_JOY1_B1 = 0;
int JOYSTICK_CONTROL_JOY1_B2 = 0;
int JOYSTICK_CONTROL_JOY1_B3 = 0;
int JOYSTICK_CONTROL_JOY1_B4 = 0;

int JOYSTICK_CONTROL_JOY2_UP = 0;
int JOYSTICK_CONTROL_JOY2_RIGHT = 0;
int JOYSTICK_CONTROL_JOY2_DOWN = 0;
int JOYSTICK_CONTROL_JOY2_LEFT = 0;
int JOYSTICK_CONTROL_JOY2_B1 = 0;
int JOYSTICK_CONTROL_JOY2_B2 = 0;

/*==================================================================*/
/* static vars                                                      */
/*==================================================================*/

static int JOY1_MIN_X = 1000;
static int JOY1_MID_X = 0;
static int JOY1_MAX_X = -1000;
static int JOY1_MIN_Y = 1000;
static int JOY1_MID_Y = 0;
static int JOY1_MAX_Y = -1000;

static int JOY2_MIN_X = 1000;
static int JOY2_MID_X = 0;
static int JOY2_MAX_X = -1000;
static int JOY2_MIN_Y = 1000;
static int JOY2_MID_Y = 0;
static int JOY2_MAX_Y = -1000;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
my_initialise_joystick (void)
{
  int result;

#ifdef DOS
#define JOY_TYPE0 JOY_TYPE_2PADS
#define JOY_TYPE1 JOY_TYPE_4BUTTON
#define JOY_TYPE2 JOY_TYPE_STANDARD
#endif

#ifdef WIN32
#define JOY_TYPE0 JOY_TYPE_AUTODETECT
#define JOY_TYPE1 JOY_TYPE_AUTODETECT
#define JOY_TYPE2 JOY_TYPE_AUTODETECT
#endif

#ifdef UNIX
#define JOY_TYPE0 JOY_TYPE_AUTODETECT
#define JOY_TYPE1 JOY_TYPE_AUTODETECT
#define JOY_TYPE2 JOY_TYPE_AUTODETECT
#endif

  if (STARTUP_JOYSTICK_ON)
    {
      int type = JOY_TYPE_AUTODETECT;

      switch (CONFIG_JOYSTICK_ON)
	{
	case 1:
	  type = JOY_TYPE0;
	  break;
	case 2:
	  type = JOY_TYPE1;
	  break;
	case 3:
	  type = JOY_TYPE2;
	  break;
	}
      result = install_joystick (type);
    }
  else
    result = -1;

  /*
   * Now we set "default" values
   */
  if (num_joysticks >= 1)
    {
      if (joy[0].num_sticks >= 1)
	{
	  if (joy[0].stick[0].num_axis >= 2)
	    {
	      JOY1_MIN_X = joy[0].stick[0].axis[0].pos;
	      JOY1_MID_X = joy[0].stick[0].axis[0].pos;
	      JOY1_MAX_X = joy[0].stick[0].axis[0].pos;
	      JOY1_MIN_Y = joy[0].stick[0].axis[1].pos;
	      JOY1_MID_Y = joy[0].stick[0].axis[1].pos;
	      JOY1_MAX_Y = joy[0].stick[0].axis[1].pos;
	    }
	}
    }
  if (num_joysticks >= 2)
    {
      if (joy[1].num_sticks >= 1)
	{
	  if (joy[1].stick[0].num_axis >= 2)
	    {
	      JOY2_MIN_X = joy[1].stick[0].axis[0].pos;
	      JOY2_MID_X = joy[1].stick[0].axis[0].pos;
	      JOY2_MAX_X = joy[1].stick[0].axis[0].pos;
	      JOY2_MIN_Y = joy[1].stick[0].axis[1].pos;
	      JOY2_MID_Y = joy[1].stick[0].axis[1].pos;
	      JOY2_MAX_Y = joy[1].stick[0].axis[1].pos;
	    }
	}
    }

  my_poll_joystick ();

  return result;
}

/*------------------------------------------------------------------*/
void
my_poll_joystick (void)
{
  if (CONFIG_JOYSTICK_ON && STARTUP_JOYSTICK_ON)
    {
      poll_joystick ();

      if (num_joysticks >= 1)
	{
	  if (joy[0].num_sticks >= 1)
	    {
	      if (joy[0].stick[0].num_axis >= 2)
		{
		  JOY1_MIN_X = MIN (joy[0].stick[0].axis[0].pos, JOY1_MIN_X);
		  JOY1_MAX_X = MAX (joy[0].stick[0].axis[0].pos, JOY1_MAX_X);
		  JOY1_MID_X = (JOY1_MIN_X + JOY1_MAX_X) / 2;
		  JOY1_MIN_Y = MIN (joy[0].stick[0].axis[1].pos, JOY1_MIN_Y);
		  JOY1_MAX_Y = MAX (joy[0].stick[0].axis[1].pos, JOY1_MAX_Y);
		  JOY1_MID_Y = (JOY1_MIN_Y + JOY1_MAX_Y) / 2;

		  JOYSTICK_CONTROL_JOY1_UP =
		    (joy[0].stick[0].axis[1].pos <
		     (JOY1_MIN_Y + 2 * JOY1_MID_Y) / 3) ? 1 : 0;
		  JOYSTICK_CONTROL_JOY1_RIGHT =
		    (joy[0].stick[0].axis[0].pos >
		     (JOY1_MAX_X + 2 * JOY1_MID_X) / 3) ? 1 : 0;
		  JOYSTICK_CONTROL_JOY1_DOWN =
		    (joy[0].stick[0].axis[1].pos >
		     (JOY1_MAX_Y + 2 * JOY1_MID_Y) / 3) ? 1 : 0;
		  JOYSTICK_CONTROL_JOY1_LEFT =
		    (joy[0].stick[0].axis[0].pos <
		     (JOY1_MIN_X + 2 * JOY1_MID_X) / 3) ? 1 : 0;
		}
	    }
	  if (joy[0].num_buttons >= 2)
	    {
	      JOYSTICK_CONTROL_JOY1_B1 = joy[0].button[0].b;
	      JOYSTICK_CONTROL_JOY1_B2 = joy[0].button[1].b;
	    }
	  if (joy[0].num_buttons >= 4)
	    {
	      JOYSTICK_CONTROL_JOY1_B3 = joy[0].button[2].b;
	      JOYSTICK_CONTROL_JOY1_B4 = joy[0].button[3].b;
	    }
	}
      if (num_joysticks >= 2)
	{
	  if (joy[1].num_sticks >= 1)
	    {
	      if (joy[1].stick[0].num_axis >= 2)
		{
		  JOY2_MIN_X = MIN (joy[1].stick[0].axis[0].pos, JOY2_MIN_X);
		  JOY2_MAX_X = MAX (joy[1].stick[0].axis[0].pos, JOY2_MAX_X);
		  JOY2_MID_Y = (JOY2_MIN_X + JOY2_MAX_X) / 2;
		  JOY2_MIN_Y = MIN (joy[1].stick[0].axis[1].pos, JOY2_MIN_Y);
		  JOY2_MAX_Y = MAX (joy[1].stick[0].axis[1].pos, JOY2_MAX_Y);
		  JOY2_MID_Y = (JOY2_MIN_Y + JOY2_MAX_Y) / 2;

		  JOYSTICK_CONTROL_JOY2_UP =
		    (joy[1].stick[0].axis[1].pos <
		     (JOY2_MIN_Y + 2 * JOY2_MID_Y) / 3) ? 1 : 0;
		  JOYSTICK_CONTROL_JOY2_RIGHT =
		    (joy[1].stick[0].axis[0].pos >
		     (JOY2_MAX_X + 2 * JOY2_MID_X) / 3) ? 1 : 0;
		  JOYSTICK_CONTROL_JOY2_DOWN =
		    (joy[1].stick[0].axis[1].pos >
		     (JOY2_MAX_Y + 2 * JOY2_MID_Y) / 3) ? 1 : 0;
		  JOYSTICK_CONTROL_JOY2_LEFT =
		    (joy[1].stick[0].axis[0].pos <
		     (JOY2_MIN_X + 2 * JOY2_MID_X) / 3) ? 1 : 0;
		}
	    }
	  if (joy[0].num_buttons >= 2)
	    {
	      JOYSTICK_CONTROL_JOY2_B1 = joy[1].button[0].b;
	      JOYSTICK_CONTROL_JOY2_B2 = joy[1].button[1].b;
	    }
	}
    }
}
