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
/* nom           : level.c                                          */
/* contenu       : choix du niveau a jouer                          */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <string.h>

#include "back.h"
#include "config.h"
#include "dialog.h"
#include "disk.h"
#include "help.h"
#include "level.h"
#include "map.h"
#include "maptex.h"
#include "menu.h"
#include "palette.h"
#include "sound.h"
#include "texture.h"
#include "macro.h"
#include "lang.h"
#include "mutxgen.h"
#include "log.h"
#include "random.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
palette_128 (DIALOG * d, int x, int y, int w, int h)
{
  int w1, h1, w2, h2, i, j, k;
  w += 2;
  h += 2;
  w1 = w / 10;
  h1 = h / 13;
  w2 = w1 - 2;
  h2 = h1 - 2;
  for (k = 0; k < 128; ++k)

    {
      d[k].proc = d_button_proc;
      d[k].key = 0;
      d[k].dp = "";
      d[k].w = w2;
      d[k].h = h2;
      d[k].fg = MENU_FG;
      d[k].flags = D_EXIT;
      d[k].bg = k + 128;
    }
  k = 0;
  for (i = 0; i < 12; ++i)
    for (j = 0; j < 10; ++j)

      {
	d[k].x = x + j * w1;
	d[k].y = y + i * h1;
	++k;
      }
  d[k].x = x;
  d[k].y = y + 12 * h1;
  d[k].w = 2 * w2 + 2;
  k++;
  for (j = 0; j < 6; ++j)

    {
      d[k].x = x + (j + 2) * w1;
      d[k].y = y + 12 * h1;
      ++k;
    }
  d[k].x = x + 8 * w1;
  d[k].y = y + 12 * h1;
  d[k].w = 2 * w2 + 2;
}


/*------------------------------------------------------------------*/
static void
retrieve_choose_level_xy (int *x1, int *x2, int *x3, int *x4, int *x5,
			  int *x6, int *x7, int *x8, int *y1, int *y2,
			  int *y3, int *y4, int *y5, int *y6, int *y7,
			  int *y8)
{
  (*x1) = (MENU_VIRTUAL_W - MENU_W_LEVEL * 2
	   - MENU_H_LEVEL * 2
	   - MENU_W_SPACE * 2 - MENU_H_SPACE * 2 - MENU_W_SAMPLE) / 2;
  (*x2) = (*x1) + MENU_H_SPACE + MENU_H_LEVEL;
  (*x3) = (*x1) + MENU_W_SPACE + MENU_W_LEVEL;
  (*x4) = (*x3) + MENU_H_LEVEL + MENU_H_SPACE;
  (*x5) = (*x4) + MENU_H_SPACE + MENU_H_LEVEL;
  (*x7) = (*x4) + MENU_W_SAMPLE + MENU_W_SPACE;
  (*x6) = (*x7) - MENU_H_SPACE - MENU_H_LEVEL;
  (*x8) = (*x7) + MENU_H_LEVEL + MENU_H_SPACE;
  (*y1) = (MENU_VIRTUAL_H - MENU_H_LEVEL * 4
	   - MENU_H_SPACE * 4 - MENU_H_SAMPLE) / 2;
  (*y2) = (*y1) + MENU_H_LEVEL + MENU_H_SPACE;
  (*y3) = (*y1) + MENU_W_LEVEL + MENU_H_SPACE;
  (*y4) = (*y1) + MENU_H_SAMPLE - MENU_H_LEVEL;
  (*y5) = (*y4) + MENU_H_LEVEL + MENU_H_SPACE;
  (*y6) = (*y5) + MENU_H_LEVEL + MENU_H_SPACE;
  (*y7) = (*y6) + MENU_H_LEVEL + MENU_H_SPACE;
  (*y8) = (*y7) + MENU_H_LEVEL + MENU_H_SPACE;
}


