/*
* Makes a bunch random lines.
*
* Copyright (C) 2003, David Redick, Chris Guirl
* Released under the GNU General Public License (v2)
*/

#include "map.h"


void
cut ()
{
  int i, size;
  int startx, starty, endx, endy;
  int fromr, fromc, tor, toc;
  int vert[8];


  size =
    (map.sec_width > map.sec_height ? map.sec_width : map.sec_height) / 8.0;
  size = (size == 0 ? 1 : size);

  /* vertical */
  fromr = 0;
  tor = map.num_row - 1;
  for (i = 0; i < map.num_col; i += 2)
    {
      fromc = rand () % map.num_col;
      toc = rand () % map.num_col;

      section_center (&startx, &starty, fromr, fromc);
      section_center (&endx, &endy, tor, toc);

      vert[0] = startx - size;
      vert[1] = 1;
      vert[2] = startx + size;
      vert[3] = 1;

      vert[4] = endx + size;
      vert[5] = map.height - 1;
      vert[6] = endx - size;
      vert[7] = map.height - 1;

      polygon (map.map, 4, vert, 255);
    }

  /* horizontial */
  fromc = 0;
  toc = map.num_col - 1;
  for (i = 0; i < map.num_row; i += 2)
    {
      fromr = rand () % map.num_row;
      tor = rand () % map.num_row;

      section_center (&startx, &starty, fromr, fromc);
      section_center (&endx, &endy, tor, toc);

      vert[0] = 1;
      vert[1] = starty + size;
      vert[2] = 1;
      vert[3] = starty - size;

      vert[4] = map.width - 2;
      vert[5] = endy - size;
      vert[6] = map.width - 2;
      vert[7] = endy + size;

      polygon (map.map, 4, vert, 255);
    }

  return;
}
