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
/* nom           : score.c                                          */
/* contenu       : calcul et affichage des scores                   */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "army.h"
#include "back.h"
#include "base.h"
#include "config.h"
#include "cursor.h"
#include "decal.h"
#include "dialog.h"
#include "disk.h"
#include "help.h"
#include "menu.h"
#include "score.h"
#include "sound.h"
#include "ticker.h"
#include "log.h"
#include "lang.h"
#include "checksum.h"
#include "startup.h"
#include "macro.h"

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_SCORE_TOMBOLA_MAX         500
#define LW_SCORE_DISP_BUFFER_SIZE     33

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int LW_SCORE_TOMBOLA_RESULT[NB_TEAMS];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static int
get_cursor (int classement)
{
  int i, j, indice_curseur = -1;

  for (i = 0; i < NB_TEAMS; ++i)
    if (CURRENT_CURSOR[i].active && CURRENT_CURSOR[i].loose_time < 0)
      {
        CURRENT_CURSOR[i].score_order = 1;

        for (j = 0; j < i; ++j)
          if (CURRENT_CURSOR[j].active &&
              ACTIVE_FIGHTERS[CURRENT_CURSOR[i].team]
              <= ACTIVE_FIGHTERS[CURRENT_CURSOR[j].team])
            CURRENT_CURSOR[i].score_order++;
        for (j = i + 1; j < NB_TEAMS; ++j)
          if (CURRENT_CURSOR[j].active &&
              ACTIVE_FIGHTERS[CURRENT_CURSOR[i].team]
              < ACTIVE_FIGHTERS[CURRENT_CURSOR[j].team])
            CURRENT_CURSOR[i].score_order++;
      }

  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (CURRENT_CURSOR[i].score_order == classement
          && (CURRENT_CURSOR[i].active || CURRENT_CURSOR[i].loose_time >= 0))
        indice_curseur = i;
    }

  return indice_curseur;
}

/*------------------------------------------------------------------*/
static void
write_score (int indice, char *buffer, int fill_level)
{
  int time, min, sec, pourmille, pourcent, decimale;
  int tombola;

  if (indice >= 0)
    {
      if (fill_level < 0)
        fill_level = 0;
      if (fill_level > 1000)
        fill_level = 1000;

      if (!STARTUP_TOMBOLA)
        {
          if (CURRENT_CURSOR[indice].score_order == 1 && PLAYING_TEAMS == 1)
            {
              LW_MACRO_STRNCPY (buffer,
                                lw_lang_string (LW_LANG_STRING_SCORE_WINNER),
                                LW_SCORE_DISP_BUFFER_SIZE);
            }
          else
            {
              if (CURRENT_CURSOR[indice].loose_time <= 0)
                {
                  pourmille =
                    (ACTIVE_FIGHTERS[CURRENT_CURSOR[indice].team] *
                     fill_level) / CURRENT_ARMY_SIZE;
                  pourcent = pourmille / 10;
                  decimale = pourmille % 10;
                  LW_MACRO_SNPRINTF2 (buffer,
                                      LW_SCORE_DISP_BUFFER_SIZE,
                                      "%d.%01d%%", pourcent, decimale);
                }
              else
                {
                  time =
                    (CURRENT_CURSOR[indice].loose_time * fill_level) / 1000;
                  min = time / 60;
                  sec = time % 60;
                  LW_MACRO_SNPRINTF2 (buffer,
                                      LW_SCORE_DISP_BUFFER_SIZE,
                                      "%02d:%02d", min, sec);
                }
            }
        }
      else
        {
          /*
           * OK, we're in "tombola" mode, so we display a random
           * number instead of the score.
           */

          tombola = (LW_SCORE_TOMBOLA_RESULT[indice] * fill_level) / 1000;
          LW_MACRO_SNPRINTF1 (buffer,
                              LW_SCORE_DISP_BUFFER_SIZE, "%03d", tombola);
        }
    }
  else
    {
      LW_MACRO_SNPRINTF0 (buffer, LW_SCORE_DISP_BUFFER_SIZE, "");
    }
}

/*------------------------------------------------------------------*/
static void
init_tombola ()
{
  int i;

  for (i = 0; i < NB_TEAMS; ++i)
    {
      LW_SCORE_TOMBOLA_RESULT[i] = random () % LW_SCORE_TOMBOLA_MAX + 1;
    }
}

/*------------------------------------------------------------------*/
static int
draw_score_bitmap (ALLEGRO_BITMAP * bitmap, int cursor, int ellipse_h,
                   int fill_level)
{
  int w, h;
  int color1 = 0, color2 = 0;
  int y_rect1, y_rect2, x_mid;
  int to_be_filled, to_be_drawn;

  w = bitmap->w;
  if (!(w & 1))
    w -= 1;
  h = bitmap->h;
  y_rect1 = ellipse_h / 2;
  y_rect2 = h - y_rect1 - 1;
  x_mid = w / 2;
  if (cursor < 0)
    fill_level = 0;
  else
    {
      color1 = CURRENT_CURSOR[cursor].color_entry + COLORS_PER_TEAM / 2;
      color2 = CURRENT_CURSOR[cursor].color_entry + COLORS_PER_TEAM - 1;
    }
  if (fill_level < 0)
    fill_level = 0;
  to_be_filled = fill_level ? 1 : 0;
  to_be_drawn = fill_level > 1000 ? 0 : 1;
  if (fill_level > 1000)
    fill_level = 1000;

  if (to_be_drawn)
    {
      fill_level = 1000 - fill_level;
      fill_level *= h - 3 * y_rect1;
      fill_level /= 1000;
      fill_level += 2 * y_rect1;

      rectfill (bitmap, 0, 0, w, h, 0);

      if (to_be_filled)
        ellipsefill (bitmap, x_mid, y_rect2, x_mid, ellipse_h / 2, color1);
      ellipse (bitmap, x_mid, y_rect2, x_mid, ellipse_h / 2, MENU_FG);
      if (to_be_filled)
        {
          rectfill (bitmap, 1, fill_level, x_mid * 2 - 1, y_rect2 - 1,
                    color1);
          ellipsefill (bitmap, x_mid, fill_level, x_mid - 1,
                       ellipse_h / 2 - 1, color2);
        }

      vline (bitmap, 0, y_rect1, y_rect2, MENU_FG);
      vline (bitmap, x_mid * 2, y_rect1, y_rect2, MENU_FG);

      ellipse (bitmap, x_mid, y_rect1, x_mid, ellipse_h / 2, MENU_FG);
    }
  return to_be_drawn;
}

