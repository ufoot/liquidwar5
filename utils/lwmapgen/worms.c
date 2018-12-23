/*
* Random little worms.
*
* Copyright (C) 2003, Chris Guirl, David Redick
* Released under the GNU General Public License (v2)
*/

#include <math.h>
#include <stdio.h>

#include "map.h"


/* math.h files do not define M_PI in ansi C  */
#ifndef M_PI
#define M_PI     3.14159265358979323846264338327950288
#endif


int
filled (int centerx, int centery, int rad, int startdeg, int col, int seg)
{
  int deg;
  int result = 0;
  int check_degrees;

  /*
   * int seg is the current segment - if this is the first segment of a
   * worm (seg = 0) check 360 degrees around it instead of just 180
   */

  if (0 == seg)
    check_degrees = 180;
  else
    check_degrees = 90;


  for (deg = startdeg - check_degrees; deg < startdeg + check_degrees;
       deg += 5)
    {

      if (getpixel
          (map.map, centerx + (rad * cos (M_PI * deg / 180)),
           centery + (rad * sin (M_PI * deg / 180))) == col)
        {
          result = 1;
          break;
        }
    }

  return result;
}


void
worms ()
{
  int r, c, segs;
  int centerx, centery;
  int radius, segments;
  int degree, change;


  /* 5-15 */
  radius = rand () % 10 + 5;
  segments = rand () % 10 + 5;


  for (r = 0; r < map.num_row; r++)
    {
      for (c = 0; c < map.num_col; c++)
        {
          rand_point_section_offset (&centerx, &centery, r, c, 0);
          degree = rand () % 360;

          for (segs = 0; segs < segments; segs++)
            {
              /* -15 to 14 */
              change = rand () % 30 - 15;
              degree += change;

              centerx += radius * cos (M_PI * degree / 180);
              centery += radius * sin (M_PI * degree / 180);

              if (filled (centerx, centery, radius + 1, degree, 0, segs) == 0)
                {
                  circlefill (map.map, centerx, centery, radius, 0);
                }
              else
                {
                  /* encountered another worm, stopping... */
                  break;
                }
            }
        }                       /* for(c..) */
    }                           /* for(r..) */

  return;
}
