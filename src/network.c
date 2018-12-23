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
/* Copyright (C) 1998-2018 Christian Mauduit                                 */
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
/* name          : network.c                                        */
/* content       : various network related stuff                    */
/* last update   : may 8th 2001                                     */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>

#include <allegro.h>

#include "network.h"
#include "config.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_NETWORK_COLOR_NB               12
#define LW_NETWORK_COLOR_DIST_MAXINT 1000000
#define LW_NETWORK_COLOR_DIST_SCALE     1000

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

int LW_NETWORK_ON = 0;
LW_WHO LW_NETWORK_INFO[NB_TEAMS];
int LW_NETWORK_ERROR_DETECTED = 0;
void *LW_NETWORK_RAW_MAP = NULL;

/*==================================================================*/
/* static funcs                                                     */
/*==================================================================*/


/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Returns the "distance" of 2 colors. For 2 adjacent which look
 * about the sane, it is 1. For radicaly different colors, it is
 * 6. If the colors are the same, it is 0.
 */
int
distance_between_colors (int col1, int col2)
{
  int dist;

  if (col1 < 0 || col2 < 0)
    {
      /*
       * Now if one of the member was -1, we
       * consider the colors are "far"
       * from each other.
       */
      dist = 6;
    }
  else
    {
      dist = abs (col1 - col2);
    }

  if (dist > LW_NETWORK_COLOR_NB)
    {
      /*
       * Normally we should never get here...
       */
      dist = LW_NETWORK_COLOR_NB / 2;
    }

  if (dist > LW_NETWORK_COLOR_NB / 2)
    {
      dist = LW_NETWORK_COLOR_NB - dist;
    }

  return dist;
}

/*------------------------------------------------------------------*/
/*
 * Calculates the "most different color", considering the colors 
 * already chosen in LW_NETWORK_INFO.
 */
int
choose_different_color ()
{
  int color;
  int closeness[LW_NETWORK_COLOR_NB];
  int closeness_min;
  int dist;
  int i, j;

  /*
   * We calculate the "closeness" of each color
   */
  for (i = 0; i < LW_NETWORK_COLOR_NB; ++i)
    {
      closeness[i] = 0;
      for (j = 0; j < NB_TEAMS; ++j)
        {
          if (LW_NETWORK_INFO[j].active)
            {
              dist = distance_between_colors (LW_NETWORK_INFO[j].color, i);
              if (dist > 0)
                {
                  if (LW_NETWORK_INFO[j].network)
                    {
                      /*
                       * We consider network color conflicts
                       * to be less important than conflicts with
                       * local colors, therefore we increase the
                       * distance for network teams.
                       */
                      dist++;
                    }
                  closeness[i] += LW_NETWORK_COLOR_DIST_SCALE / (dist * dist);
                }
              else
                {
                  closeness[i] = LW_NETWORK_COLOR_DIST_MAXINT;
                }
            }
        }
    }

  /*
   * And now we pick up the one that has the less common points
   * with existing colors.
   */
  closeness_min = LW_NETWORK_COLOR_DIST_MAXINT;
  color = 0;
  for (i = 0; i < LW_NETWORK_COLOR_NB; ++i)
    {
      if (closeness[i] < closeness_min)
        {
          color = i;
          closeness_min = closeness[i];
        }
    }

  return color;
}

/*------------------------------------------------------------------*/
/*
 * Associates a color to each team 
 */
void
lw_network_attribute_colors ()
{
  int i, color;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (LW_NETWORK_INFO[i].active)
        {
          if (LW_NETWORK_INFO[i].network)
            {
              /*
               * -1 is a temporary value which will be overwritten
               * by the _real_ value later
               */
              LW_NETWORK_INFO[i].color = -1;
            }
          else
            {
              LW_NETWORK_INFO[i].color = CONFIG_TEAM_COLOR[i];
            }
        }
    }

  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (LW_NETWORK_INFO[i].active && LW_NETWORK_INFO[i].network)
        {
          color = choose_different_color ();
          LW_NETWORK_INFO[i].color = color;
        }
    }
}

/*------------------------------------------------------------------*/
/*
 * Associates a part to each team 
 */
void
lw_network_attribute_parts ()
{
  int i;
  static int order[NB_TEAMS] = { 0, 5, 2, 3, 1, 4 };

  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (LW_NETWORK_INFO[i].active)
        {
          LW_NETWORK_INFO[i].part = order[LW_NETWORK_INFO[i].server_id];
        }
    }
}
