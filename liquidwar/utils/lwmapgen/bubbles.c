/*
* Random bubbles.
*
* start with a wall-filled field - then erase a circle,
* then fill a smaller one inside it - repeat
*
* Copyright (C) 2003, Chris Guirl
* Released under the GNU General Public License (v2)
*/

#include "map.h"
#include "misc.h"


void
bubbles ()
{
  int r, c, b;
  int size;
  int centerx, centery;
  int outradius, inradius, midradius;
  int bubblesx, bubblesy;	/* number of bubbles in a row or column */
  int numbubbles;

  clear_invert_map ();

  /*
   * outer is the main bubble
   * inner is the black dot in the bubble
   * what's the middle??
   */

  size = (map.sec_width > map.sec_height ? map.sec_width : map.sec_height);
  outradius = rand_num (size / 3, size);
  inradius = rand_num (outradius / 1.5, outradius * (7.0 / 8.0));
  midradius = (outradius + inradius) / 2;

  midradius = (midradius == 0 ? 1 : midradius);


  bubblesx = map.sec_width / midradius;
  bubblesy = map.sec_height / midradius;
  numbubbles = (bubblesx * bubblesy);

  numbubbles = (numbubbles == 0 ? 1 : numbubbles);


  for (r = 0; r < map.num_row; r++)
    {
      for (c = 0; c < map.num_col; c++)
	{
	  for (b = 0; b < numbubbles; b++)
	    {
	      rand_point_section_offset (&centerx, &centery, r, c, 0);

	      /*
	       *centerx *= (centerx / bubblesx);
	       *centery *= (centery / bubblesy);
	       */

	      circlefill (map.map, centerx, centery, outradius, 255);
	      circlefill (map.map, centerx, centery, inradius, 0);
	    }
	}			/* for(c..) */
    }				/* for(r..) */

  return;
}
