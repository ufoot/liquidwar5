/*
* Map info, misc map functions.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#ifndef LWMAPGEN_MAP_H
#define LWMAPGEN_MAP_H


#include <allegro.h>
#include "func.h"
#include "misc.h"

#define ALLEGRO_USE_CONSOLE

extern char default_filename[10];


#define MIN_MAP_SIZE     0
#define MAX_MAP_SIZE     6
#define DEFAULT_MAP_SIZE     3

extern int map_size[MAX_MAP_SIZE][2];


#define RANDOM_MAP_GRID_SIZE  0
#define MIN_MAP_GRID_SIZE     1
#define MAX_MAP_GRID_SIZE     10
extern int map_grid_size[MAX_MAP_GRID_SIZE][2];


typedef struct
{
  char *filename;               /* should not be changed */

  int width, height;            /* should not be changed */
  BITMAP *map;

  /* grid size */
  int num_row, num_col;

  /* section size */
  float sec_width, sec_height;  /* should not be changed */

  /* the color to draw with */
  int color;                    /* 0 == normal, 255 == inverted */

  int func_id;
  void (*func) ();
} map_t;

extern map_t map;


/* sets up the map for drawing */
void clear_map ();
void clear_invert_map ();

/* print all the map info to stdout */
/* really for debugging */
void print_map ();


/*****************************************************************************/
/* Misc functions */

/*
* The default sections looks like this:
*     c 0   1   2
*  r  +---+---+---+
*  0  | 0 | 1 | 2 | <-+
*     +---+---+---+   |
*  1  | 3 | 4 | 5 | <-+- i
*     +---+---+---+
*/

/* get i from r, c */
#define section(r,c)     ( (r*map.num_col)+c )

/* get r, c from i */
void section_rc (int *row, int *col, int i);


void offset (int *xoffset, int *yoffset, int r, int c);

void rand_point_section (int *x, int *y, int pad);

void rand_point_section_offset (int *x, int *y, int r, int c, int pad);

void section_center (int *x, int *y, int r, int c);

/*****************************************************************************/


#endif
