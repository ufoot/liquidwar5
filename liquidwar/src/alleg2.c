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
/* nom           : alleg2.c                                         */
/* contenu       : reprogrammation de qq fonctions d'allegro        */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <string.h>

#include "alleg2.h"
#include "exit.h"
#include "macro.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

/* typedef for the listbox callback functions */
typedef char *(*my_getfuncptr) (int, int *);

/*==================================================================*/
/* variables statiques                                              */
/*==================================================================*/
extern void _draw_scrollable_frame (DIALOG * d,
				    int listsize, int offset, int height,
				    int fg_color, int bg);
extern int isspace (int c);

static void my_handle_scrollable_scroll_click (DIALOG * d, int listsize,
					       int *offset, int height);
static void my_handle_scrollable_scroll (DIALOG * d, int listsize, int *index,
					 int *offset);
static void my_handle_listbox_click (DIALOG * d);


/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* utilitaires                                                      */
/*------------------------------------------------------------------*/

/* dotted_rect:
 *  Draws a dotted rectangle, for showing an object has the input focus.
 */
static void
my_dotted_rect (int x1, int y1, int x2, int y2, int fg, int bg)
{
  BITMAP *gui_bmp = gui_get_screen ();
  int x = ((x1 + y1) & 1) ? 1 : 0;
  int c;

  /* two loops to avoid bank switches */
  for (c = x1; c <= x2; c++)
    putpixel (gui_bmp, c, y1, (((c + y1) & 1) == x) ? fg : bg);
  for (c = x1; c <= x2; c++)
    putpixel (gui_bmp, c, y2, (((c + y2) & 1) == x) ? fg : bg);

  for (c = y1 + 1; c < y2; c++)
    {
      putpixel (gui_bmp, x1, c, (((c + x1) & 1) == x) ? fg : bg);
      putpixel (gui_bmp, x2, c, (((c + x2) & 1) == x) ? fg : bg);
    }
}

/* _draw_textbox:
 *  Helper function to draw a textbox object.
 */
static void
my_draw_textbox (char *thetext, int *listsize, int draw, int offset,
		 int wword, int tabsize, int x, int y, int w, int h,
		 int disabled, int fore, int deselect, int disable)
{
  BITMAP *gui_bmp = gui_get_screen ();
  int fg = fore;
  int y1 = y + 4;
  int x1;
  int len;
  int ww = w - 6;
  char s[16];
  char text[16];
  char space[16];
  char *printed = text;
  char *scanned = text;
  char *oldscan = text;
  char *ignore = NULL;
  char *tmp, *ptmp;
  int width;
  int line = 0;
  int i = 0;
  int noignore;

  usetc (s + usetc (s, '.'), 0);
  usetc (text + usetc (text, ' '), 0);
  usetc (space + usetc (space, ' '), 0);

  /* find the correct text */
  if (thetext != NULL)
    {
      printed = thetext;
      scanned = thetext;
    }

  /* do some drawing setup */
  if (draw)
    {
      /* initial start blanking at the top */
      rectfill (gui_bmp, x + 2, y + 2, x + w - 3, y1 - 1, deselect);
    }

  /* choose the text color */
  if (disabled)
    fg = disable;

  /* loop over the entire string */
  while (1)
    {
      width = 0;

      /* find the next break */
      while (ugetc (scanned))
	{
	  /* check for a forced break */
	  if (ugetc (scanned) == '\n')
	    {
	      scanned += uwidth (scanned);

	      /* we are done parsing the line end */
	      break;
	    }

	  /* the next character length */
	  usetc (s + usetc (s, ugetc (scanned)), 0);
	  len = text_length (font, s);

	  /* modify length if its a tab */
	  if (ugetc (s) == '\t')
	    len = tabsize * text_length (font, space);

	  /* check for the end of a line by excess width of next char */
	  if (width + len >= ww)
	    {
	      /* we have reached end of line do we go back to find start */
	      if (wword)
		{
		  /* remember where we were */
		  oldscan = scanned;
		  noignore = FALSE;

		  /* go backwards looking for start of word */
		  while (!uisspace (ugetc (scanned)))
		    {
		      /* don't wrap too far */
		      if (scanned == printed)
			{
			  /* the whole line is filled, so stop here */
			  tmp = ptmp = scanned;
			  while (ptmp != oldscan)
			    {
			      ptmp = tmp;
			      tmp += uwidth (tmp);
			    }
			  scanned = ptmp;
			  noignore = TRUE;
			  break;
			}
		      /* look further backwards to wrap */
		      tmp = ptmp = printed;
		      while (tmp < scanned)
			{
			  ptmp = tmp;
			  tmp += uwidth (tmp);
			}
		      scanned = ptmp;
		    }
		  /* put the space at the end of the line */
		  if (!noignore)
		    {
		      ignore = scanned;
		      scanned += uwidth (scanned);
		    }
		  else
		    ignore = NULL;

		  /* check for endline at the convenient place */
		  if (ugetc (scanned) == '\n')
		    scanned += uwidth (scanned);
		}
	      /* we are done parsing the line end */
	      break;
	    }

	  /* the character can be added */
	  scanned += uwidth (scanned);
	  width += len;
	}

      /* check if we are to print it */
      //if ((draw) && (line >= offset)
      //          && (y1 + text_height (font) < (y + h - 3)))
      if ((draw) && (line >= offset) && (((y1 + text_height (font) < (y + h - 3))) || (line == 0)))	// ufoot
	{
	  x1 = x + 4;

	  /* the initial blank bit */
	  rectfill (gui_bmp, x + 2, y1, x1 - 1, y1 + text_height (font),
		    deselect);

	  /* print up to the marked character */
	  while (printed != scanned)
	    {
	      /* do special stuff for each charater */
	      switch (ugetc (printed))
		{

		case '\r':
		case '\n':
		  /* don't print endlines in the text */
		  break;

		  /* possibly expand the tabs */
		case '\t':
		  for (i = 0; i < tabsize; i++)
		    {
		      usetc (s + usetc (s, ' '), 0);
		      //textout_ex(gui_bmp, font, s, x1, y1, fg, deselect);
		      textout_ex (gui_bmp, font, s, x1, y1, -1, deselect);	// ufoot
		      x1 += text_length (font, s);
		    }
		  break;

		  /* print a normal character */
		default:
		  if (printed != ignore)
		    {
		      usetc (s + usetc (s, ugetc (printed)), 0);
		      //textout_ex(gui_bmp, font, s, x1, y1, fg, deselect);
		      textout_ex (gui_bmp, font, s, x1, y1, -1, deselect);	// ufoot
		      x1 += text_length (font, s);
		    }
		}

	      /* goto the next character */
	      printed += uwidth (printed);
	    }
	  /* the last blank bit */
	  if (x1 <= x + w - 3)
	    rectfill (gui_bmp, x1, y1, x + w - 3, y1 + text_height (font) - 1,
		      deselect);

	  /* print the line end */
	  y1 += text_height (font);
	}
      printed = scanned;

      /* we have done a line */
      line++;

      /* check if we are at the end of the string */
      if (!ugetc (printed))
	{
	  /* the under blank bit */
	  if (draw)
	    rectfill (gui_bmp, x + 1, y1, x + w - 3, y + h - 1, deselect);

	  /* tell how many lines we found */
	  *listsize = line;
	  return;
	}
    }

}

