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
/* content       : provides basic thread support under windows      */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#ifdef WIN32
#include <process.h>
#include <windows.h>
#endif

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
 * Creates a joinable thread and returns a handle
 */
int
lw_thread_create (LW_THREAD_HANDLE * handle, void (*func) (void *), void *args)
{
#ifdef WIN32
  uintptr_t thread_handle;
  int result = 0;

  // Use _beginthreadex which returns a handle we can wait on
  // Unlike _beginthread, this doesn't auto-close the handle
  thread_handle = _beginthreadex (NULL, 0, (unsigned (__stdcall *)(void *))func, args, 0, NULL);

  if (thread_handle != 0)
    {
      handle->data = (void *)thread_handle;
      result = 1;
    }

  return result;
#else
  // Not on Windows, shouldn't happen but return failure
  return 0;
#endif
}

/*------------------------------------------------------------------*/
/*
 * Waits for a thread to complete
 */
int
lw_thread_join (LW_THREAD_HANDLE * handle)
{
#ifdef WIN32
  int result = 0;

  if (handle && handle->data)
    {
      // Wait for thread to complete
      WaitForSingleObject ((HANDLE)handle->data, INFINITE);
      // Close the thread handle
      CloseHandle ((HANDLE)handle->data);
      handle->data = NULL;
      result = 1;
    }

  return result;
#else
  // Not on Windows, shouldn't happen but return failure
  return 0;
#endif
}

/*------------------------------------------------------------------*/
/*
 * Starts a new thread using the given callback (detached)
 */
int
lw_thread_start (void (*func) (void *), void *args)
{
  int result = 0;

  if (_beginthread (func, 0, args) != 0)
    {
      result = 1;
    }

  return result;
}
