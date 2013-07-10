/*
* The main interface to the map generator.
* 
* This is mainly for the game liquidwar to interface with the generator.
* For the shell interface see main.c
*
* Note: I assume that all args are valid.
*
* If filename is NULL then default filename is used.
*
* size must be MIN_MAP_SIZE to MAX_MAP_SIZE-1 inclusive.
*
* grid_size must be MIN_MAP_GRID_SIZE to MAX_MAP_GRID_SIZE-1 inclusive
* or RANDOM_MAP_GRID_SIZE
*
* func_id must be MIN_FUNC to MAX_FUNC-1 inclusive
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"


void
map_generator (char *filename, int size, int grid_size, int func_id)
{
  char *fname;

  if (filename == NULL)
    fname = default_filename;
  else
    fname = filename;

  map.filename = malloc (sizeof (char) * strlen (fname));
  if (map.filename == NULL)
    {
      fprintf (stderr, "fatal error: can't malloc space for map filename.\n");
      exit (EXIT_FAILURE);
    }
  strcpy (map.filename, fname);

  printf ("map.filename = %s\n", map.filename);

  map.width = map_size[size][0];
  map.height = map_size[size][1];

  map.map = create_bitmap_ex (8, map.width, map.height);

  if (grid_size == RANDOM_MAP_GRID_SIZE)
    grid_size = rand_num (MIN_MAP_GRID_SIZE, MAX_MAP_GRID_SIZE - 1);
  map.num_row = map_grid_size[grid_size][0];
  map.num_col = map_grid_size[grid_size][1];

  map.sec_width = (float) map.width / (float) map.num_col;
  map.sec_height = (float) map.height / (float) map.num_row;

  map.color = 0;

  map.func_id = func_id;
  map.func = func[func_id].func;


  clear_map ();

  printf ("Generating random map using:\n\t%2d)  %s\t%s\n",
	  func_id, func[func_id].name, func[func_id].desc);

  print_map ();

  (*map.func) ();

  print_map ();

  return;
}
