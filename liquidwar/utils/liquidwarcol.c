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
/* nom           : liquidwarcol.c                                   */
/* contenu       : tools to convert files to 8 bits palettes        */
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
#include <time.h>
#include <allegro.h>

/*=================================================================*/
/* constants                                                       */
/*=================================================================*/

#define LW_TEXTURE_RANDOM_MAX_RETRIES 10000

/*=================================================================*/
/* variables globales                                              */
/*=================================================================*/

int ARGC;
char **ARGV;
int FLAG_BACKUP = 0;
int FLAG_HELP = 0;
int FLAG_SILENT = 0;
int FIRST_COLOR;
int NUMBER_OF_COLORS;
char *FILENAMES[65536];
int NUMBER_OF_FILES = 0;
PALETTE PALETTE_SRC;
PALETTE PALETTE_DST;
BITMAP *BITMAP_SRC = NULL;
BITMAP *BITMAP_DST = NULL;

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
  printf ("liquidwarcol by U-Foot (ufoot@ufoot.org).\n");
  printf ("This is a 'palette reducer' for 256 colors bitmaps.\n");
  printf ("It is free software, protected by the GPL.\n");
  printf ("It uses Allegro by Shawn Hargreaves.\n");
  printf ("\n");
}

/*-----------------------------------------------------------------*/
void
display_short_help (void)
{
  display_common_help ();
  printf ("Type \"liquidwarcol -?\" for more help.\n");
}

/*-----------------------------------------------------------------*/
void
display_long_help (void)
{
  display_common_help ();
  printf ("Syntax:\n");
  printf ("liquidwarcol [options] first_color number_of_colors filenames\n");
  printf ("\n");
  printf ("Options:\n");
  printf ("-? -h -H : displays this help.\n");
  printf ("-s -S :    silent mode, nothing written to the console.\n");
  printf ("\n");
  printf ("Warning: the source file will be replaced!\n");
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
	case 'b':
	case 'B':
	  FLAG_BACKUP = 1;
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
	  if (j == 0)
	    FIRST_COLOR = atoi (ARGV[i]);
	  if (j == 1)
	    NUMBER_OF_COLORS = atoi (ARGV[i]);
	  if (j >= 2)
	    FILENAMES[j - 2] = ARGV[i];
	  j++;
	}
    }
  NUMBER_OF_FILES = j - 2;
  if (NUMBER_OF_FILES > 0)
    {
      if (FIRST_COLOR < 0)
	{
	  success = 0;
	  printf ("ERROR! First color number must be >= 0.\n");
	}
      if (FIRST_COLOR > 255)
	{
	  success = 0;
	  printf ("ERROR! First color number must be < 255.\n");
	}
      if (NUMBER_OF_COLORS < 1)
	{
	  success = 0;
	  printf ("ERROR! Number of colors must be > 1.\n");
	}
      if (NUMBER_OF_COLORS + FIRST_COLOR > 256)
	{
	  success = 0;
	  printf ("ERROR! Last color number must be < 256.\n");
	}
    }
  else
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
void
save_backup (char *filename)
{
  char *bak_name = "faut encore le programmer";

  if (!FLAG_SILENT)
    printf ("Saving a copy of '%s' as '%s'.\n", filename, bak_name);
}

/*-----------------------------------------------------------------*/
void
save_file (char *filename)
{
  if (!FLAG_SILENT)
    printf ("Replacing '%s'.\n", filename);
  if (save_bitmap (filename, BITMAP_DST, PALETTE_DST))
    printf ("ERROR! Unable to save '%s'.\n", filename);
}

/*-----------------------------------------------------------------*/
/* operations de conversion                                        */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
int
find_color (PALETTE pal, RGB rgb)
{
  int i, found = -1;

  for (i = 0; i < 256 && found < 0; ++i)
    if (pal[i].r == rgb.r && pal[i].g == rgb.g && pal[i].b == rgb.b)
      found = i;

  return found;
}

/*-----------------------------------------------------------------*/
int
exist_color (PALETTE pal, RGB rgb)
{
  return (find_color (pal, rgb) >= 0);
}