/* d_textbox_proc:
 *  A text box object. The dp field points to a char * which is the text
 *  to be displayed in the text box. If the text is long, there will be
 *  a vertical scrollbar on the right hand side of the object which can
 *  be used to scroll through the text. The default is to print the text
 *  with word wrapping, but if the D_SELECTED flag is set, the text will
 *  be printed with character wrapping. The d1 field is used internally
 *  to store the number of lines of text, and d2 is used to store how far
 *  it has scrolled through the text.
 */
int
my_textbox_proc (int msg, DIALOG * d, int c)
{
  int height, bar, ret = D_O_K;
  int start, top, bottom, l;
  int used, delta;
  int fg_color;
  ASSERT (d);

  fg_color = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;
  /* calculate the actual height */
  height = (d->h - 8) / text_height (font);

  switch (msg)
    {

    case MSG_START:
      /* measure how many lines of text we contain */
      my_draw_textbox (d->dp, &d->d1, 0,	/* DONT DRAW anything */
		       d->d2, !(d->flags & D_SELECTED), 8,
		       d->x, d->y, d->w, d->h,
		       (d->flags & D_DISABLED), 0, 0, 0);
      break;

    case MSG_DRAW:
      /* tell the object to sort of draw, but only calculate the listsize */
      my_draw_textbox (d->dp, &d->d1, 0,	/* DONT DRAW anything */
		       d->d2, !(d->flags & D_SELECTED), 8,
		       d->x, d->y, d->w, d->h,
		       (d->flags & D_DISABLED), 0, 0, 0);

      if (d->d1 > height && d->d1 > 1)	// ufoot
	{
	  bar = 12;
	}
      else
	{
	  bar = 0;
	  d->d2 = 0;
	}

      if (d->d1 <= 1)
	{
	  rectfill (screen, d->x, d->y, d->x + d->w, d->y + d->h, d->bg);	// ufoot
	}

      /* now do the actual drawing */
      my_draw_textbox (d->dp, &d->d1, 1, d->d2,
		       !(d->flags & D_SELECTED), 8,
		       d->x, d->y, d->w - bar, d->h,
		       (d->flags & D_DISABLED),
		       fg_color, d->bg, gui_mg_color);

      if (d->d1 <= 1)
	{
	  /*
	   * If there's only one line, skip the scroll bar. Patched
	   * for LW by ufoot, to handle 320x240 in a nicer way.
	   */
	  rect (screen, d->x, d->y, d->x + d->w, d->y + d->h, fg_color);	// ufoot
	}
      else
	{
	  /* draw the frame around */
	  _draw_scrollable_frame (d, d->d1, d->d2, height, fg_color, d->bg);
	}
      break;

    case MSG_CLICK:
      /* figure out if it's on the text or the scrollbar */
      bar = (d->d1 > height);

      if ((!bar) || (gui_mouse_x () < d->x + d->w - 13))
	{
	  /* clicked on the text area */
	  ret = D_O_K;
	}
      else
	{
	  /* clicked on the scroll area */
	  my_handle_scrollable_scroll_click (d, d->d1, &d->d2, height);
	}
      break;

    case MSG_CHAR:
      start = d->d2;
      used = D_USED_CHAR;

      if (d->d1 > 0)
	{
	  if (d->d2 > 0)
	    top = d->d2 + 1;
	  else
	    top = 0;

	  l = (d->h - 8) / text_height (font);

	  bottom = d->d2 + l - 1;
	  if (bottom >= d->d1 - 1)
	    bottom = d->d1 - 1;
	  else
	    bottom--;

	  if ((c >> 8) == KEY_UP)
	    d->d2--;
	  else if ((c >> 8) == KEY_DOWN)
	    d->d2++;
	  else if ((c >> 8) == KEY_HOME)
	    d->d2 = 0;
	  else if ((c >> 8) == KEY_END)
	    d->d2 = d->d1 - l;
	  else if ((c >> 8) == KEY_PGUP)
	    d->d2 -= (bottom - top) ? bottom - top : 1;
	  else if ((c >> 8) == KEY_PGDN)
	    d->d2 += (bottom - top) ? bottom - top : 1;
	  else
	    used = D_O_K;

	  /* make sure that the list stays in bounds */
	  if (d->d2 > d->d1 - l)
	    d->d2 = d->d1 - l;
	  if (d->d2 < 0)
	    d->d2 = 0;
	}
      else
	used = D_O_K;

      /* if we changed something, better redraw... */
      if (d->d2 != start)
	d->flags |= D_DIRTY;

      ret = used;
      break;

    case MSG_WHEEL:
      l = (d->h - 8) / text_height (font);
      delta = (l > 3) ? 3 : 1;

      /* scroll, making sure that the list stays in bounds */
      start = d->d2;
      d->d2 =
	(c > 0) ? MAX (0, d->d2 - delta) : MIN (d->d1 - l, d->d2 + delta);

      /* if we changed something, better redraw... */
      if (d->d2 != start)
	d->flags |= D_DIRTY;

      ret = D_O_K;
      break;

    case MSG_WANTFOCUS:
      /* if we don't have a scrollbar we can't do anything with the focus */
      if (d->d1 > height)
	ret = D_WANTFOCUS;
      break;

    default:
      ret = D_O_K;
    }

  return ret;
}

