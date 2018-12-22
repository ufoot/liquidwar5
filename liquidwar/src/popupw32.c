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
/* Liquid War homepage : http://www.ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* name    : popupw32.c                                             */
/* content : popup windows used under the win32 platform            */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>
#ifdef WIN32
#include <winalleg.h>
#endif

#include "popupgen.h"
#include "lang.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_POPUP_ERROR_FLAGS MB_OK|MB_ICONEXCLAMATION

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
void
lw_popup_init_fail ()
{
  HWND hwnd;

  hwnd = (HWND) win_get_window ();
  MessageBox (hwnd,
              lw_lang_string (LW_LANG_STRING_POPUP_INITFAIL),
              lw_lang_string (LW_LANG_STRING_APPNAME), LW_POPUP_ERROR_FLAGS);
}

/*------------------------------------------------------------------*/
void
lw_popup_mem_trouble (void)
{
  HWND hwnd;

  hwnd = (HWND) win_get_window ();
  MessageBox (hwnd,
              lw_lang_string (LW_LANG_STRING_POPUP_MEMTROUBLE),
              lw_lang_string (LW_LANG_STRING_APPNAME), LW_POPUP_ERROR_FLAGS);
}

/*------------------------------------------------------------------*/
void
lw_popup_big_data (void)
{
  HWND hwnd;

  hwnd = (HWND) win_get_window ();
  MessageBox (hwnd,
              lw_lang_string (LW_LANG_STRING_POPUP_BIGDATA),
              lw_lang_string (LW_LANG_STRING_APPNAME), LW_POPUP_ERROR_FLAGS);
}

/*------------------------------------------------------------------*/
void
lw_popup_load_fail (void)
{
  HWND hwnd;

  hwnd = (HWND) win_get_window ();
  MessageBox (hwnd,
              lw_lang_string (LW_LANG_STRING_POPUP_LOADFAIL),
              lw_lang_string (LW_LANG_STRING_APPNAME), LW_POPUP_ERROR_FLAGS);
}

/*------------------------------------------------------------------*/
void
lw_popup_custom_fail (void)
{
  HWND hwnd;

  hwnd = (HWND) win_get_window ();
  MessageBox (hwnd,
              lw_lang_string (LW_LANG_STRING_POPUP_CUSTOMFAIL),
              lw_lang_string (LW_LANG_STRING_APPNAME), LW_POPUP_ERROR_FLAGS);
}

/*------------------------------------------------------------------*/
void
lw_popup_graphics_fail (void)
{
  HWND hwnd;

  hwnd = (HWND) win_get_window ();
  MessageBox (hwnd,
              lw_lang_string (LW_LANG_STRING_POPUP_GRAPHICSFAIL),
              lw_lang_string (LW_LANG_STRING_APPNAME), LW_POPUP_ERROR_FLAGS);
}
