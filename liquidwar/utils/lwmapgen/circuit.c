/*
* A map that looks like a circuit board.
*
* "Wells" are just big circles.
* "Pipes" are just lines that connect wells.
*
* Copyright (C) 2003, David Redick
* Released under the GNU General Public License (v2)
*/

#include <stdio.h>

#include "map.h"
#include "misc.h"

static char **grid;
static int *count;
static int size;

/******************************************************************************/
/*
* flip = 0 for front well connections
* flip = 1 for back well connections
*
* its just makes the map look & play better
*/

static void
draw_pipe (int fromr, int fromc, int tor, int toc, int flip)
{
  int fromx, fromy, tox, toy;


  if (fromc < toc)
    {
      /* up/down then across */
      if (flip == 0)
	{
	  /* go up/down to reach target */
	  section_center (&fromx, &fromy, fromr, fromc);
	  section_center (&tox, &toy, tor, fromc);
	  rectfill (map.map, fromx - size, fromy, tox + size, toy, 255);

	  /* across some num of sections */
	  section_center (&fromx, &fromy, tor, fromc);
	  section_center (&tox, &toy, tor, toc);
	  rectfill (map.map, fromx, fromy - size, tox, toy + size, 255);
	}
      /* across then up/down */
      else
	{
	  /* across some num of sections */
	  section_center (&fromx, &fromy, fromr, fromc);
	  section_center (&tox, &toy, fromr, toc);
	  rectfill (map.map, fromx, fromy - size, tox, toy + size, 255);

	  /* go up/down to reach target */
	  section_center (&fromx, &fromy, fromr, toc);
	  section_center (&tox, &toy, tor, toc);
	  rectfill (map.map, fromx - size, fromy, tox + size, toy, 255);
	}
    }
  else if (fromc > toc)
    {
      /* across then up/down */
      if (flip == 0)
	{
	  /* across some num of sections */
	  section_center (&fromx, &fromy, fromr, fromc);
	  section_center (&tox, &toy, fromr, toc);
	  rectfill (map.map, fromx, fromy - size, tox, toy + size, 255);

	  /* go up/down to reach target */
	  section_center (&fromx, &fromy, fromr, toc);
	  section_center (&tox, &toy, tor, toc);
	  rectfill (map.map, fromx - size, fromy, tox + size, toy, 255);
	}
      /* up/down then across */
      else
	{
	  /* go up/down to reach target */
	  section_center (&fromx, &fromy, fromr, fromc);
	  section_center (&tox, &toy, tor, fromc);
	  rectfill (map.map, fromx - size, fromy, tox + size, toy, 255);

	  /* across some num of sections */
	  section_center (&fromx, &fromy, tor, fromc);
	  section_center (&tox, &toy, tor, toc);
	  rectfill (map.map, fromx, fromy - size, tox, toy + size, 255);
	}
    }
  else
    {
      /* go up/down to reach target */
      section_center (&fromx, &fromy, fromr, fromc);
      section_center (&tox, &toy, tor, toc);
      rectfill (map.map, fromx - size, fromy, tox + size, toy, 255);
    }

  return;
}

/******************************************************************************/
/* connect all wells on in row */

static void
connect_rows ()
{
  int r, c;
  int fromc, toc;

  /* make gcc shutup about these being uninitialized */
  fromc = toc = -1;


  for (r = 0; r < map.num_row; r++)
    {
      /* can't connect if there's not 2 or more... */
      if (count[r] < 2)
	continue;

      /* find first c... */
      for (c = 0; c < map.num_col; c++)
	{
	  if (grid[r][c] == 1)
	    {
	      fromc = c;
	      break;
	    }
	}

      /* find last c... */
      for (c++; c < map.num_col; c++)
	{
	  if (grid[r][c] == 1)
	    toc = c;
	}

      draw_pipe (r, fromc, r, toc, 0);
    }


  return;
}

/******************************************************************************/
/* connect the first well on each row */
/* TODO: I don't think this works 100% */

static void
connect_front ()
{
  int r, c;
  int fromr, fromc, tor, toc;

  /* make gcc shutup about these being uninitialized */
  fromc = tor = toc = -1;


  for (r = 0; r < map.num_row; r++)
    {
      if (count[r] == 0)
	continue;

      fromr = r;
      /* find first well */
      for (c = 0; c < map.num_col; c++)
	{
	  if (grid[fromr][c] == 1)
	    {
	      fromc = c;
	      break;
	    }
	}


      /* tor = -1; */
      /* find next row with a well */
      for (r++; r < map.num_row; r++)
	{
	  if (count[r] > 0)
	    {
	      tor = r;
	      break;
	    }
	}
      /* there might be no other wells after from row */
      if (r == map.num_row)
	break;

      /* find first well on to row */
      for (c = 0; c < map.num_col; c++)
	{
	  if (grid[tor][c] == 1)
	    {
	      toc = c;
	      break;
	    }
	}

      draw_pipe (fromr, fromc, tor, toc, 0);
      printf ("FRONT: %d, %d ---- %d, %d\n", fromr, fromc, tor, toc);
      r = tor - 1;
    }

  return;
}

