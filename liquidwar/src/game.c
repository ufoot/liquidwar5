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
/* Copyright (C) 1998-2016 Christian Mauduit                                 */
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
/* nom           : game.c                                           */
/* contenu       : organisation du jeu                              */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdlib.h>
#ifdef UNIX
#include <unistd.h>
#endif
#include <allegro.h>

#include "area.h"
#include "autoplay.h"
#include "back.h"
#include "army.h"
#include "bigdata.h"
#include "config.h"
#include "cursor.h"
#include "decal.h"
#include "disp.h"
#include "distor.h"
#include "fighter.h"
#include "game.h"
#include "grad.h"
#include "info.h"
#include "map.h"
#include "maptex.h"
#include "menu.h"
#include "message.h"
#include "mouse.h"
#include "move.h"
#include "pion.h"
#include "profile.h"
#include "code.h"
#include "sound.h"
#include "ticker.h"
#include "time.h"
#include "viewport.h"
#include "watchdog.h"
#include "network.h"
#include "lang.h"
#include "exit.h"
#include "capture.h"
#include "startup.h"
#include "random.h"

/*==================================================================*/
/* definitions de constantes                                        */
/*==================================================================*/

/*==================================================================*/
/* definitions de types                                             */
/*==================================================================*/

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

int LW_GAME_RUNNING = 0;

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/*
 * this function calculates how many teams will be playing
 */
void
calc_playing_teams (void)
{
  int i;

  PLAYING_TEAMS = 0;

  /*
   * loop for all the possible teams, that's to say each of the
   * 6 areas in the "teams" menu
   */
  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (LW_NETWORK_ON)
	{
	  /*
	   * We're in a network game
	   */
	  if (LW_NETWORK_INFO[i].active)
	    {
	      PLAYING_TEAMS++;
	    }
	}
      else
	{
	  /*
	   * Not a network game, we only check the local config
	   */
	  if (CONFIG_CONTROL_TYPE[i] != CONFIG_CONTROL_TYPE_OFF)
	    {
	      PLAYING_TEAMS++;
	    }
	}
    }
}

/*------------------------------------------------------------------*/
/* gestion des actions pendant le jeu                               */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* deplacement des curseurs                                         */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*
 * associates a control method to a cursor
 */
static void
init_cursor_control_method (void)
{
  int i;


  for (i = 0; i < NB_TEAMS; ++i)
    {
      if (LW_NETWORK_ON)
	{
	  CURRENT_CURSOR[i].from_network = LW_NETWORK_INFO[i].network;
	  CURRENT_CURSOR[i].control_type = LW_NETWORK_INFO[i].control_type;
	}
      else
	{
	  /*
	   * local teams
	   */
	  CURRENT_CURSOR[i].from_network = 0;
	  CURRENT_CURSOR[i].control_type = CONFIG_CONTROL_TYPE[i];
	}
    }
}

/*------------------------------------------------------------------*/
/* on verifie si des equipes doivent disparaitre                    */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*
 * checks if a team has lost the game, and make it leave the game
 * if necessary
 */
int
check_loose_team (void)
{
  int i = 0, lost = 0;

  /*
   * loop for all the playing teams. we stop when we first find a
   * team which has lost. however, if another one has lost, it will
   * be discovered at the next game round, so it's not a problem
   * and anyway, teams rarely loose at the same round 8-)
   */
  while (i < PLAYING_TEAMS && !lost)
    {
      /*
       * if there are no fighters left, then the game is lost
       */
      if (ACTIVE_FIGHTERS[i] == 0)
	{
	  /*
	   * nothing will be calculated for this team anymore
	   */
	  eliminate_team (i);
	  lost = 1;
	}
      else
	++i;
    }
  /*
   * now we check if the previous loop has given some results
   */
  if (lost)
    {
      /*
       * sound effect
       */
      play_loose ();
      /*
       * remove the team from the info bar, so that room if freed for 
       * other teams
       */
      free_info_bar ();
    }

  return (lost);
}

