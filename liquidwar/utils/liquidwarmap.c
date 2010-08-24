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
/* Copyright (C) 1998-2008 Christian Mauduit                                 */
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
/* nom           : liquidwarmap.c                                   */
/* contenu       : tools to generate tabs                           */
/* date de modif : 3 mai 98                                         */
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
/* contantes                                                       */
/*=================================================================*/

#define LIGHT_OR_DARK_TRESHOLD 315
#define CONSIDERED_AS_DARK 0
#define CONSIDERED_AS_LIGHT 2
#define PLAYABLE_AREA 1
#define MINI_SIDE_SIZE 4
#define MINI_PLAYABLE_AREA 1024

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
BITMAP *BITMAP_DST = NULL;
signed char BUFFER[1048576];
int DST_X, DST_Y, DST_W, DST_H, DST_SIZE, BG_SIZE;
char SYSTEM_NAME[16];
char READABLE_NAME[32];

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
  printf ("liquidwarmap by U-Foot (ufoot@ufoot.org).\n");
  printf ("This program creates maps for Liquid War.\n");
  printf ("It is free software, protected by the GPL.\n");
  printf ("It uses Allegro by Shawn Hargreaves.\n");
  printf ("\n");
}

/*-----------------------------------------------------------------*/
void
display_short_help (void)
{
  display_common_help ();
  printf ("Type \"liquidwarmap -?\" for more help.\n");
}

