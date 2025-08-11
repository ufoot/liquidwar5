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
/* name          : test_simple.h                                   */
/* content       : simple unit testing framework                   */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_TEST_SIMPLE
#define LIQUID_WAR_INCLUDE_TEST_SIMPLE

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <stdlib.h>

/*==================================================================*/
/* macros                                                           */
/*==================================================================*/

static int _test_count = 0;
static int _test_passed = 0;
static int _test_failed = 0;

#define TEST_START() \
    do { \
        printf("Starting tests...\n"); \
        _test_count = 0; \
        _test_passed = 0; \
        _test_failed = 0; \
    } while(0)

#define TEST_ASSERT(condition, message) \
    do { \
        _test_count++; \
        if (condition) { \
            printf("PASS: %s\n", message); \
            _test_passed++; \
        } else { \
            printf("FAIL: %s\n", message); \
            _test_failed++; \
        } \
    } while(0)

#define TEST_ASSERT_EQ(expected, actual, message) \
    do { \
        _test_count++; \
        if ((expected) == (actual)) { \
            printf("PASS: %s (expected=%d, actual=%d)\n", message, (int)(expected), (int)(actual)); \
            _test_passed++; \
        } else { \
            printf("FAIL: %s (expected=%d, actual=%d)\n", message, (int)(expected), (int)(actual)); \
            _test_failed++; \
        } \
    } while(0)

#define TEST_ASSERT_NE(not_expected, actual, message) \
    do { \
        _test_count++; \
        if ((not_expected) != (actual)) { \
            printf("PASS: %s (not_expected=%d, actual=%d)\n", message, (int)(not_expected), (int)(actual)); \
            _test_passed++; \
        } else { \
            printf("FAIL: %s (not_expected=%d, actual=%d)\n", message, (int)(not_expected), (int)(actual)); \
            _test_failed++; \
        } \
    } while(0)

#define TEST_END() \
    do { \
        printf("\n=== Test Summary ===\n"); \
        printf("Total tests: %d\n", _test_count); \
        printf("Passed: %d\n", _test_passed); \
        printf("Failed: %d\n", _test_failed); \
        if (_test_failed == 0) { \
            printf("ALL TESTS PASSED!\n"); \
        } else { \
            printf("SOME TESTS FAILED!\n"); \
        } \
        return (_test_failed == 0) ? 0 : 1; \
    } while(0)

#endif