/*------------------------------------------------------------------*/
/* initialisations du jeu                                           */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*
 * initialization of the game, the menu system has been leaved
 * and the game has to start right away
 */
char *
init_game (void)
{
  int i;
  int big_data;
  int max_mem_reached = 0;
  int last_try = 0;
  int ok = 0;

  /*
   * this string will be used to display error messages in case
   * something goes wrong. as long as message is not NULL it
   * the game can not be launched
   */
  char *message = NULL;

  /*
   * creation of a bitmap which represents the battlefield
   * this map will be left as is and will never be changed
   * it is used to "cache" a blank map (by blank we mean with
   * no fighters drawn on it)
   */
  if (!message &&
      (CURRENT_AREA_BACK = lw_maptex_create_map
       (CONFIG_LEVEL_MAP,
	CONFIG_LEVEL_FG,
	CONFIG_LEVEL_BG,
	LW_NETWORK_ON,
	LW_RANDOM_ON,
	MIN_MAP_RES_W[LW_CONFIG_CURRENT_RULES.min_map_res],
	MIN_MAP_RES_H[LW_CONFIG_CURRENT_RULES.min_map_res],
	CONFIG_USE_DEFAULT_TEXTURE)) == NULL)
    message = lw_lang_string (LW_LANG_STRING_GAME_BACKMEMFAIL);

  /*
   * creation of the map which will be used for display
   * operations, it will be changed later, basically fighters
   * will be drawn on it
   */
  if (!message &&
      (CURRENT_AREA_DISP = lw_maptex_create_map
       (CONFIG_LEVEL_MAP,
	CONFIG_LEVEL_FG,
	CONFIG_LEVEL_BG,
	LW_NETWORK_ON,
	LW_RANDOM_ON,
	MIN_MAP_RES_W[LW_CONFIG_CURRENT_RULES.min_map_res],
	MIN_MAP_RES_H[LW_CONFIG_CURRENT_RULES.min_map_res],
	CONFIG_USE_DEFAULT_TEXTURE)) == NULL)
    message = lw_lang_string (LW_LANG_STRING_GAME_DISPMEMFAIL);

  if (!message)
    {
      for (i = 0; !max_mem_reached && !ok; ++i)
	{
	  message = NULL;
	  reset_big_data ();

	  /*
	   * creation of the mesh, see mesh.c for details
	   */
	  if (!message && create_mesh ())
	    message = lw_lang_string (LW_LANG_STRING_GAME_MESHMEMFAIL);

	  /*
	   * creation of the game area, see area.c for details
	   */
	  if (!message && create_game_area ())
	    message = lw_lang_string (LW_LANG_STRING_GAME_AREAMEMFAIL);

	  /*
	   * creation of the armies, see army.c for details
	   */
	  if (!message && create_army ())
	    message = lw_lang_string (LW_LANG_STRING_GAME_ARMYMEMFAIL);

	  if (message)
	    {
	      /*
	       * There's an error.
	       */
	      if (last_try)
		{
		  /*
		   * OK, last time we allocated memory, we couldn't
		   * allocate all we wanted to, and this is not
		   * enough -> we give up...
		   */
		  max_mem_reached = 0;
		}
	      else
		{
		  /*
		   * We increase the amount of allocated memory
		   */
		  STARTUP_BIG_DATA += LW_STARTUP_MEM_STEP;
		  if (STARTUP_BIG_DATA > LW_STARTUP_MEM_MAX)
		    {
		      last_try = 1;
		      STARTUP_BIG_DATA = LW_STARTUP_MEM_MAX;
		    }
		  big_data = STARTUP_BIG_DATA;
		  if (malloc_big_data ())
		    {
		      if (STARTUP_BIG_DATA != big_data)
			{
			  /*
			   * not all the memory could be
			   * allocated, we consider this is
			   * the last try.
			   */
			  last_try = 1;
			}
		    }
		  else
		    {
		      /*
		       * memory allocation failed...
		       */
		      max_mem_reached = 1;
		    }
		}
	    }
	  else
	    {
	      ok = 1;
	    }
	}
    }

  /*
   * what to do nothing failed (play!)
   */
  if (!message)
    {
      /*
       * these are basically initialization which need to be done 
       * and should never fail
       * check each function to know what it does!!!
       */
      reset_mesh ();
      reset_game_area ();
      reset_all_cursor ();
      init_cursor_control_method ();
      place_all_team ();
      init_move_fighters ();
      init_disp_cursor ();
    }

  /*
   * We set the network error flag to 0, if it becomes non-zero
   * then the game simply stops...
   */
  LW_NETWORK_ERROR_DETECTED = 0;

  return message;
}

