/*
* Do rand_poly then cut lines across it.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include "map.h"
#include "func_list.h"
#include "cut.h"


void
rand_poly_cut ()
{
  rand_poly ();
  cut ();
  return;
}