/*------------------------------------------------------------------*/
static void
display_chosen_level (void)
{
  int x1, x2, x3, x4, x5, x6, x7, x8;
  int y1, y2, y3, y4, y5, y6, y7, y8;
  int sample_x, sample_y, sample_w, sample_h;
  int max_w, max_h, bmp_w, bmp_h;
  BITMAP *pour_voir;
  retrieve_choose_level_xy (&x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8,
			    &y1, &y2, &y3, &y4, &y5, &y6, &y7, &y8);
  rectfill (screen, menu_real_x (x4), menu_real_y (y1),
	    menu_real_x (x4 + MENU_W_SAMPLE) - 1,
	    menu_real_y (y1 + MENU_H_SAMPLE) - 1, MENU_BG);
  lw_maptex_set_fg_palette (CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, 0,
			    LW_RANDOM_ON, CONFIG_USE_DEFAULT_TEXTURE);
  lw_maptex_set_bg_palette (CONFIG_LEVEL_MAP, CONFIG_LEVEL_BG, 0,
			    LW_RANDOM_ON, CONFIG_USE_DEFAULT_TEXTURE);
  my_set_palette ();
  pour_voir = lw_maptex_create_map
    (CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, CONFIG_LEVEL_BG, 0, LW_RANDOM_ON,
     MIN_MAP_RES_W[CONFIG_MIN_MAP_RES], MIN_MAP_RES_H[CONFIG_MIN_MAP_RES],
     CONFIG_USE_DEFAULT_TEXTURE);
  if (pour_voir)

    {
      max_w = menu_real_x (MENU_W_SAMPLE);
      max_h = menu_real_y (MENU_H_SAMPLE);
      bmp_w = pour_voir->w;
      bmp_h = pour_voir->h;
      if (max_w * bmp_h < max_h * bmp_w)

	{
	  sample_w = max_w;
	  sample_h = (max_w * bmp_h) / bmp_w;
	}

      else

	{
	  sample_w = (max_h * bmp_w) / bmp_h;
	  sample_h = max_h;
	}
      sample_x = menu_real_x (x4) + (max_w - sample_w) / 2;
      sample_y = menu_real_y (y1) + (max_h - sample_h) / 2;
      stretch_blit (pour_voir, screen, 0, 0, bmp_w, bmp_h, sample_x,
		    sample_y, sample_w, sample_h);
      destroy_bitmap (pour_voir);
    }
  if (!lw_maptex_is_custom_texture_used
      (CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, 0, LW_RANDOM_ON,
       CONFIG_USE_DEFAULT_TEXTURE))

    {
      rectfill (screen, menu_real_x (x1), menu_real_y (y1),
		menu_real_x (x1 + MENU_W_LEVEL) - 1,
		menu_real_y (y1 + MENU_W_LEVEL) - 1, MENU_BG);
      pour_voir =
	lw_maptex_create_fg (CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, 0,
			     LW_RANDOM_ON, CONFIG_USE_DEFAULT_TEXTURE);
      if (pour_voir)

	{
	  stretch_blit (pour_voir, screen, 0, 0, pour_voir->w, pour_voir->h,
			menu_real_x (x1), menu_real_x (y1),
			menu_real_x (MENU_W_LEVEL),
			menu_real_x (MENU_W_LEVEL));
	  destroy_bitmap (pour_voir);
	}
    }
  if (!lw_maptex_is_custom_texture_used
      (CONFIG_LEVEL_MAP, CONFIG_LEVEL_BG, 0, LW_RANDOM_ON,
       CONFIG_USE_DEFAULT_TEXTURE))

    {
      rectfill (screen, menu_real_x (x8), menu_real_y (y1),
		menu_real_x (x8 + MENU_W_LEVEL) - 1,
		menu_real_y (y1 + MENU_W_LEVEL) - 1, MENU_BG);
      pour_voir =
	lw_maptex_create_bg (CONFIG_LEVEL_MAP, CONFIG_LEVEL_BG, 0,
			     LW_RANDOM_ON, CONFIG_USE_DEFAULT_TEXTURE);
      if (pour_voir)

	{
	  stretch_blit (pour_voir, screen, 0, 0, pour_voir->w, pour_voir->h,
			menu_real_x (x8), menu_real_x (y1),
			menu_real_x (MENU_W_LEVEL),
			menu_real_x (MENU_W_LEVEL));
	  destroy_bitmap (pour_voir);
	}
    }
}