/*------------------------------------------------------------------*/
/*
 * cleans up things after the game is finished
 * currently only the bitmaps have to be freed since mesh, maps etc...
 * are allocated in the "big data" structure (bigdata.c)
 */
void
free_game_memory (void)
{
  /*
   * the info bar allocates bitmaps for its display, get rid of'm
   */
  free_info_bar ();
  /*
   * the bigdata is not "freed" in the malloc sense, but it's
   * just clean to tell it to reset itself.it might be usefull
   * if some day the big data system is implemented in a more
   * conventional way
   */
  reset_big_data ();
  /*
   * free the bitmap used for displays if it exists
   */
  if (CURRENT_AREA_DISP)
    {
      destroy_bitmap (CURRENT_AREA_DISP);
      CURRENT_AREA_DISP = 0;
    }
  /*
   * free the clean empty bitmap if it exists
   */
  if (CURRENT_AREA_BACK)
    {
      destroy_bitmap (CURRENT_AREA_BACK);
      CURRENT_AREA_BACK = 0;
    }
}

/*------------------------------------------------------------------*/
/*
 * a blank round. but why should one want a blank round?
 * well, it's usefull to initialize the double-buffer system
 * if you don't use this function, the display of the game
 * might be dirty, so we artificially simulate a round
 * this is just about display, no logic operations
 */
int
blank_round (void)
{
  /*
   * the first call to page flip allocates a bitmap for drawing,
   * so it might fail...
   */
  if (page_flip ())
    {
      /*
       * the distorsion displayer is used for the wave effect
       */
      init_distorsion_displayer ();
      /*
       * prepares the basic layer for the first buffer
       */
      display_back_image ();
      rect_for_viewport ();
      /*
       * displays the map in the previously defined layer
       */
      disp_all_cursors ();
      display_area ();
      undisp_all_cursors ();
      /*
       * we now show what we have done to the player
       */
      page_flip ();
      /*
       * prepares the basic layer for the second buffer
       */
      display_back_image ();
      rect_for_viewport ();
      /*
       * clean up message queue
       */
      clear_message ();
      return 0;
    }
  else
    return -1;
}

/*------------------------------------------------------------------*/
/*
 * fills the screen with the right stuff
 * can display the map or special screens if we are in some
 * sort of debug mode
 */
static void
fill_next_screen (void)
{
  /*
   * GRAD_TO_DISP is the gradient we want to show
   * normally no gradient is displayed, so the gradient is 0
   */
  if (GRAD_TO_DISP == 0)
    {
      /*
       * we add the cursors to the map, temporary
       */
      disp_all_cursors ();
      /*
       * physical drawing of the map
       */
      display_area ();
      /*
       * we remove the cursors, for they might move next time
       * so they are no longer required
       */
      undisp_all_cursors ();
    }

  /*
   * GRAD_TO_DISP is between 1 & 6, we display the info concerning
   * the selected team, this way one can see in real tiem how
   * the gradient is calculated. for curious guys
   */
  if (GRAD_TO_DISP >= 1 && GRAD_TO_DISP <= 6)
    display_gradient (GRAD_TO_DISP - 1);

  /*
   * is GRAD_TO_DISP is 7 or 8 we display the plain mesh with
   * various display options
   */
  if (GRAD_TO_DISP >= 7 && GRAD_TO_DISP <= 8)
    display_mesh (GRAD_TO_DISP - 7);
}