/* d_button_proc:
 *  A button object (the dp field points to the text string). This object
 *  can be selected by clicking on it with the mouse or by pressing its 
 *  keyboard shortcut. If the D_EXIT flag is set, selecting it will close 
 *  the dialog, otherwise it will toggle on and off.
 */
int
my_button_proc (int msg, DIALOG * d, int c)
{
  BITMAP *gui_bmp;
  int state1, state2;
  int black;
  int swap;
  int g;
  ASSERT (d);

  LW_MACRO_NOP (c);		// ufoot

  gui_bmp = gui_get_screen ();

  switch (msg)
    {

    case MSG_DRAW:
      if (d->flags & D_SELECTED)
	{
	  g = 1;
	  state1 = d->bg;
	  state2 = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;
	}
      else
	{
	  g = 0;
	  state1 = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;
	  state2 = d->bg;
	}

      rectfill (gui_bmp, d->x + 1 + g, d->y + 1 + g, d->x + d->w - 3 + g,
		d->y + d->h - 3 + g, state2);
      rect (gui_bmp, d->x + g, d->y + g, d->x + d->w - 2 + g,
	    d->y + d->h - 2 + g, state1);
      //gui_textout_ex(gui_bmp, d->dp, d->x+d->w/2+g, d->y+d->h/2-text_height(font)/2+g, state1, -1, TRUE);
      gui_textout_ex (gui_bmp, d->dp, d->x + d->w / 2 + g, d->y + d->h / 2 - text_height (font) / 2 + g, -1, -1, TRUE);	// ufoot

      if (d->flags & D_SELECTED)
	{
	  vline (gui_bmp, d->x, d->y, d->y + d->h - 2, d->bg);
	  hline (gui_bmp, d->x, d->y, d->x + d->w - 2, d->bg);
	}
      else
	{
	  black = makecol (0, 0, 0);
	  vline (gui_bmp, d->x + d->w - 1, d->y + 1, d->y + d->h - 2, black);
	  hline (gui_bmp, d->x + 1, d->y + d->h - 1, d->x + d->w - 1, black);
	}
      if ((d->flags & D_GOTFOCUS) &&
	  (!(d->flags & D_SELECTED) || !(d->flags & D_EXIT)))
	my_dotted_rect (d->x + 1 + g, d->y + 1 + g, d->x + d->w - 3 + g,
			d->y + d->h - 3 + g, state1, state2);
      break;

    case MSG_WANTFOCUS:
      return D_WANTFOCUS;

    case MSG_KEY:
      /* close dialog? */
      if (d->flags & D_EXIT)
	{
	  return D_CLOSE;
	}

      /* or just toggle */
      d->flags ^= D_SELECTED;
      object_message (d, MSG_DRAW, 0);
      break;

    case MSG_CLICK:
      /* what state was the button originally in? */
      state1 = d->flags & D_SELECTED;
      if (d->flags & D_EXIT)
	swap = FALSE;
      else
	swap = state1;

      /* track the mouse until it is released */
      while (gui_mouse_b ())
	{
	  state2 = ((gui_mouse_x () >= d->x) && (gui_mouse_y () >= d->y) &&
		    (gui_mouse_x () < d->x + d->w)
		    && (gui_mouse_y () < d->y + d->h));
	  if (swap)
	    state2 = !state2;

	  /* redraw? */
	  if (((state1) && (!state2)) || ((state2) && (!state1)))
	    {
	      d->flags ^= D_SELECTED;
	      state1 = d->flags & D_SELECTED;
	      object_message (d, MSG_DRAW, 0);
	    }

	  /* let other objects continue to animate */
	  broadcast_dialog_message (MSG_IDLE, 0);
	}

      /* should we close the dialog? */
      if ((d->flags & D_SELECTED) && (d->flags & D_EXIT))
	{
	  d->flags ^= D_SELECTED;
	  return D_CLOSE;
	}
      break;
    }

  return D_O_K;
}


/* d_text_proc:
 *  Simple dialog procedure: draws the text string which is pointed to by dp.
 */
int
my_text_proc (int msg, DIALOG * d, int c)
{
  LW_MACRO_NOP (c);		// ufoot

  ASSERT (d);
  if (msg == MSG_DRAW)
    {
      //int fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg; // ufoot
      FONT *oldfont = font;

      if (d->dp2)
	font = d->dp2;

      //gui_textout_ex(gui_get_screen(), d->dp, d->x, d->y, fg, d->bg, FALSE);
      gui_textout_ex (gui_get_screen (), d->dp, d->x, d->y, -1, d->bg, FALSE);	// ufoot

      font = oldfont;
    }

  return D_O_K;
}

