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
/* name          : test_path.c                                     */
/* content       : unit tests for path manipulation functions      */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_simple.h"
#include "path.h"

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
test_lw_path_get_system_name_basic()
{
    char *result;

    result = lw_path_get_system_name("myfile.txt");
    TEST_ASSERT(strcmp(result, "myfile") == 0, "Basic filename without path");

    result = lw_path_get_system_name("file");
    TEST_ASSERT(strcmp(result, "file") == 0, "Filename without extension");

    result = lw_path_get_system_name("test.map.txt");
    TEST_ASSERT(strcmp(result, "test") == 0, "Filename with multiple dots");
}

/*------------------------------------------------------------------*/
static void
test_lw_path_get_system_name_with_paths()
{
    char *result;

    result = lw_path_get_system_name("/home/user/test.txt");
    TEST_ASSERT(strcmp(result, "test") == 0, "Unix path with extension");

    result = lw_path_get_system_name("C:\\Users\\test\\file.map");
    TEST_ASSERT(strcmp(result, "file") == 0, "Windows path with extension");

    result = lw_path_get_system_name("/very/long/path/to/myfile.pcx");
    TEST_ASSERT(strcmp(result, "myfile") == 0, "Long Unix path");

    result = lw_path_get_system_name("relative/path/file.bmp");
    TEST_ASSERT(strcmp(result, "file") == 0, "Relative path");
}

/*------------------------------------------------------------------*/
static void
test_lw_path_get_system_name_edge_cases()
{
    char *result;

    result = lw_path_get_system_name("/path/to/");
    TEST_ASSERT(strcmp(result, "") == 0, "Path ending with separator");

    result = lw_path_get_system_name(".");
    TEST_ASSERT(strcmp(result, "") == 0, "Single dot");

    result = lw_path_get_system_name(".hidden");
    TEST_ASSERT(strcmp(result, "") == 0, "Hidden file starting with dot");

    result = lw_path_get_system_name("/path/.hidden.txt");
    TEST_ASSERT(strcmp(result, "") == 0, "Hidden file with extension");
}

/*------------------------------------------------------------------*/
static void
test_lw_path_join2_basic()
{
    char *result;

    result = lw_path_join2("home", "user");
    TEST_ASSERT(result != NULL, "join2 returns non-null");
#ifdef UNIX
    TEST_ASSERT(strcmp(result, "home/user") == 0, "Basic join2 Unix");
#else
    TEST_ASSERT(strcmp(result, "home\\user") == 0, "Basic join2 Windows");
#endif
    free(result);

    result = lw_path_join2("", "file.txt");
    TEST_ASSERT(result != NULL, "join2 with empty first part");
#ifdef UNIX
    TEST_ASSERT(strcmp(result, "/file.txt") == 0, "join2 empty first part Unix");
#else
    TEST_ASSERT(strcmp(result, "\\file.txt") == 0, "join2 empty first part Windows");
#endif
    free(result);

    result = lw_path_join2("dir", "");
    TEST_ASSERT(result != NULL, "join2 with empty second part");
#ifdef UNIX
    TEST_ASSERT(strcmp(result, "dir/") == 0, "join2 empty second part Unix");
#else
    TEST_ASSERT(strcmp(result, "dir\\") == 0, "join2 empty second part Windows");
#endif
    free(result);
}

/*------------------------------------------------------------------*/
static void
test_lw_path_join2_various_inputs()
{
    char *result;

    result = lw_path_join2("/usr/local", "bin");
#ifdef UNIX
    TEST_ASSERT(strcmp(result, "/usr/local/bin") == 0, "Absolute path join2");
#else
    TEST_ASSERT(strcmp(result, "/usr/local\\bin") == 0, "Absolute path join2 Windows");
#endif
    free(result);

    result = lw_path_join2("data", "maps");
#ifdef UNIX
    TEST_ASSERT(strcmp(result, "data/maps") == 0, "Relative path join2");
#else
    TEST_ASSERT(strcmp(result, "data\\maps") == 0, "Relative path join2 Windows");
#endif
    free(result);

    result = lw_path_join2("very_long_directory_name", "very_long_filename.extension");
    TEST_ASSERT(result != NULL, "join2 with long names");
    TEST_ASSERT(strlen(result) > 30, "join2 result has expected length");
    free(result);
}

