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
/* nom           : joystick.h                                       */
/* contenu       : gestion du joystick                              */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_JOYSTICK
#define LIQUID_WAR_INCLUDE_JOYSTICK

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define KEY_JOY1_UP    110
#define KEY_JOY1_RIGHT 111
#define KEY_JOY1_DOWN  112
#define KEY_JOY1_LEFT  113
#define KEY_JOY1_B1    114
#define KEY_JOY1_B2    115
#define KEY_JOY1_B3    116
#define KEY_JOY1_B4    117

#define KEY_JOY2_UP    118
#define KEY_JOY2_RIGHT 119
#define KEY_JOY2_DOWN  120
#define KEY_JOY2_LEFT  121
#define KEY_JOY2_B1    122
#define KEY_JOY2_B2    123

extern int JOYSTICK_CONTROL_JOY1_UP;
extern int JOYSTICK_CONTROL_JOY1_RIGHT;
extern int JOYSTICK_CONTROL_JOY1_DOWN;
extern int JOYSTICK_CONTROL_JOY1_LEFT;
extern int JOYSTICK_CONTROL_JOY1_B1;
extern int JOYSTICK_CONTROL_JOY1_B2;
extern int JOYSTICK_CONTROL_JOY1_B3;
extern int JOYSTICK_CONTROL_JOY1_B4;

extern int JOYSTICK_CONTROL_JOY2_UP;
extern int JOYSTICK_CONTROL_JOY2_RIGHT;
extern int JOYSTICK_CONTROL_JOY2_DOWN;
extern int JOYSTICK_CONTROL_JOY2_LEFT;
extern int JOYSTICK_CONTROL_JOY2_B1;
extern int JOYSTICK_CONTROL_JOY2_B2;

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

int my_initialise_joystick (void);
void my_poll_joystick (void);

#endif
