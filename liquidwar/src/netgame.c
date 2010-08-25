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
/* Copyright (C) 1998-2010 Christian Mauduit                                 */
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
/* name          : netgame.c                                        */
/* contenu       : netgame menu, where the client joins the server  */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <string.h>

#include "back.h"
#include "base.h"
#include "config.h"
#include "decal.h"
#include "dialog.h"
#include "help.h"
#include "menu.h"
#include "play.h"
#include "disk.h"
#include "protocol.h"
#include "sockgen.h"
#include "server.h"
#include "network.h"
#include "keyexch.h"
#include "error.h"
#include "internet.h"
#include "netplay.h"
#include "lang.h"
#include "netmess.h"
#include "macro.h"

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
network_game (void)
{
  DIALOG d[17];
  int choix = 4, retour = 0;
  char bufport[BUFFER_EDIT_NUMBER + 1];
  int y0;

  memset (d, 0, sizeof (d));

  LW_NETWORK_ON = 1;

  while (retour == 0)
    {
      LW_MACRO_SPRINTF1 (bufport, "%d", CONFIG_SERVER_PORT);

      standard_button (d + 4, 0, 0, 1, 10);
      standard_button (d + 5, 0, 2, 2, 10);
      standard_button (d + 6, 0, 3, 2, 10);
      standard_button (d + 7, 0, 4, 2, 10);
      standard_button (d + 8, 1, 2, 2, 10);
      standard_button (d + 9, 1, 3, 2, 10);
      standard_button (d + 10, 1, 4, 2, 10);
      standard_button (d + 11, 1, 2, 2, 10);
      standard_button (d + 12, 1, 3, 2, 10);
      standard_button (d + 13, 1, 4, 2, 10);
      standard_big_button (d + 14, 0, 7, 1, 10);
      y0 = d[14].y + d[14].h;
      standard_big_button (d + 14, 0, 5, 1, 10);
      d[14].h = y0 - d[14].y;
      standard_big_button (d + 15, 0, 9, 1, 10);

      quick_buttons (d);
      d[4].dp = lw_lang_string (LW_LANG_STRING_NETGAME_STARTGAME);
      d[5].dp = lw_lang_string (LW_LANG_STRING_NETGAME_SERVERADDR);
      d[5].proc = my_textbox_proc;
      d[6].dp = lw_lang_string (LW_LANG_STRING_NETGAME_SERVERPORT);
      d[6].proc = my_textbox_proc;
      d[7].dp = lw_lang_string (LW_LANG_STRING_NETGAME_PASSWORD);
      d[7].proc = my_textbox_proc;
      d[8].proc = d_box_proc;
      d[9].proc = d_box_proc;
      d[10].proc = d_box_proc;
      d[11].dp = CONFIG_SERVER_ADDRESS;
      d[11].d1 = SERVER_ADDRESS_SIZE;
      d[11].proc = my_edit_proc;
      d[11].flags = 0;
      d[11].x++;
      d[11].y++;
      d[11].w -= 2;
      d[11].h -= 2;
      d[12].dp = bufport;
      d[12].d1 = BUFFER_EDIT_NUMBER;
      d[12].proc = my_edit_proc;
      d[12].flags = 0;
      d[12].x++;
      d[12].y++;
      d[12].w -= 2;
      d[12].h -= 2;
      d[12].d2 = d[9].d2 = 0;
      d[13].dp = CONFIG_PASSWORD;
      d[13].d1 = PASSWORD_SIZE;
      d[13].proc = my_edit_proc;
      d[13].flags = 0;
      d[13].x++;
      d[13].y++;
      d[13].w -= 2;
      d[13].h -= 2;
      d[14].proc = my_textbox_proc;
      d[14].dp = lw_lang_string (LW_LANG_STRING_NETGAME_WEAKPASSWORD);
      d[15].dp = lw_lang_string (LW_LANG_STRING_NETGAME_SEARCH);
      d[16].proc = 0;

      choix = my_do_dialog (d, choix);

      CONFIG_SERVER_PORT = atoi (bufport);
      lw_netmess_cleanup_arg (CONFIG_PASSWORD);

      switch (choix)
	{
	case -1:
	case MENU_QUICK_BACK:
	  retour = 1;
	  break;
	case MENU_QUICK_MAIN:
	  retour = MENU_TOP;
	  break;
	case MENU_QUICK_QUIT:
	  if (confirm_quit ())
	    retour = MENU_EXIT;
	  else
	    display_back_image ();
	  break;
	case MENU_QUICK_PLAY:
	case 4:
	  /*
	   * the "Play" and "Start game" buttons have the same meaning here,
	   * so there's no break;
	   */
	  retour = netplay_sequence ();
	  break;
	case 15:
	  retour = internet_find ();
	  break;
	}
    }

  if (retour > 0)
    retour--;

  return retour;
}
