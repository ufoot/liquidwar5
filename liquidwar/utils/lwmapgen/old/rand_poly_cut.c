/*
* This creates one big polygon and cuts slices out of it.
* Cuts can be vertical, horizontial or both.
*
* Added a 1/3 chance of cutting.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include <stdio.h>

#include "map.h"
#include "func_list.h"


void rand_poly_cut()
{
     int section_width, section_height;
     int xoffset, yoffset;
     int r, c;
     int way, start;
     int cutx, cuty;
     int cutsize;


     section_size(&section_width, &section_height);


     rand_poly();


     way = rand()%3;

     /* cut along rows, horziontial cuts */
     if( way == 0 || way == 2 )
     {
          start = rand()%2;
          for( r = start; r < map.num_row; r += 2 )
          {
               if( rand()%3 != 0 )
                    continue;

               offset(&xoffset, &yoffset, r, 0);

               cutsize = rand()%(section_height/2-map.max_pad)+1;
               cuty = section_height/2 + yoffset;
               rectfill(map.map, 1, cuty-cutsize, map.width-2, cuty+cutsize, 255);
          }
     }

     /* cut along cols, vertical cuts */
     if( way == 1 || way == 2 )
     {
          start = rand()%2;
          for( c = start; c < map.num_col; c += 2 )
          {
               if( rand()%3 != 0 )
                    continue;

               offset(&xoffset, &yoffset, 0, c);

               cutsize = rand()%(section_width/2-map.max_pad)+1;
               cutx = section_width/2 + xoffset;
               rectfill(map.map, cutx-cutsize, 1, cutx+cutsize, map.height-2, 255);
          }
     }
     
return;
}
