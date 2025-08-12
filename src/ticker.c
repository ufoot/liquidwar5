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
/* Copyright (C) 1998-2025 Christian Mauduit                                 */
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
/* nom           : ticker.c                                         */
/* contenu       : gestion d'un compteur de milliemes de seconde    */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "backport.h"
#include "ticker.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

static double ticker_start_time = 0.0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/


/*------------------------------------------------------------------*/
/* mise en place du chrono                                          */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
int
start_ticker (void)
{
  // Initialize ticker with current time
  ticker_start_time = al_get_time();
  return 0; // Always succeeds with Allegro 5
}

/*------------------------------------------------------------------*/
void
stop_ticker (void)
{
  // No cleanup needed with Allegro 5's al_get_time()
  // Just reset the start time
  ticker_start_time = 0.0;
}

/*------------------------------------------------------------------*/
int
get_ticker (void)
{
  // Return elapsed time in milliseconds since start_ticker() was called
  double current_time = al_get_time();
  double elapsed_seconds = current_time - ticker_start_time;
  
  // Convert to milliseconds and return as integer
  return (int)(elapsed_seconds * 1000.0);
}
