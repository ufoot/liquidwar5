/*
* Cuts out one big polygon, that takes up most of the map.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include "map.h"
#include "func_list.h"


void invert_rand_poly()
{
     map.color = 255;
     clear_invert_map();
     rand_poly();

return;
}
