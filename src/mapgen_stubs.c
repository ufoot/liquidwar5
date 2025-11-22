/********************************************************************/
/*                                                                  */
/*            L   I  QQ  U U I DD    W   W  A  RR    555            */
/*            L   I Q  Q U U I D D   W   W A A R R   5              */
/*            L   I Q  Q U U I D D   W W W AAA RR    55             */
/*            L   I Q Q  U U I D D   WW WW A A R R     5            */
/*            LLL I  Q Q  U  I DD    W   W A A R R   55             */
/*                                                                  */
/*                             b                                    */
/*                             bb  y y                              */
/*                             b b yyy                              */
/*                             bb    y                              */
/*                                 yy                               */
/*                                                                  */
/*                     U U       FFF  O   O  TTT                    */
/*                     U U       F   O O O O  T                     */
/*                     U U TIRET FF  O O O O  T                     */
/*                     U U       F   O O O O  T                     */
/*                      U        F    O   O   T                     */
/*                                                                  */
/********************************************************************/

/*****************************************************************************/
/* Liquid War is a multiplayer wargame                                       */
/* Copyright (C) 1998-2025 Christian Mauduit                                 */
/*                                                                           */
/* This program is free software; you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation; either version 2 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; if not, write to the Free Software               */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */
/*                                                                           */
/* Liquid War homepage : http://www.ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* nom           : mapgen_stubs.c                                   */
/* content       : stub implementations for mapgen dependencies     */
/* last update   : Dec 28th 2024                                    */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "backport.h"
#include <stdio.h>

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/* Global palette for backport compatibility */
PALETTE GLOBAL_PALETTE;

/*==================================================================*/
/* stub functions                                                   */
/*==================================================================*/

/* Minimal logging stubs */
void log_print_str(char *str) {
    printf("%s", str);
}

void log_println_str(char *str) {
    printf("%s\n", str);
}

/* Minimal mutex stubs (mapgen doesn't need real threading) */
void lw_mutex_init(void *mutex) {
    (void)mutex; /* suppress unused parameter warning */
    /* stub - mapgen runs single-threaded */
}

void lw_mutex_lock(void *mutex) {
    (void)mutex; /* suppress unused parameter warning */
    /* stub - mapgen runs single-threaded */
}

void lw_mutex_unlock(void *mutex) {
    (void)mutex; /* suppress unused parameter warning */
    /* stub - mapgen runs single-threaded */
}

/* Minimal thread stubs (mapgen doesn't need real threading) */
void lw_thread_start(void *thread_func) {
    (void)thread_func; /* suppress unused parameter warning */
    /* stub - mapgen runs single-threaded */
}

/* Mouse function stub (mapgen doesn't use mouse) */
void lw_mouse_update_show_hide(void) {
    /* stub - mapgen doesn't use mouse */
}