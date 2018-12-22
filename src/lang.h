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
/* name          : lang.h                                           */
/* content       : handles language dependant stuff (locale)        */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_LANG
#define LIQUID_WAR_INCLUDE_LANG

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

/*==================================================================*/
/* constants                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* Declaration of all available languages                           */
/*------------------------------------------------------------------*/

#define LW_LANG_LANGUAGE_AUTO                     -1
#define LW_LANG_LANGUAGE_ENGLISH                   0
#define LW_LANG_LANGUAGE_FRANCAIS                  1
#define LW_LANG_LANGUAGE_DEUTSCH                   2
#define LW_LANG_LANGUAGE_DANSK                    3
#define LW_LANG_LANGUAGE_POLSKI                    4

#define LW_LANG_NB_LANGUAGES                       5

/*------------------------------------------------------------------*/
/* Declaration of all available strings                             */
/*------------------------------------------------------------------*/

#define LW_LANG_STRING_LANGUAGE                    0
#define LW_LANG_STRING_APPNAME                     1

#define LW_LANG_STRING_HELP_CREDITS                2
#define LW_LANG_STRING_HELP_COPYRIGHT              3
#define LW_LANG_STRING_HELP_LICENSE                4
#define LW_LANG_STRING_HELP_VERSION                5

#define LW_LANG_STRING_MENU_PLAY                   6
#define LW_LANG_STRING_MENU_NETGAME                7
#define LW_LANG_STRING_MENU_MAP                    8
#define LW_LANG_STRING_MENU_TEAMS                  9
#define LW_LANG_STRING_MENU_OPTIONS               10
#define LW_LANG_STRING_MENU_ABOUT                 11
#define LW_LANG_STRING_MENU_EXIT                  12
#define LW_LANG_STRING_MENU_EXITWINDOWS           13
#define LW_LANG_STRING_MENU_EXITUNIX              14
#define LW_LANG_STRING_MENU_BACK                  15

#define LW_LANG_STRING_DIALOG_QUIT                16
#define LW_LANG_STRING_DIALOG_BACK                17
#define LW_LANG_STRING_DIALOG_PLAY                18
#define LW_LANG_STRING_DIALOG_MAIN                19

#define LW_LANG_STRING_PLAY_GETREADY              20

#define LW_LANG_STRING_NETGAME_STARTGAME          21
#define LW_LANG_STRING_NETGAME_SERVERADDR         22
#define LW_LANG_STRING_NETGAME_SERVERPORT         23
#define LW_LANG_STRING_NETGAME_SEARCH             24
#define LW_LANG_STRING_NETGAME_PASSWORD           25
#define LW_LANG_STRING_NETGAME_WEAKPASSWORD       26

#define LW_LANG_STRING_LANGUAGE_AUTO              27

#define LW_LANG_STRING_TEAM_OFF                   28
#define LW_LANG_STRING_TEAM_HUMAN                 29
#define LW_LANG_STRING_TEAM_CPU                   30

#define LW_LANG_STRING_OPTIONS_GRAPHICS           31
#define LW_LANG_STRING_OPTIONS_SOUND              32
#define LW_LANG_STRING_OPTIONS_RULES              33
#define LW_LANG_STRING_OPTIONS_SPEEDS             34
#define LW_LANG_STRING_OPTIONS_CONTROLS           35
#define LW_LANG_STRING_OPTIONS_LANGUAGE           36
#define LW_LANG_STRING_OPTIONS_DEFAULT            37

#define LW_LANG_STRING_GRAPHICS_GAME              38
#define LW_LANG_STRING_GRAPHICS_MENU              39
#define LW_LANG_STRING_GRAPHICS_ON                40
#define LW_LANG_STRING_GRAPHICS_OFF               41
#define LW_LANG_STRING_GRAPHICS_FULLSCREEN        42
#define LW_LANG_STRING_GRAPHICS_WINDOWED          43
#define LW_LANG_STRING_GRAPHICS_ADVANCED          44
#define LW_LANG_STRING_GRAPHICS_VIDEOMODE         45
#define LW_LANG_STRING_GRAPHICS_BRIGHTNESS        46
#define LW_LANG_STRING_GRAPHICS_VIEWPORTSIZE      47
#define LW_LANG_STRING_GRAPHICS_PAGEFLIPPING      48
#define LW_LANG_STRING_GRAPHICS_WAVES             49

#define LW_LANG_STRING_VOLUME_SFX                 50
#define LW_LANG_STRING_VOLUME_CLICK               51
#define LW_LANG_STRING_VOLUME_GAMEWATER           52
#define LW_LANG_STRING_VOLUME_MENUWATER           53
#define LW_LANG_STRING_VOLUME_MUSIC               54

