/*
* The main interface to the map generator.
* 
* This is mainly for the game liquidwar to interface with the generator.
* For the shell interface see main.c
*
* Note: I assume that all args are valid.
*
* If filename is NULL then default filename is used.
*
* size must be MIN_MAP_SIZE to MAX_MAP_SIZE-1 inclusive.
*
* grid_size must be MIN_MAP_GRID_SIZE to MAX_MAP_GRID_SIZE-1 inclusive
* or RANDOM_MAP_GRID_SIZE
*
* func_id must be MIN_FUNC to MAX_FUNC-1 inclusive
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

/* damn thats a long name... =] */
#ifndef LWMAPGEN_MAP_GENERATOR_H
#define LWMAPGEN_MAP_GENERATOR_H


void map_generator (char *filename, int size, int grid_size, int func_id);


#endif
