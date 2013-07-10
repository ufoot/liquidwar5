#include <stdio.h>
#include <stdlib.h>

#include "misc.h"

/*****************************************************************************/
/* generate a random num from min to max inclusive on both ends */

int
rand_num (int min, int max)
{
  /* prevent /0 errors and strange numbers */
  if (min > max)
    {
      fprintf (stderr, "for rand_num(%d, %d), max must be > min.\n", min,
	       max);
      return max;
    }
  else if (min == max)
    return max;

  return rand () % (max - min + 1) + min;
}

/*****************************************************************************/

void *
create_array (int length, int size)
{
  void *array;

  if (length == 0 || size == 0)
    {
      fprintf (stderr, "can't create an array of 0 length. [%d](%d)\n",
	       length, size);
      return NULL;
    }

  array = malloc (length * size);
  if (array == NULL)
    {
      fprintf (stderr, "fatal error: can't malloc space for array.\n");
      exit (EXIT_FAILURE);
    }

  return array;
}

void
delete_array (void *array)
{
  free (array);
  return;
}

/*****************************************************************************/

void **
create_grid (int r, int c, int size)
{
  void **grid;
  int i;

  if (r == 0 || c == 0 || size == 0)
    {
      fprintf (stderr,
	       "can't create grid with 0 dimenson/size [%d][%d](%d).\n", r, c,
	       size);
      return NULL;
    }

  grid = (void **) malloc (r * sizeof (void *));
  if (grid == NULL)
    {
      fprintf (stderr, "fatal error: can't malloc space for grid rows.\n");
      exit (EXIT_FAILURE);
    }

  for (i = 0; i < r; i++)
    grid[i] = (void *) create_array (c, size);

  return grid;
}

void
delete_grid (void **grid, int r)
{
  int i;

  if (r == 0)
    fprintf (stderr, "can't free grid with 0 rows.\n");

  for (i = 0; i < r; i++)
    delete_array (grid[i]);

  free (grid);
  return;
}
