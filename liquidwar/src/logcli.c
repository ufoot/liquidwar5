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
/* nom           : logcli.c                                         */
/* contenu       : gestion de log client (affichage+fichier?)       */
/* date de modif : 1 aout 99                                        */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

/*
 * Note: we must *not* include allegro.h here, or everything's
 * wrecked with the use of windows.h...
 */
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "startup.h"
#include "log.h"
#include "macro.h"
#include "mutxgen.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_LOG_FALLBACK_PATH "lwlog.txt"

/*==================================================================*/
/* definitions de types                                             */
/*==================================================================*/

#ifndef UNIX
FILE *LW_LOG_FILE = NULL;
#endif

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int LW_LOG_BEEP = 1;
static LW_MUTEX_DATA LOG_MUTEX = { NULL };

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/


/*------------------------------------------------------------------*/
void
log_init (void)
{
  lw_mutex_lock (&LOG_MUTEX);

#ifndef UNIX
  LW_LOG_FILE = fopen (STARTUP_LOG_PATH, "w");
  if (LW_LOG_FILE == NULL)
    {
      /*
       * Unable to open the log file probably because the data/
       * directoty is not here, we try and open "lw.log" instead...
       */
      LW_LOG_FILE = fopen (LW_LOG_FALLBACK_PATH, "w");
    }
#endif

  log_flush ();
  lw_mutex_unlock (&LOG_MUTEX);
}

/*------------------------------------------------------------------*/
void
log_exit (void)
{
  lw_mutex_lock (&LOG_MUTEX);

#ifndef UNIX
  if (LW_LOG_FILE)
    {
      fclose (LW_LOG_FILE);
    }
#endif

  log_flush ();
  lw_mutex_unlock (&LOG_MUTEX);
}

/*------------------------------------------------------------------*/
void
log_print_str (char *str)
{
  lw_mutex_lock (&LOG_MUTEX);

#ifndef WIN32
  printf ("%s", str);
#endif
#ifndef UNIX
  if (LW_LOG_FILE)
    {
      fprintf (LW_LOG_FILE, "%s", str);
    }
#endif

  log_flush ();
  lw_mutex_unlock (&LOG_MUTEX);
}

/*------------------------------------------------------------------*/
void
log_print_int (int i)
{
  lw_mutex_lock (&LOG_MUTEX);

#ifndef WIN32
  printf ("%d", i);
#endif
#ifndef UNIX
  if (LW_LOG_FILE)
    {
      fprintf (LW_LOG_FILE, "%d", i);
    }
#endif

  log_flush ();
  lw_mutex_unlock (&LOG_MUTEX);
}

/*------------------------------------------------------------------*/
void
log_println ()
{
  lw_mutex_lock (&LOG_MUTEX);

#ifndef WIN32
  printf ("\n");
#endif
#ifndef UNIX
  if (LW_LOG_FILE)
    {
      fprintf (LW_LOG_FILE, "\n");
    }
#endif

  log_flush ();
  lw_mutex_unlock (&LOG_MUTEX);
}

/*------------------------------------------------------------------*/
void
log_println_str (char *str)
{
  lw_mutex_lock (&LOG_MUTEX);

#ifndef WIN32
  printf ("%s\n", str);
#endif
#ifndef UNIX
  if (LW_LOG_FILE)
    {
      fprintf (LW_LOG_FILE, "%s\n", str);
    }
#endif

  log_flush ();
  lw_mutex_unlock (&LOG_MUTEX);
}

/*------------------------------------------------------------------*/
void
log_println_int (int i)
{
  lw_mutex_lock (&LOG_MUTEX);

#ifndef WIN32
  printf ("%d\n", i);
#endif
#ifndef UNIX
  if (LW_LOG_FILE)
    {
      fprintf (LW_LOG_FILE, "%d\n", i);
    }
#endif

  log_flush ();
  lw_mutex_unlock (&LOG_MUTEX);
}

/*------------------------------------------------------------------*/
void
log_flush ()
{
#ifdef GP2X
  usleep (100);
#endif
#ifndef WIN32
  fflush (stdout);
#endif
#ifndef UNIX
  if (LW_LOG_FILE)
    {
      fflush (LW_LOG_FILE);
    }
#endif
}

/*------------------------------------------------------------------*/
void
log_beep ()
{
  lw_mutex_lock (&LOG_MUTEX);

  if (LW_LOG_BEEP)
    {
#ifdef WIN32
      /*
       * We play a beep using the Win32 API
       */
      MessageBeep (MB_OK);
#else
      /*
       * We play a beep using the console
       */
      fprintf (stderr, "\a");
#endif
    }

  log_flush ();
  lw_mutex_unlock (&LOG_MUTEX);
}

/*------------------------------------------------------------------*/
void
log_set_server_mode (int mode)
{
  /*
   * We do nothing here
   */
  LW_MACRO_NOP (mode);
}