/*-----------------------------------------------------------------*/
void
display_long_help (void)
{
  display_common_help ();
  printf ("The source must be a 256 color bitmap.\n");
  printf ("Walls must be in the same color - same index in fact.\n");
  printf ("The wall index color is given by the top left pixel,\n");
  printf ("and the map must be contained in a closed rectangle.\n");
  printf ("\n");
  printf ("Syntax:\n");
  printf ("liquidwarmap [options] filenames\n");
  printf ("\n");
  printf ("Options:\n");
  printf ("-? -h -H : displays this help.\n");
  printf ("-s -S :    silent mode, nothing written to the console.\n");
  printf ("\n");
  printf ("Remark: the created map will have a .map extension.\n");
  printf ("Warning: the source file will be replaced by a smaller file.\n");
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

  for (i = 1; i < ARGC; ++i)
    {
      if ((!acknowledge_flag (ARGV[i])))
	{
	  FILENAMES[j] = ARGV[i];
	  j++;
	}
    }
  NUMBER_OF_FILES = j;
  if (NUMBER_OF_FILES <= 0)
    {
      success = 0;
      if (!FLAG_HELP)
	printf ("ERROR! Two few arguments.\n");
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

/*------------------------------------------------------------------*/
static void
sort_light_and_dark (BITMAP * bmp, PALETTE pal)
{
  char table[256];
  int i, x, y;

  for (i = 0; i < 256; ++i)
    {
      if (6 * pal[i].r + 3 * pal[i].g + pal[i].b > LIGHT_OR_DARK_TRESHOLD)
	table[i] = CONSIDERED_AS_LIGHT;
      else
	table[i] = CONSIDERED_AS_DARK;
    }

  for (y = 0; y < bmp->h; ++y)
    for (x = 0; x < bmp->w; ++x)
      putpixel (bmp, x, y, table[getpixel (bmp, x, y)]);
}

/*-----------------------------------------------------------------*/
void
get_range (void)
{
  /*
     useless and buggy code from an old time, when maps where
     systematically drawn on 320x200 bitmaps and "getting the
     useable part had a sense". Not any more. Now it only
     wrecks some maps. Definitely useless.

     int min_x = BITMAP_SRC->w;
     int min_y = BITMAP_SRC->h;
     int max_x = -1;
     int max_y = -1;
     int x, y;

     for (y = 0; y < BITMAP_SRC->h; ++y)
     for (x = 0; x < BITMAP_SRC->w; ++x)
     if (!getpixel (BITMAP_SRC, x, y))
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
   */

  DST_X = 0;
  DST_Y = 0;
  DST_W = BITMAP_SRC->w;
  DST_H = BITMAP_SRC->h;

  if (DST_W < 1 || DST_H < 1)
    {
      printf ("ERROR! Map is too small.\n");
      DST_W = DST_H = 1;
    }
  if (BITMAP_DST != NULL)
    destroy_bitmap (BITMAP_DST);
  BITMAP_DST = create_bitmap (DST_W, DST_H);
  blit (BITMAP_SRC, BITMAP_DST, DST_X, DST_Y, 0, 0, DST_W, DST_H);
}

/*-----------------------------------------------------------------*/
/* conversion dans le buffer                                       */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
void
convert_to_buffer (void)
{
  int pos_src, pos_dst, l, wh;
  char *data;
  int wall_color;

  wh = BITMAP_DST->h * BITMAP_DST->w;
  data = BITMAP_DST->dat;
  pos_src = pos_dst = 0;
  BG_SIZE = 0;

  if (wh > 0)
    {
      wall_color = CONSIDERED_AS_DARK;
      while (pos_src < wh)
	{
	  l = 0;
	  if (data[pos_src] == wall_color)
	    {
	      while (pos_src < wh && (data[pos_src] == wall_color) && l < 127)
		{
		  l++;
		  pos_src++;
		}
	      BUFFER[pos_dst++] = l;
	    }
	  else
	    {
	      while (pos_src < wh && data[pos_src] != wall_color && l < 127)
		{
		  l++;
		  pos_src++;
		}
	      BUFFER[pos_dst++] = -l;
	      BG_SIZE += l;
	    }
	}
    }
  BUFFER[pos_dst++] = 0;

  DST_SIZE = pos_dst;
}

/*-----------------------------------------------------------------*/
void
change_ext_to_txt (char *str)
{
  char *ext;

  ext = strchr (str, '.');
  ext[1] = 't';
  ext[2] = 'x';
  ext[3] = 't';
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
void
update_readable_name (char *str)
{
  FILE *f;
  char *search;

  memset (READABLE_NAME, 0, sizeof (READABLE_NAME));

  f = fopen (str, "r");
  if (f != NULL)
    {
      /*
       * If there's a corresponding .txt file, we open it and interpret
       * it as the name of the map
       */
      fread (READABLE_NAME, sizeof (READABLE_NAME), 1, f);
      fclose (f);
      /*
       * Now we get rid of all the lines following the first one
       */
      search = strchr (READABLE_NAME, 10);
      if (search)
	{
	  (*search) = 0;
	}
      search = strchr (READABLE_NAME, 13);
      if (search)
	{
	  (*search) = 0;
	}
    }
  else
    {
      /*
       * No .txt file we just use the file name
       */
      strncpy (READABLE_NAME, str, sizeof (READABLE_NAME));
      search = strchr (READABLE_NAME, '.');
      if (search)
	{
	  (*search) = '\0';
	}
    }
}

/*-----------------------------------------------------------------*/
/* on ecrit sur le disque                                          */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
void
change_ext_to_map (char *str)
{
  char *ext;

  ext = strchr (str, '.');
  ext[1] = 'm';
  ext[2] = 'a';
  ext[3] = 'p';
}

/*-----------------------------------------------------------------*/
void
write_to_disk (char *str)
{
  FILE *f;
  short w, h;
  int size;
  unsigned char header[8];

  if (!FLAG_SILENT)
    printf ("Writing %s to disk.\n", str);
  delete_file (str);
  f = fopen (str, "w");
  if (f != NULL)
    {
      w = DST_W;
      h = DST_H;
      size = DST_SIZE;

      /*
       * Previous versions of LW used to store the size of
       * the background here, to sort map afterwards. Now
       * we store the size of the map.
       */

      /*
       * Here's an ugly serialization of ints and shorts.
       * Usefull to make the game work on Mac OS X and possible
       * any platform that does not have PC-like endianess
       */

      header[0] = 0xFF & size;
      header[1] = 0xFF & size >> 8;
      header[2] = 0xFF & size >> 16;
      header[3] = size >> 24;
      header[4] = 0xFF & w;
      header[5] = w >> 8;
      header[6] = 0xFF & h;
      header[7] = h >> 8;

      /*
         Uncomment this to invert endianess of map header.
         Used only for testing, do not uncomment it even
         when compiling on Mac, it would make the game
         unusable in network mode...
       */
      /*
         header[3] = 0xFF & size;
         header[2] = 0xFF & size>>8;
         header[1] = 0xFF & size>>16;
         header[0] = size>>24;
         header[5] = 0xFF & w;
         header[4] = w>>8;
         header[7] = 0xFF & h;
         header[6] = h>>8;
       */

      fwrite (&(header[0]), 8, 1, f);
      fwrite (SYSTEM_NAME, sizeof (SYSTEM_NAME), 1, f);
      fwrite (READABLE_NAME, sizeof (READABLE_NAME), 1, f);
      fwrite (BUFFER, size, 1, f);
      fclose (f);
    }
  else
    {
      printf ("Unable to write %s.\n", str);
    }
}

/*-----------------------------------------------------------------*/
void
write_with_new_size (char *str)
{
  if (!FLAG_SILENT)
    printf ("Writing %s to disk.\n", str);
  delete_file (str);

  save_bitmap (str, BITMAP_DST, PALETTE_SRC);
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
	      sort_light_and_dark (BITMAP_SRC, PALETTE_SRC);
	      get_range ();
	      if (BITMAP_SRC->w != BITMAP_DST->w
		  || BITMAP_SRC->h != BITMAP_DST->h)
		write_with_new_size (FILENAMES[i]);
	      convert_to_buffer ();
	      change_ext_to_txt (FILENAMES[i]);
	      update_system_name (FILENAMES[i]);
	      update_readable_name (FILENAMES[i]);
	      change_ext_to_map (FILENAMES[i]);
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
