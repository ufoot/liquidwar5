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
/* Copyright (C) 1998-2020 Christian Mauduit                                 */
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
#define LW_LANG_STRING_MENU_EXITDOS               13
#define LW_LANG_STRING_MENU_EXITWINDOWS           14
#define LW_LANG_STRING_MENU_EXITUNIX              15
#define LW_LANG_STRING_MENU_BACK                  16

#define LW_LANG_STRING_DIALOG_QUIT                17
#define LW_LANG_STRING_DIALOG_BACK                18
#define LW_LANG_STRING_DIALOG_PLAY                19
#define LW_LANG_STRING_DIALOG_MAIN                20

#define LW_LANG_STRING_PLAY_GETREADY              21

#define LW_LANG_STRING_NETGAME_STARTGAME          22
#define LW_LANG_STRING_NETGAME_SERVERADDR         23
#define LW_LANG_STRING_NETGAME_SERVERPORT         24
#define LW_LANG_STRING_NETGAME_SEARCH             25
#define LW_LANG_STRING_NETGAME_PASSWORD           26
#define LW_LANG_STRING_NETGAME_WEAKPASSWORD       27

#define LW_LANG_STRING_LANGUAGE_AUTO              28

#define LW_LANG_STRING_TEAM_OFF                   29
#define LW_LANG_STRING_TEAM_HUMAN                 30
#define LW_LANG_STRING_TEAM_CPU                   31

#define LW_LANG_STRING_OPTIONS_GRAPHICS           32
#define LW_LANG_STRING_OPTIONS_SOUND              33
#define LW_LANG_STRING_OPTIONS_RULES              34
#define LW_LANG_STRING_OPTIONS_SPEEDS             35
#define LW_LANG_STRING_OPTIONS_CONTROLS           36
#define LW_LANG_STRING_OPTIONS_LANGUAGE           37
#define LW_LANG_STRING_OPTIONS_DEFAULT            38

#define LW_LANG_STRING_GRAPHICS_GAME              39
#define LW_LANG_STRING_GRAPHICS_MENU              40
#define LW_LANG_STRING_GRAPHICS_ON                41
#define LW_LANG_STRING_GRAPHICS_OFF               42
#define LW_LANG_STRING_GRAPHICS_FULLSCREEN        43
#define LW_LANG_STRING_GRAPHICS_WINDOWED          44
#define LW_LANG_STRING_GRAPHICS_ADVANCED          45
#define LW_LANG_STRING_GRAPHICS_VIDEOMODE         46
#define LW_LANG_STRING_GRAPHICS_BRIGHTNESS        47
#define LW_LANG_STRING_GRAPHICS_VIEWPORTSIZE      48
#define LW_LANG_STRING_GRAPHICS_PAGEFLIPPING      49
#define LW_LANG_STRING_GRAPHICS_WAVES             50

#define LW_LANG_STRING_VOLUME_SFX                 51
#define LW_LANG_STRING_VOLUME_CLICK               52
#define LW_LANG_STRING_VOLUME_GAMEWATER           53
#define LW_LANG_STRING_VOLUME_MENUWATER           54
#define LW_LANG_STRING_VOLUME_MUSIC               55

#define LW_LANG_STRING_RULES_TIME                 56
#define LW_LANG_STRING_RULES_ADVANCED             57
#define LW_LANG_STRING_RULES_ARMYSIZE             58
#define LW_LANG_STRING_RULES_CURSOR               59
#define LW_LANG_STRING_RULES_DEFAULT              60
#define LW_LANG_STRING_RULES_MINMAPRES            61

#define LW_LANG_STRING_ADVANCED_ATTACK            62
#define LW_LANG_STRING_ADVANCED_DEFENSE           63
#define LW_LANG_STRING_ADVANCED_BASEHEALTH        64
#define LW_LANG_STRING_ADVANCED_WINNERHELP        65
#define LW_LANG_STRING_ADVANCED_CPUSTRENGTH       66
#define LW_LANG_STRING_ADVANCED_CPUVSHUMAN        67
#define LW_LANG_STRING_ADVANCED_ALWAYS            68
#define LW_LANG_STRING_ADVANCED_RANDOM            69
#define LW_LANG_STRING_ADVANCED_NEVER             70
#define LW_LANG_STRING_ADVANCED_ALLOWNETWORKBOTS  71
#define LW_LANG_STRING_ADVANCED_YES               72
#define LW_LANG_STRING_ADVANCED_NO                73

