/*
* Random circles.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include "map.h"


void circles()
{
     int r, c;
     int centerx, centery;
     int radius;
     int size;


     /* whats the max radius */
     size = (map.sec_width < map.sec_height ? map.sec_width : map.sec_height)/2.0;
     size = (size == 0 ? 1 : size);

     for( r = 0; r < map.num_row; r++ )
     {
          for( c = 0; c < map.num_col; c++ )
          {
               rand_point_section_offset(&centerx, &centery, r, c, 0);
               radius = rand()%size+1;
               circlefill(map.map, centerx, centery, radius, 0);
          }
     }

return;
}
