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
/* nom           : error.c                                          */
/* contenu       : messages d'erreur pendant le jeu                 */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "dialog.h"
#include "error.h"
#include "help.h"
#include "base.h"
#include "lang.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static int
return_d_close (void)
{
  return (D_CLOSE);
}

/*------------------------------------------------------------------*/
static int
standard_error (char *str)
{
  DIALOG d[3];

  memset (d, 0, sizeof (d));

  color_one_dialog (d);
  d[0].proc = my_textbox_proc;
  d[0].d2 = 0;
  d[0].dp = str;
  d[0].key = 0;
  d[0].flags = 0;
  d[0].x = MENU_VIRTUAL_W / 4;
  d[0].y = (3 * MENU_VIRTUAL_H) / 8;
  d[0].w = (MENU_VIRTUAL_W) / 2;
  d[0].h = (MENU_VIRTUAL_H) / 4;
  menu_real_coord (d);

  d[1].proc = d_keyboard_proc;
  d[1].dp = return_d_close;
  d[1].key = ' ';
  d[1].d1 = KEY_ENTER;
  d[1].d2 = KEY_ESC;

  d[2].proc = NULL;

  my_do_dialog (d, 0);

  return 0;
}

/*------------------------------------------------------------------*/
int
error_not_enough_teams (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_NOTENOUGHTEAMS)));
}

/*------------------------------------------------------------------*/
int
error_not_enough_big_data (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_NOTENOUGHBIGDATA)));
}

/*------------------------------------------------------------------*/
int
error_graphics_init_failed (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_GRAPHICSINITFAILED)));
}

/*------------------------------------------------------------------*/
int
error_no_network_support (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_NONETWORKSUPPORT)));
}

/*------------------------------------------------------------------*/
int
error_unable_to_connect (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_UNABLETOCONNECT)));
}

/*------------------------------------------------------------------*/
int
error_not_a_liquidwar_server (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_NOTALIQUIDWARSERVER)));
}

/*------------------------------------------------------------------*/
int
error_wrong_version (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_WRONGVERSION)));
}

/*------------------------------------------------------------------*/
int
error_network_problem (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_NETWORKPROBLEM)));
}

/*------------------------------------------------------------------*/
int
error_cant_send_map (void)
{
  return (standard_error (lw_lang_string (LW_LANG_STRING_ERROR_CANTSENDMAP)));
}

/*------------------------------------------------------------------*/
int
error_cant_receive_map (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_CANTRECEIVEMAP)));
}

/*------------------------------------------------------------------*/
int
error_server_full (void)
{
  return (standard_error (lw_lang_string (LW_LANG_STRING_ERROR_SERVERFULL)));
}

/*------------------------------------------------------------------*/
int
error_bad_password (void)
{
  return (standard_error (lw_lang_string (LW_LANG_STRING_ERROR_BADPASSWORD)));
}

/*------------------------------------------------------------------*/
int
error_bad_config_range (void)
{
  return (standard_error (lw_lang_string
			  (LW_LANG_STRING_ERROR_BADCONFIGRANGE)));
}
