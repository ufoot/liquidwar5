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
/* nom           : base.h                                           */
/* contenu       : global application-wide constants, macros, etc...*/
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_BASE
#define LIQUID_WAR_INCLUDE_BASE

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

/*==================================================================*/
/* constantes                                                       */
/*==================================================================*/

#define NB_TEAMS 6
#define NB_DIRS 12

#define DIR_NNE 0
#define DIR_NE 1
#define DIR_ENE 2
#define DIR_ESE 3
#define DIR_SE 4
#define DIR_SSE 5
#define DIR_SSW 6
#define DIR_SW 7
#define DIR_WSW 8
#define DIR_WNW 9
#define DIR_NW 10
#define DIR_NNW 11

#define LW_PROGRAM               "liquidwar"
#define LW_VERSION               "5.6.5"

#define NAME_SIZE 10
#define PASSWORD_SIZE 20
#define COMMENT_SIZE 100
#define URL_SIZE 300

/*
 * On windows there are problems linking with random() so we use
 * rand() instead, which does pretty much the same thing. Still
 * we keep on using random() on UNIX for it's supposed to provid
 * "more random" values than rand(). This is particularly noticeable
 * on FreeBSD.
 */
#ifdef WIN32
#define random rand
#define srandom srand
#endif

#endif
