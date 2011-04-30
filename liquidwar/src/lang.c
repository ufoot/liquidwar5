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
/* name          : lang.c                                           */
/* content       : handles language dependent stuff (locale)        */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "lang.h"
#include "langde.h"
#include "langdk.h"
#include "langen.h"
#include "langfr.h"
#include "config.h"

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* initializes the string tables                                    */
/*------------------------------------------------------------------*/
void
lw_lang_init ()
{
  lw_langde_init ();
  lw_langdk_init ();
  lw_langen_init ();
  lw_langfr_init ();
}

/*------------------------------------------------------------------*/
/* returns the string associated to an id                           */
/*------------------------------------------------------------------*/
char *
lw_lang_string (int id)
{
  char *result = NULL;
  int language;

  language = (CONFIG_LANGUAGE == LW_LANG_LANGUAGE_AUTO)
    ? lw_lang_auto () : CONFIG_LANGUAGE;

  switch (language)
    {
    case LW_LANG_LANGUAGE_DEUTSCH:
      result = lw_langde_string (id);
      break;
    case LW_LANG_LANGUAGE_DANISH:
      result = lw_langdk_string (id);
      break;
    case LW_LANG_LANGUAGE_ENGLISH:
      result = lw_langen_string (id);
      break;
    case LW_LANG_LANGUAGE_FRANCAIS:
      result = lw_langfr_string (id);
      break;
    }

  if (!result)
    {
      result = "???";
    }

  return result;
}

/*------------------------------------------------------------------*/
/* returns the string associated to a given language                */
/*------------------------------------------------------------------*/
char *
lw_lang_language (int lang)
{
  char *result = NULL;

  switch (lang)
    {
    case LW_LANG_LANGUAGE_DEUTSCH:
      result = lw_langde_string (LW_LANG_STRING_LANGUAGE);
      break;
    case LW_LANG_LANGUAGE_DANISH:
      result = lw_langdk_string (LW_LANG_STRING_LANGUAGE);
      break;
    case LW_LANG_LANGUAGE_ENGLISH:
      result = lw_langen_string (LW_LANG_STRING_LANGUAGE);
      break;
    case LW_LANG_LANGUAGE_FRANCAIS:
      result = lw_langfr_string (LW_LANG_STRING_LANGUAGE);
      break;
    }

  if (!result)
    {
      result = "???";
    }

  return result;
}

/*------------------------------------------------------------------*/
/* returns the "automatic" language code                            */
/*------------------------------------------------------------------*/
int
lw_lang_auto ()
{
  int language;

  /*
   * By default, we speak English
   */
  language = LW_LANG_LANGUAGE_ENGLISH;

#ifdef UNIX
  {
    char *env_lang;

    env_lang = getenv ("LANG");

    if (env_lang != NULL)
      {
	/*
	 * We try and check if LANG is set to "DE..." to enable German
	 * support.
	 */
	if (strncmp (env_lang, "DE", 2) == 0 ||
	    strncmp (env_lang, "de", 2) == 0)
	  {
	    language = LW_LANG_LANGUAGE_DEUTSCH;
	  }

	/*
	 * We try and check if LANG is set to "DK..." to enable Danish
	 * support.
	 */
	if (strncmp (env_lang, "DK", 2) == 0 ||
	    strncmp (env_lang, "dk", 2) == 0)
	  {
	    language = LW_LANG_LANGUAGE_DANISH;
	  }

	/*
	 * We try and check if LANG is set to "FR..." to enable French
	 * support.
	 */
	if (strncmp (env_lang, "FR", 2) == 0 ||
	    strncmp (env_lang, "fr", 2) == 0)
	  {
	    language = LW_LANG_LANGUAGE_FRANCAIS;
	  }
      }
  }
#endif

#ifdef WIN32
  {
    int lang_id;

    lang_id = GetSystemDefaultLangID ();

    /*
     * Lists of some Windows language codes, got it online:
     * http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/nls_238z.asp
     *
     * 0x0406 Danish
     * 
     * 0x040c French (Standard) 
     * 0x080c French (Belgian) 
     * 0x0c0c French (Canadian) 
     * 0x100c French (Switzerland) 
     * 0x140c French (Luxembourg) 
     * 0x180c Windows 98/Me, Windows 2000/XP: French (Monaco)
     *
     * 0x0407 German (Standard) 
     * 0x0807 German (Switzerland) 
     * 0x0c07 German (Austria) 
     * 0x1007 German (Luxembourg) 
     * 0x1407 German (Liechtenstein)
     */

    switch (lang_id)
      {
      case 0x0407:
      case 0x0807:
      case 0x0c07:
      case 0x1007:
      case 0x1407:
	language = LW_LANG_LANGUAGE_DEUTSCH;
	break;
      case 0x0406:
	language = LW_LANG_LANGUAGE_DANISH;
	break;
      case 0x040c:
      case 0x080c:
      case 0x0c0c:
      case 0x100c:
      case 0x140c:
      case 0x180c:
	language = LW_LANG_LANGUAGE_FRANCAIS;
	break;
      }
  }
#endif

  return language;
}
