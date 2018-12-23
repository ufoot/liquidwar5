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
/* Liquid War homepage : https://ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* nom           : graphics.c                                       */
/* contenu       : choix des options graphiques                     */
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
#include "gfxmode.h"
#include "graphics.h"
#include "help.h"
#include "menu.h"
#include "sound.h"
#include "wave.h"
#include "lang.h"
#include "macro.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static char *
get_game_res_str (void)
{
  static char str[30];

  LW_MACRO_SPRINTF3 (str, "%s:%dx%d",
                     lw_lang_string (LW_LANG_STRING_GRAPHICS_GAME),
                     GFX_MODE_W[CONFIG_GFX_GAME],
                     GFX_MODE_H[CONFIG_GFX_GAME]);

  return str;
}

/*------------------------------------------------------------------*/
static char *
get_menu_res_str (void)
{
  static char str[30];

  LW_MACRO_SPRINTF3 (str, "%s:%dx%d",
                     lw_lang_string (LW_LANG_STRING_GRAPHICS_MENU),
                     GFX_MODE_W[CONFIG_GFX_MENU],
                     GFX_MODE_H[CONFIG_GFX_MENU]);

  return str;
}

/*------------------------------------------------------------------*/
static char *
get_wave_mode_str (void)
{
  if (CONFIG_WAVE_ON)
    return (lw_lang_string (LW_LANG_STRING_GRAPHICS_ON));
  else
    return (lw_lang_string (LW_LANG_STRING_GRAPHICS_OFF));
}

/*------------------------------------------------------------------*/
static char *
get_page_flip_str (void)
{
  if (CONFIG_PAGE_FLIP)
    return (lw_lang_string (LW_LANG_STRING_GRAPHICS_ON));
  else
    return (lw_lang_string (LW_LANG_STRING_GRAPHICS_OFF));
}

/*------------------------------------------------------------------*/
static char *
get_fullscreen_str (void)
{
  if (CONFIG_FULLSCREEN)
    return (lw_lang_string (LW_LANG_STRING_GRAPHICS_FULLSCREEN));
  else
    return (lw_lang_string (LW_LANG_STRING_GRAPHICS_WINDOWED));
}

/*------------------------------------------------------------------*/
static int
slider_game_res (void *dp3, int d2)
{
  int *changed;

  LW_MACRO_NOP (d2);

  play_click ();

  changed = dp3;
  *changed = 1;

  return 0;
}

/*------------------------------------------------------------------*/
static int
slider_menu_res (void *dp3, int d2)
{
  int *changed;

  LW_MACRO_NOP (d2);

  play_click ();

  changed = dp3;
  *changed = 1;

  my_set_palette ();

  return 0;
}

/*------------------------------------------------------------------*/
static int
slider_brightness (void *dp3, int d2)
{
  LW_MACRO_NOP (dp3);
  LW_MACRO_NOP (d2);

  play_click ();
  CONFIG_BRIGHTNESS = d2;
  my_set_palette ();

  return 0;
}

