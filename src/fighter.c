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
/* nom           : fighter.c                                        */
/* contenu       : deplacement des mechants                         */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

/*==================================================================*/
/* includes                                                         */
/*==================================================================*/

#include <allegro5/allegro.h>

#include "army.h"
#include "config.h"
#include "cursor.h"
#include "decal.h"
#include "grad.h"
#include "mesh.h"
#include "fighter.h"
#include "palette.h"
#include "startup.h"
#include "time.h"

/*==================================================================*/
/* variables globales                                               */
/*==================================================================*/

#define SIDE_ATTACK_FACTOR 4
/* on divise l'attaque par 2^4 = 16 quand elle se fait sur le cote */

#define NB_SENS_MOVE 2
#define NB_TRY_MOVE 5
#define NB_LOCAL_DIRS 16

static int LOCAL_DIR[NB_LOCAL_DIRS * 2];

static int FIGHTER_MOVE_DIR[NB_SENS_MOVE][NB_DIRS][NB_TRY_MOVE] =
  { {{DIR_NNE, DIR_NE, DIR_NW, DIR_ENE, DIR_WNW},
     {DIR_NE, DIR_ENE, DIR_NNE, DIR_SE, DIR_NW},
     {DIR_ENE, DIR_NE, DIR_SE, DIR_NNE, DIR_SSE},
     {DIR_ESE, DIR_SE, DIR_NE, DIR_SSE, DIR_NNE},
     {DIR_SE, DIR_SSE, DIR_ESE, DIR_SW, DIR_NE},
     {DIR_SSE, DIR_SE, DIR_SW, DIR_ESE, DIR_WSW},
     {DIR_SSW, DIR_SW, DIR_SE, DIR_WSW, DIR_ESE},
     {DIR_SW, DIR_WSW, DIR_SSW, DIR_NW, DIR_SE},
     {DIR_WSW, DIR_SW, DIR_NW, DIR_SSW, DIR_NNW},
     {DIR_WNW, DIR_NW, DIR_SW, DIR_NNW, DIR_SSE},
     {DIR_NW, DIR_NNW, DIR_WNW, DIR_NE, DIR_SW},
     {DIR_NNW, DIR_NW, DIR_NE, DIR_WNW, DIR_ENE}},

{{DIR_NNE, DIR_NE, DIR_NW, DIR_ENE, DIR_WNW},
 {DIR_NE, DIR_NNE, DIR_ENE, DIR_NW, DIR_SE},
 {DIR_ENE, DIR_NE, DIR_SE, DIR_NNE, DIR_SSE},
 {DIR_ESE, DIR_SE, DIR_NE, DIR_SSE, DIR_NNE},
 {DIR_SE, DIR_ESE, DIR_SSE, DIR_NE, DIR_SW},
 {DIR_SSE, DIR_SE, DIR_SW, DIR_ESE, DIR_WSW},
 {DIR_SSW, DIR_SW, DIR_SE, DIR_WSW, DIR_ESE},
 {DIR_SW, DIR_SSW, DIR_WSW, DIR_SE, DIR_NW},
 {DIR_WSW, DIR_SW, DIR_NW, DIR_SSW, DIR_NNW},
 {DIR_WNW, DIR_NW, DIR_SW, DIR_NNW, DIR_SSE},
 {DIR_NW, DIR_WNW, DIR_NNW, DIR_SW, DIR_NE},
 {DIR_NNW, DIR_NW, DIR_NE, DIR_WNW, DIR_ENE}}
};

static int FIGHTER_MOVE_X_REF[NB_DIRS] =
  { 0, 1, 1, 1, 1, 0, 0, -1, -1, -1, -1, 0 };

static int FIGHTER_MOVE_Y_REF[NB_DIRS] =
  { -1, -1, 0, 0, 1, 1, 1, 1, 0, 0, -1, -1 };

static int FIGHTER_MOVE_OFFSET_ASM[NB_SENS_MOVE][NB_DIRS * NB_TRY_MOVE];
static int FIGHTER_MOVE_XY_ASM[NB_SENS_MOVE][NB_DIRS * NB_TRY_MOVE];

static int FIGHTER_MOVE_OFFSET[NB_SENS_MOVE][NB_DIRS][NB_TRY_MOVE];
int FIGHTER_MOVE_X[NB_SENS_MOVE][NB_DIRS][NB_TRY_MOVE];
int FIGHTER_MOVE_Y[NB_SENS_MOVE][NB_DIRS][NB_TRY_MOVE];