/* d_ctext_proc:
 *  Simple dialog procedure: draws the text string which is pointed to by dp,
 *  centering it around the object's x coordinate.
 */
int
my_ctext_proc (int msg, DIALOG * d, int c)
{
  LW_MACRO_NOP (c);		// ufoot

  ASSERT (d);
  if (msg == MSG_DRAW)
    {
      // int fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg; // ufoot
      FONT *oldfont = font;

      if (d->dp2)
	font = d->dp2;

      //gui_textout_ex(gui_get_screen(), d->dp, d->x + d->w/2, d->y, fg, d->bg, TRUE);
      gui_textout_ex (gui_get_screen (), d->dp, d->x + d->w / 2, d->y, -1, d->bg, TRUE);	// ufoot

      font = oldfont;
    }

  return D_O_K;
}


/* d_slider_proc:
 *  A slider control object. This object returns a value in d2, in the
 *  range from 0 to d1. It will display as a vertical slider if h is
 *  greater than or equal to w; otherwise, it will display as a horizontal
 *  slider. dp can contain an optional bitmap to use for the slider handle; 
 *  dp2 can contain an optional callback function, which is called each 
 *  time d2 changes. The callback function should have the following
 *  prototype:
 *
 *  int function(void *dp3, int d2);
 *
 *  The d_slider_proc object will return the value of the callback function.
 */
int
my_slider_proc (int msg, DIALOG * d, int c)
{
  BITMAP *gui_bmp = gui_get_screen ();
  BITMAP *slhan = NULL;
  int oldpos, newpos;
  int sfg;			/* slider foreground color */
  int vert = TRUE;		/* flag: is slider vertical? */
  int hh = 7;			/* handle height (width for horizontal sliders) */
  int hmar;			/* handle margin */
  int slp;			/* slider position */
  int mp;			/* mouse position */
  int irange;
  int slx, sly, slh, slw;
  int msx, msy;
  int retval = D_O_K;
  int upkey, downkey;
  int pgupkey, pgdnkey;
  int homekey, endkey;
  int delta;
  fixed slratio, slmax, slpos;
  int (*proc) (void *cbpointer, int d2value);
  int oldval;
  ASSERT (d);

  /* check for slider direction */
  if (d->h < d->w)
    vert = FALSE;

  /* set up the metrics for the control */
  if (d->dp != NULL)
    {
      slhan = (BITMAP *) d->dp;
      if (vert)
	hh = slhan->h;
      else
	hh = slhan->w;
    }

  hmar = hh / 2;
  irange = (vert) ? d->h : d->w;
  slmax = itofix (irange - hh);
  slratio = slmax / (d->d1);
  slpos = slratio * d->d2;
  slp = fixtoi (slpos);

  switch (msg)
    {

    case MSG_DRAW:
      sfg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;

      if (vert)
	{
	  rectfill (gui_bmp, d->x, d->y, d->x + d->w / 2 - 2, d->y + d->h - 1,
		    d->bg);
	  rectfill (gui_bmp, d->x + d->w / 2 - 1, d->y, d->x + d->w / 2 + 1,
		    d->y + d->h - 1, sfg);
	  rectfill (gui_bmp, d->x + d->w / 2 + 2, d->y, d->x + d->w - 1,
		    d->y + d->h - 1, d->bg);
	}
      else
	{
	  rectfill (gui_bmp, d->x, d->y, d->x + d->w - 1, d->y + d->h / 2 - 2,
		    d->bg);
	  rectfill (gui_bmp, d->x, d->y + d->h / 2 - 1, d->x + d->w - 1,
		    d->y + d->h / 2 + 1, sfg);
	  rectfill (gui_bmp, d->x, d->y + d->h / 2 + 2, d->x + d->w - 1,
		    d->y + d->h - 1, d->bg);
	}

      /*
       * Following line added to draw a box arround the slider
       */
      rect (screen, d->x, d->y, d->x + d->w, d->y + d->h, sfg);	// ufoot

      /* okay, background and slot are drawn, now draw the handle */
      if (slhan)
	{
	  if (vert)
	    {
	      slx = d->x + (d->w / 2) - (slhan->w / 2);
	      sly = d->y + (d->h - 1) - (hh + slp);
	    }
	  else
	    {
	      slx = d->x + slp;
	      sly = d->y + (d->h / 2) - (slhan->h / 2);
	    }
	  draw_sprite (gui_bmp, slhan, slx, sly);
	}
      else
	{
	  /* draw default handle */
	  if (vert)
	    {
	      slx = d->x;
	      sly = d->y + (d->h) - (hh + slp);
	      slw = d->w - 1;
	      slh = hh - 1;
	    }
	  else
	    {
	      slx = d->x + slp;
	      sly = d->y;
	      slw = hh - 1;
	      slh = d->h - 1;
	    }

	  /* draw body */
	  rectfill (gui_bmp, slx + 2, sly, slx + (slw - 2), sly + slh, sfg);
	  vline (gui_bmp, slx + 1, sly + 1, sly + slh - 1, sfg);
	  vline (gui_bmp, slx + slw - 1, sly + 1, sly + slh - 1, sfg);
	  vline (gui_bmp, slx, sly + 2, sly + slh - 2, sfg);
	  vline (gui_bmp, slx + slw, sly + 2, sly + slh - 2, sfg);
	  vline (gui_bmp, slx + 1, sly + 2, sly + slh - 2, d->bg);
	  hline (gui_bmp, slx + 2, sly + 1, slx + slw - 2, d->bg);
	  putpixel (gui_bmp, slx + 2, sly + 2, d->bg);
	}

      if (d->flags & D_GOTFOCUS)
	// my_dotted_rect(d->x, d->y, d->x+d->w-1, d->y+d->h-1, sfg, d->bg);
	my_dotted_rect (d->x + 1, d->y + 1, d->x + d->w - 1, d->y + d->h - 1, sfg, d->bg);	// ufoot
      break;

    case MSG_WANTFOCUS:
    case MSG_LOSTFOCUS:
      return D_WANTFOCUS;

    case MSG_KEY:
      if (!(d->flags & D_GOTFOCUS))
	return D_WANTFOCUS;
      else
	return D_O_K;

    case MSG_CHAR:
      /* handle movement keys to move slider */
      c >>= 8;

      if (vert)
	{
	  upkey = KEY_UP;
	  downkey = KEY_DOWN;
	  pgupkey = KEY_PGUP;
	  pgdnkey = KEY_PGDN;
	  homekey = KEY_END;
	  endkey = KEY_HOME;
	}
      else
	{
	  upkey = KEY_RIGHT;
	  downkey = KEY_LEFT;
	  pgupkey = KEY_PGDN;
	  pgdnkey = KEY_PGUP;
	  homekey = KEY_HOME;
	  endkey = KEY_END;
	}

      if (c == upkey)
	delta = 1;
      else if (c == downkey)
	delta = -1;
      else if (c == pgdnkey)
	delta = -d->d1 / 16;
      else if (c == pgupkey)
	delta = d->d1 / 16;
      else if (c == homekey)
	delta = -d->d2;
      else if (c == endkey)
	delta = d->d1 - d->d2;
      else
	delta = 0;

      if (delta)
	{
	  oldpos = slp;
	  oldval = d->d2;

	  while (1)
	    {
	      d->d2 = d->d2 + delta;
	      slpos = slratio * d->d2;
	      slp = fixtoi (slpos);
	      if ((slp != oldpos) || (d->d2 <= 0) || (d->d2 >= d->d1))
		break;
	    }

	  if (d->d2 < 0)
	    d->d2 = 0;
	  if (d->d2 > d->d1)
	    d->d2 = d->d1;

	  retval = D_USED_CHAR;

	  if (d->d2 != oldval)
	    {
	      /* call callback function here */
	      if (d->dp2)
		{
		  proc = d->dp2;
		  retval |= (*proc) (d->dp3, d->d2);
		}

	      object_message (d, MSG_DRAW, 0);
	    }
	}
      break;

    case MSG_WHEEL:
      oldval = d->d2;
      d->d2 = MID (0, d->d2 + c, d->d1);
      if (d->d2 != oldval)
	{
	  /* call callback function here */
	  if (d->dp2)
	    {
	      proc = d->dp2;
	      retval |= (*proc) (d->dp3, d->d2);
	    }

	  object_message (d, MSG_DRAW, 0);
	}
      break;

    case MSG_CLICK:
      /* track the mouse until it is released */
      mp = slp;

      while (gui_mouse_b ())
	{
	  msx = gui_mouse_x ();
	  msy = gui_mouse_y ();
	  oldval = d->d2;
	  if (vert)
	    mp = (d->y + d->h - hmar) - msy;
	  else
	    mp = msx - (d->x + hmar);
	  if (mp < 0)
	    mp = 0;
	  if (mp > irange - hh)
	    mp = irange - hh;
	  slpos = itofix (mp);
	  slmax = fixdiv (slpos, slratio);
	  newpos = fixtoi (slmax);
	  if (newpos != oldval)
	    {
	      d->d2 = newpos;

	      /* call callback function here */
	      if (d->dp2 != NULL)
		{
		  proc = d->dp2;
		  retval |= (*proc) (d->dp3, d->d2);
		}

	      object_message (d, MSG_DRAW, 0);
	    }

	  /* let other objects continue to animate */
	  broadcast_dialog_message (MSG_IDLE, 0);
	}
      break;
    }

  return retval;
}


