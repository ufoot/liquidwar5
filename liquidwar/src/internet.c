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
/* name          : internet.c                                       */
/* content       : menu with the list of current internet games     */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <string.h>

#include "internet.h"
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
#include "back.h"
#include "wwwcli.h"
#include "log.h"
#include "netplay.h"
#include "lang.h"
#include "macro.h"

/*==================================================================*/
/* contants                                                         */
/*==================================================================*/

#define LW_INTERNET_LIST_SIZE 1000

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

static LW_WWWCLI_DATA *list_data = NULL;
static int *list_size = NULL;
static char buffer_ping[50];
static char buffer_port[50];
static char buffer_password[50];
static char buffer_uptime[50];
static char buffer_comment[COMMENT_SIZE + 1];
static DIALOG *dialog_list = NULL;
static DIALOG *dialog_ping = NULL;
static DIALOG *dialog_port = NULL;
static DIALOG *dialog_password = NULL;
static DIALOG *dialog_uptime = NULL;
static DIALOG *dialog_comment = NULL;

/*==================================================================*/
/* static functions                                                 */
/*==================================================================*/

static char *list_callback (int index, int *list_size);
static void position_list (DIALOG * d);
static void position_comment (DIALOG * d);
static void update_text_ping (int i);
static void update_text_port (int i);
static void update_text_password (int i);
static void update_text_uptime (int i);
static void update_text_comment (int i);

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
int
internet_find (void)
{
  DIALOG d[13];
  int choix = 4, retour = 0;
  LW_WWWCLI_DATA data[LW_INTERNET_LIST_SIZE];
  int size;
  int cur;
  int i;

  memset (d, 0, sizeof (d));

  size = LW_INTERNET_LIST_SIZE;
  lw_wwwcli_get_games (data, &size);

  list_data = data;
  list_size = &size;

  //  log_println_str(data[0].ip);

  while (retour == 0)
    {
      position_list (d + 4);

      standard_button (d + 5, 1, 0, 2, 10);
      standard_button (d + 6, 1, 1, 2, 10);
      standard_button (d + 7, 1, 3, 2, 10);
      standard_button (d + 8, 1, 4, 2, 10);
      standard_button (d + 9, 1, 5, 2, 10);
      standard_button (d + 10, 1, 6, 2, 10);

      for (i = 0; i < 6; ++i)
	{
	  d[5 + i].w =
	    menu_real_x ((MENU_W_INTERNET_LIST + MENU_W_STANDARD) / 2);
	}

      position_comment (d + 11);

      quick_buttons (d);

      d[4].dp = list_callback;
      d[4].d1 = d[4].d2 = 0;
      d[4].dp2 = d[4].dp3 = NULL;
      d[4].proc = my_list_proc;

      d[5].dp = lw_lang_string (LW_LANG_STRING_INTERNET_REFRESH);
      d[6].dp = lw_lang_string (LW_LANG_STRING_INTERNET_JOINNOW);

      d[7].dp = buffer_ping;
      d[8].dp = buffer_port;
      d[9].dp = buffer_password;
      d[10].dp = buffer_uptime;
      d[11].dp = buffer_comment;
      d[7].proc = d[8].proc = d[9].proc = d[10].proc = d[11].proc =
	my_textbox_proc;
      d[12].proc = 0;

      dialog_list = d + 4;
      dialog_ping = d + 7;
      dialog_port = d + 8;
      dialog_password = d + 9;
      dialog_uptime = d + 10;
      dialog_comment = d + 11;

      update_text_ping (0);
      update_text_port (0);
      update_text_password (0);
      update_text_uptime (0);
      update_text_comment (0);

      choix = my_do_dialog (d, choix);

      /*
       * First test to select the current list value if
       * it exists, and make it the current server.
       */
      cur = d[4].d1;
      if (choix == MENU_QUICK_PLAY || choix == 4 || choix == 6)
	{
	  if (cur >= 0 && cur < size)
	    {
	      strncpy (CONFIG_SERVER_ADDRESS, data[cur].ip,
		       SERVER_ADDRESS_SIZE);
	      CONFIG_SERVER_ADDRESS[SERVER_ADDRESS_SIZE] = '\0';
	      CONFIG_SERVER_PORT = data[cur].port;
	    }
	}

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
	  retour = netplay_sequence ();
	  break;
	case 5:
	  size = LW_INTERNET_LIST_SIZE;
	  lw_wwwcli_get_games (data, &size);
	  break;
	case 4:
	case 6:
	  if (cur >= 0 && cur < size)
	    {
	      retour = netplay_sequence ();
	    }
	  break;
	}
    }

  if (retour > 0)
    retour--;

  return retour;
}

