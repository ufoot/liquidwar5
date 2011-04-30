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
/* Copyright (C) 1998-2011 Christian Mauduit                                 */
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
/* nom           : parser.c                                         */
/* contenu       : lecture des parametres de ligne de commande      */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <string.h>

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define MAX_ARGUMENT_NUMBER 256

char *EXECUTABLE_NAME;

static char *ARGUMENT_NAME[MAX_ARGUMENT_NUMBER];
static char *ARGUMENT_VALUE[MAX_ARGUMENT_NUMBER];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static int
is_arg_marker (char c)
{
#ifdef UNIX
  /*
   * Under Unix, we do not accept / as an option key,
   * since it is used for paths
   */
  return (c == '-');
#else
  return (c == '/' || c == '-');
#endif
}

/*------------------------------------------------------------------*/
void
read_command_line (int argc, char **argv)
{
  int i, j;
  char *str;

  for (i = 0; i < MAX_ARGUMENT_NUMBER; ++i)
    ARGUMENT_NAME[i] = ARGUMENT_VALUE[i] = "";

  str = EXECUTABLE_NAME = argv[0];
  while (*str)
    {
      if ((*str) == '/' || (*str) == '\\')
	EXECUTABLE_NAME = (str + 1);
      str++;
    }

  for (i = 1, j = 0; i < argc && j < MAX_ARGUMENT_NUMBER; ++i)
    {
      str = argv[i];
      if (is_arg_marker (str[0]))
	{
	  ARGUMENT_NAME[j] = str + 1;
	  if (i < argc - 1)
	    {
	      str = argv[i + 1];
	      if (!is_arg_marker (str[0]))
		{
		  ARGUMENT_VALUE[j] = str;
		  i++;
		}
	    }
	  j++;
	}
    }
}

/*------------------------------------------------------------------*/
int
exist_argument (char *str)
{
  int i, found = 0;

  for (i = 0; i < MAX_ARGUMENT_NUMBER && !found; ++i)
    found = !strcmp (str, ARGUMENT_NAME[i]);

  return found;
}

/*------------------------------------------------------------------*/
int
exist_argument_value (char *str)
{
  int i, found = 0;

  for (i = 0; i < MAX_ARGUMENT_NUMBER && !found; ++i)
    found = (!strcmp (str, ARGUMENT_NAME[i]))
      && (strcmp ("", ARGUMENT_VALUE[i]));

  return found;
}

/*------------------------------------------------------------------*/
char *
get_argument_str (char *str)
{
  int i, place = 0, found = 0;

  for (i = 0; i < MAX_ARGUMENT_NUMBER && !found; ++i)
    if ((found = (!strcmp (str, ARGUMENT_NAME[i]))) != 0)
      place = i;

  if (found)
    return ARGUMENT_VALUE[place];
  else
    return "";
}

/*------------------------------------------------------------------*/
int
get_argument_int (char *str)
{
  return (atoi (get_argument_str (str)));
}
