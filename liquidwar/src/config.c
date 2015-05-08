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
/* Copyright (C) 1998-2015 Christian Mauduit                                 */
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
/* nom           : config.c                                         */
/* contenu       : tout ce qui se rattache a la config              */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro.h>
#include <string.h>

#include "config.h"
#include "disk.h"
#include "joystick.h"
#include "keyboard.h"
#include "mouse.h"
#include "palette.h"
#include "startup.h"
#include "server.h"
#include "lang.h"
#include "macro.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define DUMMY_VALUE 5003000

#define LABEL_MAIN "MAIN"
#define LABEL_DUMMY "DUMMY"
#define LABEL_GFX_GAME "GFX_GAME"
#define LABEL_GFX_MENU "GFX_MENU"
#define LABEL_FULLSCREEN "FULLSCREEN"
#define LABEL_VIEWPORT_SIZE "VIEWPORT_SIZE"
#define LABEL_WAVE_NUMBER "WAVE_NUMBER"
#define LABEL_WAVE_AMPLI "WAVE_AMPLI"
#define LABEL_WAVE_SPEED "WAVE_SPEED"
#define LABEL_BRIGHTNESS "BRIGHTNESS"
#define LABEL_CURSOR_INCREASE_SPEED "CURSOR_INCREASE_SPEED"
#define LABEL_FIGHTER_ATTACK "FIGHTER_ATTACK"
#define LABEL_FIGHTER_DEFENSE "FIGHTER_DEFENSE"
#define LABEL_FIGHTER_NEW_HEALTH "FIGHTER_NEW_HEALTH"
#define LABEL_NUMBER_INFLUENCE "NUMBER_INFLUENCE"
#define LABEL_SFX_VOLUME "SFX_VOLUME"
#define LABEL_CLICK_VOLUME "CLICK_VOLUME"
#define LABEL_WATER_VOLUME_MENU "WATER_VOLUME_MENU"
#define LABEL_WATER_VOLUME_GAME "WATER_VOLUME_GAME"
#define LABEL_MUSIC_VOLUME "MUSIC_VOLUME"
#define LABEL_LEVEL_MAP "LEVEL_MAP"
#define LABEL_LEVEL_FG "LEVEL_FG"
#define LABEL_LEVEL_BG "LEVEL_BG"
#define LABEL_FIGHTER_NUMBER "FIGHTER_NUMBER"
#define LABEL_KEY_UP "KEY_UP"
#define LABEL_KEY_LEFT "KEY_LEFT"
#define LABEL_KEY_DOWN "KEY_DOWN"
#define LABEL_KEY_RIGHT "KEY_RIGHT"
#define LABEL_TEAM_COLOR "TEAM_COLOR"
#define LABEL_CONTROL_TYPE "CONTROL_TYPE"
#define LABEL_PLAYER_NAME "PLAYER_NAME"
#define LABEL_GAME_TIME "GAME_TIME"
#define LABEL_INFO_BAR "INFO_BAR"
#define LABEL_MOUSE_GAP "MOUSE_GAP"
#define LABEL_JOYSTICK_ON "JOYSTICK_ON"
#define LABEL_CPU_ADVANTAGE "CPU_ADVANTAGE"
#define LABEL_CPU_VS_HUMAN "CPU_VS_HUMAN"
#define LABEL_PAGE_FLIP "PAGE_FLIP"
#define LABEL_LANGUAGE "LANGUAGE"
#define LABEL_WAVE_ON "WAVE_ON"
#define LABEL_FRAMES_PER_SEC_LIMIT "FRAMES_PER_SEC_LIMIT"
#define LABEL_ROUNDS_PER_SEC_LIMIT "ROUNDS_PER_SEC_LIMIT"
#define LABEL_SERVER_ADDRESS "SERVER_ADDRESS"
#define LABEL_SERVER_PORT "SERVER_PORT"
#define LABEL_ASM_ALGORITHM "ASM_ALGORITHM"
#define LABEL_PASSWORD "PASSWORD"
#define LABEL_MIN_MAP_RES "MIN_MAP_RES"
#define LABEL_USE_DEFAULT_TEXTURE "USE_DEFAULT_TEXTURE"
#define LABEL_ALLOW_NETWORK_BOTS "ALLOW_NETWORK_BOTS"
#define LABEL_LW6_ADVERTISING_DONE "LW6_ADVERTISING_DONE"

