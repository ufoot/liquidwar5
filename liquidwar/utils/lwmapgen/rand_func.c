/*
* Picks a random fuction.
*
* This is really just a nice way to wrap up some code.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include <stdio.h>
#include <stdlib.h>

#include "func.h"
#include "misc.h"


void rand_func()
{
     int i;
     void (*rfunc)();

     /*
     * NOTE: I'm assuming that rand_func is the
     * first function listed in func.c
     * that way I can just pick from 1 to MAX_FUNC-1
     * and the code will be simple.
     */
     i = rand_num(1, MAX_FUNC-1);
     rfunc = func[i].func;

     printf("Random Function:  %2d) %s\t%s\n", i, func[i].name, func[i].desc);
     (*rfunc)();

return;
}
