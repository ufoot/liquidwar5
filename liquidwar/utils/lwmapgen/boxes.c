/*
* A bunch of boxes of the same size.
*
* This works best if r/c are large and close to each other.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include "map.h"


void boxes()
{
     int r, c;
     int startx, starty, endx, endy;
     int pad;

     pad = 1;

     for( r = 0; r < map.num_row; r++ )
     {
          for( c = 0; c < map.num_col; c++ )
          {
               /* 1 out 2 chance to draw box */
               if( rand()%2 == 0 )
               {
                    /*
                    * if box is on the edge of the map i use
                    * the absolute pixel
                    */
                    if( c == 0 )
                         startx = pad+1;
                    else
                         startx = map.sec_width*c+pad;

                    if( r == 0 )
                         starty = pad+1;
                    else
                         starty = map.sec_height*r+pad;

                    if( c == map.num_col-1 )
                         endx = map.width-pad-2;
                    else
                         endx = map.sec_width*(c+1)-pad;

                    if( r == map.num_row-1 )
                         endy = map.height-pad-2;
                    else
                         endy = map.sec_height*(r+1)-pad;

                    rectfill(map.map, startx, starty, endx, endy, 0);
               }
          }
     }

return;
}
