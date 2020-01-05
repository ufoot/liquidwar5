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
/* name          : macro.h                                          */
/* content       : application wide macros                          */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_MACRO
#define LIQUID_WAR_INCLUDE_MACRO

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <string.h>

#ifdef SNPRINTF
#include "../utils/snprintf/snprintf.h"
#endif

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#define LW_MACRO_MEMSET0(BUFFER) { memset((BUFFER),0,sizeof(BUFFER)); }

#define LW_MACRO_STRNCPY(DST,SRC,SIZE) { memset((DST),0,(SIZE)); strncpy((DST),(SRC),(SIZE)); (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_STRCPY(DST,SRC) { LW_MACRO_STRNCPY((DST),(SRC),sizeof(DST)); }

#define LW_MACRO_STRNCAT(DST,SRC,SIZE) { int len,pos; pos=strlen(DST); len=(SIZE)-1-pos; if (len>0) { int end; strncat((DST),(SRC),len); end = pos+strlen(SRC); if (end<(int) ((SIZE)-1)) { (DST)[end]='\0'; } } (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_STRCAT(DST,SRC) { LW_MACRO_STRNCAT((DST),(SRC),sizeof(DST)); }

/*
 * GCC 3.0 supports "..." parameters, together with __VA_ARGS__, however
 * GCC 2.95 does not seem to support it. Since we want LW to be
 * "GCC 2.95 friendly" we do not use this stuff. It's in C99 however...
 * So that's why there are zillion macros depending on how many args are
 * provided to the snprintf function. 8-(
 */
#define LW_MACRO_SNPRINTF0(DST,SIZE,FMT) { LW_MACRO_STRNCPY((DST),(FMT),(SIZE)); }
#define LW_MACRO_SNPRINTF1(DST,SIZE,FMT,A) { memset((DST),0,(SIZE)); snprintf((DST),(SIZE),(FMT),(A)); (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_SNPRINTF2(DST,SIZE,FMT,A,B) { memset((DST),0,(SIZE)); snprintf((DST),(SIZE),(FMT),(A),(B)); (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_SNPRINTF3(DST,SIZE,FMT,A,B,C) { memset((DST),0,(SIZE)); snprintf((DST),(SIZE),(FMT),(A),(B),C); (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_SNPRINTF4(DST,SIZE,FMT,A,B,C,D) { memset((DST),0,(SIZE)); snprintf((DST),(SIZE),(FMT),(A),(B),C,(D)); (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_SNPRINTF5(DST,SIZE,FMT,A,B,C,D,E) { memset((DST),0,(SIZE)); snprintf((DST),(SIZE),(FMT),(A),(B),C,(D),(E)); (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_SNPRINTF6(DST,SIZE,FMT,A,B,C,D,E,F) { memset((DST),0,(SIZE)); snprintf((DST),(SIZE),(FMT),(A),(B),C,(D),(E),(F)); (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_SNPRINTF7(DST,SIZE,FMT,A,B,C,D,E,F,G) { memset((DST),0,(SIZE)); snprintf((DST),(SIZE),(FMT),(A),(B),C,(D),(E),(F),(G)); (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_SNPRINTF8(DST,SIZE,FMT,A,B,C,D,E,F,G,H) { memset((DST),0,(SIZE)); snprintf((DST),(SIZE),(FMT),(A),(B),(C),(D),(E),(F),(G),(H)); (DST)[(SIZE)-1]='\0'; }
#define LW_MACRO_SNPRINTF9(DST,SIZE,FMT,A,B,C,D,E,F,G,H,I) { memset((DST),0,(SIZE)); snprintf((DST),(SIZE),(FMT),(A),(B),(C),(D),(E),(F),(G),(H),(I)); (DST)[(SIZE)-1]='\0'; }

#define LW_MACRO_SPRINTF0(DST,FMT) { LW_MACRO_SNPRINTF0((DST),sizeof(DST),(FMT)); }
#define LW_MACRO_SPRINTF1(DST,FMT,A) { LW_MACRO_SNPRINTF1((DST),sizeof(DST),(FMT),(A)); }
#define LW_MACRO_SPRINTF2(DST,FMT,A,B) { LW_MACRO_SNPRINTF2((DST),sizeof(DST),(FMT),(A),(B)); }
#define LW_MACRO_SPRINTF3(DST,FMT,A,B,C) { LW_MACRO_SNPRINTF3((DST),sizeof(DST),(FMT),(A),(B),(C)); }
#define LW_MACRO_SPRINTF4(DST,FMT,A,B,C,D) { LW_MACRO_SNPRINTF4((DST),sizeof(DST),(FMT),(A),(B),(C),(D)); }
#define LW_MACRO_SPRINTF5(DST,FMT,A,B,C,D,E) { LW_MACRO_SNPRINTF5((DST),sizeof(DST),(FMT),(A),(B),(C),(D),(E)); }
#define LW_MACRO_SPRINTF6(DST,FMT,A,B,C,D,E,F) { LW_MACRO_SNPRINTF6((DST),sizeof(DST),(FMT),(A),(B),(C),(D),(E),(F)); }
#define LW_MACRO_SPRINTF7(DST,FMT,A,B,C,D,E,F,G) { LW_MACRO_SNPRINTF7((DST),sizeof(DST),(FMT),(A),(B),(C),(D),(E),(F),(G)); }
#define LW_MACRO_SPRINTF8(DST,FMT,A,B,C,D,E,F,G,H) { LW_MACRO_SNPRINTF8((DST),sizeof(DST),(FMT),(A),(B),(C),(D),(E),(F),(G),(H)); }
#define LW_MACRO_SPRINTF9(DST,FMT,A,B,C,D,E,F,G,H,I) { LW_MACRO_SNPRINTF9((DST),sizeof(DST),(FMT),(A),(B),(C),(D),(E),(F),(G),(H),(I)); }

/*
 * Macros that does nothing, used to get rid of "unused variable"
 * messages when one still wants to keep a parameter in a function
 * for prototype consistency.
 */
#define LW_MACRO_NOP(X) { (X)=(X); }

#endif