/*------------------------------------------------------------------*/
int
graphic_options (void)
{

  /* la gestion des paramtres curseur a ete mise en commentaire... */

  int menu_res_changed = 0;
  int fullscreen_changed = 0;
  int game_res_changed = 0;
  DIALOG_PLAYER *dp;
  DIALOG d[20];
  int i, choix = 4, retour = 0;
  static int y_pos[7] = { 0, 1, 3, 4, 5, 6, 8 };
  int *temp;

  memset (d, 0, sizeof (d));

  display_back_image ();
  while (retour == 0)
    {
      for (i = 0; i < 7; ++i)
        {
          standard_button (d + i + 4, 0, y_pos[i], 2, 10);
          d[i + 4].proc = my_textbox_proc;
        }
      for (i = 0; i < 7; ++i)
        {
          standard_button (d + i + 11, 1, y_pos[i], 2, 10);
          d[i + 11].proc = my_slider_proc;
        }

      standard_button (d + 18, 0, 9, 1, 10);
      d[18].dp = lw_lang_string (LW_LANG_STRING_GRAPHICS_ADVANCED);

      quick_buttons (d);
      d[4].dp = lw_lang_string (LW_LANG_STRING_GRAPHICS_VIDEOMODE);
      d[5].dp = lw_lang_string (LW_LANG_STRING_GRAPHICS_BRIGHTNESS);
      d[6].dp = get_menu_res_str ();
      d[7].dp = get_game_res_str ();
      d[8].dp = lw_lang_string (LW_LANG_STRING_GRAPHICS_VIEWPORTSIZE);
      d[9].dp = lw_lang_string (LW_LANG_STRING_GRAPHICS_PAGEFLIPPING);
      d[10].dp = lw_lang_string (LW_LANG_STRING_GRAPHICS_WAVES);
      d[11].dp = d[12].dp = d[13].dp = d[14].dp = d[15].dp = d[16].dp =
        d[17].dp = NULL;
      d[12].d1 = 18;
      d[13].d1 = d[14].d1 = 4;
      d[15].d1 = 8;
      d[16].d1 = 8;
      d[15].dp3 = &CONFIG_VIEWPORT_SIZE;
      for (i = 15; i < 16; ++i)
        {
          d[i].dp = NULL;
          d[i].dp2 = slider_int;
          temp = d[i].dp3;
          d[i].d2 = *temp;
        }
      d[12].dp = d[13].dp = d[14].dp = NULL;
      d[12].d2 = CONFIG_BRIGHTNESS;
      d[13].d2 = CONFIG_GFX_MENU;
      d[14].d2 = CONFIG_GFX_GAME;
      d[12].dp2 = slider_brightness;
      d[13].dp2 = slider_menu_res;
      d[14].dp2 = slider_game_res;
      d[13].dp3 = &menu_res_changed;
      d[14].dp3 = &game_res_changed;

      d[11].proc = my_button_proc;
      d[11].dp = get_fullscreen_str ();
#ifdef DOS
      /*
       * Under DOS, fullscreen or windowed makes no sense, so we simply
       * remove the button to avoid confusion.
       */
      d[4].flags |= D_HIDDEN;
      d[11].flags = D_EXIT | D_HIDDEN;
#else
      d[11].flags = D_EXIT;
#endif

      d[16].proc = my_button_proc;
      d[16].dp = get_page_flip_str ();
      d[16].flags = D_EXIT;

      d[17].proc = my_button_proc;
      d[17].dp = get_wave_mode_str ();
      d[17].flags = D_EXIT;

      d[19].proc = 0;

      game_res_changed = 0;
      if (menu_res_changed || fullscreen_changed)
        {
          menu_res_changed = 0;
          fullscreen_changed = 0;
          display_back_image ();
          dp = my_init_dialog (d, choix);
          my_fade_in ();
        }
      else
        dp = my_init_dialog (d, choix);

      show_mouse (screen);
      while ((menu_res_changed == 0) &&
             (game_res_changed == 0) && my_update_dialog (dp))
        ;
      choix = shutdown_dialog (dp);


      if (menu_res_changed)
        {
          if (d[13].d2 != CONFIG_GFX_MENU)
            {
              my_fade_out ();
              if (set_resolution (d[13].d2, 0, CONFIG_FULLSCREEN, NULL))
                CONFIG_GFX_MENU = 0;
              else
                CONFIG_GFX_MENU = d[13].d2;
            }
          d[13].dp = get_menu_res_str ();
        }
      else
        {
          if (game_res_changed)
            {
              CONFIG_GFX_GAME = d[14].d2;
              d[14].dp = get_game_res_str ();
              retour = 0;
            }
          else
            {
              play_click ();
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
                  retour = MENU_PLAY;
                  break;
                case 11:
                  CONFIG_FULLSCREEN = CONFIG_FULLSCREEN ? 0 : 1;
                  d[11].dp = get_fullscreen_str ();
                  scare_mouse ();
                  my_button_proc (MSG_DRAW, d + 11, 0);
                  unscare_mouse ();
                  fullscreen_changed = 1;
                  my_fade_out ();
                  set_resolution (d[13].d2, 0, CONFIG_FULLSCREEN, NULL);
                  break;
                case 16:
                  CONFIG_PAGE_FLIP = CONFIG_PAGE_FLIP ? 0 : 1;
                  d[16].dp = get_page_flip_str ();
                  scare_mouse ();
                  my_button_proc (MSG_DRAW, d + 16, 0);
                  unscare_mouse ();
                  break;
                case 17:
                  CONFIG_WAVE_ON = CONFIG_WAVE_ON ? 0 : 1;
                  d[17].dp = get_wave_mode_str ();
                  scare_mouse ();
                  my_button_proc (MSG_DRAW, d + 17, 0);
                  unscare_mouse ();
                  break;
                case 18:
                  retour = wave_options ();
                  display_back_image ();
                  break;
                }
            }
        }
    }

  if (retour > 0)
    retour--;

  return retour;
}
