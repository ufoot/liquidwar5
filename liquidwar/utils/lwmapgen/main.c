/*
* A Random map generator for Liquid War.
*
* Copyright (C) 2003, David Redick, Christian Mauduit
* Released under the GNU General Public License (v2)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "map.h"
#include "func.h"
#include "map_generator.h"

static char *filename = NULL;
static int size = DEFAULT_MAP_SIZE;
static int grid_size = RANDOM_MAP_GRID_SIZE;
static int func_id = MIN_FUNC;

void do_args (int argc, char **argv);
void print_help ();
void print_version ();

#ifdef DOS
/*
 * These macros reduce the size of the DOS executable
 */
BEGIN_GFX_DRIVER_LIST
  END_GFX_DRIVER_LIST
  BEGIN_COLOR_DEPTH_LIST
  COLOR_DEPTH_8
  END_COLOR_DEPTH_LIST
  BEGIN_DIGI_DRIVER_LIST
  END_DIGI_DRIVER_LIST
  BEGIN_MIDI_DRIVER_LIST
  END_MIDI_DRIVER_LIST BEGIN_JOYSTICK_DRIVER_LIST END_JOYSTICK_DRIVER_LIST
#endif
#ifdef WIN32
/*
 * Under Win32 we want a console executable so we don't use
 * the "END_OF_MAIN" stuff, and this implies to undef "main".
 */
#undef main
#endif
  int
main (int argc, char **argv)
{
  PALETTE pal;
  int i;

  /* init allegro and init palette */
  install_allegro (SYSTEM_NONE, &errno, atexit);
  set_color_depth (8);
  set_color_conversion (COLORCONV_REDUCE_TO_256);
  for (i = 0; i < 256; i++)
    {
      /* divided by 4 because the colour value ranges from 0-63 */
      pal[i].r = pal[i].g = pal[i].b = i / 4;
    }

  srand (time (NULL));

  do_args (argc, argv);

  printf ("filename = %s\n", filename);

  map_generator (filename, size, grid_size, func_id);

  printf ("map-- %s\n", map.filename);
  save_bitmap (map.filename, map.map, pal);
  printf ("map-- %s\n", map.filename);

  return EXIT_SUCCESS;
}

#ifndef WIN32
END_OF_MAIN ();
#endif

/*****************************************************************************/

