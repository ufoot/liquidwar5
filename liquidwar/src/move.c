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
/* nom           : move.c                                           */
/* contenu       : deplacement des curseurs                         */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>

#include "area.h"
#include "autoplay.h"
#include "config.h"
#include "cursor.h"
#include "joystick.h"
#include "keyboard.h"
#include "mesh.h"
#include "mouse.h"
#include "move.h"
#include "time.h"
#include "keyexch.h"
#include "network.h"
#include "checksum.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define LW_MOVE_SIDE_LIMIT 10

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * moves a cursor in a given direction if the place is free
 * one just has to give the x & y values of the cursors as params
 */
static int
move_if_free (int *x, int *y, int dir)
{
  int moved = 0;

  /*
   * we check if the direction is between 1 and 8
   * if not force it into this range
   */
  while (dir > 8)
    dir -= 8;
  while (dir < 1)
    dir += 8;

  /*
   * switch between all dirs
   * in all the cases, the method is the same, we check if
   * there's a mesh pointer defined for the target position
   * if yes, this implies there are no walls. check area.c and mesh.c
   * to understand while testing the mesh attribute is OK.
   * then we keep track of the result of the move, for later
   * we'll need to know if the cursor has really been moved
   * this result is stored in the moved value
   */
  switch (dir)
    {
      /*
       * trying to go NORTH
       */
    case 1:
      if (CURRENT_AREA[((*y) - 1) * CURRENT_AREA_W + (*x)].mesh)
	{
	  (*y)--;
	  moved = 1;
	}
      break;
      /*
       * trying to go NORTH EAST
       */
    case 2:
      if (CURRENT_AREA[((*y) - 1) * CURRENT_AREA_W + (*x) + 1].mesh)
	{
	  (*y)--;
	  (*x)++;
	  moved = 1;
	}
      break;
      /*
       * trying to go EAST
       */
    case 3:
      if (CURRENT_AREA[(*y) * CURRENT_AREA_W + (*x) + 1].mesh)
	{
	  (*x)++;
	  moved = 1;
	}
      break;
      /*
       * trying to go SOUTH EAST
       */
    case 4:
      if (CURRENT_AREA[((*y) + 1) * CURRENT_AREA_W + (*x) + 1].mesh)
	{
	  (*x)++;
	  (*y)++;
	  moved = 1;
	}
      break;
      /*
       * trying to go SOUTH
       */
    case 5:
      if (CURRENT_AREA[((*y) + 1) * CURRENT_AREA_W + (*x)].mesh)
	{
	  (*y)++;
	  moved = 1;
	}
      break;
      /*
       * trying to go SOUTH WEST
       */
    case 6:
      if (CURRENT_AREA[((*y) + 1) * CURRENT_AREA_W + (*x) - 1].mesh)
	{
	  (*y)++;
	  (*x)--;
	  moved = 1;
	}
      break;
      /*
       * trying to go WEST
       */
    case 7:
      if (CURRENT_AREA[(*y) * CURRENT_AREA_W + (*x) - 1].mesh)
	{
	  (*x)--;
	  moved = 1;
	}
      break;
      /*
       * trying to go NORTH WEST
       */
    case 8:
      if (CURRENT_AREA[((*y) - 1) * CURRENT_AREA_W + (*x) - 1].mesh)
	{
	  (*x)--;
	  (*y)--;
	  moved = 1;
	}
    }
  return moved;
}

/*------------------------------------------------------------------*/
/*
 * moves a cursor
 * in fact it interprets the key_state value , calculates the
 * direction to use and then calls move_if_free
 */