int CONFIG_GFX_GAME;
int CONFIG_GFX_MENU;
int CONFIG_FULLSCREEN;
int CONFIG_VIEWPORT_SIZE;
int CONFIG_WAVE_NUMBER[4];
int CONFIG_WAVE_AMPLI[4];
int CONFIG_WAVE_SPEED[4];
int CONFIG_BRIGHTNESS;
int CONFIG_CURSOR_INCREASE_SPEED;
int CONFIG_FIGHTER_ATTACK;
int CONFIG_FIGHTER_DEFENSE;
int CONFIG_FIGHTER_NEW_HEALTH;
int CONFIG_NUMBER_INFLUENCE;
int CONFIG_SFX_VOLUME;
int CONFIG_CLICK_VOLUME;
int CONFIG_WATER_VOLUME_MENU;
int CONFIG_WATER_VOLUME_GAME;
int CONFIG_MUSIC_VOLUME;
int CONFIG_LEVEL_MAP;
int CONFIG_LEVEL_FG;
int CONFIG_LEVEL_BG;
int CONFIG_FIGHTER_NUMBER;
int CONFIG_KEY_UP[NB_TEAMS];
int CONFIG_KEY_LEFT[NB_TEAMS];
int CONFIG_KEY_DOWN[NB_TEAMS];
int CONFIG_KEY_RIGHT[NB_TEAMS];
int CONFIG_TEAM_COLOR[NB_TEAMS];
int CONFIG_CONTROL_TYPE[NB_TEAMS];
char CONFIG_PLAYER_NAME[NB_TEAMS][NAME_SIZE + 1];
int CONFIG_GAME_TIME;
int CONFIG_INFO_BAR;
int CONFIG_MOUSE_GAP;
int CONFIG_JOYSTICK_ON;
int CONFIG_CPU_ADVANTAGE;
int CONFIG_CPU_VS_HUMAN;
int CONFIG_PAGE_FLIP;
int CONFIG_LANGUAGE;
int CONFIG_WAVE_ON;
int CONFIG_FRAMES_PER_SEC_LIMIT;
int CONFIG_ROUNDS_PER_SEC_LIMIT;
char CONFIG_SERVER_ADDRESS[SERVER_ADDRESS_SIZE + 1];
int CONFIG_SERVER_PORT;
int CONFIG_ASM_ALGORITHM;
char CONFIG_PASSWORD[PASSWORD_SIZE + 1];
int CONFIG_MIN_MAP_RES;
int CONFIG_USE_DEFAULT_TEXTURE;
int CONFIG_ALLOW_NETWORK_BOTS;
int CONFIG_LW6_ADVERTISING_DONE;

LW_NETCONF LW_CONFIG_CURRENT_RULES;

/*==================================================================*/
/* constantes globales                                              */
/*==================================================================*/

int DISPLAY_DELAY_MIN[9] = { 0, 10, 15, 20, 25,
  33, 40, 55, 100
};

int LOGIC_DELAY_MIN[9] = { 0, 1, 2, 5, 10,
  15, 20, 33, 50
};
int MIN_MAP_RES_W[9] = { 0, 80, 120, 160, 200, 240, 320, 480, 640 };
int MIN_MAP_RES_H[9] = { 0, 60, 90, 120, 150, 180, 240, 360, 480 };

