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
/* Copyright (C) 1998-2020 Christian Mauduit                                 */
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
/* name          : execunix.c                                       */
/* content       : code used to launch external programs on Windows */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#ifdef WIN32
#include <windows.h>
#endif

#include "execgen.h"
#include "macro.h"
#include "log.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_EXEC_ERROR_SIZE 1000

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

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
  BOOL create_process_ret;
  STARTUPINFO startup_info;
  PROCESS_INFORMATION process_info;

  memset (&startup_info, 0, sizeof (STARTUPINFO));
  memset (&process_info, 0, sizeof (PROCESS_INFORMATION));

  startup_info.cb = sizeof (STARTUPINFO);

  create_process_ret = CreateProcess (NULL,     //lpApplicationName
                                      cmd,      //lpCommandLine
                                      NULL,     //lpProcessAttributes
                                      NULL,     //lpThreadAttributes
                                      FALSE,    //bInheritHandles
                                      0,        //dwCreationFlags
                                      NULL,     //lpEnvironment
                                      NULL,     //lpCurrentDirectory
                                      &startup_info,    //lpStartupInfo
                                      &process_info     //lpProcessInformation
    );


  if (create_process_ret == 0)
    {
      int error_int;
      char error_str[LW_EXEC_ERROR_SIZE];
      int error_len;
      int crlf_found;
      LPVOID message_buf;

      error_int = (int) GetLastError ();

      if (FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error_int, MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),      // Default language
                         (LPTSTR) & message_buf, 0, NULL))
        {
          LW_MACRO_SPRINTF2 (error_str, "%d: \"%s", error_int,
                             (char *) message_buf);
          LocalFree (message_buf);

          /*
           * Now we trim ending CR-LF chars
           */
          crlf_found = 1;
          while (crlf_found)
            {
              error_len = strlen (error_str);
              crlf_found = 0;
              if (error_len > 1)
                {
                  if (error_str[error_len - 1] == '\n'
                      || error_str[error_len - 1] == '\r')
                    {
                      error_str[error_len - 1] = '\0';
                      crlf_found = 1;
                    }
                }
            }
          LW_MACRO_STRCAT (error_str, "\"");
        }
      else
        {
          LW_MACRO_SPRINTF1 (error_str, "%d", error_int);
        }

      log_print_str ("Can't execute \"");
      log_print_str (cmd);
      log_print_str ("\", error ");
      log_println_str (error_str);
    }
  else
    {
      result = 1;
    }

  return result;
}
