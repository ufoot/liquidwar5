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
/* Copyright (C) 1998-2025 Christian Mauduit                                 */
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
/* nom           : dialog.h                                         */
/* contenu       : utilitaires gui                                  */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_DIALOG
#define LIQUID_WAR_INCLUDE_DIALOG

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>

#include "backport.h"
#include "backportgui.h"
#include "alleg2.h"
#include "palette.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define BUFFER_EDIT_NUMBER 33

#define MENU_TOP -1
#define MENU_EXIT -2
#define MENU_PLAY -3
#define MENU_OK -4

#define MENU_QUICK_QUIT 0
#define MENU_QUICK_PLAY 1
#define MENU_QUICK_MAIN 2
#define MENU_QUICK_BACK 3

#define MENU_VIRTUAL_W 320
#define MENU_VIRTUAL_H 240

#define MENU_H_STANDARD 15
#define MENU_H_QUICK 15
#define MENU_H_SPACE 3
#define MENU_H_LEVEL 15
#define MENU_H_SAMPLE 120
#define MENU_H_TEAM 90
#define MENU_H_INTERNET_LIST 175
#define MENU_W_STANDARD 100
#define MENU_W_QUICK 60
#define MENU_W_SPACE 3
#define MENU_W_LEVEL 40
#define MENU_W_SAMPLE 160
#define MENU_W_TEAM 75
#define MENU_W_INTERNET_LIST 150

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

int menu_real_x (int x);
int menu_real_y (int y);
void menu_real_coord (DIALOG * d);
int find_center (int pos, int num, int space, int size);
void color_one_dialog (DIALOG * d);
void center_one_dialog (DIALOG * d, int x, int y, int w, int h);
void quick_buttons (DIALOG * d);
void standard_button (DIALOG * d, int x_pos, int y_pos, int x_num, int y_num);
void standard_small_button (DIALOG * d, int x_pos, int y_pos,
                            int x_num, int y_num);
void standard_big_button (DIALOG * d, int x_pos, int y_pos,
                          int x_num, int y_num);
void main_message (DIALOG * d, char *str);
void main_info (DIALOG * d, char *str);
DIALOG_PLAYER *my_init_dialog (DIALOG * d, int f);
int my_do_dialog_no_clear (DIALOG * d, int f);
int my_do_dialog (DIALOG * d, int f);
int my_update_dialog (DIALOG_PLAYER * d);
void display_center_message (char *str);
int slider_int (void *dp3, int d2);

#endif