/*------------------------------------------------------------------*/
/* fonctions de gestion des parametres de configuration             */
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
void
load_default_options (void)
{
#ifdef DOS
  CONFIG_GFX_GAME = 0;
  CONFIG_GFX_MENU = 2;
#else
  CONFIG_GFX_GAME = 2;
  CONFIG_GFX_MENU = 2;
#endif
#ifdef UNIX
  /*
   * Under UNIX/X11, we default to windowed mode since
   * fullscreen modes under Xfree86 can be unconfigured and/or
   * buggy depending on window manager and other stuff
   */
  CONFIG_FULLSCREEN = 0;
#else
  CONFIG_FULLSCREEN = 1;
#endif
  CONFIG_VIEWPORT_SIZE = 6;

  CONFIG_WAVE_NUMBER[0] = 2;
  CONFIG_WAVE_NUMBER[1] = 2;
  CONFIG_WAVE_NUMBER[2] = 2;
  CONFIG_WAVE_NUMBER[3] = 2;

  CONFIG_WAVE_AMPLI[0] = 8;
  CONFIG_WAVE_AMPLI[1] = 8;
  CONFIG_WAVE_AMPLI[2] = 8;
  CONFIG_WAVE_AMPLI[3] = 8;

  CONFIG_WAVE_SPEED[0] = 6;
  CONFIG_WAVE_SPEED[1] = 7;
  CONFIG_WAVE_SPEED[2] = 8;
  CONFIG_WAVE_SPEED[3] = 9;

  CONFIG_BRIGHTNESS = 8;
  CONFIG_SFX_VOLUME = 12;
  CONFIG_CLICK_VOLUME = 8;
  CONFIG_WATER_VOLUME_MENU = 6;
  CONFIG_WATER_VOLUME_GAME = 10;
  CONFIG_MUSIC_VOLUME = 12;
  CONFIG_LEVEL_MAP = 0;
  CONFIG_LEVEL_FG = 135;
  CONFIG_LEVEL_BG = 131;

  CONFIG_KEY_UP[0] = KEY_UP;
  CONFIG_KEY_DOWN[0] = KEY_DOWN;
  CONFIG_KEY_LEFT[0] = KEY_LEFT;
  CONFIG_KEY_RIGHT[0] = KEY_RIGHT;
  CONFIG_TEAM_COLOR[0] = 0;
  CONFIG_CONTROL_TYPE[0] = CONFIG_CONTROL_TYPE_HUMAN;
  LW_MACRO_STRCPY (CONFIG_PLAYER_NAME[0], "Napoleon");

  CONFIG_KEY_UP[1] = KEY_I;
  CONFIG_KEY_DOWN[1] = KEY_K;
  CONFIG_KEY_LEFT[1] = KEY_J;
  CONFIG_KEY_RIGHT[1] = KEY_L;
  CONFIG_TEAM_COLOR[1] = 4;
  CONFIG_CONTROL_TYPE[1] = CONFIG_CONTROL_TYPE_OFF;
  LW_MACRO_STRCPY (CONFIG_PLAYER_NAME[1], "Clovis");

  CONFIG_KEY_UP[2] = KEY_UP;
  CONFIG_KEY_DOWN[2] = KEY_DOWN;
  CONFIG_KEY_LEFT[2] = KEY_LEFT;
  CONFIG_KEY_RIGHT[2] = KEY_RIGHT;
  CONFIG_TEAM_COLOR[2] = 8;
  CONFIG_CONTROL_TYPE[2] = CONFIG_CONTROL_TYPE_OFF;
  LW_MACRO_STRCPY (CONFIG_PLAYER_NAME[2], "Henri IV");

  CONFIG_KEY_UP[3] = KEY_Q;
  CONFIG_KEY_DOWN[3] = KEY_A;
  CONFIG_KEY_LEFT[3] = KEY_Z;
  CONFIG_KEY_RIGHT[3] = KEY_X;
  CONFIG_TEAM_COLOR[3] = 6;
  CONFIG_CONTROL_TYPE[3] = CONFIG_CONTROL_TYPE_OFF;
  LW_MACRO_STRCPY (CONFIG_PLAYER_NAME[3], "Cesar");

  CONFIG_KEY_UP[4] = KEY_8_PAD;
  CONFIG_KEY_DOWN[4] = KEY_5_PAD;
  CONFIG_KEY_LEFT[4] = KEY_4_PAD;
  CONFIG_KEY_RIGHT[4] = KEY_6_PAD;
  CONFIG_TEAM_COLOR[4] = 10;
  CONFIG_CONTROL_TYPE[4] = CONFIG_CONTROL_TYPE_OFF;
  LW_MACRO_STRCPY (CONFIG_PLAYER_NAME[4], "Geronimo");

  CONFIG_KEY_UP[5] = KEY_MOUSE_UP;
  CONFIG_KEY_DOWN[5] = KEY_MOUSE_DOWN;
  CONFIG_KEY_LEFT[5] = KEY_MOUSE_LEFT;
  CONFIG_KEY_RIGHT[5] = KEY_MOUSE_RIGHT;
  CONFIG_TEAM_COLOR[5] = 3;
  CONFIG_CONTROL_TYPE[5] = CONFIG_CONTROL_TYPE_CPU;
  LW_MACRO_STRCPY (CONFIG_PLAYER_NAME[5], "Attila");

  CONFIG_INFO_BAR = 1;
  CONFIG_MOUSE_GAP = 4;
  CONFIG_JOYSTICK_ON = 1;
  CONFIG_PAGE_FLIP = 0;
  CONFIG_LANGUAGE = LW_LANG_LANGUAGE_AUTO;
  /*
   * Previous versions used to have waves disabled on behalf
   * of performance, with today computers being 100 times faster
   * than primitive stuff that ran LW in the first place, one
   * might admit this is on by default. Looks better.
   */
  CONFIG_WAVE_ON = 1;

  CONFIG_FRAMES_PER_SEC_LIMIT = 6;
  CONFIG_ROUNDS_PER_SEC_LIMIT = 4;

  LW_MACRO_STRCPY (CONFIG_SERVER_ADDRESS, LW_SERVER_DEFAULT_ADDRESS);
  CONFIG_SERVER_PORT = LW_SERVER_DEFAULT_PORT;

  LW_MACRO_STRCPY (CONFIG_PASSWORD, "toto");

  CONFIG_USE_DEFAULT_TEXTURE = 1;

  CONFIG_LW6_ADVERTISING_DONE = 0;

  load_default_rules ();
}

