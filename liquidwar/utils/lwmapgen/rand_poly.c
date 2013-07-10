/*
* One big, solid, random polygon that takes up most of the map.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include "map.h"
#include "misc.h"

#define PAD     5

void
rand_poly ()
{
  int r, c;
  int num_vert;
  int *vert;
  int v;


  /* -4 is for the corners */
  num_vert = (2 * map.num_col) + (2 * map.num_row) - 4;
  vert = create_array (num_vert * 2, sizeof (int));

  v = 0;

  /* top */
  r = 0;
  for (c = 0; c < map.num_col; c++)
    {
      rand_point_section_offset (&vert[v], &vert[v + 1], r, c, PAD);
      v += 2;
    }


  /* right side */
  c = map.num_col - 1;
  for (r = 1; r < map.num_row; r++)
    {
      rand_point_section_offset (&vert[v], &vert[v + 1], r, c, PAD);
      v += 2;
    }


  /* bottom */
  r = map.num_row - 1;
  for (c = map.num_col - 2; c >= 0; c--)
    {
      rand_point_section_offset (&vert[v], &vert[v + 1], r, c, PAD);
      v += 2;
    }


  /* left side */
  c = 0;
  for (r = map.num_row - 2; r >= 1; r--)
    {
      rand_point_section_offset (&vert[v], &vert[v + 1], r, c, PAD);
      v += 2;
    }


  /* draw poly */
  polygon (map.map, num_vert, vert, map.color);

  delete_array (vert);

  return;
}