/*------------------------------------------------------------------*/
static void
position_list (DIALOG * d)
{
  d->x = (MENU_VIRTUAL_W - MENU_W_SPACE) / 2 - MENU_W_INTERNET_LIST;
  d->y = (MENU_VIRTUAL_H - MENU_H_INTERNET_LIST) / 2;
  d->w = MENU_W_INTERNET_LIST;
  d->h = MENU_H_INTERNET_LIST;

  color_one_dialog (d);
  menu_real_coord (d);
  d->key = 0;
  d->dp = "";
  d->flags = D_EXIT;
}

/*------------------------------------------------------------------*/
static void
position_comment (DIALOG * d)
{
  d->x = (MENU_VIRTUAL_W + MENU_W_SPACE) / 2;
  d->y = (MENU_VIRTUAL_H - MENU_H_INTERNET_LIST) / 2 +
    7 * MENU_H_STANDARD + 7 * MENU_H_SPACE;
  d->w = MENU_W_INTERNET_LIST;
  d->h = 3 * MENU_H_STANDARD + (3 * MENU_H_SPACE) / 2;

  color_one_dialog (d);
  menu_real_coord (d);
  d->key = 0;
  d->dp = "";
  d->flags = D_EXIT;
}

/*------------------------------------------------------------------*/
/*
 * Callback called for the list by the Allegro GUI engine
 */
static char *
list_callback (int index, int *size)
{
  char *result = NULL;

  if (index >= 0)
    {
      if (index < *list_size)
	{
	  result = list_data[index].label;

	  /*
	   * Now this _is_ a hack: I wanted the port
	   * password, uptime and comment fields to
	   * be updated automatically when selecting
	   * a line in the list, without even double-clicking
	   * it. So what I did is that when the list engine
	   * asks for the label of the current line, I
	   * automatically recalculate and redraw these fields.
	   * Since the list engine has to hilight the current
	   * label, I assume it asks for the current label
	   * everytime the selection is changed.
	   */
	  if (index == dialog_list->d1)
	    {
	      update_text_ping (dialog_list->d1);
	      update_text_port (dialog_list->d1);
	      update_text_password (dialog_list->d1);
	      update_text_uptime (dialog_list->d1);
	      update_text_comment (dialog_list->d1);

	      scare_mouse ();
	      my_textbox_proc (MSG_DRAW, dialog_ping, 0);
	      my_textbox_proc (MSG_DRAW, dialog_port, 0);
	      my_textbox_proc (MSG_DRAW, dialog_password, 0);
	      my_textbox_proc (MSG_DRAW, dialog_uptime, 0);
	      my_textbox_proc (MSG_DRAW, dialog_comment, 0);
	      unscare_mouse ();
	    }
	}
      else
	{
	  result = "";
	}
    }
  else
    {
      *size = *list_size;
    }

  return result;
}

/*------------------------------------------------------------------*/
static void
update_text_ping (int i)
{
  if (i >= 0)
    {
      if (i < *list_size)
	{
	  if (list_data[i].ping_delay >= 0)
	    {
	      LW_MACRO_SPRINTF3 (buffer_ping, "%s: %d %s",
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_PING),
				 list_data[i].ping_delay,
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_MSEC));
	    }
	  else
	    {
	      LW_MACRO_STRCPY (buffer_ping,
			       lw_lang_string
			       (LW_LANG_STRING_INTERNET_UNREACHABLE));
	    }
	}
      else
	{
	  LW_MACRO_SPRINTF1 (buffer_ping,
			     "%s:",
			     lw_lang_string (LW_LANG_STRING_INTERNET_PING));
	}
    }
}

