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
/* name          : test_backport_pixel.c                           */
/* content       : unit tests for putpixel/getpixel functions      */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_simple.h"
#include "backport.h"
#include "palette.h"

/*==================================================================*/
/* functions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
init_test_environment()
{
    // Initialize Allegro
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro\n");
        exit(1);
    }
    
    // Initialize a basic palette for testing
    // This is a simplified palette initialization
    for (int i = 0; i < PALETTE_SIZE; i++) {
        float r = ((i >> 5) & 0x07) / 7.0f;  // 3 bits red
        float g = ((i >> 2) & 0x07) / 7.0f;  // 3 bits green  
        float b = (i & 0x03) / 3.0f;         // 2 bits blue
        GLOBAL_PALETTE[i] = al_map_rgb_f(r, g, b);
    }
}

/*------------------------------------------------------------------*/
static void
test_putpixel_getpixel_basic()
{
    // Create a test bitmap
    ALLEGRO_BITMAP *test_bitmap = al_create_bitmap(100, 100);
    TEST_ASSERT(test_bitmap != NULL, "Test bitmap created");
    
    // Test basic putpixel/getpixel round-trip
    int test_color = 42;
    putpixel(test_bitmap, 50, 50, test_color);
    int retrieved_color = getpixel(test_bitmap, 50, 50);
    
    TEST_ASSERT_EQ(test_color, retrieved_color, "Basic putpixel/getpixel round-trip");
    
    al_destroy_bitmap(test_bitmap);
}

/*------------------------------------------------------------------*/
static void
test_putpixel_getpixel_multiple_colors()
{
    ALLEGRO_BITMAP *test_bitmap = al_create_bitmap(100, 100);
    TEST_ASSERT(test_bitmap != NULL, "Test bitmap created for multiple colors");
    
    // Test multiple colors
    int test_colors[] = {0, 1, 15, 63, 127, 255};
    int num_colors = sizeof(test_colors) / sizeof(test_colors[0]);
    
    for (int i = 0; i < num_colors; i++) {
        int x = 10 + i * 10;
        int y = 10;
        int color = test_colors[i];
        
        putpixel(test_bitmap, x, y, color);
        int retrieved = getpixel(test_bitmap, x, y);
        
        char msg[100];
        snprintf(msg, sizeof(msg), "Color %d round-trip at (%d,%d)", color, x, y);
        TEST_ASSERT_EQ(color, retrieved, msg);
    }
    
    al_destroy_bitmap(test_bitmap);
}

/*------------------------------------------------------------------*/
static void
test_putpixel_getpixel_edge_cases()
{
    ALLEGRO_BITMAP *test_bitmap = al_create_bitmap(100, 100);
    TEST_ASSERT(test_bitmap != NULL, "Test bitmap created for edge cases");
    
    // Test corners and edges
    int positions[][2] = {{0, 0}, {99, 0}, {0, 99}, {99, 99}, {50, 0}, {50, 99}};
    int num_positions = sizeof(positions) / sizeof(positions[0]);
    
    for (int i = 0; i < num_positions; i++) {
        int x = positions[i][0];
        int y = positions[i][1];
        int color = 64 + i;  // Different color for each position
        
        putpixel(test_bitmap, x, y, color);
        int retrieved = getpixel(test_bitmap, x, y);
        
        char msg[100];
        snprintf(msg, sizeof(msg), "Edge case color %d at (%d,%d)", color, x, y);
        TEST_ASSERT_EQ(color, retrieved, msg);
    }
    
    al_destroy_bitmap(test_bitmap);
}

/*------------------------------------------------------------------*/
static void
test_putpixel_getpixel_pattern()
{
    ALLEGRO_BITMAP *test_bitmap = al_create_bitmap(50, 50);
    TEST_ASSERT(test_bitmap != NULL, "Test bitmap created for pattern");
    
    // Create a checkerboard pattern
    for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 50; x++) {
            int color = ((x + y) % 2) ? 100 : 150;
            putpixel(test_bitmap, x, y, color);
        }
    }
    
    // Verify the pattern
    int correct_pixels = 0;
    for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 50; x++) {
            int expected = ((x + y) % 2) ? 100 : 150;
            int actual = getpixel(test_bitmap, x, y);
            if (expected == actual) {
                correct_pixels++;
            }
        }
    }
    
    TEST_ASSERT_EQ(2500, correct_pixels, "Checkerboard pattern verification (50x50 = 2500 pixels)");
    
    al_destroy_bitmap(test_bitmap);
}

/*------------------------------------------------------------------*/
static void
test_palette_boundary_values()
{
    ALLEGRO_BITMAP *test_bitmap = al_create_bitmap(10, 10);
    TEST_ASSERT(test_bitmap != NULL, "Test bitmap created for boundary values");
    
    // Test first and last valid palette indices
    putpixel(test_bitmap, 0, 0, 0);  // First palette color
    int first_color = getpixel(test_bitmap, 0, 0);
    TEST_ASSERT_EQ(0, first_color, "First palette color (0)");
    
    putpixel(test_bitmap, 1, 1, PALETTE_SIZE - 1);  // Last palette color
    int last_color = getpixel(test_bitmap, 1, 1);
    TEST_ASSERT_EQ(PALETTE_SIZE - 1, last_color, "Last palette color");
    
    al_destroy_bitmap(test_bitmap);
}

/*------------------------------------------------------------------*/
int
main()
{
    TEST_START();
    
    init_test_environment();
    
    printf("\n=== Testing putpixel/getpixel functions ===\n");
    test_putpixel_getpixel_basic();
    test_putpixel_getpixel_multiple_colors();
    test_putpixel_getpixel_edge_cases();
    test_putpixel_getpixel_pattern();
    test_palette_boundary_values();
    
    TEST_END();
}