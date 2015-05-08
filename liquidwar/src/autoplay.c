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
/* nom           : autoplay.c                                       */
/* contenu       : l'ordino joue tout seul                          */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>

#include "army.h"
#include "autoplay.h"
#include "cursor.h"
#include "fighter.h"
#include "mesh.h"
#include "move.h"
#include "time.h"
#include "config.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_AUTOPLAY_RANDOM_LIMIT 10000

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

static char COMPUTER_PATH_KEYS[NB_TEAMS][COMPUTER_PATH_MAX];
static int COMPUTER_PATH_SIZE[NB_TEAMS];
static int COMPUTER_PATH_WAIT[NB_TEAMS];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
calculate_computer_path (int dst_x, int dst_y, int cursor)
{
  int team, src_x, src_y, pos, dir, x, y, dx, dy;
  MESH *path_mesh, *src_mesh;
  int sens, start;
  int table;

  table = GLOBAL_CLOCK % 2;

  team = CURRENT_CURSOR[cursor].team;

  src_x = CURRENT_CURSOR[cursor].x;
  src_y = CURRENT_CURSOR[cursor].y;
  src_mesh = CURRENT_AREA[src_y * CURRENT_AREA_W + src_x].mesh;

  x = dst_x;
  y = dst_y;
  path_mesh = CURRENT_AREA[y * CURRENT_AREA_W + x].mesh;
  pos = sens = start = 0;
  while (pos < COMPUTER_PATH_MAX && path_mesh != 0 && path_mesh != src_mesh)
    {
      dir = get_main_dir (path_mesh, team, sens, start);

      x += (dx = FIGHTER_MOVE_X[table][dir][0]);
      y += (dy = FIGHTER_MOVE_Y[table][dir][0]);

      path_mesh = CURRENT_AREA[y * CURRENT_AREA_W + x].mesh;
      if (path_mesh)
	{
	  COMPUTER_PATH_KEYS[cursor][pos] = 0;
	  if (dy > 0)
	    COMPUTER_PATH_KEYS[cursor][pos] += CURSOR_KEY_UP;
	  if (dx < 0)
	    COMPUTER_PATH_KEYS[cursor][pos] += CURSOR_KEY_RIGHT;
	  if (dy < 0)
	    COMPUTER_PATH_KEYS[cursor][pos] += CURSOR_KEY_DOWN;
	  if (dx > 0)
	    COMPUTER_PATH_KEYS[cursor][pos] += CURSOR_KEY_LEFT;
	  pos++;
	}
      start++;
      if (start == NB_DIRS)
	{
	  start = 0;
	  sens = !sens;
	}
    }

  if (path_mesh == src_mesh)
    {
      while (pos < COMPUTER_PATH_MAX && (x != src_x || y != src_y))
	{
	  COMPUTER_PATH_KEYS[cursor][pos] = 0;

	  if (y < src_y)
	    {
	      y++;
	      COMPUTER_PATH_KEYS[cursor][pos] += CURSOR_KEY_UP;
	    }
	  if (x > src_x)
	    {
	      x--;
	      COMPUTER_PATH_KEYS[cursor][pos] += CURSOR_KEY_RIGHT;
	    }
	  if (y > src_y)
	    {
	      y--;
	      COMPUTER_PATH_KEYS[cursor][pos] += CURSOR_KEY_DOWN;
	    }
	  if (x < src_x)
	    {
	      x++;
	      COMPUTER_PATH_KEYS[cursor][pos] += CURSOR_KEY_LEFT;
	    }
	  pos++;
	}
      COMPUTER_PATH_SIZE[cursor] = pos;
    }
  else
    COMPUTER_PATH_SIZE[cursor] = 0;

  COMPUTER_PATH_WAIT[cursor] = COMPUTER_PATH_SIZE[cursor];
}

/*------------------------------------------------------------------*/
void
reset_computer_path (void)
{
  int i;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      COMPUTER_PATH_SIZE[i] = 0;
      COMPUTER_PATH_WAIT[i] = 0;
    }
}

/*------------------------------------------------------------------*/
static int
random_free_xy_by_control_type (int *x, int *y, int team, int control_type)
{
  int i, found = 0, foundable = 0;
  int control_type_array[NB_TEAMS];
  int random_limit;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      control_type_array[i] = CONFIG_CONTROL_TYPE_OFF;
    }

  for (i = 0; i < NB_TEAMS; ++i)
    {
      if ((CURRENT_CURSOR[i].control_type == control_type
	   || control_type == CONFIG_CONTROL_TYPE_OFF)
	  && CURRENT_CURSOR[i].active)
	{
	  control_type_array[CURRENT_CURSOR[i].team] = control_type;
	}
    }

  for (i = 0; i < CURRENT_ARMY_SIZE && !foundable; ++i)
    {
      foundable |= ((CURRENT_ARMY[i].team != team) &&
		    (control_type_array[(int) (CURRENT_ARMY[i].team)] ==
		     control_type
		     || control_type == CONFIG_CONTROL_TYPE_OFF));
    }

  if (foundable)
    {
      random_limit = 0;

      while ((!found) && random_limit < LW_AUTOPLAY_RANDOM_LIMIT)
	{
	  i = random () % CURRENT_ARMY_SIZE;
	  found = ((CURRENT_ARMY[i].team != team) &&
		   (control_type_array[(int) (CURRENT_ARMY[i].team)] ==
		    control_type || control_type == CONFIG_CONTROL_TYPE_OFF));
	  random_limit++;
	}
    }

  if (!found)
    {
      /*
       * OK, we found nothing, we a default random value
       */
      i = random () % CURRENT_ARMY_SIZE;
    }

  (*x) = CURRENT_ARMY[i].x;
  (*y) = CURRENT_ARMY[i].y;

  return found;
}

/*------------------------------------------------------------------*/
static void
random_free_xy_different_team (int *x, int *y, int team)
{
  int control_type = 0;

  switch (LW_CONFIG_CURRENT_RULES.cpu_vs_human)
    {
    case CONFIG_CPU_VS_HUMAN_ALWAYS:
      control_type = CONFIG_CONTROL_TYPE_HUMAN;
      break;
    case CONFIG_CPU_VS_HUMAN_NEVER:
      control_type = CONFIG_CONTROL_TYPE_CPU;
      break;
    default:
      control_type = CONFIG_CONTROL_TYPE_OFF;
      break;
    }

  if (!random_free_xy_by_control_type (x, y, team, control_type))
    {
      /*
       * We found nobody in the right category, we fallback
       * on the the default search.
       */
      random_free_xy_by_control_type (x, y, team, CONFIG_CONTROL_TYPE_OFF);
    }
}

/*------------------------------------------------------------------*/
char
get_computer_next_move (int cursor)
{
  FIGHTER *f;
  char key_info;
  int x, y;
  int meme_equipe, team;

  if (COMPUTER_PATH_SIZE[cursor] > 0)
    {
      key_info = COMPUTER_PATH_KEYS[cursor][--COMPUTER_PATH_SIZE[cursor]];
    }
  else
    {
      key_info = 0;
      team = CURRENT_CURSOR[cursor].team;
      f = CURRENT_AREA[CURRENT_CURSOR[cursor].y * CURRENT_AREA_W
		       + CURRENT_CURSOR[cursor].x].fighter;
      if (f)
	meme_equipe = (f->team == team);
      else
	meme_equipe = 1;
      if ((--COMPUTER_PATH_WAIT[cursor]) < 0 || meme_equipe)
	{
	  random_free_xy_different_team (&x, &y, team);
	  calculate_computer_path (x, y, cursor);
	}
    }

  return key_info;
}