/*------------------------------------------------------------------*/
void
load_default_rules (void)
{
  CONFIG_CURSOR_INCREASE_SPEED = 0;
  CONFIG_FIGHTER_ATTACK = 8;
  CONFIG_FIGHTER_DEFENSE = 8;
  CONFIG_FIGHTER_NEW_HEALTH = 8;
  CONFIG_NUMBER_INFLUENCE = 8;
  CONFIG_FIGHTER_NUMBER = 16;
  CONFIG_GAME_TIME = 8;
  CONFIG_CPU_ADVANTAGE = 0;
  CONFIG_CPU_VS_HUMAN = CONFIG_CPU_VS_HUMAN_RANDOM;
  CONFIG_ASM_ALGORITHM = 1;
  CONFIG_MIN_MAP_RES = 4;
  CONFIG_ALLOW_NETWORK_BOTS = 0;
}

/*------------------------------------------------------------------*/
static int
load_disk_options (void)
{
  char number_string[2] = { '0', 0 };
  int i;

  load_default_options ();

  set_config_file (STARTUP_CFG_PATH);
  if (get_config_int (LABEL_MAIN, LABEL_DUMMY, 0) == DUMMY_VALUE)
    {
      CONFIG_GFX_GAME =
	get_config_int (LABEL_MAIN, LABEL_GFX_GAME, CONFIG_GFX_GAME);
      CONFIG_GFX_MENU =
	get_config_int (LABEL_MAIN, LABEL_GFX_MENU, CONFIG_GFX_MENU);
      CONFIG_FULLSCREEN =
	get_config_int (LABEL_MAIN, LABEL_FULLSCREEN, CONFIG_FULLSCREEN);
      CONFIG_VIEWPORT_SIZE =
	get_config_int (LABEL_MAIN, LABEL_VIEWPORT_SIZE,
			CONFIG_VIEWPORT_SIZE);

      for (i = 0; i < 4; ++i)
	{
	  number_string[0] = '0' + i;

	  CONFIG_WAVE_NUMBER[i] =
	    get_config_int (LABEL_WAVE_NUMBER, number_string,
			    CONFIG_WAVE_NUMBER[i]);
	  CONFIG_WAVE_AMPLI[i] =
	    get_config_int (LABEL_WAVE_AMPLI, number_string,
			    CONFIG_WAVE_AMPLI[i]);
	  CONFIG_WAVE_SPEED[i] =
	    get_config_int (LABEL_WAVE_SPEED, number_string,
			    CONFIG_WAVE_SPEED[i]);
	}

      CONFIG_BRIGHTNESS =
	get_config_int (LABEL_MAIN, LABEL_BRIGHTNESS, CONFIG_BRIGHTNESS);
      CONFIG_CURSOR_INCREASE_SPEED =
	get_config_int (LABEL_MAIN, LABEL_CURSOR_INCREASE_SPEED,
			CONFIG_CURSOR_INCREASE_SPEED);
      CONFIG_FIGHTER_ATTACK =
	get_config_int (LABEL_MAIN, LABEL_FIGHTER_ATTACK,
			CONFIG_FIGHTER_ATTACK);
      CONFIG_FIGHTER_DEFENSE =
	get_config_int (LABEL_MAIN, LABEL_FIGHTER_DEFENSE,
			CONFIG_FIGHTER_DEFENSE);
      CONFIG_FIGHTER_NEW_HEALTH =
	get_config_int (LABEL_MAIN, LABEL_FIGHTER_NEW_HEALTH,
			CONFIG_FIGHTER_NEW_HEALTH);
      CONFIG_NUMBER_INFLUENCE =
	get_config_int (LABEL_MAIN, LABEL_NUMBER_INFLUENCE,
			CONFIG_NUMBER_INFLUENCE);
      CONFIG_SFX_VOLUME =
	get_config_int (LABEL_MAIN, LABEL_SFX_VOLUME, CONFIG_SFX_VOLUME);
      CONFIG_CLICK_VOLUME =
	get_config_int (LABEL_MAIN, LABEL_CLICK_VOLUME, CONFIG_CLICK_VOLUME);
      CONFIG_WATER_VOLUME_MENU =
	get_config_int (LABEL_MAIN, LABEL_WATER_VOLUME_MENU,
			CONFIG_WATER_VOLUME_MENU);
      CONFIG_WATER_VOLUME_GAME =
	get_config_int (LABEL_MAIN, LABEL_WATER_VOLUME_GAME,
			CONFIG_WATER_VOLUME_GAME);
      CONFIG_MUSIC_VOLUME =
	get_config_int (LABEL_MAIN, LABEL_MUSIC_VOLUME, CONFIG_MUSIC_VOLUME);

      CONFIG_LEVEL_MAP =
	get_config_int (LABEL_MAIN, LABEL_LEVEL_MAP, CONFIG_LEVEL_MAP);
      CONFIG_LEVEL_FG =
	get_config_int (LABEL_MAIN, LABEL_LEVEL_FG, CONFIG_LEVEL_FG);
      CONFIG_LEVEL_BG =
	get_config_int (LABEL_MAIN, LABEL_LEVEL_BG, CONFIG_LEVEL_BG);

      CONFIG_FIGHTER_NUMBER =
	get_config_int (LABEL_MAIN, LABEL_FIGHTER_NUMBER,
			CONFIG_FIGHTER_NUMBER);

      for (i = 0; i < 6; ++i)
	{
	  number_string[0] = '0' + i;

	  CONFIG_KEY_UP[i] =
	    get_config_int (LABEL_KEY_UP, number_string, CONFIG_KEY_UP[i]);
	  CONFIG_KEY_LEFT[i] =
	    get_config_int (LABEL_KEY_LEFT, number_string,
			    CONFIG_KEY_LEFT[i]);
	  CONFIG_KEY_DOWN[i] =
	    get_config_int (LABEL_KEY_DOWN, number_string,
			    CONFIG_KEY_DOWN[i]);
	  CONFIG_KEY_RIGHT[i] =
	    get_config_int (LABEL_KEY_RIGHT, number_string,
			    CONFIG_KEY_RIGHT[i]);
	  CONFIG_TEAM_COLOR[i] =
	    get_config_int (LABEL_TEAM_COLOR, number_string,
			    CONFIG_TEAM_COLOR[i]);
	  CONFIG_CONTROL_TYPE[i] =
	    get_config_int (LABEL_CONTROL_TYPE, number_string,
			    CONFIG_CONTROL_TYPE[i]);
	  strncpy (CONFIG_PLAYER_NAME[i],
		   get_config_string (LABEL_PLAYER_NAME, number_string,
				      CONFIG_PLAYER_NAME[i]), NAME_SIZE);
	  CONFIG_PLAYER_NAME[i][NAME_SIZE] = '\0';
	}

      CONFIG_GAME_TIME =
	get_config_int (LABEL_MAIN, LABEL_GAME_TIME, CONFIG_GAME_TIME);
      CONFIG_INFO_BAR =
	get_config_int (LABEL_MAIN, LABEL_INFO_BAR, CONFIG_INFO_BAR);
      CONFIG_MOUSE_GAP =
	get_config_int (LABEL_MAIN, LABEL_MOUSE_GAP, CONFIG_MOUSE_GAP);
      CONFIG_JOYSTICK_ON =
	get_config_int (LABEL_MAIN, LABEL_JOYSTICK_ON, CONFIG_JOYSTICK_ON);
      CONFIG_CPU_ADVANTAGE =
	get_config_int (LABEL_MAIN, LABEL_CPU_ADVANTAGE,
			CONFIG_CPU_ADVANTAGE);
      CONFIG_CPU_VS_HUMAN =
	get_config_int (LABEL_MAIN, LABEL_CPU_VS_HUMAN, CONFIG_CPU_VS_HUMAN);
      CONFIG_PAGE_FLIP =
	get_config_int (LABEL_MAIN, LABEL_PAGE_FLIP, CONFIG_PAGE_FLIP);
      CONFIG_LANGUAGE =
	get_config_int (LABEL_MAIN, LABEL_LANGUAGE, CONFIG_LANGUAGE);
      CONFIG_WAVE_ON =
	get_config_int (LABEL_MAIN, LABEL_WAVE_ON, CONFIG_WAVE_ON);
      CONFIG_FRAMES_PER_SEC_LIMIT =
	get_config_int (LABEL_MAIN, LABEL_FRAMES_PER_SEC_LIMIT,
			CONFIG_FRAMES_PER_SEC_LIMIT);
      CONFIG_ROUNDS_PER_SEC_LIMIT =
	get_config_int (LABEL_MAIN, LABEL_ROUNDS_PER_SEC_LIMIT,
			CONFIG_ROUNDS_PER_SEC_LIMIT);

      strncpy (CONFIG_SERVER_ADDRESS,
	       get_config_string (LABEL_MAIN, LABEL_SERVER_ADDRESS,
				  CONFIG_SERVER_ADDRESS),
	       sizeof (CONFIG_SERVER_ADDRESS));
      CONFIG_SERVER_ADDRESS[sizeof (CONFIG_SERVER_ADDRESS) - 1] = '\0';

      CONFIG_SERVER_PORT =
	get_config_int (LABEL_MAIN, LABEL_SERVER_PORT, CONFIG_SERVER_PORT);

      CONFIG_ASM_ALGORITHM =
	get_config_int (LABEL_MAIN, LABEL_ASM_ALGORITHM,
			CONFIG_ASM_ALGORITHM);

      strncpy (CONFIG_PASSWORD,
	       get_config_string (LABEL_MAIN, LABEL_PASSWORD,
				  CONFIG_PASSWORD), sizeof (CONFIG_PASSWORD));
      CONFIG_PASSWORD[sizeof (CONFIG_PASSWORD) - 1] = '\0';

      CONFIG_MIN_MAP_RES =
	get_config_int (LABEL_MAIN, LABEL_MIN_MAP_RES, CONFIG_MIN_MAP_RES);
      CONFIG_USE_DEFAULT_TEXTURE =
	get_config_int (LABEL_MAIN, LABEL_USE_DEFAULT_TEXTURE,
			CONFIG_USE_DEFAULT_TEXTURE);
      CONFIG_ALLOW_NETWORK_BOTS =
	get_config_int (LABEL_MAIN, LABEL_ALLOW_NETWORK_BOTS,
			CONFIG_ALLOW_NETWORK_BOTS);
      CONFIG_LW6_ADVERTISING_DONE =
	get_config_int (LABEL_MAIN, LABEL_LW6_ADVERTISING_DONE,
			CONFIG_LW6_ADVERTISING_DONE);

      return 1;
    }
  else
    {
      return 0;
    }
}

