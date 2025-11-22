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
/* nom           : config.h                                         */
/* contenu       : chargement des donnees de configuration          */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_CONFIG
#define LIQUID_WAR_INCLUDE_CONFIG

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include "base.h"
#include "netconf.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* variables du fichier de configuration                            */
/*------------------------------------------------------------------*/

#define SERVER_ADDRESS_SIZE 50

extern int CONFIG_GFX_GAME;
extern int CONFIG_GFX_MENU;
extern int CONFIG_FULLSCREEN;
extern int CONFIG_VIEWPORT_SIZE;
extern int CONFIG_WAVE_NUMBER[4];
extern int CONFIG_WAVE_AMPLI[4];
extern int CONFIG_WAVE_SPEED[4];
extern int CONFIG_BRIGHTNESS;
extern int CONFIG_CURSOR_INCREASE_SPEED;
extern int CONFIG_FIGHTER_ATTACK;
extern int CONFIG_FIGHTER_DEFENSE;
extern int CONFIG_FIGHTER_NEW_HEALTH;
extern int CONFIG_NUMBER_INFLUENCE;
extern int CONFIG_SFX_VOLUME;
extern int CONFIG_CLICK_VOLUME;
extern int CONFIG_WATER_VOLUME_MENU;
extern int CONFIG_WATER_VOLUME_GAME;
extern int CONFIG_MUSIC_VOLUME;
extern int CONFIG_LEVEL_MAP;
extern int CONFIG_LEVEL_FG;
extern int CONFIG_LEVEL_BG;
extern int CONFIG_FIGHTER_NUMBER;
extern int CONFIG_KEY_UP[NB_TEAMS];
extern int CONFIG_KEY_LEFT[NB_TEAMS];
extern int CONFIG_KEY_DOWN[NB_TEAMS];
extern int CONFIG_KEY_RIGHT[NB_TEAMS];
extern int CONFIG_TEAM_COLOR[NB_TEAMS];
extern int CONFIG_CONTROL_TYPE[NB_TEAMS];
extern char CONFIG_PLAYER_NAME[NB_TEAMS][NAME_SIZE + 1];
extern int CONFIG_GAME_TIME;
extern int CONFIG_INFO_BAR;
extern int CONFIG_MOUSE_GAP;
extern int CONFIG_JOYSTICK_ON;
extern int CONFIG_CPU_ADVANTAGE;
extern int CONFIG_CPU_VS_HUMAN;;
extern int CONFIG_PAGE_FLIP;
extern int CONFIG_LANGUAGE;
extern int CONFIG_WAVE_ON;
extern int CONFIG_FRAMES_PER_SEC_LIMIT;
extern int CONFIG_ROUNDS_PER_SEC_LIMIT;
extern char CONFIG_SERVER_ADDRESS[SERVER_ADDRESS_SIZE + 1];
extern int CONFIG_SERVER_PORT;
extern char CONFIG_PASSWORD[PASSWORD_SIZE + 1];
extern int CONFIG_MIN_MAP_RES;
extern int CONFIG_USE_DEFAULT_TEXTURE;
extern int CONFIG_ALLOW_NETWORK_BOTS;
extern int CONFIG_LW6_ADVERTISING_SKIP;
extern int CONFIG_LW6_ADVERTISING_DONE;

#define CONFIG_CONTROL_TYPE_OFF    0
#define CONFIG_CONTROL_TYPE_HUMAN  1
#define CONFIG_CONTROL_TYPE_CPU    2

#define CONFIG_CPU_VS_HUMAN_ALWAYS 0
#define CONFIG_CPU_VS_HUMAN_RANDOM 1
#define CONFIG_CPU_VS_HUMAN_NEVER  2

extern LW_NETCONF LW_CONFIG_CURRENT_RULES;

/*==================================================================*/
/* constantes globales                                              */
/*==================================================================*/

extern int DISPLAY_DELAY_MIN[9];
extern int LOGIC_DELAY_MIN[9];
extern int MIN_MAP_RES_W[9];
extern int MIN_MAP_RES_H[9];

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void load_default_options (void);
void load_default_rules (void);
int load_config_options (void);
int save_config_options (void);
void check_config_range (void);
void lw_config_set_current_rules_to_default (void);

#endif
