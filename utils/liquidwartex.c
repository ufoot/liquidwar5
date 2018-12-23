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
/* Copyright (C) 1998-2018 Christian Mauduit                                 */
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
/* nom           : liquidwartex.c                                   */
/* contenu       : tool to generate textures                        */
/* date de modif : March 2000                                       */
/********************************************************************/

/*=================================================================*/
/* defines                                                         */
/*=================================================================*/

#define ALLEGRO_USE_CONSOLE

/*=================================================================*/
/* includes                                                        */
/*=================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <allegro.h>
#include <sys/stat.h>

/*=================================================================*/
/* variables globales                                              */
/*=================================================================*/

int ARGC;
char **ARGV;
int FLAG_HELP = 0;
int FLAG_SILENT = 0;
char *FILENAMES[65536];
int NUMBER_OF_FILES = 0;
PALETTE PALETTE_SRC;
BITMAP *BITMAP_SRC = NULL;
int DST_X, DST_Y, DST_W, DST_H, DST_SIZE;
int FIRST_COLOR = 32;
int COLOR_NUMBER = 32;
char BUFFER[1048576];
char SYSTEM_NAME[16];


/*=================================================================*/
/* fonctions                                                       */
/*=================================================================*/

/*-----------------------------------------------------------------*/
/* affichages d'aide                                               */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
void
display_common_help (void)
{
  printf ("liquidwartex by U-Foot (ufoot@ufoot.org).\n");
  printf ("This program creates textures for Liquid War.\n");
  printf ("It is free software, protected by the GPL.\n");
  printf ("It uses Allegro by Shawn Hargreaves.\n");
  printf ("\n");
}

/*-----------------------------------------------------------------*/
void
display_short_help (void)
{
  display_common_help ();
  printf ("Type \"liquidwartex -?\" for more help.\n");
}

/*-----------------------------------------------------------------*/
void
display_long_help (void)
{
  display_common_help ();
  printf
    ("The source must be a 256 color bitmap using color from first_color to first_color+number_of_colors-1.\n");
  printf ("\n");
  printf ("Syntax:\n");
  printf ("liquidwartex [options] first_color number_of_colors filenames\n");
  printf ("\n");
  printf ("Options:\n");
  printf ("-? -h -H : displays this help.\n");
  printf ("-s -S :    silent mode, nothing written to the console.\n");
  printf ("\n");
  printf ("Remark: the created texture will have a .tex extension.\n");
}

/*-----------------------------------------------------------------*/
/* lecture des parametres de la ligne de commande                  */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
int
acknowledge_flag (char *str)
{
  int found = 0;

  if (str[0] == '-' || str[0] == '/')
    {
      found = 1;
      switch (str[1])
        {
        case '?':
        case 'h':
        case 'H':
          FLAG_HELP = 1;
          break;
        case 's':
        case 'S':
          FLAG_SILENT = 1;
          break;
        default:
          found = 0;
        }
    }
  return found;
}

/*-----------------------------------------------------------------*/
int
read_command_line (void)
{
  int i, j = 0, success = 1;

  if (ARGC <= 2)
    {
      success = 0;
      if (!FLAG_HELP)
        printf ("ERROR! Too few arguments.\n");
    }
  else
    {
      for (i = 1; i < ARGC; ++i)
        {
          if ((!acknowledge_flag (ARGV[i])))
            {
              if (j == 0)
                FIRST_COLOR = atoi (ARGV[i]);
              if (j == 1)
                COLOR_NUMBER = atoi (ARGV[i]);
              if (j >= 2)
                FILENAMES[j - 2] = ARGV[i];
              j++;
            }
        }
      if (COLOR_NUMBER < 8 || COLOR_NUMBER > 32)
        {
          /*
           * COLOR_NUMBER must be <=32 for weird implementation
           * reasons. There's no easy way to change this easily.
           */
          success = 0;
          if (!FLAG_HELP)
            printf ("ERROR! Color number must be between 8 and 32");
        }
      else
        {
          NUMBER_OF_FILES = j - 2;
          if (NUMBER_OF_FILES <= 0)
            {
              success = 0;
              if (!FLAG_HELP)
                printf ("ERROR! Too few arguments.\n");
            }
        }
    }
  return success;
}

/*-----------------------------------------------------------------*/
/* acces disque                                                    */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
int
load_file (char *filename)
{
  int success;

  if (!FLAG_SILENT)
    printf ("Loading '%s'.\n", filename);

  if (BITMAP_SRC != NULL)
    destroy_bitmap (BITMAP_SRC);
  BITMAP_SRC = load_bitmap (filename, PALETTE_SRC);
  success = BITMAP_SRC != NULL;
  if (!success)
    printf ("ERROR! Unable to read '%s'.\n", filename);
  return success;
}