/*------------------------------------------------------------------*/
int
display_scores (void)
{
  int last_draw_done = 0;
  int first_ticker, diff_ticker;
  int retour = 0, choix = MENU_QUICK_MAIN;
  DIALOG d[11];
  DIALOG_PLAYER *dp;
  char buf[3][LW_SCORE_DISP_BUFFER_SIZE];
  char buf_old[3][LW_SCORE_DISP_BUFFER_SIZE];
  int x[3];
  int h[3];
  int w, y;
  int ellipse_h;
  int i;
  int cursor[3];
  ALLEGRO_BITMAP *eprouvette[3];
  int time_delay[3] = { 500, 2500, 1500 };
  int fill_level;
  int done[3];

  memset (d, 0, sizeof (d));

  quick_buttons (d);

  for (i = 0; i < 3; ++i)
    {
      standard_small_button (d + i + 4, i, 10, 3, 11);
      d[i + 4].proc = my_textbox_proc;
    }

  d[4].dp = "";
  d[5].dp = "";
  d[6].dp = "";

  standard_button (d + 7, 0, 4, 3, 11);
  standard_button (d + 8, 1, 0, 3, 11);
  standard_button (d + 9, 2, 2, 3, 11);

  for (i = 0; i < 3; ++i)
    {
      d[i + 7].proc = my_textbox_proc;
      d[i + 7].flags = D_HIDDEN;
    }

  d[7].dp = "";
  d[8].dp = "";
  d[9].dp = "";

  d[10].proc = NULL;

  init_tombola ();

  ellipse_h = d[4].h;
  y = d[4].y - menu_real_y (MENU_H_SPACE);
  h[1] = 2 * y - SCREEN_H;
  h[0] = h[1] / 2;
  h[2] = (3 * h[1]) / 4;
  w = d[4].w;
  x[0] = d[4].x;
  x[1] = d[5].x;
  x[2] = d[6].x;

  cursor[0] = get_cursor (3);
  cursor[1] = get_cursor (1);
  cursor[2] = get_cursor (2);

  for (i = 0; i < 3; ++i)
    {
      done[i] = 0;
      write_score (cursor[i], buf[i], 0);
      buf_old[i][0] = '\0';
      eprouvette[i] = my_create_bitmap (w, h[i]);
    }

  d[MENU_QUICK_QUIT].flags = D_HIDDEN;
  d[MENU_QUICK_PLAY].flags = D_HIDDEN;

  display_back_image ();
  dp = my_init_dialog (d, 0);
  my_update_dialog (dp);
  shutdown_dialog (dp);

  my_fade_in ();

  play_win ();
  first_ticker = get_ticker ();

  while (retour == 0)
    {
      if (!last_draw_done)
        {
          dp = my_init_dialog (d, choix);
          while (my_update_dialog (dp) && !last_draw_done)
            {
              scare_mouse ();
              diff_ticker = get_ticker () - first_ticker;
              if (cursor[0] < 0)
                diff_ticker += 1000;

              for (i = 0; i < 3; ++i)
                {
                  if (!done[i])
                    {
                      fill_level = diff_ticker - time_delay[i];
                      if (draw_score_bitmap
                          (eprouvette[i], cursor[i], ellipse_h, fill_level))
                        {
                          draw_sprite (screen, eprouvette[i], x[i], y - h[i]);
                          if (fill_level > 0)
                            {
                              write_score (cursor[i], buf[i], fill_level);
                              if (strcmp (buf[i], buf_old[i]) != 0)
                                {
                                  d[4 + i].dp = buf[i];
                                  d[4 + i].proc (MSG_DRAW, d + 4 + i, 0);
                                  LW_MACRO_STRCPY (buf_old[i], buf[i]);
                                }
                            }
                        }
                      else
                        {
                          if (i == 1)
                            {
                              last_draw_done = 1;
                            }

                          d[7 + i].dp = CURRENT_CURSOR[cursor[i]].name;
                          d[7 + i].flags = 0;
                          d[7 + i].proc (MSG_DRAW, d + 7 + i, 0);

                          done[i] = 1;
                        }
                    }
                }
              unscare_mouse ();

              rest (10);
            }
          choix = shutdown_dialog (dp);
        }

      if (last_draw_done)
        {
          scare_mouse ();
          for (i = 0; i < 3; ++i)
            draw_sprite (screen, eprouvette[i], x[i], y - h[i]);
          unscare_mouse ();

          d[MENU_QUICK_QUIT].flags = D_EXIT;
          d[MENU_QUICK_PLAY].flags = D_EXIT;
          choix = my_do_dialog_no_clear (d,
                                         last_draw_done == 1 ? MENU_QUICK_BACK
                                         : choix);
          last_draw_done = 2;
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
          retour = MENU_PLAY;
          break;
        }
    }

  for (i = 0; i < 3; ++i)
    destroy_bitmap (eprouvette[i]);
  if (retour > 0)
    retour--;

  return retour;
}
