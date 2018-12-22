/*
* Random boxes.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include "map.h"


void
rand_box ()
{
  int r, c;
  int xoffset, yoffset;

  /* the start is the upper left */
  int starty, startx;
  int bwidth, bheight;


  for (r = 0; r < map.num_row; r++)
    {
      for (c = 0; c < map.num_col; c++)
        {
          offset (&xoffset, &yoffset, r, c);

          rand_point_section (&startx, &starty, 1);

          /* +1 because we don't want 0 dimensions =] */
          bwidth = rand () % (int) map.sec_width + 1;
          bheight = rand () % (int) map.sec_height + 1;

          /*
           * if the box goes outside the section
           * then just make the box smaller
           * NOTE: -1 is for the padding
           */
          if (startx + bwidth > map.sec_width - 1)
            bwidth -= (startx + bwidth) - (map.sec_width - 1);

          if (starty + bheight > map.sec_height - 1)
            bheight -= (starty + bheight) - (map.sec_height - 1);

          rectfill (map.map, startx + xoffset, starty + yoffset,
                    startx + bwidth + xoffset, starty + bheight + yoffset, 0);
        }
    }

  return;
}