/* d_edit_proc:
 *  An editable text object (the dp field points to the string). When it
 *  has the input focus (obtained by clicking on it with the mouse), text
 *  can be typed into this object. The d1 field specifies the maximum
 *  number of characters that it will accept, and d2 is the text cursor 
 *  position within the string.
 */
int
my_edit_proc (int msg, DIALOG * d, int c)
{
  static int ignore_next_uchar = FALSE;
  BITMAP *gui_bmp;
  int last_was_space, new_pos, i, k;
  int f, l, p, w, x, fg, b, scroll;
  char buf[16];
  char *s, *t;
  ASSERT (d);

  gui_bmp = gui_get_screen ();

  s = d->dp;
  l = ustrlen (s);
  if (d->d2 > l)
    d->d2 = l;

  /* calculate maximal number of displayable characters */
  if (d->d2 == l)
    {
      usetc (buf + usetc (buf, ' '), 0);
      x = text_length (font, buf);
    }
  else
    x = 0;

  b = 0;

  for (p = d->d2; p >= 0; p--)
    {
      usetc (buf + usetc (buf, ugetat (s, p)), 0);
      x += text_length (font, buf);
      b++;
      if (x > d->w)
	break;
    }

  if (x <= d->w)
    {
      b = l;
      scroll = FALSE;
    }
  else
    {
      b--;
      scroll = TRUE;
    }

  switch (msg)
    {

    case MSG_START:
      d->d2 = l;
      break;

    case MSG_DRAW:
      fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;
      x = 0;

      if (scroll)
	{
	  p = d->d2 - b + 1;
	  b = d->d2;
	}
      else
	p = 0;

      for (; p <= b; p++)
	{
	  f = ugetat (s, p);
	  usetc (buf + usetc (buf, (f) ? f : ' '), 0);
	  w = text_length (font, buf);
	  if (x + w > d->w)
	    break;
	  f = ((p == d->d2) && (d->flags & D_GOTFOCUS));
	  //textout_ex(gui_bmp, font, buf, d->x+x, d->y, (f) ? d->bg : fg, (f) ? fg : d->bg);
	  textout_ex (gui_bmp, font, buf, d->x + x, d->y, -1, (f) ? d->fg : d->bg);	// ufoot
	  x += w;
	}
      if (x < d->w)
	rectfill (gui_bmp, d->x + x, d->y, d->x + d->w - 1,
		  d->y + text_height (font) - 1, d->bg);
      break;

    case MSG_CLICK:
      x = d->x;

      if (scroll)
	{
	  p = d->d2 - b + 1;
	  b = d->d2;
	}
      else
	p = 0;

      for (; p < b; p++)
	{
	  usetc (buf + usetc (buf, ugetat (s, p)), 0);
	  x += text_length (font, buf);
	  if (x > gui_mouse_x ())
	    break;
	}
      d->d2 = MID (0, p, l);
      object_message (d, MSG_DRAW, 0);
      break;

    case MSG_WANTFOCUS:
    case MSG_LOSTFOCUS:
    case MSG_KEY:
      return D_WANTFOCUS;

    case MSG_CHAR:
      ignore_next_uchar = FALSE;

      if ((c >> 8) == KEY_LEFT)
	{
	  if (d->d2 > 0)
	    {
	      if (key_shifts & KB_CTRL_FLAG)
		{
		  last_was_space = TRUE;
		  new_pos = 0;
		  t = s;
		  for (i = 0; i < d->d2; i++)
		    {
		      k = ugetx (&t);
		      if (uisspace (k))
			last_was_space = TRUE;
		      else if (last_was_space)
			{
			  last_was_space = FALSE;
			  new_pos = i;
			}
		    }
		  d->d2 = new_pos;
		}
	      else
		d->d2--;
	    }
	}
      else if ((c >> 8) == KEY_RIGHT)
	{
	  if (d->d2 < l)
	    {
	      if (key_shifts & KB_CTRL_FLAG)
		{
		  t = s + uoffset (s, d->d2);
		  for (k = ugetx (&t); uisspace (k); k = ugetx (&t))
		    d->d2++;
		  for (; k && !uisspace (k); k = ugetx (&t))
		    d->d2++;
		}
	      else
		d->d2++;
	    }
	}
      else if ((c >> 8) == KEY_HOME)
	{
	  d->d2 = 0;
	}
      else if ((c >> 8) == KEY_END)
	{
	  d->d2 = l;
	}
      else if ((c >> 8) == KEY_DEL)
	{
	  if (d->d2 < l)
	    uremove (s, d->d2);
	}
      else if ((c >> 8) == KEY_BACKSPACE)
	{
	  if (d->d2 > 0)
	    {
	      d->d2--;
	      uremove (s, d->d2);
	    }
	}
      else if ((c >> 8) == KEY_ENTER)
	{
	  if (d->flags & D_EXIT)
	    {
	      object_message (d, MSG_DRAW, 0);
	      return D_CLOSE;
	    }
	  else
	    return D_O_K;
	}
      else if ((c >> 8) == KEY_TAB)
	{
	  ignore_next_uchar = TRUE;
	  return D_O_K;
	}
      else
	{
	  /* don't process regular keys here: MSG_UCHAR will do that */
	  break;
	}
      object_message (d, MSG_DRAW, 0);
      return D_USED_CHAR;

    case MSG_UCHAR:
      // if ((c >= ' ') && (uisok (c)) && (!ignore_next_uchar))
      if ((c >= ' ') && (c < 128) && (uisok (c)) && (!ignore_next_uchar))	// ufoot
	{
	  if (l < d->d1)
	    {
	      uinsert (s, d->d2, c);
	      d->d2++;

	      object_message (d, MSG_DRAW, 0);
	    }
	  return D_USED_CHAR;
	}
      break;
    }

  return D_O_K;
}



