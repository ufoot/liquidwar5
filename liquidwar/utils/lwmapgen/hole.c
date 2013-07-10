/*
* Randomly shaped hole.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include "map.h"
#include "func_list.h"


void
hole ()
{
  map.color = 255;
  clear_invert_map ();
  rand_poly ();

  return;
}