/*------------------------------------------------------------------*/
static void
set_dialog_visibility (DIALOG * d, int state)
{
  if (state)

    {
      d->flags &= ~D_HIDDEN;
    }

  else

    {
      d->flags |= D_HIDDEN;
    }
  if (!(d->flags & D_HIDDEN))

    {
      d->proc (MSG_DRAW, d, 0);
    }
}


/*------------------------------------------------------------------*/
static void
set_fg_dialogs_visibility (DIALOG * d, int *just_hidden)
{
  int state;
  static int last_state = 1;
  int i;
  state =
    !lw_maptex_is_custom_texture_used (CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, 0,
				       LW_RANDOM_ON,
				       CONFIG_USE_DEFAULT_TEXTURE);
  if (state == 0 && last_state != 0)

    {
      *just_hidden = 1;
    }
  last_state = state;
  set_dialog_visibility (d + 5, state);
  set_dialog_visibility (d + 9, state);
  set_dialog_visibility (d + 10, state);
  set_dialog_visibility (d + 14, state);
  for (i = 21; i < 149; ++i)

    {
      set_dialog_visibility (d + i, state);
    }
}


/*------------------------------------------------------------------*/
static void
set_bg_dialogs_visibility (DIALOG * d, int *just_hidden)
{
  int state;
  static int last_state = 1;
  int i;
  state =
    !lw_maptex_is_custom_texture_used (CONFIG_LEVEL_MAP, CONFIG_LEVEL_BG, 0,
				       LW_RANDOM_ON,
				       CONFIG_USE_DEFAULT_TEXTURE);
  if (state == 0 && last_state != 0)

    {
      *just_hidden = 1;
    }
  last_state = state;
  set_dialog_visibility (d + 6, state);
  set_dialog_visibility (d + 11, state);
  set_dialog_visibility (d + 12, state);
  set_dialog_visibility (d + 15, state);
  for (i = 149; i < 277; ++i)

    {
      set_dialog_visibility (d + i, state);
    }
}


/*------------------------------------------------------------------*/
static void
box_for_choose_map (void)
{
  DIALOG d[10];
  DIALOG_PLAYER *dp;
  int i;
  int x1, x2, x3, x4, x5, x6, x7, x8;
  int y1, y2, y3, y4, y5, y6, y7, y8;
  memset (d, 0, sizeof (d));
  retrieve_choose_level_xy (&x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8,
			    &y1, &y2, &y3, &y4, &y5, &y6, &y7, &y8);
  d[0].x = x4;
  d[0].y = y1;
  d[0].w = MENU_W_SAMPLE;
  d[0].h = MENU_H_SAMPLE;
  d[1].x = x1;
  d[1].y = y1;
  d[1].w = MENU_W_LEVEL;
  d[1].h = MENU_W_LEVEL;
  d[2].x = x8;
  d[2].y = y1;
  d[2].w = MENU_W_LEVEL;
  d[2].h = MENU_W_LEVEL;
  d[3].x = x8;
  d[3].y = y5;
  d[3].w = MENU_W_LEVEL;
  d[3].h = MENU_H_LEVEL;
  d[4].x = x1;
  d[4].y = y4;
  d[4].w = MENU_W_LEVEL;
  d[4].h = MENU_H_LEVEL;
  d[5].x = x8;
  d[5].y = y4;
  d[5].w = MENU_W_LEVEL;
  d[5].h = MENU_H_LEVEL;
  for (i = 0; i < 6; ++i)

    {
      d[i].proc = d_box_proc;
      d[i].flags = 0;
      color_one_dialog (d + i);
      menu_real_coord (d + i);
      d[i].x--;
      d[i].y--;
      d[i].w += 2;
      d[i].h += 2;
    }
  if (lw_maptex_is_custom_texture_used
      (CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, 0, LW_RANDOM_ON,
       CONFIG_USE_DEFAULT_TEXTURE))

    {
      d[1].flags = d[4].flags |= D_HIDDEN;
    }
  if (lw_maptex_is_custom_texture_used
      (CONFIG_LEVEL_MAP, CONFIG_LEVEL_BG, 0, LW_RANDOM_ON,
       CONFIG_USE_DEFAULT_TEXTURE))

    {
      d[2].flags = d[5].flags |= D_HIDDEN;
    }
  d[6].proc = NULL;		/* 6 remplace par 9 pour boites des sliders */

  /* caduque depuis que my_slider fait tout tout seul */
  dp = my_init_dialog (d, 0);
  my_update_dialog (dp);
  shutdown_dialog (dp);
}