/*------------------------------------------------------------------*/
/*
 * the logic function is the one which moves players, calculates stuff
 * etc... the more often this function is called, the faster the game
 * will go. it does nothing such as display, which is an independant
 * thing. every time this function is called on can consider that
 * "game round" has ellapsed. indeed, it's here that the global
 * GLOBAL_CLOCK is incremented 8-)
 */
static void
logic (void)
{
  /*
   * if the game is paused, we do not do everything
   * this test is done again later
   */
  if (!PAUSE_ON)
    {
      /*
       * moves the cursors. this will ask the keyboard module
       * for information, ask the AI functions what to do...
       */
      move_all_cursors ();
      /*
       * applying the cursors means that we communicate to
       * the game area the information about where the cursors are
       */
      apply_all_cursor ();
    }
  /*
   * the profile module enables CPU time tracking, so that the player
   * can now how much time is spent on various tasks
   */
  start_profile (SPREAD_PROFILE);
  if (!PAUSE_ON)
    {
      /*
       * spreads the gradient, ie calculates in the game area how far
       * each point is from the cursors, this information depends
       * on where the cursors are, this information being given
       * by apply_all_cursor
       */
      spread_single_gradient ();
    }
  /*
   * this call ends the time tracking associated to the spread function
   */
  stop_profile (SPREAD_PROFILE);

  start_profile (MOVE_PROFILE);
  if (!PAUSE_ON)
    {
      /*
       * now we move the fighters, ie each fighter of each team
       * will either move, attack his neighbor, die...
       */
      move_fighters ();
    }

  stop_profile (MOVE_PROFILE);

  if (!PAUSE_ON)
    {
      /*
       * we check if a team has lost
       *
       * This code used to be in the "display" functions, which
       * is a very bad idea for it caused network inconsistencies...
       */
      check_loose_team ();
    }

  /*
   * increments the clock, this way one can know how many game ticks
   * there has been. there's actually no link between this clock
   * and the GMT, indeed everything depends on the speed of the
   * computer where the game is run
   */
  GLOBAL_CLOCK++;
}

/*------------------------------------------------------------------*/
/*
 * this function is responsible for displaying stuff on the screen
 * it does nothing such as calculating fighters positions for instance
 * if this function is called very often, then the game will look
 * smooth, the wave effect will be nicer, but the cursors won't move
 * any faster
 * this function does also other things than display, in fact it does
 * everything which is not directly linked to a game cycle
 */
static void
display (void)
{
  /*
   * the watchdog waits for secret codes to be entered
   * 
   * It's also very important to call this for it will call
   * keypressed() and so will also automatically call poll_keyboard()
   * if needed.
   */
  watchdog_update ();
  /*
   * calls the watchdog module to see if a secret code has been
   * entered, and does the required stuff if the answer is yes
   */
  check_code ();
  /*
   * keeps track of how long the mext operation takes
   */
  start_profile (DISP_PROFILE);
  /*
   * now we call the function that actually draws stuff
   */
  fill_next_screen ();
  stop_profile (DISP_PROFILE);

  /*
   * like check_code but dedicated to the info mode on/off
   * ie it detects if F1 has been pressed and decides wether
   * the info bar must be displayed or not
   */
  check_info_state ();

  /*
   * updates some time values, telling how many weeks this tremendous
   * liquid war session has been running 8-) 
   */
  update_play_time ();

  /*
   * Now this is a little trick, we change the order of display
   * depending on the capture mode.
   */
  if (lw_capture_get_mode ())
    {
      /*
       * We're in capture mode: we display info before the dump
       * for we want it to be there but we display the messages
       * after the dump since they are ugly and useless in a 
       * video capture.
       */
      display_info ();
      lw_capture_dump_game ();
      display_message ();
    }
  else
    {
      /*
       * We're not in capture mode: we display the messages first,
       * for displaying them after the info might give them an
       * ugly blinking look if we're not in double-buffered mode.
       */
      display_message ();
      display_info ();
    }

  /*
   * it's interesting to know how much time is wasted on page flips
   */
  start_profile (FLIP_PROFILE);
  /*
   * switches to the next screen, when page flipping is off,
   * it does not do much...
   */
  page_flip ();
  stop_profile (FLIP_PROFILE);
}

