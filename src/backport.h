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
/* Liquid War homepage : http://www.ufoot.org/liquidwar/v5                   */
/* Contact author      : ufoot@ufoot.org                                     */
/*****************************************************************************/

/********************************************************************/
/* nom           : backport.h                                       */
/* content       : missing allegro 4 stuff                          */
/* last update   : Dec 24th 2018                                    */
/********************************************************************/

#ifndef LIQUID_WAR_INCLUDE_BACKPORT
#define LIQUID_WAR_INCLUDE_BACKPORT

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

/*==================================================================*/
/* defines                                                          */
/*==================================================================*/

#ifndef AL_CONST
#define AL_CONST const
#endif // AL_CONST

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef MIN
#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#endif // MIN
#ifndef MAX
#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#endif // MAX
#ifndef MID
/* Returns the median of x, y, z */
#define MID(x,y,z)   ((x) > (y) ? ((y) > (z) ? (y) : ((x) > (z) ?    \
                       (z) : (x))) : ((y) > (z) ? ((z) > (x) ? (z) : \
                       (x)): (y)))
#endif // MID
#ifndef CLAMP
/* Optimized version of MID for when x <= z. */
#define CLAMP(x,y,z) MAX((x), MIN((y), (z)))
#endif // CLAMP
#ifndef ABS
#define ABS(x)       (((x) >= 0) ? (x) : (-(x)))
#endif // ABS

#ifndef SGN
#define SGN(x)       (((x) >= 0) ? 1 : -1)
#endif // SGN

#define _AL_MALLOC(size) al_malloc(size)
#define _AL_FREE(ptr) al_free(ptr)

#define LOCK_DATA(d, s)
#define LOCK_CODE(c, s)
#define UNLOCK_DATA(d, s)
#define LOCK_VARIABLE(x)
#define LOCK_FUNCTION(x)

/*==================================================================*/
/* types                                                            */
/*==================================================================*/

#define PALETTE_SIZE 256
typedef ALLEGRO_COLOR PALETTE[PALETTE_SIZE];

#define MAX_JOYSTICKS            8
#define MAX_JOYSTICK_AXIS        3
#define MAX_JOYSTICK_STICKS      5
#define MAX_JOYSTICK_BUTTONS     32

#define JOY_TYPE_AUTODETECT      0
#define JOY_TYPE_2PADS           1
#define JOY_TYPE_4BUTTON         2
#define JOY_TYPE_STANDARD        3

/* information about a single joystick axis */
typedef struct JOYSTICK_AXIS_INFO
{
  int pos;
  int d1, d2;
  const char *name;
} JOYSTICK_AXIS_INFO;

/* information about one or more axis (a slider or directional control) */
typedef struct JOYSTICK_STICK_INFO
{
  int flags;
  int num_axis;
  JOYSTICK_AXIS_INFO axis[MAX_JOYSTICK_AXIS];
  const char *name;
} JOYSTICK_STICK_INFO;

/* information about a joystick button */
typedef struct JOYSTICK_BUTTON_INFO
{
  int b;
  const char *name;
} JOYSTICK_BUTTON_INFO;

/* information about an entire joystick */
typedef struct JOYSTICK_INFO
{
  //  int flags;
  const char *name;
  int num_sticks;
  int num_buttons;
  JOYSTICK_STICK_INFO stick[MAX_JOYSTICK_STICKS];
  JOYSTICK_BUTTON_INFO button[MAX_JOYSTICK_BUTTONS];
} JOYSTICK_INFO;

#define KEY_MAX 127

#define KB_NORMAL    0x01
#define KB_EXTENDED  0x02

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

extern ALLEGRO_BITMAP *screen;
extern int SCREEN_W;
extern int SCREEN_H;
extern ALLEGRO_FONT *font;
extern volatile int mouse_x;
extern volatile int mouse_y;
extern volatile int mouse_z;
extern volatile int mouse_b;
extern int *allegro_errno;
extern volatile JOYSTICK_INFO joy[MAX_JOYSTICKS];
extern int num_joysticks;
extern char empty_string[];
extern volatile char key[];

/*==================================================================*/
/* fonctions globales                                               */
/*==================================================================*/

void scare_mouse ();
void scare_mouse_area (int x, int y, int w, int h);
void unscare_mouse ();
int poll_mouse (void);
int mouse_needs_poll (void);

void clear_to_color (ALLEGRO_BITMAP * bitmap, int color);
void putpixel (ALLEGRO_BITMAP * bitmap, int x, int y, int color);
int getpixel (ALLEGRO_BITMAP * bitmap, int x, int y);
void rect (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2,
           int color);
void rectfill (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2,
               int color);
void rectfill_dotted (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2,
                      int fg, int bg);