static void
move_cursor (int indice)
{
  int dir = 0, up, down, right, left, horiz = 0, vert = 0, moved = 0, team;
  char key_state;
  MESH *mesh;

  /*
   * we get the team associated to the cursor
   */
  team = CURRENT_CURSOR[indice].team;
  /*
   * we get the mesh item associated to the current position of the cursor
   */
  mesh = CURRENT_AREA[CURRENT_CURSOR[indice].y * CURRENT_AREA_W
		      + CURRENT_CURSOR[indice].x].mesh;
  /*
   * we tell the mesh that the info won't be up to date anymore concerning
   * this position
   */
  mesh->info[team].update.time = -1;

  /*
   * the key_state value gives the state of all pressed/unpressed keys
   */
  key_state = CURRENT_CURSOR[indice].key_state;

  /*
   * we interpret the content of the key_state value with binary masks
   */
  up = key_state & CURSOR_KEY_UP;
  right = key_state & CURSOR_KEY_RIGHT;
  down = key_state & CURSOR_KEY_DOWN;
  left = key_state & CURSOR_KEY_LEFT;

  /*
   * if both left and right keys are pressed, no horizontal move!
   */
  if ((right && left) || ((!right) && (!left)))
    horiz = 0;
  /*
   * if right key is pressed but not left key, let's go right
   */
  if (right && (!left))
    horiz = 1;
  /*
   * if left key is pressed but not right key, let's go left
   */
  if ((!right) && left)
    horiz = -1;

  /*
   * if both up and down keys are pressed, no verical move!
   */
  if ((up && down) || ((!up) && (!down)))
    vert = 0;
  /*
   * if up key is pressed but not down key, let's go up
   */
  if (up && (!down))
    vert = -1;
  /*
   * if down key is pressed but not up key, let's go down
   */
  if ((!up) && down)
    vert = 1;

  /*
   * the cursor does not move at all
   */
  if (horiz == 0 && vert == 0)
    dir = 0;
  /*
   * the cursor heads NORTH
   */
  if (horiz == 0 && vert == -1)
    dir = 1;
  /*
   * the cursor heads NORTH EAST
   */
  if (horiz == 1 && vert == -1)
    dir = 2;
  /*
   * the cursor heads EAST
   */
  if (horiz == 1 && vert == 0)
    dir = 3;
  /*
   * the cursor heads SOUTH EAST
   */
  if (horiz == 1 && vert == 1)
    dir = 4;
  /*
   * the cursor heads SOUTH
   */
  if (horiz == 0 && vert == 1)
    dir = 5;
  /*
   * the cursor heads SOUTH WEST
   */
  if (horiz == -1 && vert == 1)
    dir = 6;
  /*
   * the cursor heads WEST
   */
  if (horiz == -1 && vert == 0)
    dir = 7;
  /*
   * the cursor heads NORTH WEST
   */
  if (horiz == -1 && vert == -1)
    dir = 8;

  /*
   * if no direction has been found, we do nothing
   */
  if (dir)
    {
      /*
       * we first try to move in the direction we have calculated
       */
      moved = move_if_free (&(CURRENT_CURSOR[indice].x),
			    &(CURRENT_CURSOR[indice].y), dir);
      /*
       * if it did not work, we try another direction
       * for instance if the direction was NORTH WEST we try WEST
       */
      if (!moved)
	moved = move_if_free (&(CURRENT_CURSOR[indice].x),
			      &(CURRENT_CURSOR[indice].y), dir - 1);
      /*
       * if it did not work, we try another direction
       * for instance if the direction was NORTH WEST we try NORTH
       */
      if (!moved)
	moved = move_if_free (&(CURRENT_CURSOR[indice].x),
			      &(CURRENT_CURSOR[indice].y), dir + 1);
      /*
       * The following code allows the cursor to really "slip" on walls.
       * Not very elegant, but it works 8-)
       */
      if (!moved)
	{
	  int xa1, ya1;
	  int xb1, yb1;
	  int xa2, ya2;
	  int xb2, yb2;
	  int i = 0;

	  xa1 = xb1 = CURRENT_CURSOR[indice].x;
	  ya1 = yb1 = CURRENT_CURSOR[indice].y;

	  while (!moved && i < LW_MOVE_SIDE_LIMIT)
	    {
	      if (move_if_free (&xa1, &ya1, dir - 2))
		{
		  xa2 = xa1;
		  ya2 = ya1;
		  if (move_if_free (&xa2, &ya2, dir - 1))
		    {
		      /*
		       * Normally moved should be true after this
		       */
		      moved = move_if_free (&(CURRENT_CURSOR[indice].x),
					    &(CURRENT_CURSOR[indice].y),
					    dir - 2);
		    }
		}

	      if (move_if_free (&xb1, &yb1, dir + 2))
		{
		  xb2 = xb1;
		  yb2 = yb1;
		  if (move_if_free (&xb2, &yb2, dir + 1))
		    {
		      /*
		       * Normally moved should be true after this
		       */
		      moved = move_if_free (&(CURRENT_CURSOR[indice].x),
					    &(CURRENT_CURSOR[indice].y),
					    dir + 2);
		    }
		}

	      i++;
	    }
	}
    }
  /*
   * now this is quite a subtle line
   * we decrement the value of the cursor when
   * - the cursor has moved, this is required since it means
   *   that the cursor is now 1 pixel farer from all the fighters
   *   as long as the gradient spreading has proven the contrary
   * or
   * - a condition base on the global clock is filled.
   *   this avoids cyclic behaviors and some "lost" fighters which
   *   might otherwise start looping in a corner of the map
   */
  if (moved || !(GLOBAL_CLOCK % (NB_DIRS + 1)))
    {
      CURRENT_CURSOR[indice].val--;
    }

  /*
   * we retrieve the new mesh item associated to the new position
   */
  mesh = CURRENT_AREA[CURRENT_CURSOR[indice].y * CURRENT_AREA_W
		      + CURRENT_CURSOR[indice].x].mesh;
  /*
   * we tell the mesh where the cursor is
   * this information is redundant but it avoids complex things such as 
   * my_struct->....->...   ...->...->my_attribute;
   */
  mesh->info[team].update.cursor.x = CURRENT_CURSOR[indice].x;
  mesh->info[team].update.cursor.y = CURRENT_CURSOR[indice].y;
}