/*------------------------------------------------------------------*/
/*
 * *THE* main game loop
 */
int
game (void)
{
  int retour = 0;
  int last_display_time = get_ticker ();
  int last_logic_time = get_ticker ();
  int lr;

  /*
   * we initialize the time module telling him that now it's 0
   * and he it can start recording stuff
   */
  start_play_time ();
  /*
   * resets the secret code sequences
   */
  watchdog_reset ();
  reset_code ();
  /*
   * resets input buffers and control interfaces
   */
  lw_mouse_reset_control ();
  reset_computer_path ();
  /*
   * resets profile information, so that they are not affected
   * by previous games
   */
  reset_all_profile ();

  /*
   * everything's ready, POOOOOOOOOT the game starts and you hear it
   */
  play_go ();

  if (1 /*!init_distorsion_displayer() */ )
    {
      /*
       * so we loop (almost) for ever
       * execptions are
       * - we have detected a key press on ESCAPE
       * - there are less than 2 teams playing, ie there's a winner
       * - the game time is elapsed
       */
      while ((!WATCHDOG_SCANCODE[KEY_ESC])
	     && (PLAYING_TEAMS >= 2)
	     && (TIME_LEFT > 0) && (!LW_NETWORK_ERROR_DETECTED))
	{
	  /*
	   * global time checking
	   */
	  start_profile (GLOBAL_PROFILE);

	  /*
	   * will contain how mush called to logic have been performed
	   */
	  lr = 0;
	  do
	    {
	      /*
	       * we call logic, calculates stuff, move players etc...
	       */
	      logic ();

	      /*
	       * Now we decide to go idel if it happens that the computer
	       * is calculating this "too fast". This will enable people
	       * too play Liquid War even on a very very fast machine
	       */
	      while (get_ticker () < last_logic_time
		     + LOGIC_DELAY_MIN[CONFIG_ROUNDS_PER_SEC_LIMIT])
		{
#ifdef UNIX
		  /*
		   * The usleep calls prevents the game from eating 100% of the
		   * CPU time on UNIX platforms.
		   */
		  usleep (1000);
#else
		  /*
		   * Rest does not seem to spare CPU time, but at least it
		   * slows the game down
		   */
		  rest (1);
#endif
		  //yield_timeslice (); deprecated
		}
	      last_logic_time = get_ticker ();

	      lr++;
	    }
	  while
	    /*
	     * if CONFIG_FPS_LIMIT is 0 we stop right away, ie
	     * there will be one logic() operation for each display()
	     * if it is 1, we keep going until we exceed the
	     * CONFIG_FPS_LIMIT value. this way we avoid drawing 
	     * 300 frames / second, which is useless
	     * this method speeds the game on powerfull machines
	     * for one can limit the display to 20 frames / seconds
	     * and keep the rest of the CPU for calls to logic()
	     */
	    (CONFIG_FRAMES_PER_SEC_LIMIT
	     && (get_ticker () < last_display_time
		 + DISPLAY_DELAY_MIN[CONFIG_FRAMES_PER_SEC_LIMIT]));
	  /*
	   * tells the profile module about how much logic operations
	   * have been done for this display
	   */
	  update_logic_rate (lr);

	  /*
	   * we get the information "when has the last call to display() 
	   * been done"
	   */
	  last_display_time = get_ticker ();

	  /*
	   * now we really display stuff, (takes time...)
	   */
	  display ();

	  /*
	   * Now we check if the "Close" button has been clicked
	   */
	  my_exit_poll ();

	  stop_profile (GLOBAL_PROFILE);
	}
    }
  /*
   * cleans up the double buffer system
   */
  last_flip ();

  /*
   * empties the key buffer
   */
  clear_keybuf ();

  return retour;
}
