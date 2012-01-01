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
/* Copyright (C) 1998-2012 Christian Mauduit                                 */
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
/* nom           : bigdata.c                                        */
/* contenu       : gestion de la "grosse" zone de memoire           */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <allegro.h>
#include <string.h>

#include "alleg2.h"
#include "bigdata.h"
#include "exit.h"
#include "log.h"
#include "startup.h"
#include "init.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define BIG_DATA_NUMBER 64
#define MEMORY_TO_LEAVE 1024

static void *BIG_DATA_PTR = 0;
static int BIG_DATA_SIZE = 0;
static int BIG_DATA_BOTTOM = 0;
static int BIG_DATA_TOP = BIG_DATA_NUMBER - 1;
static int BIG_DATA_TABLE[BIG_DATA_NUMBER];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* mecanisme d'allocation des grandes zones                         */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
void
reset_big_data (void)
{
  BIG_DATA_BOTTOM = 0;
  BIG_DATA_TOP = BIG_DATA_NUMBER - 1;
  BIG_DATA_TABLE[BIG_DATA_BOTTOM] = 0;
  BIG_DATA_TABLE[BIG_DATA_TOP] = BIG_DATA_SIZE;

  if (BIG_DATA_PTR)
    {
      memset (BIG_DATA_PTR, 0, BIG_DATA_SIZE);
    }
}

/*------------------------------------------------------------------*/
void *
malloc_big_data (void)
{
  int nb_try = 0;

  log_print_str ("Allocating ");
  log_print_int (STARTUP_BIG_DATA);
  log_print_str (" Mb");
  log_flush ();

  if (BIG_DATA_PTR)
    {
      free (BIG_DATA_PTR);
      BIG_DATA_PTR = NULL;
    }

  STARTUP_BIG_DATA++;

  while (STARTUP_BIG_DATA >= 2 &&
	 BIG_DATA_PTR == NULL && (STARTUP_AUTO || nb_try == 0))
    {
      nb_try++;
      STARTUP_BIG_DATA--;
      BIG_DATA_SIZE = STARTUP_BIG_DATA * 1024 * 1024;
      BIG_DATA_PTR = malloc (BIG_DATA_SIZE);
    }

  if (nb_try > 1)
    {
      log_print_str ("Last try with ");
      log_print_int (STARTUP_BIG_DATA);
      log_print_str (" Mb");
    }

  if (BIG_DATA_PTR == NULL)
    BIG_DATA_SIZE = 0;

  display_success (BIG_DATA_SIZE);
  reset_big_data ();

  return (BIG_DATA_PTR);
}

/*------------------------------------------------------------------*/
void *
malloc_in_big_data_bottom (int size)
{
  char *ptr;
  int bottom, top, new_offset;

  bottom = BIG_DATA_TABLE[BIG_DATA_BOTTOM];
  top = BIG_DATA_TABLE[BIG_DATA_TOP];
  new_offset = ((bottom + size + 15) / 16) * 16;
  if (new_offset < top && BIG_DATA_BOTTOM < (BIG_DATA_TOP - 1))
    {
      ptr = BIG_DATA_PTR;
      ptr += bottom;
      BIG_DATA_TABLE[++BIG_DATA_BOTTOM] = new_offset;

      /*
       * This is theorically not needed, but in case there's a
       * bug in the assembly code for instance, this will increase
       * the chances that things behave the same on every computer
       */
      memset (ptr, 0, size);
    }
  else
    ptr = NULL;

  return ptr;
}

/*------------------------------------------------------------------*/
void *
malloc_in_big_data_top (int size)
{
  char *ptr;
  int bottom, top, new_offset;

  bottom = BIG_DATA_TABLE[BIG_DATA_BOTTOM];
  top = BIG_DATA_TABLE[BIG_DATA_TOP];
  new_offset = ((top - size) / 16) * 16;
  if (new_offset >= bottom && BIG_DATA_BOTTOM < (BIG_DATA_TOP - 1))
    {
      ptr = BIG_DATA_PTR;
      ptr += new_offset;
      BIG_DATA_TABLE[--BIG_DATA_TOP] = new_offset;

      /*
       * This is theorically not needed, but in case there's a
       * bug in the assembly code for instance, this will increase
       * the chances that things behave the same on every computer
       */
      memset (ptr, 0, size);
    }
  else
    ptr = NULL;

  return ptr;
}

/*------------------------------------------------------------------*/
void
free_last_big_data_bottom (void)
{
  if (BIG_DATA_BOTTOM > 0)
    BIG_DATA_BOTTOM--;
}

/*------------------------------------------------------------------*/
void
free_last_big_data_top (void)
{
  if (BIG_DATA_TOP < BIG_DATA_NUMBER - 1)
    BIG_DATA_TOP++;
}

/*------------------------------------------------------------------*/
int
check_free_memory (void)
{
  BITMAP *result;

  result = my_create_bitmap (MEMORY_TO_LEAVE / 2, 1024);
  if (result)
    destroy_bitmap (result);

  return result != NULL;
}