/* _handle_scrollable_click:
 *  Helper to process a click on a scrollable object.
 */
static void
my_handle_scrollable_scroll_click (DIALOG * d, int listsize, int *offset,
				   int height)
{
  int xx, yy;
  int hh = d->h - 5;

  while (gui_mouse_b ())
    {
      int i = (hh * height + listsize / 2) / listsize;
      int len = (hh * (*offset) + listsize / 2) / listsize + 2;

      if ((gui_mouse_y () >= d->y + len)
	  && (gui_mouse_y () <= d->y + len + i))
	{
	  xx = gui_mouse_y () - len + 2;
	  while (gui_mouse_b ())
	    {
	      yy = (listsize * (gui_mouse_y () - xx) + hh / 2) / hh;
	      if (yy > listsize - height)
		yy = listsize - height;

	      if (yy < 0)
		yy = 0;

	      if (yy != *offset)
		{
		  *offset = yy;
		  object_message (d, MSG_DRAW, 0);
		}

	      /* let other objects continue to animate */
	      broadcast_dialog_message (MSG_IDLE, 0);
	    }
	}
      else
	{
	  if (gui_mouse_y () <= d->y + len)
	    yy = *offset - height;
	  else
	    yy = *offset + height;

	  if (yy > listsize - height)
	    yy = listsize - height;

	  if (yy < 0)
	    yy = 0;

	  if (yy != *offset)
	    {
	      *offset = yy;
	      object_message (d, MSG_DRAW, 0);
	    }
	}

      /* let other objects continue to animate */
      broadcast_dialog_message (MSG_IDLE, 0);
    }
}


/* _handle_scrollable_scroll:
 *  Helper function to scroll through a scrollable object.
 */