/*-----------------------------------------------------------------*/
/* lecture des parametres du tableau                               */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
void
get_range (void)
{
  int min_x = BITMAP_SRC->w;
  int min_y = BITMAP_SRC->h;
  int max_x = -1;
  int max_y = -1;
  int x, y;

  for (y = 0; y < BITMAP_SRC->h; ++y)
    for (x = 0; x < BITMAP_SRC->w; ++x)
      if (getpixel (BITMAP_SRC, x, y) >= FIRST_COLOR ||
          getpixel (BITMAP_SRC, x, y) < FIRST_COLOR + COLOR_NUMBER)
        {
          if (min_x > x)
            min_x = x;
          if (min_y > y)
            min_y = y;
          if (max_x < x)
            max_x = x;
          if (max_y < y)
            max_y = y;
        }
  DST_X = min_x;
  DST_Y = min_y;
  DST_W = max_x - min_x + 1;
  DST_H = max_y - min_y + 1;

  if (DST_W < 1 || DST_H < 1)
    {
      printf ("ERROR! Texture is too small.\n");
      DST_W = DST_H = 1;
    }
  DST_SIZE = 3 * COLOR_NUMBER + ((DST_W * DST_H + 7) / 8) * 5;
}

/*-----------------------------------------------------------------*/
void
update_system_name (char *str)
{
  char *search;

  memset (SYSTEM_NAME, 0, sizeof (SYSTEM_NAME));

  /*
   * No extension we just use the file name
   */
  strncpy (SYSTEM_NAME, str, sizeof (SYSTEM_NAME));
  search = strchr (SYSTEM_NAME, '.');
  if (search)
    {
      (*search) = '\0';
    }
}

/*-----------------------------------------------------------------*/
/* conversion dans le buffer                                       */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
void
convert_to_buffer (void)
{
  int pos = 0, pos8 = 0, x, y, i;
  char octet[5], toadd;
  int coul;

  for (i = 0; i < COLOR_NUMBER; ++i)
    {
      BUFFER[pos++] = PALETTE_SRC[FIRST_COLOR + i].r;
      BUFFER[pos++] = PALETTE_SRC[FIRST_COLOR + i].g;
      BUFFER[pos++] = PALETTE_SRC[FIRST_COLOR + i].b;
    }

  for (i = 0; i < 5; ++i)
    octet[i] = 0;

  for (y = 0; y < DST_H; ++y)
    for (x = 0; x < DST_W; ++x)
      {
        coul = getpixel (BITMAP_SRC, DST_X + x, DST_Y + y) - FIRST_COLOR;
        toadd = 1 << pos8;
        octet[0] |= (coul & 1) ? toadd : 0;
        octet[1] |= (coul & 2) ? toadd : 0;
        octet[2] |= (coul & 4) ? toadd : 0;
        octet[3] |= (coul & 8) ? toadd : 0;
        octet[4] |= (coul & 16) ? toadd : 0;

        if (pos8 == 7 || (y == DST_H - 1 && x == DST_W - 1))
          {
            for (i = 0; i < 5; ++i)
              {
                BUFFER[pos++] = octet[i];
                octet[i] = 0;
              }
            pos8 = 0;
          }
        else
          pos8++;
      }
}

/*-----------------------------------------------------------------*/
/* on ecrit sur le disque                                          */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
void
change_ext (char *str)
{
  char *ext;

  ext = strchr (str, '.');
  ext[1] = 't';
  ext[2] = 'e';
  ext[3] = 'x';
}

/*-----------------------------------------------------------------*/
void
write_to_disk (char *str)
{
  FILE *f;
  short w, h;
  char header[4];

  if (!FLAG_SILENT)
    printf ("Writing %s to disk.\n", str);
  delete_file (str);
  f = fopen (str, "w");
  if (f != NULL)
    {
      w = DST_W;
      h = DST_H;

      /*
       * This is supposed to match the endianess used in 
       * the serial.h/serial.c files of the core source.
       */
      header[0] = 0xFF & w;
      header[1] = w >> 8;
      header[2] = 0xFF & h;
      header[3] = h >> 8;

      fwrite (header, 4, 1, f);

      fwrite (SYSTEM_NAME, sizeof (SYSTEM_NAME), 1, f);
      fwrite (BUFFER, DST_SIZE, 1, f);
      fclose (f);
    }
  else
    {
      printf ("Unable to write %s.\n", str);
    }
}

/*=================================================================*/
/* fonctions main                                                  */
/*=================================================================*/

/*-----------------------------------------------------------------*/
int
main (int argc, char **argv)
{
  int i;

  ARGC = argc;
  ARGV = argv;

#ifdef ALLEGRO_UNIX
  install_allegro (SYSTEM_NONE, &errno, atexit);
#else
  allegro_init ();
#endif

  set_color_depth (8);
  set_color_conversion (COLORCONV_REDUCE_TO_256);

  if (read_command_line ())
    {
      for (i = 0; i < NUMBER_OF_FILES; ++i)
        {
          if (load_file (FILENAMES[i]))
            {
              get_range ();
              convert_to_buffer ();
              update_system_name (FILENAMES[i]);
              change_ext (FILENAMES[i]);
              write_to_disk (FILENAMES[i]);
            }
        }
    }
  else
    {
      if (FLAG_HELP)
        display_long_help ();
      else
        display_short_help ();
    }
  allegro_exit ();

  return 0;

}

END_OF_MAIN ()
