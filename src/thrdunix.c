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
/* Liquid War homepage : https://ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* name          : thrdunix.c                                       */
/* content       : provides basic thread support under unix         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <pthread.h>
#include <stdlib.h>

#include "thrdgen.h"

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

/*==================================================================*/
/* macros                                                           */
/*==================================================================*/

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* 
 * Internal structure to pass both function and args to pthread wrapper
 */
typedef struct {
  void (*func)(void *);
  void *args;
} lw_thread_wrapper_data;

/*------------------------------------------------------------------*/
/*
 * Wrapper function that matches pthread signature and calls the original function
 */
static void *
lw_thread_wrapper (void *data)
{
  lw_thread_wrapper_data *wrapper_data = (lw_thread_wrapper_data *) data;
  
  // Call the original function
  wrapper_data->func (wrapper_data->args);
  
  // Clean up the wrapper data
  free (wrapper_data);
  
  return NULL;
}

/*------------------------------------------------------------------*/
/*
 * Starts a new thread using the given callback
 */
int
lw_thread_start (void (*func) (void *), void *args)
{
  pthread_t thread;
  int result = 0;
  lw_thread_wrapper_data *wrapper_data;

  // Allocate wrapper data structure
  wrapper_data = malloc (sizeof (lw_thread_wrapper_data));
  if (wrapper_data == NULL)
    {
      return 0;
    }

  wrapper_data->func = func;
  wrapper_data->args = args;

  if (pthread_create (&thread, NULL, lw_thread_wrapper, wrapper_data) == 0)
    {
      if (pthread_detach (thread) == 0)
        {
          result = 1;
        }
      else
        {
          // If detach fails, we should still clean up the wrapper data
          // Note: this is a rare edge case, but we'll let the thread clean up
          // since it was successfully created
        }
    }
  else
    {
      // Thread creation failed, clean up wrapper data
      free (wrapper_data);
    }

  return result;
}