void
do_args (int argc, char **argv)
{
  int i;

  for (i = 1; i < argc; i++)
    {
      if (strcmp (argv[i], "-o") == 0 || strcmp (argv[i], "--out") == 0)
	{
	  if (i >= argc - 1)
	    {
	      fprintf (stderr,
		       "fatal error: %s option without any file, need one more arg.\n",
		       argv[i]);
	      exit (EXIT_FAILURE);
	    }
	  int len = strlen (argv[++i]);
	  filename = malloc (sizeof (char) * len);
	  if (filename == NULL)
	    {
	      fprintf (stderr,
		       "fatal error: can't malloc space for map name.\n");
	      exit (EXIT_FAILURE);
	    }
	  strcpy (filename, argv[i]);
	}
      else if (strcmp (argv[i], "-s") == 0 || strcmp (argv[i], "--size") == 0)
	{
	  if (i >= argc - 1)
	    {
	      fprintf (stderr,
		       "fatal error: %s option without any size, need one more arg.\n",
		       argv[i]);
	      exit (EXIT_FAILURE);
	    }
	  size = atoi (argv[++i]);
	  if (size < MIN_MAP_SIZE)
	    {
	      fprintf (stderr,
		       "map size too small using: %d\n", MIN_MAP_SIZE);
	      size = MIN_MAP_SIZE;
	    }
	  else if (size >= MAX_MAP_SIZE)
	    {
	      fprintf (stderr,
		       "map size too large using: %d\n", MAX_MAP_SIZE - 1);
	      size = MAX_MAP_SIZE - 1;
	    }
	}
      else if (strcmp (argv[i], "-g") == 0 || strcmp (argv[i], "--grid") == 0)
	{
	  if (i >= argc - 1)
	    {
	      fprintf (stderr,
		       "fatal error: %s option without any value, need one more arg.\n",
		       argv[i]);
	      exit (EXIT_FAILURE);
	    }
	  grid_size = atoi (argv[++i]);
	  if (grid_size == RANDOM_MAP_GRID_SIZE)
	    {
	      /* ok.. don't do anything */
	    }
	  else if (grid_size < MIN_MAP_GRID_SIZE)
	    {
	      fprintf (stderr,
		       "map grid too small using: %d\n", MIN_MAP_GRID_SIZE);
	      grid_size = MIN_MAP_GRID_SIZE;
	    }
	  else if (grid_size >= MAX_MAP_GRID_SIZE)
	    {
	      fprintf (stderr,
		       "map grid too large using: %d\n",
		       MAX_MAP_GRID_SIZE - 1);
	      grid_size = MAX_MAP_GRID_SIZE - 1;
	    }
	}
      else if (strcmp (argv[i], "-f") == 0
	       || strcmp (argv[i], "--function") == 0)
	{
	  int f;

	  if (i >= argc - 1)
	    {
	      fprintf (stderr,
		       "fatal error: %s option without any function, need one more arg.\n",
		       argv[i]);
	      exit (EXIT_FAILURE);
	    }
	  i++;
	  for (f = 0; f < MAX_FUNC; f++)
	    {
	      if (strcmp (argv[i], func[f].name) == 0)
		break;
	    }
	  if (f >= MAX_FUNC)
	    {
	      fprintf (stderr,
		       "fatal error: can't find function: %s\n", argv[i]);
	      exit (EXIT_FAILURE);
	    }
	  else
	    func_id = f;
	}
      else if (strcmp (argv[i], "-l") == 0 || strcmp (argv[i], "--list") == 0)
	{
	  int f;
	  printf ("Random map generating functions:\n");
	  for (f = 0; f < MAX_FUNC; f++)
	    printf ("%2d)  %s\t%s\n", f, func[f].name, func[f].desc);

	  exit (EXIT_SUCCESS);
	}
      else if (strcmp (argv[i], "--help") == 0)
	{
	  print_version ();
	  print_help ();
	  exit (EXIT_SUCCESS);
	}
      else if (strcmp (argv[i], "--version") == 0)
	{
	  print_version ();
	  exit (EXIT_SUCCESS);
	}
      else
	{
	  fprintf (stderr, "error: unknown arg: %s\n", argv[i]);
	  fprintf (stderr, "ignoring...\n");
	}
    }

  return;
}

/*****************************************************************************/

void
print_help ()
{
  int i;

  printf ("\n");
  printf
    ("-o    --out       <filename>        Save bitmap to <filename>.    [%s]\n",
     default_filename);
  printf
    ("-s    --size      <%d-%d>             Bitmap size.                  [%d]\n",
     0, MAX_MAP_SIZE - 1, DEFAULT_MAP_SIZE);
  printf
    ("-g    --grid      <%d-%d>             Map Grid Size                 [0]\n",
     0, MAX_MAP_GRID_SIZE - 1);
  printf ("\n");
  printf
    ("-f    --function  <function_name>   Which function to use.        [rand_func]\n");
  printf ("-l    --list                        List all functions.\n");
  printf ("\n");
  printf ("      --help                        Print this help.\n");
  printf ("      --version                     Print Version.\n");

  printf ("\nMap Sizes (WxH):\n");
  for (i = MIN_MAP_SIZE; i < MAX_MAP_SIZE; i++)
    printf ("(%d)%dx%d  ", i, map_size[i][0], map_size[i][1]);
  printf ("\n");

  printf ("\nGrid Sizes (RxC):\n");
  printf ("(0) Random  ");
  for (i = MIN_MAP_GRID_SIZE; i < MAX_MAP_GRID_SIZE; i++)
    {
      if (i > 0 && i % 8 == 0)
	printf ("\n");
      printf ("(%d)%dx%d  ", i, map_grid_size[i][0], map_grid_size[i][1]);
    }
  printf ("\n");

  return;
}

/*****************************************************************************/

void
print_version ()
{
  printf ("Liquid War Random Map Generator Version 0.0.1\n"
	  "Copyright (C) 2003, David Redick, Chris Guirl, Christian Mauduit.\n"
	  "Released under the GNU General Public License (v2).\n");

  return;
}

/*****************************************************************************/