/*------------------------------------------------------------------*/
static void
update_choose_map (DIALOG * d, int map, int fg, int bg, int random,
		   int use_default_texture)
{
  static char res_buffer[30];
  int res_w, res_h;
  int just_hidden = 0;
  int i;
  static LW_MUTEX_DATA mutex = { NULL };
  if (lw_mutex_trylock (&mutex))

    {

      /*
       * These parameters are passed to the function and set globally within
       * the mutex to avoid possible bugs due to the fact that a parameter
       * could be changed while this function is executed.
       */
      CONFIG_LEVEL_MAP = map;
      CONFIG_LEVEL_FG = fg;
      CONFIG_LEVEL_BG = bg;
      LW_RANDOM_ON = random;
      CONFIG_USE_DEFAULT_TEXTURE = use_default_texture;
      scare_mouse ();
      set_fg_dialogs_visibility (d, &just_hidden);
      set_bg_dialogs_visibility (d, &just_hidden);
      if (just_hidden)

	{

	  /*
	   * OK, now we need to "hide" some part of the screen
	   * therefore we simply paste the background image and
	   * redraw everything. This is called a performance killer.
	   */
	  display_back_image ();
	  for (i = 0; (d + i)->proc != NULL; ++i)

	    {
	      if (!((d + i)->flags & D_HIDDEN))

		{
		  (d + i)->proc (MSG_DRAW, d + i, 0);
		}
	    }
	}
      box_for_choose_map ();
      LW_MACRO_SNPRINTF1 ((char *) d[13].dp, BUFFER_EDIT_NUMBER, "%d",
			  CONFIG_LEVEL_MAP + 1);
      if (!((d + 13)->flags & D_HIDDEN))

	{
	  my_edit_proc (MSG_DRAW, d + 13, 0);
	}
      LW_MACRO_SNPRINTF1 ((char *) d[14].dp, BUFFER_EDIT_NUMBER, "%d",
			  CONFIG_LEVEL_FG + 1);
      if (!((d + 14)->flags & D_HIDDEN))

	{
	  my_edit_proc (MSG_DRAW, d + 14, 0);
	}
      LW_MACRO_SNPRINTF1 ((char *) d[15].dp, BUFFER_EDIT_NUMBER, "%d",
			  CONFIG_LEVEL_BG + 1);
      if (!((d + 15)->flags & D_HIDDEN))

	{
	  my_edit_proc (MSG_DRAW, d + 15, 0);
	}
      d[4].d2 = CONFIG_LEVEL_MAP;
      if (CONFIG_LEVEL_FG >= CHOOSE_COLOR_NUMBER)
	d[5].d2 = CONFIG_LEVEL_FG - CHOOSE_COLOR_NUMBER;
      if (CONFIG_LEVEL_BG >= CHOOSE_COLOR_NUMBER)
	d[6].d2 = CONFIG_LEVEL_BG - CHOOSE_COLOR_NUMBER;
      if (d[4].d2 < 0)
	d[4].d2 = d[4].d1;
      if (d[4].d2 > d[4].d1)
	d[4].d2 = 0;
      if (!((d + 4)->flags & D_HIDDEN))

	{
	  my_slider_proc (MSG_DRAW, d + 4, 0);
	}
      if (d[5].d2 < 0)
	d[5].d2 = d[5].d1;
      if (d[5].d2 > d[5].d1)
	d[5].d2 = 0;
      if (!((d + 5)->flags & D_HIDDEN))

	{
	  my_slider_proc (MSG_DRAW, d + 5, 0);
	}
      if (d[6].d2 < 0)
	d[6].d2 = d[6].d1;
      if (d[6].d2 > d[6].d1)
	d[6].d2 = 0;
      if (!((d + 6)->flags & D_HIDDEN))

	{
	  my_slider_proc (MSG_DRAW, d + 6, 0);
	}
      lw_map_get_res (CONFIG_LEVEL_MAP, &res_w, &res_h, 0, LW_RANDOM_ON,
		      MIN_MAP_RES_W[CONFIG_MIN_MAP_RES],
		      MIN_MAP_RES_H[CONFIG_MIN_MAP_RES]);
      LW_MACRO_SPRINTF2 (res_buffer, "%dx%d", res_w, res_h);
      d[16].dp = res_buffer;
      my_textbox_proc (MSG_DRAW, d + 16, 0);
      d[17].dp = lw_map_get_readable_name (CONFIG_LEVEL_MAP, 0, LW_RANDOM_ON);
      my_textbox_proc (MSG_DRAW, d + 17, 0);
      display_chosen_level ();
      unscare_mouse ();
      lw_mutex_unlock (&mutex);
    }

  else

    {
      log_println_str ("Warning: too many calls to \"update_choose_map\"...");
    }
}