/*==================================================================*/
/* fonctions                                                        */
/*==================================================================*/

/*------------------------------------------------------------------*/
static void
erase_fighter (FIGHTER * f)
{
  putpixel (CURRENT_AREA_DISP, f->x, f->y,
            getpixel (CURRENT_AREA_BACK, f->x, f->y));
}

/*------------------------------------------------------------------*/
static void
disp_fighter (FIGHTER * f)
{
  putpixel (CURRENT_AREA_DISP, f->x, f->y,
            COLOR_FIRST_ENTRY[(int) (f->team)]
            + (f->health * COLORS_PER_TEAM) / MAX_FIGHTER_HEALTH);
}

/*------------------------------------------------------------------*/
void
init_move_fighters (void)
{
  int i, j, k, dir;

  for (k = 0; k < NB_SENS_MOVE; ++k)
    for (i = 0; i < NB_DIRS; ++i)
      for (j = 0; j < NB_TRY_MOVE; ++j)
        {
          dir = FIGHTER_MOVE_DIR[k][i][j];
          FIGHTER_MOVE_X[k][i][j] = FIGHTER_MOVE_X_REF[dir];
          FIGHTER_MOVE_Y[k][i][j] = FIGHTER_MOVE_Y_REF[dir];

          FIGHTER_MOVE_OFFSET[k][i][j] = 0;
          if (FIGHTER_MOVE_X[k][i][j] == 1)
            ++(FIGHTER_MOVE_OFFSET[k][i][j]);
          if (FIGHTER_MOVE_X[k][i][j] == -1)
            --(FIGHTER_MOVE_OFFSET[k][i][j]);
          if (FIGHTER_MOVE_Y[k][i][j] == 1)
            FIGHTER_MOVE_OFFSET[k][i][j] += CURRENT_AREA_W;
          if (FIGHTER_MOVE_Y[k][i][j] == -1)
            FIGHTER_MOVE_OFFSET[k][i][j] -= CURRENT_AREA_W;

          FIGHTER_MOVE_OFFSET_ASM[k][i * NB_TRY_MOVE + j] =
            FIGHTER_MOVE_OFFSET[k][i][j];
          FIGHTER_MOVE_XY_ASM[k][i * NB_TRY_MOVE + j] =
            FIGHTER_MOVE_Y[k][i][j] * 65536 + FIGHTER_MOVE_X[k][i][j];
        }

  for (i = 0; i < CURRENT_ARMY_SIZE; ++i)
    disp_fighter (CURRENT_ARMY + i);

  /*
   * note:
   * NB_LOCAL_DIRS is 16 but it's true that 12 is enough.
   * However, I find it safer to reserve 16 slots, since
   * this way all the combinations of 1,2,4 and 8 can
   * be handled correctly - even if some are impossible.
   * And it has the advantage to avoid confusion with
   * the 12 directions represented by NB_DIRS.
   */
  for (i = 1; i <= NB_LOCAL_DIRS; ++i)
    {
      for (j = 0; j < 2; ++j)
        {
          k = -1;
          switch (i)
            {
            case 1:
              k = j ? DIR_NNW : DIR_NNE;
              break;
            case 3:
              k = DIR_NE;
              break;
            case 2:
              k = j ? DIR_ENE : DIR_ESE;
              break;
            case 6:
              k = DIR_SE;
              break;
            case 4:
              k = j ? DIR_SSE : DIR_SSW;
              break;
            case 12:
              k = DIR_SW;
              break;
            case 8:
              k = j ? DIR_WSW : DIR_WNW;
              break;
            case 9:
              k = DIR_NW;
              break;
            }
          LOCAL_DIR[(i - 1) * 2 + j] = k;
        }
    }
}

/*------------------------------------------------------------------*/
int
get_main_dir (MESH * mesh, int team, int sens, int start)
{
  int i, dir, dist;
  MESH *mesh2;

  dist = AREA_START_GRADIENT;
  dir = -1;
  i = start;

  if (sens)
    {
      do
        {
          if ((mesh2 = mesh->link[i]))
            if (mesh2->info[team].state.grad < dist)
              {
                dir = i;
                dist = mesh2->info[team].state.grad;
              }
          i = (i < NB_DIRS - 1) ? i + 1 : 0;
        }
      while (i != start);
    }
  else
    {
      do
        {
          if ((mesh2 = mesh->link[i]))
            if (mesh2->info[team].state.grad < dist)
              {
                dir = i;
                dist = mesh2->info[team].state.grad;
              }
          i = (i > 0) ? i - 1 : NB_DIRS - 1;
        }
      while (i != start);
    }

  if (dir >= 0)
    return dir;
  else
    return (GLOBAL_CLOCK % NB_TEAMS);
}

