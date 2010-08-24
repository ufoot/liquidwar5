/*
* A map that looks like a bunch of streets.
*
* This works best if r & c are rather large (but not too large).
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/


#include <stdlib.h>

#include "map.h"
#include "misc.h"


#define MAX_MAIN_ROW     3
#define MAX_MAIN_COL     3

void street()
{
     int main_row[MAX_MAIN_ROW];
     int main_col[MAX_MAIN_COL];
     int startx, starty, endx, endy;
     int r, c, tor, toc;
     int size;
     int start, way;
     int i;


     clear_invert_map();

     /*
     * TODO: its possible only have 1 main and thats bad...
     * 2x3 is best... tho' it may not matter much.
     * What about basing the numbers off map.num_row/col??
     * yeah I like that better than macros.
     * maybe MMR = #rows % 4 + 1..
     */

     /* pick the main rows/cols to branch from */
     for( i = 0; i < MAX_MAIN_ROW; i++ )
          main_row[i] = rand()%map.num_row;
     for( i = 0; i < MAX_MAIN_COL; i++ )
          main_col[i] = rand()%map.num_col;


     size = (map.sec_width > map.sec_height ? map.sec_width : map.sec_height)/8;
     size = (size == 0 ? 1 : size);


     /* branch off rows */
     for( i = 0; i < MAX_MAIN_ROW; i++ )
     {
          r = main_row[i];

          /* draw main row */
          section_center(&startx, &starty, r, 0);
          section_center(&endx, &endy, r, map.num_col-1);
          rectfill(map.map, startx, starty-size, endx, endy+size, 255);


          /* do the branches */
          start = rand()%2;
          for( c = start; c < map.num_col; c += 2 )
          {
               section_center(&startx, &starty, r, c);

               way = rand()%3;

               /* draw up */
               if( r != 0 && (way == 0 || way == 2) )
               {
                    tor = rand()%r;
                    section_center(&endx, &endy, tor, c);
                    rectfill(map.map, startx-size, starty, endx+size, endy, 255);
               }

               /* draw down */
               if( r != map.num_row-1 && (way == 1 || way == 2) )
               {

                    tor = rand_num(r, map.num_row-r);
                    /* tor = rand()%(map.num_row-r-1)+r+1; */
                    section_center(&endx, &endy, tor, c);
                    rectfill(map.map, startx-size, starty, endx+size, endy, 255);
               }               
          }
     }


     /* branch off cols */
     for( i = 0; i < MAX_MAIN_COL; i++ )
     {
          c = main_col[i];

          /* draw main col */
          section_center(&startx, &starty, 0, c);
          section_center(&endx, &endy, map.num_row-1, c);
          rectfill(map.map, startx-size, starty, endx+size, endy, 255);


          /* do the branches */
          start = rand()%2;
          for( r = start; r < map.num_row; r += 2 )
          {
               section_center(&startx, &starty, r, c);

               way = rand()%3;

               /* draw left */
               if( c != 0 && (way == 0 || way == 2) )
               {
                    toc = rand()%c;
                    section_center(&endx, &endy, r, toc);
                    rectfill(map.map, startx, starty-size, endx, endy+size, 255);
               }

               /* draw right */
               if( c != map.num_col-1 && (way == 1 || way == 2) )
               {
                    toc = rand_num(c, map.num_col-c);
                    /* toc = rand()%(map.num_col-c-1)+c+1; */
                    section_center(&endx, &endy, r, toc);
                    rectfill(map.map, startx-size, starty-size, endx, endy+size, 255);
               }               
          }
     }

return;
}
