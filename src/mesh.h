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
/* nom           : mesh.h                                           */
/* contenu       : calcul du maillage                               */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_MESH
#define LIQUID_WAR_INCLUDE_MESH

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>

#include "base.h"

/*==================================================================*/
/* constantes                                                       */
/*==================================================================*/

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

typedef union
{
  int time;
  struct
  {
    short x;
    short y;
  }
  cursor;
}
MESH_UPDATE;

typedef struct
{
  int dir:8;
  int grad:24;
}
MESH_STATE;

typedef struct
{
  MESH_UPDATE update;
  MESH_STATE state;
}
MESH_INFO;

typedef struct
{
  // char decal_for_dir :8; -> causes GPF under Win32.
  // MSVC allocates 64 bits for the struct if decal_for_dir
  // is declared as a char, and 32 bit if it's declared
  // as an int. Go figure...
  int decal_for_dir:8;
  int size:24;
}
MESH_SIDE;

typedef struct
{
  short x;
  short y;
  MESH_SIDE side;
  MESH_INFO info[NB_TEAMS];
  void *link[NB_DIRS];
}
MESH;

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

extern MESH *CURRENT_MESH;
extern int CURRENT_MESH_SIZE;
extern int CURRENT_AREA_W;
extern int CURRENT_AREA_H;
extern ALLEGRO_BITMAP *CURRENT_AREA_DISP;
extern ALLEGRO_BITMAP *CURRENT_AREA_BACK;

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void reset_mesh (void);
void test_mesh (void);
int create_mesh (void);
ALLEGRO_BITMAP *create_mesh_bitmap (int mode);

#endif
