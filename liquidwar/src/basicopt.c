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
/* name          : basicopt.c                                       */
/* content       : handles basic options                            */
/* last update   : April 18th 2001                                  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <string.h>

#include "base.h"
#include "basicopt.h"
#include "log.h"
#include "parser.h"
#include "sockgen.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
lw_basicopt_process (char *name)
{
  int result = 1;

  /*
   * "-v" prints the version number
   */
  if (exist_argument ("v") || exist_argument ("-version"))
    {
      result = 0;
      log_println_str (LW_VERSION);
    }

  /*
   * "-v" or "-h" gives a short description & copyright information
   */
  if (exist_argument ("h") ||
      exist_argument ("-help") || exist_argument ("?"))
    {
      result = 0;
      lw_basicopt_legal_info (name);
      log_println ();
      log_println_str
	("Documentation is available in HTML, ascii text, PostScript, PDF... Read it 8-)");
    }

  /*
   * "-netlog" forces the socket messages to be traced
   */
  if (exist_argument ("netlog"))
    {
      LW_SOCK_LOG = 1;
    }

  /*
   * "-nobeep" disables the console beeps
   */
  if (exist_argument ("nobeep"))
    {
      LW_LOG_BEEP = 0;
    }

  return result;
}

/*------------------------------------------------------------------*/
void
lw_basicopt_legal_info (char *name)
{
  log_println ();
  log_println_str (name);
  log_println ();
  log_println_str ("v" LW_VERSION " compiled on " __DATE__ " at " __TIME__);
  log_println_str ("Copyright (C) 1998-2012 Christian Mauduit");
  log_println ();
  log_println_str
    ("This program is free software; you can redistribute it and/or modify");
  log_println_str
    ("it under the terms of the GNU General Public License as published by");
  log_println_str
    ("the Free Software Foundation; either version 2 of the License, or");
  log_println_str ("(at your option) any later version.");
  log_println ();

  log_println_str
    ("This program is distributed in the hope that it will be useful,");
  log_println_str
    ("but WITHOUT ANY WARRANTY; without even the implied warranty of");
  log_println_str
    ("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the");
  log_println_str ("GNU General Public License for more details.");

  log_println ();
  log_println_str
    ("You should have received a copy of the GNU General Public License");
  log_println_str
    ("along with this program; if not, write to the Free Software");
  log_println_str
    ("Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA");
  log_println ();
  log_println_str ("Liquid War homepage : http://www.ufoot.org/liquidwar/v5");
  log_println_str ("Contact author      : ufoot@ufoot.org");
}
