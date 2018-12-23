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
/* Copyright (C) 1998-2018 Christian Mauduit                                 */
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
/* nom           : logsrv.c                                         */
/* contenu       : gestion de log serveur (affichage+fichier?)      */
/* date de modif : 1 aout 99                                        */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "log.h"
#include "server.h"
#include "macro.h"

/*==================================================================*/
/* definitions de types                                             */
/*==================================================================*/

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int LW_LOG_BEEP = 1;

static FILE *LW_LOG_FILE = NULL;
static int LW_LOG_NEW_LINE = 1;
static int LW_LOG_SERVER_MODE = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Displays date and time at the beginning of each line
 */
static void
display_date_and_time ()
{
  time_t time_stamp;
  struct tm *time_struct;
  char time_str[50];

  if (LW_LOG_FILE && LW_LOG_NEW_LINE && LW_LOG_SERVER_MODE)
    {
      time_stamp = time (NULL);
      time_struct = localtime (&time_stamp);
      LW_MACRO_SPRINTF6 (time_str, "%04d-%02d-%02d %02d:%02d:%02d: ",
                         time_struct->tm_year + 1900,
                         time_struct->tm_mon + 1,
                         time_struct->tm_mday,
                         time_struct->tm_hour,
                         time_struct->tm_min, time_struct->tm_sec);

      fprintf (LW_LOG_FILE, "%s", time_str);
    }
  LW_LOG_NEW_LINE = 0;
}

/*------------------------------------------------------------------*/
static void
prepare_new_line ()
{
  if (LW_LOG_FILE)
    {
      fflush (LW_LOG_FILE);
    }
  LW_LOG_NEW_LINE = 1;
}

/*------------------------------------------------------------------*/
void
log_init (void)
{
  char *filename;

  filename = lw_server_get_log ();

  if (filename && strlen (filename) > 0)
    {
      LW_LOG_FILE = fopen (filename, "a");
    }

  /*
   * By default, we output stuff to stdout
   */
  if (LW_LOG_FILE == NULL)
    {
      LW_LOG_FILE = stdout;
    }
}

/*------------------------------------------------------------------*/
void
log_exit (void)
{
  if (LW_LOG_FILE && LW_LOG_FILE != stdout)
    {
      fclose (LW_LOG_FILE);
    }
}

/*------------------------------------------------------------------*/
void
log_print_str (char *str)
{
  if (LW_LOG_FILE)
    {
      display_date_and_time ();
      fprintf (LW_LOG_FILE, "%s", str);
    }
}

/*------------------------------------------------------------------*/
void
log_print_int (int i)
{
  if (LW_LOG_FILE)
    {
      display_date_and_time ();
      fprintf (LW_LOG_FILE, "%d", i);
    }
}

/*------------------------------------------------------------------*/
void
log_println ()
{
  if (LW_LOG_FILE)
    {
      display_date_and_time ();
      fprintf (LW_LOG_FILE, "\n");
      log_flush ();
      prepare_new_line ();
    }
}

/*------------------------------------------------------------------*/
void
log_println_str (char *str)
{
  if (LW_LOG_FILE)
    {
      display_date_and_time ();
      fprintf (LW_LOG_FILE, "%s\n", str);
      log_flush ();
      prepare_new_line ();
    }
}

/*------------------------------------------------------------------*/
void
log_println_int (int i)
{
  if (LW_LOG_FILE)
    {
      display_date_and_time ();
      fprintf (LW_LOG_FILE, "%d\n", i);
      log_flush ();
      prepare_new_line ();
    }
}

/*------------------------------------------------------------------*/
void
log_flush ()
{
  if (LW_LOG_FILE)
    {
      fflush (LW_LOG_FILE);
    }
}

/*------------------------------------------------------------------*/
void
log_beep ()
{
  if (LW_LOG_BEEP)
    {
      /*
       * We play a beep
       */
      fprintf (stderr, "\a");
    }
}

/*------------------------------------------------------------------*/
void
log_set_server_mode (int mode)
{
  LW_LOG_SERVER_MODE = mode;
}
