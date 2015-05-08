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
/* Copyright (C) 1998-2015 Christian Mauduit                                 */
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
/* name          : execunix.c                                       */
/* content       : code used to launch external programs under UNIX */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "execgen.h"
#include "log.h"
#include "macro.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_EXECUNIX_SHELL_CMD  "/bin/sh"
#define LW_EXECUNIX_SHELL_ARG1 "-c"
#define LW_EXEC_ERROR_SIZE 1000

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static void
handler_sigchld (int sig)
{
  sig = sig;			// useless parameter
  wait (NULL);
  return;
}

/*==================================================================*/
/* global functions                                                 */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * Launches a command line in another process.
 */
int
lw_exec_cmd (char *cmd)
{
  int result = 0;
  pid_t pid;
  char error_str[LW_EXEC_ERROR_SIZE];
  struct sigaction action;

  sigemptyset (&action.sa_mask);
  action.sa_flags = 0;
  action.sa_handler = handler_sigchld;

  if (sigaction (SIGCHLD, &action, NULL) == -1)
    {
      log_println_str ("Unable to set SIGCHLD callback!");
    }

  pid = fork ();

  switch (pid)
    {
    case 0:
      /*
       * We're in the child
       */
      execl (LW_EXECUNIX_SHELL_CMD,
	     LW_EXECUNIX_SHELL_CMD, LW_EXECUNIX_SHELL_ARG1, cmd, NULL);

      /*
       * If we get here, it means launching the command failed...
       */
      LW_MACRO_SPRINTF5 (error_str,
			 "Can't execute \"%s %s %s\", error %d: \"%s\"",
			 LW_EXECUNIX_SHELL_CMD,
			 LW_EXECUNIX_SHELL_ARG1,
			 cmd, errno, strerror (errno));
      log_println_str (error_str);
      exit (1);
      break;
    case -1:
      /*
       * Error forking
       */
      log_println_str ("Unable to fork new process!");
      break;
    default:
      /*
       * We're in the parent.
       */
      log_print_str ("Forking with pid ");
      log_println_int (pid);

      result = 1;
      break;
    }

  return result;
}
