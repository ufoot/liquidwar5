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
/* nom           : cursor.h                                         */
/* contenu       : placement et initialisation des curseurs         */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_CURSOR
#define LIQUID_WAR_INCLUDE_CURSOR

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "base.h"

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

typedef struct
{
  /*
   * value which will be used when the
   * will be put into the map. this  
   * value decreases at each round.  
   */
  int val;
  /* x position of the cursor         */
  int x;
  /* y position of the cursor         */
  int y;
  /* 
   * true if the player who owns this 
   * cursor is playing with it        
   */
  int active;
  /* team the cursor belongs to       */
  int team;
  /* 
   * time at which this cursor has    
   * lost. it seems that the diff     
   * between cursor and team is not   
   * well defined 8-(                 
   */
  int loose_time;
  /* wether you are 1st or 2nd...     */
  int score_order;
  /* 
   * first color used by this cursor  
   * in the 256 color palette. again  
   * a cursor is associated to a team 
   */
  int color_entry;
  /* 
   *      this value holds the state of all
   * the keys associated to the cursor
   * it is a logical or between   
   * - CURSOR_KEY_UP    
   * - CURSOR_KEY_RIGHT 
   * - CURSOR_KEY_DOWN  
   * - CURSOR_KEY_LEFT  
   * which are defined in move.h      
   */
  int key_state;
  /* true if the player is a computer */
  int control_type;
  /* 
   * true if the player is located on 
   * a remote machine                 
   */
  int from_network;
  /*
   * the name of the player
   */
  char name[NAME_SIZE + 1];
}
CURSOR;

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

extern CURSOR CURRENT_CURSOR[NB_TEAMS];

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void reset_all_cursor (void);
void apply_all_cursor (void);
void auto_cursor (int index, int team, char *name);

#endif
