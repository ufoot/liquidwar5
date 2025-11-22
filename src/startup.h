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
/* nom           : startup.h                                        */
/* contenu       : parametres de demarrage                          */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_STARTUP
#define LIQUID_WAR_INCLUDE_STARTUP

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

#define LW_STARTUP_MEM_MAX  128
#define LW_STARTUP_MEM_STEP   8

#define LW_STARTUP_MAX_PATH_LENGTH           1000

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

extern int STARTUP_BIG_DATA;
extern int STARTUP_DIGI_CARD;
extern int STARTUP_MIDI_CARD;
extern int STARTUP_JOYSTICK_ON;
extern int STARTUP_BACK_STATE;
extern int STARTUP_TEXTURE_STATE;
extern int STARTUP_WATER_STATE;
extern int STARTUP_SFX_STATE;
extern int STARTUP_MUSIC_STATE;
extern int STARTUP_CUSTOM_STATE;
extern int STARTUP_SVGA;
extern int STARTUP_STOP;
extern int STARTUP_AUTO;
extern int STARTUP_CHECK;
extern int STARTUP_CAPTURE;
extern int STARTUP_TOMBOLA;

extern char STARTUP_CFG_PATH[];
extern char STARTUP_DAT_PATH[];
extern char STARTUP_LOG_PATH[];
extern char STARTUP_MAP_PATH[];
extern char STARTUP_TEX_PATH[];
extern char STARTUP_MID_PATH[];
extern char STARTUP_SRV_PATH[];
extern char STARTUP_GEN_PATH[];

extern char STARTUP_METASERVER[];

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

void set_startup (void);

#endif