#define LW_LANG_STRING_RULES_TIME                 55
#define LW_LANG_STRING_RULES_ADVANCED             56
#define LW_LANG_STRING_RULES_ARMYSIZE             57
#define LW_LANG_STRING_RULES_CURSOR               58
#define LW_LANG_STRING_RULES_DEFAULT              59
#define LW_LANG_STRING_RULES_MINMAPRES            60

#define LW_LANG_STRING_ADVANCED_ATTACK            61
#define LW_LANG_STRING_ADVANCED_DEFENSE           62
#define LW_LANG_STRING_ADVANCED_BASEHEALTH        63
#define LW_LANG_STRING_ADVANCED_WINNERHELP        64
#define LW_LANG_STRING_ADVANCED_CPUSTRENGTH       65
#define LW_LANG_STRING_ADVANCED_CPUVSHUMAN        66
#define LW_LANG_STRING_ADVANCED_ALWAYS            67
#define LW_LANG_STRING_ADVANCED_RANDOM            68
#define LW_LANG_STRING_ADVANCED_NEVER             69
#define LW_LANG_STRING_ADVANCED_ALLOWNETWORKBOTS  70
#define LW_LANG_STRING_ADVANCED_YES               71
#define LW_LANG_STRING_ADVANCED_NO                72

#define LW_LANG_STRING_SPEEDS_FPS                 73
#define LW_LANG_STRING_SPEEDS_NOFPSLIMIT          74
#define LW_LANG_STRING_SPEEDS_RPS                 75
#define LW_LANG_STRING_SPEEDS_NORPSLIMIT          76

#define LW_LANG_STRING_CONTROLS_JOYSTICK          77
#define LW_LANG_STRING_CONTROLS_OFF               78
#define LW_LANG_STRING_CONTROLS_2JOYSTICKS        79
#define LW_LANG_STRING_CONTROLS_4BUTTONS          80
#define LW_LANG_STRING_CONTROLS_STANDARD          81
#define LW_LANG_STRING_CONTROLS_MOUSESENSIB       82

#define LW_LANG_STRING_ABOUT_CREDITS              83
#define LW_LANG_STRING_ABOUT_COPYRIGHT            84
#define LW_LANG_STRING_ABOUT_LICENSE              85
#define LW_LANG_STRING_ABOUT_VERSION              86

#define LW_LANG_STRING_INTERNET_REFRESH           87
#define LW_LANG_STRING_INTERNET_JOINNOW           88
#define LW_LANG_STRING_INTERNET_PORT              89
#define LW_LANG_STRING_INTERNET_PASSWORD          90
#define LW_LANG_STRING_INTERNET_YES               91
#define LW_LANG_STRING_INTERNET_NO                92
#define LW_LANG_STRING_INTERNET_UPTIME            93
#define LW_LANG_STRING_INTERNET_DAYS              94
#define LW_LANG_STRING_INTERNET_DAY               95
#define LW_LANG_STRING_INTERNET_HOURS             96
#define LW_LANG_STRING_INTERNET_MINUTES           97
#define LW_LANG_STRING_INTERNET_SECONDS           98
#define LW_LANG_STRING_INTERNET_UNREACHABLE      99
#define LW_LANG_STRING_INTERNET_PING             100
#define LW_LANG_STRING_INTERNET_MSEC             101

#define LW_LANG_STRING_WAVE_AMPLI                102
#define LW_LANG_STRING_WAVE_NUMBER               103
#define LW_LANG_STRING_WAVE_SPEED                104
#define LW_LANG_STRING_WAVE_WX                   105
#define LW_LANG_STRING_WAVE_HY                   106
#define LW_LANG_STRING_WAVE_WY                   107
#define LW_LANG_STRING_WAVE_HX                   108

#define LW_LANG_STRING_CONNECT_STARTNOW          109
#define LW_LANG_STRING_CONNECT_WAITING           110
#define LW_LANG_STRING_CONNECT_WAITINGFOR        111
#define LW_LANG_STRING_CONNECT_TEAMS             112
#define LW_LANG_STRING_CONNECT_SENDMESSAGE       113

#define LW_LANG_STRING_NETPLAY_CONNECT           114
#define LW_LANG_STRING_NETPLAY_PING              115
#define LW_LANG_STRING_NETPLAY_CHECKPROGRAM      116
#define LW_LANG_STRING_NETPLAY_CHECKVERSION      117
#define LW_LANG_STRING_NETPLAY_SENDPASSWORD      118
#define LW_LANG_STRING_NETPLAY_FREETEAMS         119
#define LW_LANG_STRING_NETPLAY_RESERVETEAMS      120
#define LW_LANG_STRING_NETPLAY_SENDCONFIG        121
#define LW_LANG_STRING_NETPLAY_SENDMAP           122
#define LW_LANG_STRING_NETPLAY_WAITING           123
#define LW_LANG_STRING_NETPLAY_WHOPLAYS          124
#define LW_LANG_STRING_NETPLAY_RECVCONFIG        125
#define LW_LANG_STRING_NETPLAY_RECVMAP           126

