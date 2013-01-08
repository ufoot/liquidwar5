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
/* Copyright (C) 1998-2013 Christian Mauduit                                 */
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
/* name          : thrdunix.c                                       */
/* content       : provides basic mutex support under windows       */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#ifdef WIN32
#include <windows.h>
#endif

#include "mutxgen.h"
#include "log.h"

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
int
lw_mutex_init (LW_MUTEX_DATA * mutex)
{
  int result = 0;
  HANDLE *h;

  mutex->data = malloc (sizeof (HANDLE));
  if (mutex->data != NULL)
    {
      h = (HANDLE *) mutex->data;
      (*h) = CreateMutex (NULL, TRUE, NULL);

      if ((*h) != NULL)
	{
	  result = 1;
	}
      else
	{
	  log_println ("Error: can't create mutex...");
	  free (mutex->data);
	  mutex->data = NULL;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
int
lw_mutex_lock (LW_MUTEX_DATA * mutex)
{
  int result = 0;
  HANDLE *h;

  h = (HANDLE *) mutex->data;
  if (!h)
    {
      lw_mutex_init (mutex);
    }
  h = (HANDLE *) mutex->data;
  /*
   * It's important to use another if(h) test since
   * mutex might have just been initialized.
   */
  if (h)
    {
      if (WaitForSingleObject (h, INFINITE) == WAIT_OBJECT_0)
	{
	  result = 1;
	}
    }

  return result;
}

/*------------------------------------------------------------------*/
int
lw_mutex_trylock (LW_MUTEX_DATA * mutex)
{
  int result = 1;

  HANDLE *h;

  h = (HANDLE *) mutex->data;
  if (!h)
    {
      lw_mutex_init (mutex);
    }
  h = (HANDLE *) mutex->data;
  /*
   * It's important to use another if(h) test since
   * mutex might have just been initialized.
   */
  if (h)
    {
      if (WaitForSingleObject (h, 0) == WAIT_OBJECT_0)
	{
	  result = 1;
	}
    }
  return result;
}

/*------------------------------------------------------------------*/
int
lw_mutex_unlock (LW_MUTEX_DATA * mutex)
{
  int result = 1;
  HANDLE *h;

  h = (HANDLE *) mutex->data;
  if (!h)
    {
      ReleaseMutex (h);
    }

  return result;
}