/*------------------------------------------------------------------*/
static void
test_lw_path_join3_basic()
{
    char *result;

    result = lw_path_join3("home", "user", "documents");
    TEST_ASSERT(result != NULL, "join3 returns non-null");
#ifdef UNIX
    TEST_ASSERT(strcmp(result, "home/user/documents") == 0, "Basic join3 Unix");
#else
    TEST_ASSERT(strcmp(result, "home\\user\\documents") == 0, "Basic join3 Windows");
#endif
    free(result);

    result = lw_path_join3("", "middle", "end");
    TEST_ASSERT(result != NULL, "join3 with empty first part");
#ifdef UNIX
  TEST_ASSERT(strcmp(result, "/middle/end") == 0, "join3 empty first Unix");
#else
    TEST_ASSERT(strcmp(result, "\\middle\\end") == 0, "join3 empty first Windows");
#endif
    free(result);

    result = lw_path_join3("start", "", "end");
    TEST_ASSERT(result != NULL, "join3 with empty middle part");
#ifdef UNIX
    TEST_ASSERT(strcmp(result, "start//end") == 0, "join3 empty middle Unix");
#else
    TEST_ASSERT(strcmp(result, "start\\\\end") == 0, "join3 empty middle Windows");
#endif
    free(result);
}

/*------------------------------------------------------------------*/
static void
test_lw_path_join3_various_inputs()
{
    char *result;

    result = lw_path_join3("/usr", "local", "share");
#ifdef UNIX
    TEST_ASSERT(strcmp(result, "/usr/local/share") == 0, "Absolute path join3");
#else
    TEST_ASSERT(strcmp(result, "/usr\\local\\share") == 0, "Absolute path join3 Windows");
#endif
    free(result);

    result = lw_path_join3("data", "maps", "level1.map");
#ifdef UNIX
    TEST_ASSERT(strcmp(result, "data/maps/level1.map") == 0, "File path join3");
#else
    TEST_ASSERT(strcmp(result, "data\\maps\\level1.map") == 0, "File path join3 Windows");
#endif
    free(result);

    result = lw_path_join3("a", "b", "c");
    TEST_ASSERT(result != NULL, "join3 with single chars");
    TEST_ASSERT(strlen(result) == 5, "join3 single chars correct length");
    free(result);
}

/*------------------------------------------------------------------*/
static void
test_memory_management()
{
    char *result1, *result2, *result3;

    result1 = lw_path_join2("test", "path");
    result2 = lw_path_join2("another", "test");
    result3 = lw_path_join3("three", "part", "path");

    TEST_ASSERT(result1 != NULL && result2 != NULL && result3 != NULL, "Multiple allocations successful");
    TEST_ASSERT(result1 != result2 && result2 != result3 && result1 != result3, "Different memory addresses");

    free(result1);
    free(result2);
    free(result3);

    result1 = lw_path_join2("after", "free");
    TEST_ASSERT(result1 != NULL, "Allocation after free works");
    free(result1);
}

/*------------------------------------------------------------------*/
int
main()
{
    TEST_START();

    printf("\n=== Testing lw_path_get_system_name ===\n");
    test_lw_path_get_system_name_basic();
    test_lw_path_get_system_name_with_paths();
    test_lw_path_get_system_name_edge_cases();

    printf("\n=== Testing lw_path_join2 ===\n");
    test_lw_path_join2_basic();
    test_lw_path_join2_various_inputs();

    printf("\n=== Testing lw_path_join3 ===\n");
    test_lw_path_join3_basic();
    test_lw_path_join3_various_inputs();

    printf("\n=== Testing Memory Management ===\n");
    test_memory_management();

    TEST_END();
}