#define LW_LANG_STRING_SPEEDS_FPS                 74
#define LW_LANG_STRING_SPEEDS_NOFPSLIMIT          75
#define LW_LANG_STRING_SPEEDS_RPS                 76
#define LW_LANG_STRING_SPEEDS_NORPSLIMIT          77

#define LW_LANG_STRING_CONTROLS_JOYSTICK          78
#define LW_LANG_STRING_CONTROLS_OFF               79
#define LW_LANG_STRING_CONTROLS_2JOYSTICKS        80
#define LW_LANG_STRING_CONTROLS_4BUTTONS          81
#define LW_LANG_STRING_CONTROLS_STANDARD          82
#define LW_LANG_STRING_CONTROLS_MOUSESENSIB       83

#define LW_LANG_STRING_ABOUT_CREDITS              84
#define LW_LANG_STRING_ABOUT_COPYRIGHT            85
#define LW_LANG_STRING_ABOUT_LICENSE              86
#define LW_LANG_STRING_ABOUT_VERSION              87

#define LW_LANG_STRING_INTERNET_REFRESH           88
#define LW_LANG_STRING_INTERNET_JOINNOW           89
#define LW_LANG_STRING_INTERNET_PORT              90
#define LW_LANG_STRING_INTERNET_PASSWORD          91
#define LW_LANG_STRING_INTERNET_YES               92
#define LW_LANG_STRING_INTERNET_NO                93
#define LW_LANG_STRING_INTERNET_UPTIME            94
#define LW_LANG_STRING_INTERNET_DAYS              95
#define LW_LANG_STRING_INTERNET_DAY               96
#define LW_LANG_STRING_INTERNET_HOURS             97
#define LW_LANG_STRING_INTERNET_MINUTES           98
#define LW_LANG_STRING_INTERNET_SECONDS           99
#define LW_LANG_STRING_INTERNET_UNREACHABLE      100
#define LW_LANG_STRING_INTERNET_PING             101
#define LW_LANG_STRING_INTERNET_MSEC             102

#define LW_LANG_STRING_WAVE_AMPLI                103
#define LW_LANG_STRING_WAVE_NUMBER               104
#define LW_LANG_STRING_WAVE_SPEED                105
#define LW_LANG_STRING_WAVE_WX                   106
#define LW_LANG_STRING_WAVE_HY                   107
#define LW_LANG_STRING_WAVE_WY                   108
#define LW_LANG_STRING_WAVE_HX                   109

#define LW_LANG_STRING_CONNECT_STARTNOW          110
#define LW_LANG_STRING_CONNECT_WAITING           111
#define LW_LANG_STRING_CONNECT_WAITINGFOR        112
#define LW_LANG_STRING_CONNECT_TEAMS             113
#define LW_LANG_STRING_CONNECT_SENDMESSAGE       114

#define LW_LANG_STRING_NETPLAY_CONNECT           115
#define LW_LANG_STRING_NETPLAY_PING              116
#define LW_LANG_STRING_NETPLAY_CHECKPROGRAM      117
#define LW_LANG_STRING_NETPLAY_CHECKVERSION      118
#define LW_LANG_STRING_NETPLAY_SENDPASSWORD      119
#define LW_LANG_STRING_NETPLAY_FREETEAMS         120
#define LW_LANG_STRING_NETPLAY_RESERVETEAMS      121
#define LW_LANG_STRING_NETPLAY_SENDCONFIG        122
#define LW_LANG_STRING_NETPLAY_SENDMAP           123
#define LW_LANG_STRING_NETPLAY_WAITING           124
#define LW_LANG_STRING_NETPLAY_WHOPLAYS          125
#define LW_LANG_STRING_NETPLAY_RECVCONFIG        126
#define LW_LANG_STRING_NETPLAY_RECVMAP           127

