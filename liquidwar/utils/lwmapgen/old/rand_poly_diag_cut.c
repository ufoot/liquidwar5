/*
* Creates one big random polygon, that takes up most of the map
* and cuts slices out of it.  Slices can are cut diagonal.
*
* Copyright (C) 2003, David Redick, Chris Guirl
* Released under the GNU General Public License (v2)
*/

#include <stdio.h>

#include "map.h"
#include "func_list.h"


void rand_poly_diag_cut()
{
     int section_width, section_height;
     int xoffset, yoffset;
     int r, c;
     int way, start;
     /* int cutx, cuty; */
     int cutsize;
     int *vert;
     

     vert = malloc( sizeof(int) * 4 * 2);
     if( vert == NULL )
     {
          fprintf(stderr, "fatal error: can't malloc space for verts.\n");
          exit(EXIT_FAILURE);
     }


     section_size(&section_width, &section_height);

     rand_poly();

/*     int i;
     for ( i = 0; i < rand()%10; i++ )
     {
*/
         /* was %2... fixed so that both slanted
          * rows & cols could happen -- david
          */
          way = rand()%3;
          
          /* cut slanted rows */
          if( way == 0 || way == 2 )
          {
               start = rand()%2;
               for( r = start; r < map.num_row; r += 2 )
               {
		   
                    offset(&xoffset, &yoffset, r, 0);

                    cutsize = rand()%(section_height/2-map.max_pad)+1;

                    /* following 2 lines removed by c. guirl 2003/11/09
                    cuty = section_height/2 + yoffset;
                    rectfill(map.map, 1, cuty-cutsize, map.width-2, cuty+cutsize, 255);
                    */

                    int topdist_left = rand()%(section_width-cutsize);
                    int topdist_right = rand()%(section_width-cutsize);

                    vert[0] = 1;
                    vert[1] = yoffset + topdist_left;

                    vert[2] = map.width-2;
                    vert[3] = yoffset + topdist_right;

                    vert[4] = map.width-2;
                    vert[5] = yoffset + topdist_right + cutsize;

                    vert[6] = 1;
                    vert[7] = yoffset + topdist_left + cutsize;

                    polygon (map.map, 4, vert, 255);
 	          }
          }

          /* cut slanted cols */
          if( way == 1 || way == 2 )
          {
               start = rand()%2;
               for( c = start; c < map.num_col; c += 2 )
               {
                    offset(&xoffset, &yoffset, 0, c);

                    cutsize = rand()%(section_width/2-map.max_pad)+1;

                    /* following 2 lines removed by c. guirl 2003/11/09
                    cutx = section_width/2 + xoffset;
                    rectfill(map.map, cutx-cutsize, 1, cutx+cutsize, map.height-2, 255);
                    */


                    int leftdist_top = rand()%(section_width-cutsize);
                    int leftdist_bottom = rand()%(section_width-cutsize);

                    vert[0] = xoffset + leftdist_top;
                    vert[1] = 1;

                    vert[2] = xoffset + leftdist_bottom;
                    vert[3] = map.height-2;

                    vert[4] = xoffset + leftdist_bottom + cutsize;
                    vert[5] = map.height-2;

                    vert[6] = xoffset + leftdist_top + cutsize;
                    vert[7] = 1;

                    polygon (map.map, 4, vert, 255);
               }
          }
/*     } */

return;
}