/*------------------------------------------------------------------*/
int
load_config_options (void)
{
  int result;

  result = load_disk_options ();

  /*
   * Any special hack can go here 8-)
   */
  lw_config_set_current_rules_to_default ();

  return result;
}

/*------------------------------------------------------------------*/
int
save_config_options (void)
{
  char number_string[2] = { '0', 0 };
  int i;

  set_config_file (STARTUP_CFG_PATH);

  set_config_int (LABEL_MAIN, LABEL_DUMMY, DUMMY_VALUE);

  set_config_int (LABEL_MAIN, LABEL_GFX_GAME, CONFIG_GFX_GAME);
  set_config_int (LABEL_MAIN, LABEL_GFX_MENU, CONFIG_GFX_MENU);
  set_config_int (LABEL_MAIN, LABEL_FULLSCREEN, CONFIG_FULLSCREEN);
  set_config_int (LABEL_MAIN, LABEL_VIEWPORT_SIZE, CONFIG_VIEWPORT_SIZE);

  for (i = 0; i < 4; ++i)
    {
      number_string[0] = '0' + i;

      set_config_int (LABEL_WAVE_NUMBER, number_string,
		      CONFIG_WAVE_NUMBER[i]);
      set_config_int (LABEL_WAVE_AMPLI, number_string, CONFIG_WAVE_AMPLI[i]);
      set_config_int (LABEL_WAVE_SPEED, number_string, CONFIG_WAVE_SPEED[i]);
    }

  set_config_int (LABEL_MAIN, LABEL_BRIGHTNESS, CONFIG_BRIGHTNESS);
  set_config_int (LABEL_MAIN, LABEL_CURSOR_INCREASE_SPEED,
		  CONFIG_CURSOR_INCREASE_SPEED);
  set_config_int (LABEL_MAIN, LABEL_FIGHTER_ATTACK, CONFIG_FIGHTER_ATTACK);
  set_config_int (LABEL_MAIN, LABEL_FIGHTER_DEFENSE, CONFIG_FIGHTER_DEFENSE);
  set_config_int (LABEL_MAIN, LABEL_FIGHTER_NEW_HEALTH,
		  CONFIG_FIGHTER_NEW_HEALTH);
  set_config_int (LABEL_MAIN, LABEL_NUMBER_INFLUENCE,
		  CONFIG_NUMBER_INFLUENCE);
  set_config_int (LABEL_MAIN, LABEL_SFX_VOLUME, CONFIG_SFX_VOLUME);
  set_config_int (LABEL_MAIN, LABEL_CLICK_VOLUME, CONFIG_CLICK_VOLUME);
  set_config_int (LABEL_MAIN, LABEL_WATER_VOLUME_MENU,
		  CONFIG_WATER_VOLUME_MENU);
  set_config_int (LABEL_MAIN, LABEL_WATER_VOLUME_GAME,
		  CONFIG_WATER_VOLUME_GAME);
  set_config_int (LABEL_MAIN, LABEL_MUSIC_VOLUME, CONFIG_MUSIC_VOLUME);

  set_config_int (LABEL_MAIN, LABEL_LEVEL_MAP, CONFIG_LEVEL_MAP);
  set_config_int (LABEL_MAIN, LABEL_LEVEL_FG, CONFIG_LEVEL_FG);
  set_config_int (LABEL_MAIN, LABEL_LEVEL_BG, CONFIG_LEVEL_BG);

  set_config_int (LABEL_MAIN, LABEL_FIGHTER_NUMBER, CONFIG_FIGHTER_NUMBER);

  for (i = 0; i < 6; ++i)
    {
      number_string[0] = '0' + i;

      set_config_int (LABEL_KEY_UP, number_string, CONFIG_KEY_UP[i]);
      set_config_int (LABEL_KEY_LEFT, number_string, CONFIG_KEY_LEFT[i]);
      set_config_int (LABEL_KEY_DOWN, number_string, CONFIG_KEY_DOWN[i]);
      set_config_int (LABEL_KEY_RIGHT, number_string, CONFIG_KEY_RIGHT[i]);
      set_config_int (LABEL_TEAM_COLOR, number_string, CONFIG_TEAM_COLOR[i]);
      set_config_int (LABEL_CONTROL_TYPE, number_string,
		      CONFIG_CONTROL_TYPE[i]);
      set_config_string (LABEL_PLAYER_NAME, number_string,
			 CONFIG_PLAYER_NAME[i]);
    }

  set_config_int (LABEL_MAIN, LABEL_GAME_TIME, CONFIG_GAME_TIME);
  set_config_int (LABEL_MAIN, LABEL_INFO_BAR, CONFIG_INFO_BAR);
  set_config_int (LABEL_MAIN, LABEL_MOUSE_GAP, CONFIG_MOUSE_GAP);
  set_config_int (LABEL_MAIN, LABEL_JOYSTICK_ON, CONFIG_JOYSTICK_ON);
  set_config_int (LABEL_MAIN, LABEL_CPU_ADVANTAGE, CONFIG_CPU_ADVANTAGE);
  set_config_int (LABEL_MAIN, LABEL_CPU_VS_HUMAN, CONFIG_CPU_VS_HUMAN);
  set_config_int (LABEL_MAIN, LABEL_PAGE_FLIP, CONFIG_PAGE_FLIP);
  set_config_int (LABEL_MAIN, LABEL_LANGUAGE, CONFIG_LANGUAGE);
  set_config_int (LABEL_MAIN, LABEL_WAVE_ON, CONFIG_WAVE_ON);

  set_config_int (LABEL_MAIN, LABEL_FRAMES_PER_SEC_LIMIT,
		  CONFIG_FRAMES_PER_SEC_LIMIT);
  set_config_int (LABEL_MAIN, LABEL_ROUNDS_PER_SEC_LIMIT,
		  CONFIG_ROUNDS_PER_SEC_LIMIT);

  set_config_string (LABEL_MAIN, LABEL_SERVER_ADDRESS, CONFIG_SERVER_ADDRESS);
  set_config_int (LABEL_MAIN, LABEL_SERVER_PORT, CONFIG_SERVER_PORT);

  set_config_int (LABEL_MAIN, LABEL_ASM_ALGORITHM, CONFIG_ASM_ALGORITHM);

  set_config_string (LABEL_MAIN, LABEL_PASSWORD, CONFIG_PASSWORD);

  set_config_int (LABEL_MAIN, LABEL_MIN_MAP_RES, CONFIG_MIN_MAP_RES);
  set_config_int (LABEL_MAIN, LABEL_USE_DEFAULT_TEXTURE,
		  CONFIG_USE_DEFAULT_TEXTURE);
  set_config_int (LABEL_MAIN, LABEL_ALLOW_NETWORK_BOTS,
		  CONFIG_ALLOW_NETWORK_BOTS);
  set_config_int (LABEL_MAIN, LABEL_LW6_ADVERTISING_DONE,
		  CONFIG_LW6_ADVERTISING_DONE);

  return 0;
}