/*------------------------------------------------------------------*/
static int
get_close_dir (MESH * mesh, FIGHTER * f, int team, int sens, int start)
{
  int cursor_x, cursor_y, fighter_x, fighter_y;
  int code_dir = 0, dir;

  fighter_x = f->x;
  fighter_y = f->y;
  cursor_x = mesh->info[team].update.cursor.x;
  cursor_y = mesh->info[team].update.cursor.y;

  if (cursor_y < fighter_y)
    code_dir += 1;
  if (cursor_x > fighter_x)
    code_dir += 2;
  if (cursor_y > fighter_y)
    code_dir += 4;
  if (cursor_x < fighter_x)
    code_dir += 8;

  if (code_dir)
    dir = LOCAL_DIR[(code_dir - 1) * 2 + (sens ? 1 : 0)];
  else
    dir = start;

  return dir;
}

/*------------------------------------------------------------------*/
void
move_fighters (void)
{
  int attack[NB_TEAMS], defense[NB_TEAMS], new_health[NB_TEAMS];
  int i, dir, team, coef;
  FIGHTER *f;
  PLACE *p, *p0, *p1, *p2, *p3, *p4;
  int *move_offset, *move_x, *move_y;
  int sens, start, table;
  int cpu_influence[NB_TEAMS] = { 0, 0, 0, 0, 0, 0 };

  sens = 0;

  for (i = 0; i < NB_TEAMS; ++i)
    if (CURRENT_CURSOR[i].control_type == CONFIG_CONTROL_TYPE_CPU
        && CURRENT_CURSOR[i].active)
      cpu_influence[CURRENT_CURSOR[i].team] =
        LW_CONFIG_CURRENT_RULES.cpu_advantage;

  for (i = 0; i < PLAYING_TEAMS; ++i)
    {
      coef = ACTIVE_FIGHTERS[i] * PLAYING_TEAMS - CURRENT_ARMY_SIZE;
      coef *= 256;
      coef /= CURRENT_ARMY_SIZE;
      if (coef > 256)
        coef = 256;

      coef *=
        (LW_CONFIG_CURRENT_RULES.number_influence -
         8) * (LW_CONFIG_CURRENT_RULES.number_influence - 8);
      coef /= 64;
      if (LW_CONFIG_CURRENT_RULES.number_influence < 8)
        coef = -coef;
      if (coef < 0)
        coef /= 2;
      coef += 256;

      attack[i] = (coef *
                   fixsqrt (fixsqrt
                            (1 <<
                             (LW_CONFIG_CURRENT_RULES.fighter_attack +
                              cpu_influence[i])))) / (256 * 8);
      if (attack[i] >= MAX_FIGHTER_HEALTH)
        attack[i] = MAX_FIGHTER_HEALTH - 1;
      if (attack[i] < 1)
        attack[i] = 1;

      defense[i] = (coef *
                    fixsqrt (fixsqrt
                             (1 <<
                              (LW_CONFIG_CURRENT_RULES.fighter_defense +
                               cpu_influence[i])))) / (256 * 256);
      if (defense[i] >= MAX_FIGHTER_HEALTH)
        defense[i] = MAX_FIGHTER_HEALTH - 1;
      if (defense[i] < 1)
        defense[i] = 1;

      new_health[i] = (coef *
                       fixsqrt (fixsqrt
                                (1 <<
                                 (LW_CONFIG_CURRENT_RULES.fighter_new_health +
                                  cpu_influence[i])))) / (256 * 4);
      if (new_health[i] >= MAX_FIGHTER_HEALTH)
        new_health[i] = MAX_FIGHTER_HEALTH - 1;
      if (new_health[i] < 1)
        new_health[i] = 1;

      ACTIVE_FIGHTERS[i] = 0;
    }
  start = (GLOBAL_CLOCK / 6) % NB_DIRS;
  table = (GLOBAL_CLOCK / 3) % 2;
  f = CURRENT_ARMY;

    for (i = 0; i < CURRENT_ARMY_SIZE; ++i)
      {
        team = f->team;
        ACTIVE_FIGHTERS[team]++;
        start = (start < NB_DIRS - 1) ? start + 1 : 0;

        p = CURRENT_AREA + (f->y * CURRENT_AREA_W + f->x);

        if (p->mesh->info[team].update.time >= 0)
          {
            p->mesh->info[team].state.dir =
              get_close_dir (p->mesh, f, team, (sens++) % 2, start);
          }
        else if ((-p->mesh->info[team].update.time) < GLOBAL_CLOCK)
          {
            p->mesh->info[team].state.dir =
              get_main_dir (p->mesh, team, (sens++) % 2, start);
            p->mesh->info[team].update.time = -GLOBAL_CLOCK;
          }

        dir = p->mesh->info[team].state.dir;

        move_offset = FIGHTER_MOVE_OFFSET[table][dir];
        move_x = FIGHTER_MOVE_X[table][dir];
        move_y = FIGHTER_MOVE_Y[table][dir];

        if (((p0 = p + move_offset[0])->mesh) && (!p0->fighter))
          {
            erase_fighter (f);
            p0->fighter = f;
            p->fighter = NULL;
            f->x += move_x[0];
            f->y += move_y[0];
            disp_fighter (f);
          }
        else
          {
            if (((p1 = p + move_offset[1])->mesh) && (!p1->fighter))
              {
                erase_fighter (f);
                p1->fighter = f;
                p->fighter = NULL;
                f->x += move_x[1];
                f->y += move_y[1];
                disp_fighter (f);
              }
            else
              {
                if (((p2 = p + move_offset[2])->mesh) && (!p2->fighter))
                  {
                    erase_fighter (f);
                    p2->fighter = f;
                    p->fighter = NULL;
                    f->x += move_x[2];
                    f->y += move_y[2];
                    disp_fighter (f);
                  }
                else
                  {
                    if (((p3 = p + move_offset[3])->mesh) && (!p3->fighter))
                      {
                        erase_fighter (f);
                        p3->fighter = f;
                        p->fighter = NULL;
                        f->x += move_x[3];
                        f->y += move_y[3];
                        disp_fighter (f);
                      }
                    else
                      {
                        if (((p4 = p + move_offset[4])->mesh)
                            && (!p4->fighter))
                          {
                            erase_fighter (f);
                            p4->fighter = f;
                            p->fighter = NULL;
                            f->x += move_x[4];
                            f->y += move_y[4];
                            disp_fighter (f);
                          }
                        else
                          {
                            if (p0->mesh
                                && p0->fighter && p0->fighter->team != team)
                              {
                                p0->fighter->health -= attack[team];
                                if (p0->fighter->health < 0)
                                  {
                                    while (p0->fighter->health < 0)
                                      p0->fighter->health += new_health[team];
                                    p0->fighter->team = team;
                                  }
                                disp_fighter (p0->fighter);
                              }
                            else
                              {
                                if (p1->mesh
                                    && p1->fighter
                                    && p1->fighter->team != team)
                                  {
                                    p1->fighter->health -= attack[team]
                                      >> SIDE_ATTACK_FACTOR;
                                    if (p1->fighter->health < 0)
                                      {
                                        while (p1->fighter->health < 0)
                                          p1->fighter->health +=
                                            new_health[team];
                                        p1->fighter->team = team;
                                      }
                                    disp_fighter (p1->fighter);
                                  }
                                else
                                  {
                                    if (p2->mesh
                                        && p2->fighter
                                        && p2->fighter->team != team)
                                      {
                                        p2->fighter->health -= attack[team]
                                          >> SIDE_ATTACK_FACTOR;
                                        if (p2->fighter->health < 0)
                                          {
                                            while (p2->fighter->health < 0)
                                              p2->fighter->health +=
                                                new_health[team];
                                            p2->fighter->team = team;
                                          }
                                        disp_fighter (p2->fighter);
                                      }
                                    else
                                      {
                                        if (p0->mesh
                                            && p0->fighter
                                            && p0->fighter->team == team)
                                          {
                                            p0->fighter->health +=
                                              defense[team];
                                            if (p0->fighter->health >=
                                                MAX_FIGHTER_HEALTH)
                                              p0->fighter->health =
                                                MAX_FIGHTER_HEALTH - 1;
                                            disp_fighter (p0->fighter);
                                          }
                                      }
                                  }
                              }
                          }
                      }
                  }
              }
          }
        f++;
      }
}
