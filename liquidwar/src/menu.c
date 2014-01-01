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
/* Copyright (C) 1998-2014 Christian Mauduit                                 */
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
/* nom           : menu.c                                           */
/* contenu       : aaffichages des menus principaux                 */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "back.h"
#include "dialog.h"
#include "disk.h"
#include "help.h"
#include "level.h"
#include "menu.h"
#include "options.h"
#include "play.h"
#include "netplay.h"
#include "team.h"
#include "netgame.h"
#include "network.h"
#include "error.h"
#include "about.h"
#include "lang.h"
#include "log.h"

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
confirm_quit (void)
{
  DIALOG d[3];

  memset (d, 0, sizeof (d));

  d[2].proc = NULL;
  standard_button (d, 0, 0, 2, 1);
  standard_button (d + 1, 1, 0, 2, 1);
  d[0].dp = lw_lang_string (LW_LANG_STRING_MENU_EXIT);
#ifdef DOS
  d[0].dp = lw_lang_string (LW_LANG_STRING_MENU_EXITDOS);
#endif
#ifdef WIN32
  d[0].dp = lw_lang_string (LW_LANG_STRING_MENU_EXITWINDOWS);
#endif
#ifdef UNIX
  d[0].dp = lw_lang_string (LW_LANG_STRING_MENU_EXITUNIX);
#endif
  d[1].dp = lw_lang_string (LW_LANG_STRING_MENU_BACK);
  d[0].key = 'q';
  d[1].key = 'w';

  return (my_do_dialog (d, 0) == 0);
}

/*------------------------------------------------------------------*/
/* lancement du menu principal                                      */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
void
main_menu (int fade_in)
{
  DIALOG d[11];
  int i, choix = 4, retour = 0;

  memset (d, 0, sizeof (d));

  display_back_image ();

  if (fade_in)
    {
      my_fade_in ();
    }

  while (retour != MENU_EXIT)
    {
      for (i = 0; i < 6; ++i)
	standard_button (d + i + 4, 0, i, 1, 6);

      quick_buttons (d);
      d[MENU_QUICK_BACK].flags = D_HIDDEN;
      d[MENU_QUICK_MAIN].flags = D_HIDDEN;
      d[4].dp = lw_lang_string (LW_LANG_STRING_MENU_PLAY);
      d[5].dp = lw_lang_string (LW_LANG_STRING_MENU_NETGAME);
      d[6].dp = lw_lang_string (LW_LANG_STRING_MENU_MAP);
      d[7].dp = lw_lang_string (LW_LANG_STRING_MENU_TEAMS);
      d[8].dp = lw_lang_string (LW_LANG_STRING_MENU_OPTIONS);
      d[9].dp = lw_lang_string (LW_LANG_STRING_MENU_ABOUT);
      d[10].proc = 0;

      choix = my_do_dialog (d, choix);
      switch (choix)
	{
	case -1:
	case MENU_QUICK_QUIT:
	  if (confirm_quit ())
	    retour = MENU_EXIT;
	  break;
	case MENU_QUICK_PLAY:
	  retour = MENU_PLAY;
	  break;
	case 4:
	  LW_NETWORK_ON = 0;
	  retour = play_sequence ();
	  break;
	case 5:
#ifndef DOS
	  retour = network_game ();
#else
	  retour = error_no_network_support ();
#endif
	  break;
	case 6:
	  retour = choose_map ();
	  break;
	case 7:
	  retour = choose_teams ();
	  break;
	case 8:
	  retour = options ();
	  break;
	case 9:
	  retour = lw_about ();
	  break;
	}
      /*
       * We handle the quick top-right "Play" button
       */
      if (retour == MENU_PLAY)
	{
	  if (LW_NETWORK_ON)
	    {
#ifndef DOS
	      retour = netplay_sequence ();
#else
	      retour = play_sequence ();
#endif
	    }
	  else
	    {
	      retour = play_sequence ();
	    }
	}
    }
}
