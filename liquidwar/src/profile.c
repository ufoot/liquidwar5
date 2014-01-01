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
/* nom           : profile.c                                        */
/* contenu       : chronometrage des fonctions                      */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#include "profile.h"
#include "ticker.h"

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

typedef struct
{
  int start;
  int record_number;
  int total_time;
}
PROFILE_REC;

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define PROFILE_LIMIT 20

static PROFILE_REC PROFILE_DATA[MAX_PROFILE];
static int DISPLAY_NUMBER;
static int LOGIC_NUMBER;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
reset_all_profile (void)
{
  int i;

  for (i = 0; i < MAX_PROFILE; ++i)
    reset_profile (i);
}

/*------------------------------------------------------------------*/
void
reset_profile (int indice)
{
  DISPLAY_NUMBER = LOGIC_NUMBER = 0;
  PROFILE_DATA[indice].record_number = PROFILE_DATA[indice].total_time = 0;
}

/*------------------------------------------------------------------*/
void
start_profile (int indice)
{
  PROFILE_DATA[indice].start = get_ticker ();
}

/*------------------------------------------------------------------*/
void
stop_profile (int indice)
{
  PROFILE_DATA[indice].record_number++;
  PROFILE_DATA[indice].total_time += get_ticker ()
    - PROFILE_DATA[indice].start;
  if (PROFILE_DATA[indice].record_number >= PROFILE_LIMIT)
    {
      PROFILE_DATA[indice].record_number /= 2;
      PROFILE_DATA[indice].total_time /= 2;
    }
}

/*------------------------------------------------------------------*/
int
get_profile (int indice)
{
  if (PROFILE_DATA[indice].record_number > 0)
    return (PROFILE_DATA[indice].total_time
	    / PROFILE_DATA[indice].record_number);
  else
    return 0;
}

/*------------------------------------------------------------------*/
void
update_logic_rate (int logic_number)
{
  DISPLAY_NUMBER++;
  LOGIC_NUMBER += logic_number;

  if (DISPLAY_NUMBER > PROFILE_LIMIT)
    {
      DISPLAY_NUMBER /= 2;
      LOGIC_NUMBER /= 2;
    }
}

/*------------------------------------------------------------------*/
int
get_logic_rate (void)
{
  return ((get_profile (GLOBAL_PROFILE) * DISPLAY_NUMBER) / LOGIC_NUMBER);
}
