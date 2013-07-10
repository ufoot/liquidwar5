/*
* REMEMBER TO UPDATE MAX_FUNC IN FUNC.H
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include "func.h"
#include "func_list.h"


func_t func[MAX_FUNC] = {
  /* keep this first! */
  {"rand_func", "Picks a random function.", rand_func}
  ,

  {"big_quad", "One big, solid quad that takes up most of the map.", big_quad}
  ,
  {"boxes", "A bunch of boxes of the same size.", boxes}
  ,
  {"bubbles", "Random bubbles.", bubbles}
  ,
  {"circles", "Random circles.", circles}
  ,
  {"circuit", "A map that looks like a circuit board.", circuit}
  ,
  {"hole", "Randomly shaped hole.", hole}
  ,
  {"lines", "A grid of random lines.", lines}
  ,
  {"rand_box", "Random boxes.", rand_box}
  ,
  {"rand_poly",
   "One big, solid, random polygon that takes up most of the map.", rand_poly}
  ,
  {"rand_poly_cut", "Do rand_poly then cut lines across it.", rand_poly_cut}
  ,
  {"street", "A map that looks like a bunch of streets.", street}
  ,
  {"worms", "Random little worms.", worms}
};
