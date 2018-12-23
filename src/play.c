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
/* nom           : play.c                                           */
/* contenu       : fonction play du menu principal                  */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>
#include <allegro.h>

#include "back.h"
#include "config.h"
#include "decal.h"
#include "dialog.h"
#include "error.h"
#include "game.h"
#include "gfxmode.h"
#include "palette.h"
#include "maptex.h"
#include "play.h"
#include "score.h"
#include "sound.h"
#include "sock2gen.h"
#include "keyexch.h"
#include "music.h"
#include "lang.h"
#include "mouse.h"
#include "log.h"
#include "network.h"
#include "random.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * this function is basically what is launched when one presses
 * the play button. in fact it performs initializations and sanity
 * checks and then launches the game function located in game.c
 */
int
play_sequence (void)
{
  int result;

  lw_config_set_current_rules_to_default ();

  result = play_sequence_ex ();

  return result;
}

/*------------------------------------------------------------------*/
/*
 * Internal version of play_sequence, used for network games
 */
int
play_sequence_ex (void)
{
  char *message;
  int retour = 0;
  DIALOG d[2];
  DIALOG_PLAYER *dp;
  int flip_enabled = 0;

  memset (d, 0, sizeof (d));

  d[1].proc = 0;

  /*
   * we clear the screen by blitting the Liquid War 5 logo
   */
  display_back_image ();
  /*
   * we display a "Get ready!" message so that the user does not worry
   * about what's going on
   */
  main_message (d, lw_lang_string (LW_LANG_STRING_PLAY_GETREADY));
  /*
   * we call update dialog artificially so that the "Get ready"
   * message is actually blitted on the screen
   */
  dp = my_init_dialog (d, 0);
  my_update_dialog (dp);
  shutdown_dialog (dp);

  /*
   * we set up the 256 color palette so that it corresponds to
   * the textures the user has chosen
   */
  lw_maptex_set_fg_palette (CONFIG_LEVEL_MAP, CONFIG_LEVEL_FG,
                            LW_NETWORK_ON, LW_RANDOM_ON,
                            CONFIG_USE_DEFAULT_TEXTURE);
  lw_maptex_set_bg_palette (CONFIG_LEVEL_MAP, CONFIG_LEVEL_BG,
                            LW_NETWORK_ON, LW_RANDOM_ON,
                            CONFIG_USE_DEFAULT_TEXTURE);
  /*
   * we calculate how many teams are playing
   */
  calc_playing_teams ();
  /*
   * we set up the Allegro palette and LW's palette parameters so that
   * an optimized number of colors is used for each team's fighters
   */
  set_playing_teams_palette ();

  /*
   * if there's only one team selected we stop right away and quit
   */
  if (PLAYING_TEAMS < 2)
    {
      free_game_memory ();
      error_not_enough_teams ();
    }
  else
    {
      /*
       * we launch init_game from the game module, this function may fail
       * and if it does it returns a string containing the error message
       */
      if (!(message = init_game ()))
        {
          /*
           * we set up the sound of waves a little louder so that the player
           * understands he's playing *now*
           */
          WATER_VOLUME = CONFIG_WATER_VOLUME_GAME;
          /*
           * fade out cause we're going to switch video modes
           */
          my_fade_out ();

          /*
           * we change the resolution, but only if the game resolution is
           * actually different from the menu resolution
           */
          if (need_to_change_mode (CONFIG_GFX_GAME,
                                   CONFIG_GFX_MENU, CONFIG_PAGE_FLIP))
            set_resolution (CONFIG_GFX_GAME,
                            CONFIG_PAGE_FLIP,
                            CONFIG_FULLSCREEN, &flip_enabled);

          lw_mouse_hide ();

          /*
           * the blank round function initializes the double-buffer system
           * and the main display target, it might fail because of lacking memory
           */
          if (!blank_round ())
            {
              /*
               * fiat lux
               */
              my_fade_in ();

              /*
               * start playing some music
               */
              start_music ();

              /*
               * OK, here we go, let's enter the main game loop
               */
              game ();

              /*
               * stop the music
               */
              stop_music ();
            }

          /*
           * let's be clean 8-)
           */
          free_game_memory ();

          /*
           * we close the network socket abruptly
           */
          lw_sock_close (&LW_KEYEXCH_SOCK);

          /*
           * we turn the volume down
           */
          WATER_VOLUME = CONFIG_WATER_VOLUME_MENU;

          /*
           * fade out before video mode switching
           */
          my_fade_out ();
          /*
           * again, we change the video mode only if it is really required
           */
          if (need_to_change_mode (CONFIG_GFX_GAME,
                                   CONFIG_GFX_MENU,
                                   CONFIG_PAGE_FLIP && flip_enabled))
            set_resolution (CONFIG_GFX_MENU, 0, CONFIG_FULLSCREEN, NULL);

          lw_mouse_show ();

          /*
           * displays the 3 tubes with the score
           */
          retour = display_scores ();
        }
      else
        {
          /*
           * the main reason why init_game should fail is a lack of big data
           * this can be cured by selecting a smaller level or increasing the
           * amount of data reserved at startup (command line parameter)
           */
          free_game_memory ();

          /*
           * we close the network socket abruptly
           */
          lw_sock_close (&LW_KEYEXCH_SOCK);

          error_not_enough_big_data ();
        }
    }

  return retour;
}