/*------------------------------------------------------------------*/
static void
update_text_port (int i)
{
  if (i >= 0)
    {
      if (i < *list_size)
	{
	  LW_MACRO_SPRINTF2 (buffer_port,
			     "%s: %d",
			     lw_lang_string
			     (LW_LANG_STRING_INTERNET_PORT),
			     list_data[i].port);
	}
      else
	{
	  LW_MACRO_SPRINTF1 (buffer_port,
			     "%s:",
			     lw_lang_string (LW_LANG_STRING_INTERNET_PORT));
	}
    }
}

/*------------------------------------------------------------------*/
static void
update_text_password (int i)
{
  if (i >= 0)
    {
      if (i < *list_size)
	{
	  if (list_data[i].password)
	    {
	      LW_MACRO_SPRINTF2 (buffer_password,
				 "%s: %s",
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_PASSWORD),
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_YES));
	    }
	  else
	    {
	      LW_MACRO_SPRINTF2 (buffer_password,
				 "%s: %s",
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_PASSWORD),
				 lw_lang_string (LW_LANG_STRING_INTERNET_NO));
	    }
	}
      else
	{
	  LW_MACRO_SPRINTF1 (buffer_password,
			     "%s:",
			     lw_lang_string
			     (LW_LANG_STRING_INTERNET_PASSWORD));
	}
    }
}

/*------------------------------------------------------------------*/
static void
update_text_uptime (int i)
{
  int days, hours, minutes, seconds;
  if (i >= 0)
    {
      if (i < *list_size)
	{
	  seconds = list_data[i].uptime % 60;
	  minutes = (list_data[i].uptime / 60) % 60;
	  hours = (list_data[i].uptime / 3600) % 24;
	  days = (list_data[i].uptime / (24 * 3600));
	  if (days > 0)
	    {
	      if (days > 1)
		{
		  LW_MACRO_SPRINTF3 (buffer_uptime,
				     "%s: %d %s",
				     lw_lang_string
				     (LW_LANG_STRING_INTERNET_UPTIME),
				     days,
				     lw_lang_string
				     (LW_LANG_STRING_INTERNET_DAYS));
		}
	      else
		{
		  LW_MACRO_SPRINTF2 (buffer_uptime,
				     "%s: 1 %s",
				     lw_lang_string
				     (LW_LANG_STRING_INTERNET_UPTIME),
				     lw_lang_string
				     (LW_LANG_STRING_INTERNET_DAY));
		}
	    }
	  else if (hours > 0)
	    {
	      LW_MACRO_SPRINTF3 (buffer_uptime,
				 "%s: %d %s",
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_UPTIME),
				 hours,
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_HOURS));
	    }
	  else if (minutes > 0)
	    {
	      LW_MACRO_SPRINTF3 (buffer_uptime,
				 "%s: %d %s",
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_UPTIME),
				 minutes,
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_MINUTES));
	    }
	  else
	    {
	      LW_MACRO_SPRINTF3 (buffer_uptime,
				 "%s: %d %s",
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_UPTIME),
				 seconds,
				 lw_lang_string
				 (LW_LANG_STRING_INTERNET_SECONDS));
	    }
	}
      else
	{
	  LW_MACRO_SPRINTF1 (buffer_uptime,
			     "%s:",
			     lw_lang_string (LW_LANG_STRING_INTERNET_UPTIME));
	}
    }
}

/*------------------------------------------------------------------*/
static void
update_text_comment (int i)
{
  if (i >= 0)
    {
      if (i < *list_size)
	{
	  LW_MACRO_SPRINTF1 (buffer_comment, "%s", list_data[i].comment);
	}
      else
	{
	  LW_MACRO_SPRINTF0 (buffer_comment, "");
	}
    }
}