/*------------------------------------------------------------------*/
void
check_config_range (void)
{
  if (CONFIG_LEVEL_MAP > RAW_MAP_NUMBER - 1)
    CONFIG_LEVEL_MAP = (RAW_MAP_NUMBER - 1) / 2;
  if (CONFIG_LEVEL_FG > RAW_TEXTURE_NUMBER + CHOOSE_COLOR_NUMBER - 1)
    CONFIG_LEVEL_FG = CHOOSE_COLOR_WHITE;
  if (CONFIG_LEVEL_BG > RAW_TEXTURE_NUMBER + CHOOSE_COLOR_NUMBER - 1)
    CONFIG_LEVEL_BG = CHOOSE_COLOR_BLACK;

  lw_netconf_check (&LW_CONFIG_CURRENT_RULES);
}

/*------------------------------------------------------------------*/
void
lw_config_set_current_rules_to_default (void)
{
  LW_NETCONF *config;

  config = &LW_CONFIG_CURRENT_RULES;

  config->cursor_increase_speed = CONFIG_CURSOR_INCREASE_SPEED;
  config->fighter_attack = CONFIG_FIGHTER_ATTACK;
  config->fighter_defense = CONFIG_FIGHTER_DEFENSE;
  config->fighter_new_health = CONFIG_FIGHTER_NEW_HEALTH;
  config->number_influence = CONFIG_NUMBER_INFLUENCE;
  config->fighter_number = CONFIG_FIGHTER_NUMBER;
  config->game_time = CONFIG_GAME_TIME;
  config->cpu_advantage = CONFIG_CPU_ADVANTAGE;
  config->cpu_vs_human = CONFIG_CPU_VS_HUMAN;
  config->asm_algorithm = CONFIG_ASM_ALGORITHM && STARTUP_ASM;
  config->min_map_res = CONFIG_MIN_MAP_RES;
}