#define LW_LANG_STRING_LEVEL_USEDEFAULTTEXTURE   127
#define LW_LANG_STRING_LEVEL_ON                  128
#define LW_LANG_STRING_LEVEL_OFF                 129
#define LW_LANG_STRING_LEVEL_RANDOMMAP           130

#define LW_LANG_STRING_CODE_FPS                  131
#define LW_LANG_STRING_CODE_RPS                  132
#define LW_LANG_STRING_CODE_PERCENTCALC          133
#define LW_LANG_STRING_CODE_PERCENTDISPLAY       134
#define LW_LANG_STRING_CODE_SPREADGRADTIME       135
#define LW_LANG_STRING_CODE_MOVEARMYTIME         136
#define LW_LANG_STRING_CODE_DISPLAYTIME          137
#define LW_LANG_STRING_CODE_FLIPTIME             138
#define LW_LANG_STRING_CODE_OTHERCALCTIME        139
#define LW_LANG_STRING_CODE_GLOBALTIME           140
#define LW_LANG_STRING_CODE_STANDARDDISPLAY      141
#define LW_LANG_STRING_CODE_GRADIENT1            142
#define LW_LANG_STRING_CODE_GRADIENT2            143
#define LW_LANG_STRING_CODE_GRADIENT3            144
#define LW_LANG_STRING_CODE_GRADIENT4            145
#define LW_LANG_STRING_CODE_GRADIENT5            146
#define LW_LANG_STRING_CODE_GRADIENT6            147
#define LW_LANG_STRING_CODE_MESHSIZE             148
#define LW_LANG_STRING_CODE_MESHSTRUCTURE        149
#define LW_LANG_STRING_CODE_PAUSEOFF             150
#define LW_LANG_STRING_CODE_PAUSEON              151
#define LW_LANG_STRING_CODE_WAVEEFFECTSOFF       152
#define LW_LANG_STRING_CODE_WAVEEFFECTSON        153
#define LW_LANG_STRING_CODE_CAPTUREOFF           154
#define LW_LANG_STRING_CODE_CAPTUREON            155

#define LW_LANG_STRING_SCORE_WINNER              156

#define LW_LANG_STRING_GAME_BACKMEMFAIL          157
#define LW_LANG_STRING_GAME_DISPMEMFAIL          158
#define LW_LANG_STRING_GAME_MESHMEMFAIL          159
#define LW_LANG_STRING_GAME_AREAMEMFAIL          160
#define LW_LANG_STRING_GAME_ARMYMEMFAIL          161

#define LW_LANG_STRING_ERROR_NOTENOUGHTEAMS      162
#define LW_LANG_STRING_ERROR_NOTENOUGHBIGDATA    163
#define LW_LANG_STRING_ERROR_GRAPHICSINITFAILED  164
#define LW_LANG_STRING_ERROR_NONETWORKSUPPORT    165
#define LW_LANG_STRING_ERROR_UNABLETOCONNECT     166
#define LW_LANG_STRING_ERROR_NOTALIQUIDWARSERVER 167
#define LW_LANG_STRING_ERROR_WRONGVERSION        168
#define LW_LANG_STRING_ERROR_NETWORKPROBLEM      169
#define LW_LANG_STRING_ERROR_CANTSENDMAP         170
#define LW_LANG_STRING_ERROR_CANTRECEIVEMAP      171
#define LW_LANG_STRING_ERROR_SERVERFULL          172
#define LW_LANG_STRING_ERROR_BADPASSWORD         173
#define LW_LANG_STRING_ERROR_BADCONFIGRANGE      174

#define LW_LANG_STRING_POPUP_INITFAIL            175
#define LW_LANG_STRING_POPUP_MEMTROUBLE          176
#define LW_LANG_STRING_POPUP_BIGDATA             177
#define LW_LANG_STRING_POPUP_LOADFAIL            178
#define LW_LANG_STRING_POPUP_CUSTOMFAIL          179
#define LW_LANG_STRING_POPUP_GRAPHICSFAIL        180

#define LW_LANG_STRING_LW6_INFO                  181
#define LW_LANG_STRING_LW6_OK                    182

#define LW_LANG_NB_STRINGS                       183

/*==================================================================*/
/* globals                                                          */
/*==================================================================*/

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

extern void lw_lang_init ();
extern char *lw_lang_string (int id);
extern char *lw_lang_language (int lang);
extern int lw_lang_auto ();

#endif