/*------------------------------------------------------------------*/
static int
slider_map (void *dp3, int d2)
{
  DIALOG *d;
  int map;
  play_click ();
  d = dp3;
  map = d2;
  update_choose_map (d, map, CONFIG_LEVEL_FG, CONFIG_LEVEL_BG, 0,
		     CONFIG_USE_DEFAULT_TEXTURE);
  return 0;
}


/*------------------------------------------------------------------*/
static int
slider_fg (void *dp3, int d2)
{
  DIALOG *d;
  int fg;
  play_click ();
  d = dp3;
  fg = d2 + CHOOSE_COLOR_NUMBER;
  update_choose_map (d, CONFIG_LEVEL_MAP, fg, CONFIG_LEVEL_BG, LW_RANDOM_ON,
		     CONFIG_USE_DEFAULT_TEXTURE);
  return 0;
}


/*------------------------------------------------------------------*/
static int
slider_bg (void *dp3, int d2)
{
  DIALOG *d;
  int bg;
  play_click ();
  d = dp3;
  bg = d2 + CHOOSE_COLOR_NUMBER;
  update_choose_map (d, CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, bg, LW_RANDOM_ON,
		     CONFIG_USE_DEFAULT_TEXTURE);
  return 0;
}


/*------------------------------------------------------------------*/
static void
update_use_default_texture_state (DIALOG * d)
{
  static char state_buffer[30];
  scare_mouse ();
  LW_MACRO_SPRINTF0 (state_buffer,
		     lw_lang_string (CONFIG_USE_DEFAULT_TEXTURE ?
				     LW_LANG_STRING_LEVEL_ON :
				     LW_LANG_STRING_LEVEL_OFF));
  d[19].dp = state_buffer;
  my_button_proc (MSG_DRAW, d + 19, 0);
  unscare_mouse ();
}


