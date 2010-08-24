/*
* One big, solid quad that takes up most of the map.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include "map.h"
#include "misc.h"


void big_quad()
{
     int vert[8];

     /*
     * NOTE: we don't want to overlap the areas or we get strange polys
     * The 2 and the "-3" are to make sure the quad doesn't touch the
     * edge of the map.
     */

     /* left side of map */
     vert[0] = rand_num(2, map.sec_width);
     vert[1] = rand_num(2, map.height-3);

     /* top side of map */
     vert[2] = rand_num(map.sec_width+1, map.width-3);
     vert[3] = rand_num(2, map.sec_height);
     
     /* right side of map */
     vert[4] = rand_num(map.width-map.sec_width , map.width-3);
     vert[5] = rand_num(map.sec_height+1, map.height-3);

     /* bottom side of map */
     vert[6] = rand_num(map.sec_width+1, map.width-map.sec_width-1);
     vert[7] = rand_num(map.height-map.sec_height, map.height-3);

     polygon(map.map, 4, vert, 0);

return;
}