/*------------------------------------------------------------------*/
/*
 * updates the key_state value for a local player, ie polls the keys
 * it might also poll the joystick but this is transparent
 */
static void
update_key_local_cursor (int indice)
{
  /*
   * the following code is pretty much the same for each key.
   * the is_touched_key will automatically poll the right device,
   * keyboard, mouse or joystick and return true if the logical
   * is in a "pressed" state
   */
  CURRENT_CURSOR[indice].key_state
    = is_touched_key (CONFIG_KEY_UP[indice]) ? CURSOR_KEY_UP : 0;
  CURRENT_CURSOR[indice].key_state
    += is_touched_key (CONFIG_KEY_RIGHT[indice]) ? CURSOR_KEY_RIGHT : 0;
  CURRENT_CURSOR[indice].key_state
    += is_touched_key (CONFIG_KEY_DOWN[indice]) ? CURSOR_KEY_DOWN : 0;
  CURRENT_CURSOR[indice].key_state
    += is_touched_key (CONFIG_KEY_LEFT[indice]) ? CURSOR_KEY_LEFT : 0;
}

/*------------------------------------------------------------------*/
/*
 * updates the key_state for a cursor which is AI driven
 */
static void
update_key_cpu_cursor (int indice)
{
  /*
   * get_computer_next_move is the main AI function
   */
  CURRENT_CURSOR[indice].key_state = get_computer_next_move (indice);
}

/*------------------------------------------------------------------*/
/*
 * moves all the cursors, (logical, eh?)
 * the kind of function one might wish to use in the main game loop
 */
void
move_all_cursors (void)
{
  int i, j;
  int checksum;

  /*
   * polls the module which wraps mouse moves to virtual keys
   */
  lw_mouse_update_control ();
  /*
   * polls the joystick
   */
  my_poll_joystick ();
  /*
   * one might wish to poll network stuff here but I'm not sure it's
   * the right place because of performance issues
   */

  for (j = 0; j <= LW_CONFIG_CURRENT_RULES.cursor_increase_speed; ++j)
    {
      /*
       * loop for all the teams, the cursor of which is active
       */
      for (i = 0; i < NB_TEAMS; ++i)
	{
	  if (CURRENT_CURSOR[i].active)
	    {
	      /*
	       * If the cursor is network controlled, we do nothing yet
	       */
	      if (!CURRENT_CURSOR[i].from_network)
		{
		  if (CURRENT_CURSOR[i].control_type ==
		      CONFIG_CONTROL_TYPE_CPU)
		    {
		      /*
		       * the cursor is controlled by the computer
		       */
		      update_key_cpu_cursor (i);
		    }
		  else
		    {
		      /*
		       * the cursor is controlled by a human local player
		       */
		      update_key_local_cursor (i);
		    }
		}
	    }
	}
      /*
       * we flush network informations if needed.
       */
      if (LW_NETWORK_ON)
	{
	  checksum = lw_checksum_calc (CURRENT_ARMY, CURRENT_ARMY_SIZE);

	  /*
	   * This call will automatically overwrite the key states
	   * with those received from the network
	   */
	  if (!lw_keyexch_flush (CURRENT_CURSOR, checksum))
	    {
	      LW_NETWORK_ERROR_DETECTED = 1;
	    }
	}

      for (i = 0; i < NB_TEAMS; ++i)
	{
	  if (CURRENT_CURSOR[i].active)
	    {
	      move_cursor (i);
	    }
	}
    }
}