static void
my_handle_scrollable_scroll (DIALOG * d, int listsize, int *index,
			     int *offset)
{
  int height = (d->h - 4) / text_height (font);

  if (listsize <= 0)
    {
      *index = *offset = 0;
      return;
    }

  /* check selected item */
  if (*index < 0)
    *index = 0;
  else if (*index >= listsize)
    *index = listsize - 1;

  /* check scroll position */
  while ((*offset > 0) && (*offset + height > listsize))
    (*offset)--;

  if (*offset >= *index)
    {
      if (*index < 0)
	*offset = 0;
      else
	*offset = *index;
    }
  else
    {
      while ((*offset + height - 1) < *index)
	(*offset)++;
    }
}



/* idle_cb:
 *  rest_callback() routine to keep dialogs animating nice and smoothly.
 */
static void
my_idle_cb (void)
{
  broadcast_dialog_message (MSG_IDLE, 0);
}


/* _handle_listbox_click:
 *  Helper to process a click on a listbox, doing hit-testing and moving
 *  the selection.
 */
static void
my_handle_listbox_click (DIALOG * d)
{
  char *sel = d->dp2;
  int listsize, height;
  int i, j;

  (*(my_getfuncptr) d->dp) (-1, &listsize);
  if (!listsize)
    return;

  height = (d->h - 4) / text_height (font);

  i = MID (0, ((gui_mouse_y () - d->y - 2) / text_height (font)),
	   ((d->h - 4) / text_height (font) - 1));
  i += d->d2;
  if (i < d->d2)
    i = d->d2;
  else
    {
      if (i > d->d2 + height - 1)
	i = d->d2 + height - 1;
      if (i >= listsize)
	i = listsize - 1;
    }

  if (gui_mouse_y () <= d->y)
    i = MAX (i - 1, 0);
  else if (gui_mouse_y () >= d->y + d->h - 1)
    i = MIN (i + 1, listsize - 1);

  if (i != d->d1)
    {
      if (sel)
	{
	  if (key_shifts & (KB_SHIFT_FLAG | KB_CTRL_FLAG))
	    {
	      if ((key_shifts & KB_SHIFT_FLAG) || (d->flags & D_INTERNAL))
		{
		  for (j = MIN (i, d->d1); j <= MAX (i, d->d1); j++)
		    sel[j] = TRUE;
		}
	      else
		sel[i] = !sel[i];
	    }
	  else
	    sel[i] = TRUE;
	}

      d->d1 = i;
      i = d->d2;
      my_handle_scrollable_scroll (d, listsize, &d->d1, &d->d2);

      d->flags |= D_DIRTY;

      if (i != d->d2)
	rest_callback (MID (10, text_height (font) * 16 - d->h - 1, 100),
		       my_idle_cb);
    }
  else
    {
      if (!(d->flags & D_INTERNAL))
	{
	  if (sel)
	    {
	      if ((key_shifts & KB_CTRL_FLAG))
		sel[i] = !sel[i];
	      else
		sel[i] = TRUE;

	      d->flags |= D_DIRTY;
	    }
	}
    }
}



/* draw_listbox:
 *  Helper function to draw a listbox object.
 */
static void
my_draw_listbox (DIALOG * d)
{
  BITMAP *gui_bmp = gui_get_screen ();
  int height, listsize, i, len, bar, x, y, w;
  int fg_color, fg, bg;
  char *sel = d->dp2;
  char s[1024];

  (*(my_getfuncptr) d->dp) (-1, &listsize);
  height = (d->h - 4) / text_height (font);
  bar = (listsize > height);
  w = (bar ? d->w - 15 : d->w - 3);
  fg_color = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;

  /* draw box contents */
  for (i = 0; i < height; i++)
    {
      if (d->d2 + i < listsize)
	{
	  if (sel)
	    {
	      if ((sel[d->d2 + i]) && (d->d2 + i == d->d1))
		{
		  fg = d->bg;
		  bg = fg_color;
		}
	      else if (sel[d->d2 + i])
		{
		  fg = d->bg;
		  bg = gui_mg_color;
		}
	      else
		{
		  fg = fg_color;
		  bg = d->bg;
		}
	    }
	  else if (d->d2 + i == d->d1)
	    {
	      fg = d->bg;
	      bg = fg_color;
	    }
	  else
	    {
	      fg = fg_color;
	      bg = d->bg;
	    }
	  ustrzcpy (s, sizeof (s),
		    (*(my_getfuncptr) d->dp) (i + d->d2, NULL));
	  x = d->x + 2;
	  y = d->y + 2 + i * text_height (font);
	  rectfill (gui_bmp, x, y, x + 7, y + text_height (font) - 1, bg);
	  x += 8;
	  len = ustrlen (s);
	  while (text_length (font, s) >= MAX (d->w - 1 - (bar ? 22 : 10), 1))
	    {
	      len--;
	      usetat (s, len, 0);
	    }
	  //textout_ex(gui_bmp, font, s, x, y, fg, bg);
	  textout_ex (gui_bmp, font, s, x, y, -1, bg);	// ufoot
	  x += text_length (font, s);
	  if (x <= d->x + w)
	    rectfill (gui_bmp, x, y, d->x + w, y + text_height (font) - 1,
		      bg);
	  if (d->d2 + i == d->d1)
	    my_dotted_rect (d->x + 1, y, d->x + d->w - (bar ? 12 : 0),
			    y + text_height (font) - 1, d->fg, d->bg);
	}
      else
	{
	  rectfill (gui_bmp, d->x + 2, d->y + 2 + i * text_height (font),
		    d->x + w, d->y + 1 + (i + 1) * text_height (font), d->bg);
	}
    }

  if (d->y + 2 + i * text_height (font) <= d->y + d->h - 3)
    rectfill (gui_bmp, d->x + 2, d->y + 2 + i * text_height (font),
	      d->x + w, d->y + d->h - 3, d->bg);

  /* draw frame, maybe with scrollbar */
  _draw_scrollable_frame (d, listsize, d->d2, height, fg_color, d->bg);
}


