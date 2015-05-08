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
/* nom           : ticker.c                                         */
/* contenu       : gestion d'un compteur de milliemes de seconde    */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#include "ticker.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#ifdef DOS
#define TICKER_STEP 5
#else
#define TICKER_STEP 10
#endif
static int TICKER_VALUE = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/


/*------------------------------------------------------------------*/
/* mise en place du chrono                                          */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
static void
ticker_handler (void)
{
  TICKER_VALUE += TICKER_STEP;
}

END_OF_FUNCTION (ticker_handler);

/*------------------------------------------------------------------*/
int
start_ticker (void)
{
  int result = 0;
  //#ifdef DOS
  LOCK_FUNCTION (ticker_handler);
  LOCK_VARIABLE (TICKER_VALUE);

  result = install_int_ex (ticker_handler, MSEC_TO_TIMER (TICKER_STEP));
  //#endif
  return result;
}

/*------------------------------------------------------------------*/
void
stop_ticker (void)
{
  //#ifdef DOS
  remove_int (ticker_handler);
  //#endif
}

/*------------------------------------------------------------------*/
int
get_ticker (void)
{
  //#ifdef DOS
  return TICKER_VALUE;
  //#else
  //return ++TICKER_VALUE;
  //#endif
}
