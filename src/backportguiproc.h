/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      GUI routines.
 *
 *      By Shawn Hargreaves.
 *
 *      See readme.txt for copyright information.
 */


#ifndef ALLEGRO_GUIPROC_H
#define ALLEGRO_GUIPROC_H

/*
 * [DISCLAIMER:ufoot] this is not really from the allegro source tree.
 * Those functions are defined in guiproc.c then exposed in aintern.h.
 * But I won't backport all of aintern.h so just extracted those gui funcs.
 */

int _gui_shadow_box_proc(int msg, DIALOG *d, int c);
int _gui_ctext_proc(int msg, DIALOG *d, int c);
int _gui_button_proc(int msg, DIALOG *d, int c);
int _gui_edit_proc(int msg, DIALOG *d, int c);
int _gui_list_proc(int msg, DIALOG *d, int c);
int _gui_text_list_proc(int msg, DIALOG *d, int c);

#endif // ALLEGRO_GUIPROC_H