/*-----------------------------------------------------------------*/
void
recalculate_number_of_colors ()
{
  int i, x, y, n = 1;
  RGB color;
  PALETTE pal;

  //nb_of_colors=NUMBER_OF_COLORS;
  color = PALETTE_SRC[getpixel (BITMAP_SRC, 0, 0)];

  for (i = 0; i < 256; ++i)
    pal[i] = color;

  for (y = 0; y < BITMAP_SRC->h && n < NUMBER_OF_COLORS; ++y)
    for (x = 0; x < BITMAP_SRC->w && n < NUMBER_OF_COLORS; ++x)
      {
	color = PALETTE_SRC[getpixel (BITMAP_SRC, x, y)];
	if (!exist_color (pal, color))
	  pal[n++] = color;
      }
  NUMBER_OF_COLORS = n;
}

/*-----------------------------------------------------------------*/
void
create_new_palette (void)
{
  int i, x, y, index;
  int nb_retries = 0;
  RGB color;

  x = random () % BITMAP_SRC->w;
  y = random () % BITMAP_SRC->h;
  color = PALETTE_SRC[getpixel (BITMAP_SRC, x, y)];

  for (i = 0; i < 256; ++i)
    PALETTE_DST[i] = color;

  for (i = 1; i < NUMBER_OF_COLORS;)
    {
      x = random () % BITMAP_SRC->w;
      y = random () % BITMAP_SRC->h;
      index = getpixel (BITMAP_SRC, x, y);
      color = PALETTE_SRC[index];
      if ((!exist_color (PALETTE_DST, color)) ||
	  (nb_retries > LW_TEXTURE_RANDOM_MAX_RETRIES))
	{
	  PALETTE_DST[FIRST_COLOR + (i++)] = color;
	  nb_retries = 0;
	}
      else
	{
	  nb_retries++;
	}
    }
}

/*-----------------------------------------------------------------*/
void
correct_palette (void)
{
  int i;

  for (i = 0; i < FIRST_COLOR; ++i)
    {
      PALETTE_DST[i].r = 0;
      PALETTE_DST[i].g = 0;
      PALETTE_DST[i].b = 0;
    }
  for (i = FIRST_COLOR + NUMBER_OF_COLORS; i < 256; ++i)
    {
      PALETTE_DST[i].r = 63;
      PALETTE_DST[i].g = 63;
      PALETTE_DST[i].b = 63;
    }
}

/*-----------------------------------------------------------------*/
void
create_converted_bitmap (void)
{
  char corres[256];
  int i, x, y, index;

  for (i = 0; i < 256; ++i)
    corres[i] = bestfit_color (PALETTE_DST,
			       PALETTE_SRC[i].r,
			       PALETTE_SRC[i].g, PALETTE_SRC[i].b);
  BITMAP_DST = create_bitmap (BITMAP_SRC->w, BITMAP_SRC->h);
  for (y = 0; y < BITMAP_SRC->h; ++y)
    for (x = 0; x < BITMAP_SRC->w; ++x)
      {
	index = corres[getpixel (BITMAP_SRC, x, y)];
	index = (index < FIRST_COLOR ||
		 index >= FIRST_COLOR + NUMBER_OF_COLORS) ?
	  FIRST_COLOR : index;
	putpixel (BITMAP_DST, x, y, index);
      }
}

/*-----------------------------------------------------------------*/
void
convert_bitmap ()
{
  if (BITMAP_DST != NULL)
    destroy_bitmap (BITMAP_DST);

  recalculate_number_of_colors ();
  create_new_palette ();
  create_converted_bitmap ();
  correct_palette ();
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

  srandom (time (NULL));
  // srandom(1);

  if (read_command_line ())
    {
      for (i = 0; i < NUMBER_OF_FILES; ++i)
	{
	  if (load_file (FILENAMES[i]))
	    {
	      if (FLAG_BACKUP)
		save_backup (FILENAMES[i]);
	      convert_bitmap (FILENAMES[i]);
	      save_file (FILENAMES[i]);
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

END_OF_MAIN ();
