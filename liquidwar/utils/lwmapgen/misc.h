/*
* Just misc. functions...
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#ifndef LWMAPGEN_MISC_H
#define LWMAPGEN_MISC_H


/* a random number from min to max inclusive on both ends */
int rand_num( int min, int max);


/* size == the size of the elements in the array/grid */

void * create_array( int length, int size );
void delete_array( void *array );

void ** create_grid( int r, int c, int size );
void delete_grid( void **grid, int r );


#endif