/* d_list_proc:
 *  A list box object. The dp field points to a function which it will call
 *  to obtain information about the list. This should follow the form:
 *     const char *<list_func_name> (int index, int *list_size);
 *  If index is zero or positive, the function should return a pointer to
 *  the string which is to be displayed at position index in the list. If
 *  index is  negative, it should return null and list_size should be set
 *  to the number of items in the list. The list box object will allow the
 *  user to scroll through the list and to select items list by clicking
 *  on them, and if it has the input focus also by using the arrow keys. If 
 *  the D_EXIT flag is set, double clicking on a list item will cause it to 
 *  close the dialog. The index of the selected item is held in the d1 
 *  field, and d2 is used to store how far it has scrolled through the list.
 */
int
my_list_proc (int msg, DIALOG * d, int c)
{
  int listsize, i, bottom, height, bar, orig;
  char *sel = d->dp2;
  int redraw = FALSE;
  ASSERT (d);

  switch (msg)
    {

    case MSG_START:
      (*(my_getfuncptr) d->dp) (-1, &listsize);
      my_handle_scrollable_scroll (d, listsize, &d->d1, &d->d2);
      break;

    case MSG_DRAW:
      my_draw_listbox (d);
      break;

    case MSG_CLICK:
      (*(my_getfuncptr) d->dp) (-1, &listsize);
      height = (d->h - 4) / text_height (font);
      bar = (listsize > height);
      if ((!bar) || (gui_mouse_x () < d->x + d->w - 13))
	{
	  if ((sel) && (!(key_shifts & KB_CTRL_FLAG)))
	    {
	      for (i = 0; i < listsize; i++)
		{
		  if (sel[i])
		    {
		      redraw = TRUE;
		      sel[i] = FALSE;
		    }
		}
	      if (redraw)
		object_message (d, MSG_DRAW, 0);
	    }
	  my_handle_listbox_click (d);
	  while (gui_mouse_b ())
	    {
	      broadcast_dialog_message (MSG_IDLE, 0);
	      d->flags |= D_INTERNAL;
	      my_handle_listbox_click (d);
	      d->flags &= ~D_INTERNAL;
	    }
	}
      else
	{
	  my_handle_scrollable_scroll_click (d, listsize, &d->d2, height);
	}
      break;

    case MSG_DCLICK:
      (*(my_getfuncptr) d->dp) (-1, &listsize);
      height = (d->h - 4) / text_height (font);
      bar = (listsize > height);
      if ((!bar) || (gui_mouse_x () < d->x + d->w - 13))
	{
	  if (d->flags & D_EXIT)
	    {
	      if (listsize)
		{
		  i = d->d1;
		  object_message (d, MSG_CLICK, 0);
		  if (i == d->d1)
		    return D_CLOSE;
		}
	    }
	}
      break;

    case MSG_WHEEL:
      (*(my_getfuncptr) d->dp) (-1, &listsize);
      height = (d->h - 4) / text_height (font);
      if (height < listsize)
	{
	  int delta = (height > 3) ? 3 : 1;
	  if (c > 0)
	    i = MAX (0, d->d2 - delta);
	  else
	    i = MIN (listsize - height, d->d2 + delta);
	  if (i != d->d2)
	    {
	      d->d2 = i;
	      object_message (d, MSG_DRAW, 0);
	    }
	}
      break;

    case MSG_KEY:
      (*(my_getfuncptr) d->dp) (-1, &listsize);
      if ((listsize) && (d->flags & D_EXIT))
	return D_CLOSE;
      break;

    case MSG_WANTFOCUS:
      return D_WANTFOCUS;

    case MSG_CHAR:
      (*(my_getfuncptr) d->dp) (-1, &listsize);

      if (listsize)
	{
	  c >>= 8;

	  bottom = d->d2 + (d->h - 4) / text_height (font) - 1;
	  if (bottom >= listsize - 1)
	    bottom = listsize - 1;

	  orig = d->d1;

	  if (c == KEY_UP)
	    d->d1--;
	  else if (c == KEY_DOWN)
	    d->d1++;
	  else if (c == KEY_HOME)
	    d->d1 = 0;
	  else if (c == KEY_END)
	    d->d1 = listsize - 1;
	  else if (c == KEY_PGUP)
	    {
	      if (d->d1 > d->d2)
		d->d1 = d->d2;
	      else
		d->d1 -= (bottom - d->d2) ? bottom - d->d2 : 1;
	    }
	  else if (c == KEY_PGDN)
	    {
	      if (d->d1 < bottom)
		d->d1 = bottom;
	      else
		d->d1 += (bottom - d->d2) ? bottom - d->d2 : 1;
	    }
	  else
	    return D_O_K;

	  if (sel)
	    {
	      if (!(key_shifts & (KB_SHIFT_FLAG | KB_CTRL_FLAG)))
		{
		  for (i = 0; i < listsize; i++)
		    sel[i] = FALSE;
		}
	      else if (key_shifts & KB_SHIFT_FLAG)
		{
		  for (i = MIN (orig, d->d1); i <= MAX (orig, d->d1); i++)
		    {
		      if (key_shifts & KB_CTRL_FLAG)
			sel[i] = (i != d->d1);
		      else
			sel[i] = TRUE;
		    }
		}
	    }

	  /* if we changed something, better redraw... */
	  my_handle_scrollable_scroll (d, listsize, &d->d1, &d->d2);
	  d->flags |= D_DIRTY;
	  return D_USED_CHAR;
	}
      break;
    }

  return D_O_K;
}

/*------------------------------------------------------------------*/
BITMAP *
my_create_bitmap (int w, int h)
{
  BITMAP *bmp;

  bmp = create_bitmap (w, h);
  if (bmp == NULL)
    my_exit (EXIT_CODE_MEM_TROUBLE);

  return bmp;
}