/******************************************************************************/
/* connect the last well on each row */
/* TODO: I don't think this works 100% */

static void
connect_back ()
{
  int r, c;
  int fromr, fromc, tor, toc;


  /* make gcc shutup about these being uninitialized */
  fromc = tor = toc = -1;


  for (r = 0; r < map.num_row; r++)
    {
      if (count[r] == 0)
	continue;

      fromr = r;
      /* find last well */
      for (c = map.num_col - 1; c >= 0; c--)
	{
	  if (grid[fromr][c] == 1)
	    {
	      fromc = c;
	      break;
	    }
	}

      /* tor = -1; */
      /* find next row with well */
      for (r++; r < map.num_row; r++)
	{
	  if (count[r] > 0)
	    {
	      tor = r;
	      break;
	    }
	}
      /* there might not be another row after from row */
      if (r == map.num_row)
	break;

      /* find last well */
      for (c = map.num_col - 1; c >= 0; c--)
	{
	  if (grid[tor][c] == 1)
	    {
	      toc = c;
	      break;
	    }
	}

      draw_pipe (fromr, fromc, tor, toc, 1);
      printf ("BACK: %d, %d ---- %d, %d\n", fromr, fromc, tor, toc);
      r = tor - 1;
    }

  return;
}

/******************************************************************************/

void
connect_mid ()
{
  int r, c;
  int fromr, fromc, tor, toc;
  int mid, i;


  /* make gcc shutup about these being uninitialized */
  fromc = tor = toc = -1;


  for (r = 0; r < map.num_row; r++)
    {
      printf ("row == %d, count[%d] = %d\n", r, r, count[r]);
      if (count[r] == 0)
	continue;

      fromr = r;

      /* find first middle well */
      mid = count[fromr] / 2;
      i = 0;
      for (c = 0; c < map.num_col; c++)
	{
	  if (grid[fromr][c] == 1)
	    {
	      fromc = c;
	      if (i == mid)
		break;
	      i++;
	    }
	}


      /* tor = -1; */
      /* find to row */
      for (r++; r < map.num_row; r++)
	{
	  if (count[r] > 0)
	    {
	      tor = r;
	      break;
	    }
	}
      /* there might not be another row after from row */
      if (r == map.num_row)
	break;


      /* find first middle well */
      mid = count[tor] / 2;
      i = 0;
      printf ("count[tor]/2 == %d/2 == %d\n", count[tor], mid);
      for (c = 0; c < map.num_col; c++)
	{
	  if (grid[tor][c] == 1)
	    {
	      toc = c;
	      if (i == mid)
		break;
	      i++;
	    }
	}

      draw_pipe (fromr, fromc, tor, toc, 0);
      printf ("pipe= %d, %d --- %d, %d\n", fromr, fromc, tor, toc);
      r = tor - 1;
    }
}

/******************************************************************************/

void
circuit ()
{
  int r, c;
  int do_cut;
  int radius;
  int centerx, centery;


  clear_invert_map ();

  grid = (char **) create_grid (map.num_row, map.num_col, sizeof (char));
  count = (int *) create_array (map.num_row, sizeof (int));

  radius =
    (map.sec_width > map.sec_height ? map.sec_width : map.sec_height) / 4.0;
  radius = (radius == 0 ? 1 : radius);

  size =
    (map.sec_width > map.sec_height ? map.sec_width : map.sec_height) / 12.0;
  size = (size == 0 ? 1 : size);

  for (r = 0; r < map.num_row; r++)
    {
      count[r] = 0;
      for (c = 0; c < map.num_col; c++)
	{
	  if ((r == 0 || r == map.num_row - 1)
	      && (c == 0 || c == map.num_col / 2 || c == map.num_col - 1))
	    do_cut = 1;
	  else
	    do_cut = rand () % 4;

	  if (do_cut != 1)
	    {
	      grid[r][c] = 0;
	      continue;
	    }

	  section_center (&centerx, &centery, r, c);
	  circlefill (map.map, centerx, centery, radius, 255);

	  grid[r][c] = 1;
	  count[r]++;
	}
    }


  connect_rows ();
  connect_front ();
  connect_back ();
  connect_mid ();

  delete_grid ((void **) grid, map.num_row);
  delete_array (count);

  /* redraw outline */
  /* TODO: once in a while if cuts off the edge.. */
  rect (map.map, 0, 0, map.width - 1, map.height - 1, 0);

  return;
}