/*------------------------------------------------------------------*/
int
choose_map (void)
{
  int retour = 0, choix = MENU_QUICK_BACK;
  int i;
  int x1, x2, x3, x4, x5, x6, x7, x8;
  int y1, y2, y3, y4, y5, y6, y7, y8;
  DIALOG d[278];
  char buf1[BUFFER_EDIT_NUMBER + 1];
  char buf2[BUFFER_EDIT_NUMBER + 1];
  char buf3[BUFFER_EDIT_NUMBER + 1];
  memset (d, 0, sizeof (d));
  buf1[0] = 0;
  buf2[0] = 0;
  display_back_image ();
  set_palette_for_choose_color ();
  quick_buttons (d);
  retrieve_choose_level_xy (&x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8,
			    &y1, &y2, &y3, &y4, &y5, &y6, &y7, &y8);
  d[4].proc = d[5].proc = d[6].proc = my_slider_proc;
  d[4].dp = d[5].dp = d[6].dp = NULL;
  d[4].key = d[5].key = d[6].key = 0;
  d[4].dp2 = slider_map;
  d[5].dp2 = slider_fg;
  d[6].dp2 = slider_bg;
  d[4].dp3 = d[5].dp3 = d[6].dp3 = d;
  d[4].flags = d[5].flags = d[6].flags = 0;
  d[4].d1 = RAW_MAP_NUMBER - 1;
  d[5].d1 = d[6].d1 = RAW_TEXTURE_NUMBER - 1;
  d[4].d2 = CONFIG_LEVEL_MAP;
  d[5].d2 = CONFIG_LEVEL_FG;
  d[6].d2 = CONFIG_LEVEL_BG;
  d[4].x = x4;
  d[4].y = y5;
  d[4].w = MENU_W_SAMPLE;
  d[4].h = MENU_H_LEVEL;
  d[5].x = x3;
  d[6].x = x7;
  d[5].y = d[6].y = y2;
  d[5].w = d[6].w = MENU_H_LEVEL;
  d[5].h = d[6].h = y4 - y2 - MENU_H_SPACE;
  for (i = 7; i < 13; ++i)

    {
      d[i].proc = my_button_proc;
      d[i].key = 0;
      d[i].flags = D_EXIT;
      d[i].w = d[i].h = MENU_H_LEVEL;
    };
  d[7].dp = d[9].dp = d[11].dp = "-";
  d[8].dp = d[10].dp = d[12].dp = "+";
  d[7].x = x3;
  d[7].y = d[8].y = y5;
  d[8].x = x7;
  d[9].x = d[10].x = x3;
  d[11].x = d[12].x = x7;
  d[9].y = d[11].y = y4;
  d[10].y = d[12].y = y1;
  d[13].proc = d[14].proc = d[15].proc = my_edit_proc;
  d[13].flags = d[14].flags = d[15].flags = D_EXIT;
  d[13].dp = buf1;
  d[14].dp = buf2;
  d[15].dp = buf3;
  d[13].d1 = d[14].d1 = d[15].d1 = BUFFER_EDIT_NUMBER;
  d[13].d2 = d[14].d2 = d[15].d2 = 0;
  d[13].x = d[15].x = x8;
  d[14].x = x1;
  d[13].w = d[14].w = d[15].w = MENU_W_LEVEL;
  d[13].h = d[14].h = d[15].h = MENU_H_LEVEL;
  d[13].y = y5;
  d[14].y = d[15].y = y4;
  d[16].proc = my_textbox_proc;
  d[16].x = x1;
  d[16].y = y6;
  d[16].w = x5 - x1 - MENU_W_SPACE;
  d[16].h = MENU_H_LEVEL;
  d[16].flags = 0;
  d[16].dp = "";
  d[17].proc = my_textbox_proc;
  d[17].x = x5;
  d[17].y = y6;
  d[17].w = x8 - x5 + MENU_W_LEVEL;
  d[17].h = MENU_H_LEVEL;
  d[17].flags = 0;
  d[17].dp = "";
  d[18].proc = my_textbox_proc;
  d[18].x = x1;
  d[18].y = y7;
  d[18].w = x6 - x1 - MENU_W_SPACE;
  d[18].h = MENU_H_LEVEL;
  d[18].flags = 0;
  d[18].dp = lw_lang_string (LW_LANG_STRING_LEVEL_USEDEFAULTTEXTURE);
  d[19].proc = my_button_proc;
  d[19].x = x6;
  d[19].y = y7;
  d[19].w = x8 - x6 + MENU_W_LEVEL;
  d[19].h = MENU_H_LEVEL;
  d[19].flags = D_EXIT;
  d[19].dp = "";
  d[20].proc = my_button_proc;
  d[20].x = x3;
  d[20].y = y8;
  d[20].w = x8 - x3 - MENU_W_SPACE;
  d[20].h = MENU_H_LEVEL;
  d[20].flags = D_EXIT;
  d[20].dp = lw_lang_string (LW_LANG_STRING_LEVEL_RANDOMMAP);;

#ifdef DOS
  d[20].flags = D_HIDDEN;
#endif

  for (i = 4; i < 21; ++i)
    {
      color_one_dialog (d + i);
      menu_real_coord (d + i);
    }
  palette_128 (d + 21, menu_real_x (x1), menu_real_y (y3),
	       menu_real_x (MENU_W_LEVEL),
	       menu_real_y (y4 - y3 - MENU_H_SPACE));
  palette_128 (d + 149, menu_real_x (x8), menu_real_y (y3),
	       menu_real_x (MENU_W_LEVEL),
	       menu_real_y (y4 - y3 - MENU_H_SPACE));
  box_for_choose_map ();
  d[277].proc = NULL;
  update_use_default_texture_state (d);
  update_choose_map (d, CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, CONFIG_LEVEL_BG,
		     LW_RANDOM_ON, CONFIG_USE_DEFAULT_TEXTURE);
  while (retour == 0)

    {
      choix = my_do_dialog_no_clear (d, choix);
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

	    {
	      display_back_image ();
	      box_for_choose_map ();
	      update_choose_map (d, CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG,
				 CONFIG_LEVEL_BG, LW_RANDOM_ON,
				 CONFIG_USE_DEFAULT_TEXTURE);
	    }
	  break;
	case MENU_QUICK_PLAY:
	  retour = MENU_PLAY;
	  break;
	case 7:
	  d[4].d2--;
	  if (d[4].d2 < 0)
	    d[4].d2 = d[4].d1;
	  slider_map (d[4].dp3, d[4].d2);
	  break;
	case 8:
	  d[4].d2++;
	  if (d[4].d2 > d[4].d1)
	    d[4].d2 = 0;
	  slider_map (d[4].dp3, d[4].d2);
	  break;
	case 9:
	  d[5].d2--;
	  if (d[5].d2 < 0)
	    d[5].d2 = d[5].d1;
	  slider_fg (d[5].dp3, d[5].d2);
	  break;
	case 10:
	  d[5].d2++;
	  if (d[5].d2 > d[5].d1)
	    d[5].d2 = 0;
	  slider_fg (d[5].dp3, d[5].d2);
	  break;
	case 11:
	  d[6].d2--;
	  if (d[6].d2 < 0)
	    d[6].d2 = d[6].d1;
	  slider_bg (d[6].dp3, d[6].d2);
	  break;
	case 12:
	  d[6].d2++;
	  if (d[6].d2 > d[6].d1)
	    d[6].d2 = 0;
	  slider_bg (d[6].dp3, d[6].d2);
	  break;
	case 13:
	  i = atoi (d[13].dp) - 1;
	  if (!(i >= 0 && i < RAW_MAP_NUMBER))

	    {
	      i = CONFIG_LEVEL_MAP;
	    }
	  play_click ();
	  update_choose_map (d, i, CONFIG_LEVEL_FG, CONFIG_LEVEL_BG, 0,
			     CONFIG_USE_DEFAULT_TEXTURE);
	  break;
	case 14:
	  i = atoi (d[14].dp) - 1;
	  if (!(i >= 0 && i < RAW_TEXTURE_NUMBER + CHOOSE_COLOR_NUMBER))

	    {
	      i = CONFIG_LEVEL_FG;
	    }
	  play_click ();
	  update_choose_map (d, CONFIG_LEVEL_MAP, i, CONFIG_LEVEL_BG,
			     LW_RANDOM_ON, CONFIG_USE_DEFAULT_TEXTURE);
	  break;
	case 15:
	  i = atoi (d[15].dp) - 1;
	  if (!(i >= 0 && i < RAW_TEXTURE_NUMBER + CHOOSE_COLOR_NUMBER))

	    {
	      i = CONFIG_LEVEL_BG;
	    }
	  play_click ();
	  update_choose_map (d, CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, i,
			     LW_RANDOM_ON, CONFIG_USE_DEFAULT_TEXTURE);
	  break;
	case 19:
	  i = CONFIG_USE_DEFAULT_TEXTURE ? 0 : 1;
	  update_choose_map (d, CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG,
			     CONFIG_LEVEL_BG, LW_RANDOM_ON, i);
	  update_use_default_texture_state (d);
	  break;
	case 20:
	  if (lw_random_generate_map ())

	    {
	      update_choose_map (d, CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG,
				 CONFIG_LEVEL_BG, 1,
				 CONFIG_USE_DEFAULT_TEXTURE);
	    }
	  break;
	default:
	  if (choix >= 21 && choix < 149)

	    {
	      i = choix - 21;
	      update_choose_map (d, CONFIG_LEVEL_MAP, i, CONFIG_LEVEL_BG,
				 LW_RANDOM_ON, CONFIG_USE_DEFAULT_TEXTURE);
	    }
	  if (choix >= 149 && choix < 277)

	    {
	      i = choix - 149;
	      update_choose_map (d, CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG, i,
				 LW_RANDOM_ON, CONFIG_USE_DEFAULT_TEXTURE);
	    }
	}
    }
  if (retour > 0)
    retour--;
  return retour;
}