void vline (ALLEGRO_BITMAP * bitmap, int x, int y1, int y2, int color);
void hline (ALLEGRO_BITMAP * bitmap, int x1, int y, int x2, int color);
void line (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2,
           int color);

int usetc (char *s, int c);
int ugetc (const char *s);
int uwidth (const char *s);
int ustrlen (const char *s);
int ucwidth (int c);
int uisspace (int c);
int ugetat (char *s, int index);
int usetat (char *s, int index, int c, int max_size);
int uinsert (char *s, int index, int c, int max_size);
int uremove (char *s, int index);
int uisok (int c);
int utolower (int c);
char *ustrdup (const char *src);
int ugetx (char **s);
int ugetxc (const char **s);
char *ustrtok_r (char *s, AL_CONST char *set, char **last);
char *uconvert_ascii (const char *s, char buf[], int size);

int text_length (const ALLEGRO_FONT * f, const char *s);
int text_height (const ALLEGRO_FONT * f);
void textout_ex (ALLEGRO_BITMAP * bmp, const ALLEGRO_FONT * f, const char *s,
                 int x, int y, int color, int bg);

void rest_callback (unsigned int time, void (*callback) (void));
void rest (unsigned int time);
int install_timer ();
void remove_timer ();
int install_int (void (*proc) (), int speed);
void remove_int (void (*proc) ());

void draw_sprite (ALLEGRO_BITMAP * bmp, ALLEGRO_BITMAP * sprite, int x,
                  int y);
void blit (ALLEGRO_BITMAP * source, ALLEGRO_BITMAP * dest,
           int source_x, int source_y,
           int dest_x, int dest_y, int dest_width, int dest_height);
void stretch_blit (ALLEGRO_BITMAP * source, ALLEGRO_BITMAP * dest,
                   int source_x, int source_y, int source_width,
                   int source_height, int dest_x, int dest_y, int dest_width,
                   int dest_height);

void acquire_bitmap (ALLEGRO_BITMAP * bmp);
void acquire_screen ();
void release_bitmap (ALLEGRO_BITMAP * bmp);
void release_screen ();
void set_clip_rect (ALLEGRO_BITMAP * bitmap, int x1, int y1, int x2, int y2);

int install_joystick (int type);
int poll_joystick ();

int keypressed ();
int readkey ();
int ureadkey (int *scancode);
void clear_keybuf ();
void simulate_keypress (int key);
int scancode_to_ascii (int scancode);

void set_config_file(char *filename);
int get_config_int(const char *section, const char *key, int def);
const char *get_config_string(const char *section, const char *key, const char *def);
void set_config_int(const char *section, const char *key, int value);
void set_config_string(const char *section, const char *key, const char *value);
void flush_config_file();

int exists(const char *filename);

// Graphics mode constants (Allegro 4 compatibility)
#define GFX_AUTODETECT_FULLSCREEN 1
#define GFX_AUTODETECT_WINDOWED 2
#define GFX_DIRECTX 3
#define GFX_DIRECTX_WIN 4
#define GFX_MODEX 5
#define GFX_VESA2L 6

// Graphics mode functions
int set_gfx_mode(int card, int w, int h, int v_w, int v_h);
void set_palette(void *palette);
void set_window_title(const char *title);

// Graphics driver info
typedef struct {
  char *ascii_name;
} LW_GFX_DRIVER_INFO;

extern LW_GFX_DRIVER_INFO *gfx_driver;
extern void *black_palette;

// Allegro 4 compatibility constants
#define U_ASCII 0
#define SYSTEM_NONE 0
#define COLORCONV_REDUCE_TO_256 0

// Audio constants
#define DIGI_NONE 0
#define MIDI_NONE 0

// Driver info structures (Allegro 4 compatibility)
extern LW_GFX_DRIVER_INFO *timer_driver;
extern LW_GFX_DRIVER_INFO *keyboard_driver;
extern LW_GFX_DRIVER_INFO *mouse_driver;
extern LW_GFX_DRIVER_INFO *digi_driver;
extern LW_GFX_DRIVER_INFO *midi_driver;
extern LW_GFX_DRIVER_INFO *joystick_driver;

// Allegro initialization functions
int allegro_init(void);
int install_allegro(int system_id, int *errno_ptr, int (*atexit_ptr)());
int install_keyboard(void);
void remove_keyboard(void);
int install_mouse(void);
int install_sound(int digi_card, int midi_card, const char *cfg_path);
void set_uformat(int format);
void set_color_depth(int depth);
void set_color_conversion(int flags);
void set_close_button_callback(void (*callback)(void));

// Global variables for compatibility
extern char *allegro_id;

#endif // LIQUID_WAR_INCLUDE_BACKPORT
