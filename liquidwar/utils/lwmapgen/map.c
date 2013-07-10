/*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include <stdio.h>
#include <stdlib.h>

#include "map.h"


char default_filename[10] = "lwmap.bmp";


/* [2] for w == 0, h == 1 */
int map_size[MAX_MAP_SIZE][2] = {
  {128, 95},			/* 0 */
  {160, 120},			/* 1 */
  {256, 190},			/* 2 */
  {320, 240},			/* 3 */
  {512, 380},			/* 4 */
  {640, 480}			/* 5 */
};


/* [2] for r == 0, c == 1 */
/* the maps just look better if the ratio is the same */
int map_grid_size[MAX_MAP_GRID_SIZE][2] = {
  /* this is random. keep at top */
  {0, 0},			/* 0 */

  {2, 3},			/* 1 */
  {4, 6},			/* 2 */
  {6, 9},			/* 3 */
  {8, 12},			/* 4 */
  {10, 15},			/* 5 */
  {12, 18},			/* 6 */
  {14, 21},			/* 7 */
  {16, 24},			/* 8 */
  {18, 26}			/* 9 */
};


map_t map;

/*****************************************************************************/

void
clear_map ()
{
  clear_to_color (map.map, 255);
  /* draw outline */
  rect (map.map, 0, 0, map.width - 1, map.height - 1, 0);
}

/*****************************************************************************/

void
clear_invert_map ()
{
  clear_to_color (map.map, 0);
}

/*****************************************************************************/
/* print map info to stdout */
/* really for debugging */

void
print_map ()
{
  printf ("Name:     %s\n", map.filename);
  printf ("Size:     %dx%d\n", map.width, map.height);
  printf ("Row/Col:  %d/%d\n", map.num_row, map.num_col);
  printf ("Section:  %fx%f\n", map.sec_width, map.sec_height);
  printf ("Function: (%d) %s\t%s\n", map.func_id,
	  func[map.func_id].name, func[map.func_id].desc);
}

/*****************************************************************************/
/* get r, c from i */
void
section_rc (int *r, int *c, int i)
{
  *r = i / map.num_col;
  *c = i % map.num_col;

  return;
}

/*****************************************************************************/

void
offset (int *xoffset, int *yoffset, int r, int c)
{
  *xoffset = c * map.sec_width;
  *yoffset = r * map.sec_height;

  return;
}

/*****************************************************************************/
/* TODO: does this cause rounding errors? */

void
rand_point_section (int *x, int *y, int pad)
{
  *x = rand_num (pad, map.sec_width - pad);
  *y = rand_num (pad, map.sec_height - pad);

  return;
}

/*****************************************************************************/
/* TODO: does this cause rounding errors? */

void
rand_point_section_offset (int *x, int *y, int r, int c, int pad)
{
  int xoffset, yoffset;

  offset (&xoffset, &yoffset, r, c);

  *x = rand_num (pad, map.sec_width - pad) + xoffset;
  *y = rand_num (pad, map.sec_height - pad) + yoffset;

  return;
}

/*****************************************************************************/

void
section_center (int *x, int *y, int r, int c)
{
  int xoffset, yoffset;

  offset (&xoffset, &yoffset, r, c);

  *x = (map.sec_width / 2.0) + xoffset;
  *y = (map.sec_height / 2.0) + yoffset;

  return;
}

/*****************************************************************************/