#define LW_LANG_STRING_LEVEL_USEDEFAULTTEXTURE   128
#define LW_LANG_STRING_LEVEL_ON                  129
#define LW_LANG_STRING_LEVEL_OFF                 130
#define LW_LANG_STRING_LEVEL_RANDOMMAP           131

#define LW_LANG_STRING_CODE_FPS                  132
#define LW_LANG_STRING_CODE_RPS                  133
#define LW_LANG_STRING_CODE_PERCENTCALC          134
#define LW_LANG_STRING_CODE_PERCENTDISPLAY       135
#define LW_LANG_STRING_CODE_SPREADGRADTIME       136
#define LW_LANG_STRING_CODE_MOVEARMYTIME         137
#define LW_LANG_STRING_CODE_DISPLAYTIME          138
#define LW_LANG_STRING_CODE_FLIPTIME             139
#define LW_LANG_STRING_CODE_OTHERCALCTIME        140
#define LW_LANG_STRING_CODE_GLOBALTIME           141
#define LW_LANG_STRING_CODE_STANDARDDISPLAY      142
#define LW_LANG_STRING_CODE_GRADIENT1            143
#define LW_LANG_STRING_CODE_GRADIENT2            144
#define LW_LANG_STRING_CODE_GRADIENT3            145
#define LW_LANG_STRING_CODE_GRADIENT4            146
#define LW_LANG_STRING_CODE_GRADIENT5            147
#define LW_LANG_STRING_CODE_GRADIENT6            148
#define LW_LANG_STRING_CODE_MESHSIZE             149
#define LW_LANG_STRING_CODE_MESHSTRUCTURE        150
#define LW_LANG_STRING_CODE_PAUSEOFF             151
#define LW_LANG_STRING_CODE_PAUSEON              152
#define LW_LANG_STRING_CODE_WAVEEFFECTSOFF       153
#define LW_LANG_STRING_CODE_WAVEEFFECTSON        154
#define LW_LANG_STRING_CODE_CAPTUREOFF           155
#define LW_LANG_STRING_CODE_CAPTUREON            156

#define LW_LANG_STRING_SCORE_WINNER              157

#define LW_LANG_STRING_GAME_BACKMEMFAIL          158
#define LW_LANG_STRING_GAME_DISPMEMFAIL          159
#define LW_LANG_STRING_GAME_MESHMEMFAIL          160
#define LW_LANG_STRING_GAME_AREAMEMFAIL          161
#define LW_LANG_STRING_GAME_ARMYMEMFAIL          162

#define LW_LANG_STRING_ERROR_NOTENOUGHTEAMS      163
#define LW_LANG_STRING_ERROR_NOTENOUGHBIGDATA    164
#define LW_LANG_STRING_ERROR_GRAPHICSINITFAILED  165
#define LW_LANG_STRING_ERROR_NONETWORKSUPPORT    166
#define LW_LANG_STRING_ERROR_UNABLETOCONNECT     167
#define LW_LANG_STRING_ERROR_NOTALIQUIDWARSERVER 168
#define LW_LANG_STRING_ERROR_WRONGVERSION        169
#define LW_LANG_STRING_ERROR_NETWORKPROBLEM      170
#define LW_LANG_STRING_ERROR_CANTSENDMAP         171
#define LW_LANG_STRING_ERROR_CANTRECEIVEMAP      172
#define LW_LANG_STRING_ERROR_SERVERFULL          173
#define LW_LANG_STRING_ERROR_BADPASSWORD         174
#define LW_LANG_STRING_ERROR_BADCONFIGRANGE      175

#define LW_LANG_STRING_POPUP_INITFAIL            176
#define LW_LANG_STRING_POPUP_MEMTROUBLE          177
#define LW_LANG_STRING_POPUP_BIGDATA             178
#define LW_LANG_STRING_POPUP_LOADFAIL            179
#define LW_LANG_STRING_POPUP_CUSTOMFAIL          180
#define LW_LANG_STRING_POPUP_GRAPHICSFAIL        181

#define LW_LANG_STRING_LW6_INFO                  182
#define LW_LANG_STRING_LW6_OK                    183

#define LW_LANG_NB_STRINGS                       184

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
