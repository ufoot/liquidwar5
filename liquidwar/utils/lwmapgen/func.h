/*
* Info on the functions to generate the map.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#ifndef LWMAPGEN_FUNC_H
#define LWMAPGEN_FUNC_H


#define MIN_FUNC      0
#define MAX_FUNC     13


typedef struct
{
  char *name;
  char *desc;
  void (*func) ();
} func_t;

extern func_t func[MAX_FUNC];


#endif
