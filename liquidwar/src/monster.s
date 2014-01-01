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
/* Copyright (C) 1998-2014 Christian Mauduit                                 */
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
/* nom           : monster.s                                        */
/* contenu       : deplacement des streums accelere                 */
/* date de modif : 3 mai 98                                         */
/********************************************************************/


/*typedef union          */
/*        {              */
/*         int time;     */
/*         struct        */
/*          {            */
/*           short x;    */
/*           short y;    */
/*          } cursor;    */
/*        } MESH_UPDATE; */

/* typedef struct           */
/*        {                 */
/*         int dir :8;      */
/*         int grad:24;     */
/*        } MESH_STATE;     */

/*typedef struct              */
/*        {                   */
/*         MESH_UPDATE update;*/
/*         MESH_STATE state;  */
/*        } MESH_INFO;        */

.equ SIZE_OF_INFO,8

.equ INFO_UPDATE,0
.equ INFO_STATE,4


/*typedef struct                   */
/*        {                        */
/*         char decal_for_dir :8;  */
/*         int  size          :24; */
/*        } MESH_SIDE;             */ 

/*typedef struct                     */
/*        {                          */
/*         short x;                  */
/*         short y;                  */
/*         MESH_SIDE side;           */
/*         MESH_INFO info[NB_TEAMS]; */
/*         void *link[NB_DIRS];      */
/*        } MESH;                    */

.equ SIZE_OF_MESH,104

.equ MESH_INFO,8
.equ MESH_LINK,56



/*typedef struct              */
/*        {                   */
/*         short x;           */
/*         short y;           */
/*         short   health;    */
/*         char team;         */
/*        char last_dir;      */
/*        } FIGHTER;          */


.equ SIZE_OF_FIGHTER,8
.equ FIGHTER_XY,0
.equ FIGHTER_X,0
.equ FIGHTER_Y,2
.equ FIGHTER_HEALTH,4
.equ FIGHTER_TEAM,6
.equ FIGHTER_LAST_DIR,7


/*typedef struct              */
/*        {                   */
/*         MESH    *mesh;     */
/*         FIGHTER *fighter;  */
/*        } PLACE;            */


.equ SIZE_OF_PLACE,8
.equ PLACE_MESH,0
.equ PLACE_FIGHTER,4

.equ MOVE_BLOCK_NUMBER,5

.equ ARG_CURRENT_ARMY,             8
.equ ARG_CURRENT_ARMY_SIZE,       12
.equ ARG_CURRENT_AREA,            16
.equ ARG_CURRENT_AREA_W,          20
.equ ARG_ACTIVE_FIGHTERS,         24
.equ ARG_ATTACK,                  28
.equ ARG_DEFENSE,                 32
.equ ARG_NEW_HEALTH,              36
.equ ARG_MAX_FIGHTER_HEALTH_1,    40
.equ ARG_SENS,                    44
.equ ARG_START,                   48
.equ ARG_NEG_GLOBAL_CLOCK,        52
.equ ARG_FIGHTER_MOVE_XY,         56
.equ ARG_FIGHTER_MOVE_OFFSET,     60
.equ ARG_CURRENT_AREA_BACK,       64
.equ ARG_CURRENT_AREA_DISP,       68
.equ ARG_COLOR_FIRST_ENTRY,       72
.equ ARG_COLORS_PER_TEAM,         76
.equ ARG_LOCAL_DIR,               80
.equ ARG_TEMP1,                   84
.equ ARG_TEMP2,                   88
.equ ARG_TEMP3,                   92
.equ ARG_TEMP4,                   96
.equ ARG_TEMP5,                   100
.equ ARG_TEMP6,                   104
.equ ARG_TEMP7,                   108
.equ ARG_TEMP8,                   112

.equ OFFSET_P0,0
.equ OFFSET_P1,4
.equ OFFSET_P2,8
.equ OFFSET_P3,12
.equ OFFSET_P4,16

.equ SIDE_ATTACK_FACTOR,4     #on divise attaque par 2^4=16 qd
                              #l attaque se fait sur le cote

/*#define AREA_START_GRADIENT 2000000*/
.equ AREA_START_GRADIENT,2000000*256

.text



/*==================================================================*/
/* grosse fonction enorme, on fait ce qu on pneu                    */
/*==================================================================*/

/*--------------------------------------------------------------------*/
/* void boost_move_fighters (FIGHTER *current_army,                   */
/*                          int current_army_size,                    */
/*                          PLACE *current_area,                      */
/*                          int current_area_w,                       */
/*                          int *active_fighters,                     */
/*                          int *attack,                              */
/*                          int *defense,                             */
/*                          int *new_health,                          */
/*                          int max_fighter_health_1,                 */
/*                          int sens,                                 */
/*                          int start,                                */
/*                          int neg_global_clock,                     */
/*                          int *fighter_move_xy,                     */
/*                          int *fighter_move_offset,                 */
/*                          char *current_area_back_data,             */
/*                          char *current_area_disp_data,             */
/*                          int *color_first_entry,                   */
/*                          int colors_per_team,                      */
/*                          int *local_dir,                           */
/*                          int temp1,                                */
/*                          int temp2,                                */
/*                          int temp3,                                */
/*                          int temp4,                                */
/*                          int temp5,                                */
/*                          int temp6,                                */
/*                          int temp7,                                */
/*                          int temp8);                               */
/*--------------------------------------------------------------------*/
.globl _boost_move_fighters
.globl boost_move_fighters
   .align 4
_boost_move_fighters:	
boost_move_fighters:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es
   pushw %fs
   pushw %gs

   movw  %ds,%ax
   movw  %ax,%es
   movw  %ax,%fs
   movw  %ax,%gs

   movl  ARG_CURRENT_ARMY_SIZE(%ebp),%ecx  # ecx=nombre de streums
   movl  ARG_CURRENT_ARMY(%ebp),%esi       # esi=adresse du streum courant

main_loop:
   push  %ecx

/*------------------------------------------------------------------*/
/* indice de l equipe                                               */
/*------------------------------------------------------------------*/
   xor   %ecx,%ecx                  # ecx a zero
   movb  FIGHTER_TEAM(%esi),%cl     # equipe dans le champ ecx
   movl  %ecx,%ds:ARG_TEMP1(%ebp)   # temp1 prend team
   shl   $2,%ecx                    # ecx contient maintenant team*4

/*------------------------------------------------------------------*/
/* mise a jour du nombre de combattants de l equipe                 */
/*------------------------------------------------------------------*/
   mov   ARG_ACTIVE_FIGHTERS(%ebp),%edi  # recuperation de pointeur
   add   %ecx,%edi                       # ajout de l offset pour l equipe
   incl  %ds:(%edi)                      # mise a jour du fighter number

/*------------------------------------------------------------------*/
/* localisation du streum courant                                   */
/*------------------------------------------------------------------*/
   movl  FIGHTER_XY(%esi),%eax        # eax contient fighter->xy
   xor   %ebx,%ebx                    # ebx a 0
   movw  %ax,%bx                      # ebx contient fighter->x
   shr   $16,%eax                     # eax contient fighter->y
   cdq                                # je crois que ca sert pour imul
   mull  %ds:ARG_CURRENT_AREA_W(%ebp) # eax prend fighter->y*CURRENT_AREA_W
   add   %ebx,%eax                    # eax contient un joli offset
   mov   %eax,%ds:ARG_TEMP2(%ebp)     # temp2 prend y*CURRENT_AREA_W+1
   shl   $3,%eax                      # eax*=8 (taille de PLACE)
   add   ARG_CURRENT_AREA(%ebp),%eax
   mov   %eax,%edi                    # edi=adresse dans CURRENT_AREA
                                      # situant la place du streum courant
/*------------------------------------------------------------------*/
/* on recupere le parametre update du streum courant                */
/*------------------------------------------------------------------*/
   shl   $1,%ecx                      # ecx contient maintenant team*8
   add   $MESH_INFO,%ecx              # ecx=offset pour info%team
   mov   PLACE_MESH(%edi),%ebx        # ebx prend l adresse du maillage
   add   %ecx,%ebx                    # sur l instant de mise a jour
   mov   INFO_UPDATE(%ebx),%eax       # eax=instant mise a jour

/*------------------------------------------------------------------*/
/* on calcule la direction principale de deplacement                */
/*------------------------------------------------------------------*/
   cmp   $0,%eax
   jge   fdl_start

/*------------------------------------------------------------------*/
/* calcul base sur les gradients                                    */
/*------------------------------------------------------------------*/
   mov   ARG_NEG_GLOBAL_CLOCK(%ebp),%edx  # edx=-GLOBAL_CLOCK
   cmp   %edx,%eax                        # direction a jour???
   je    fdg_dir_ok                       # oui...
                                          # non, on la met, en
   mov   %edx,INFO_UPDATE(%ebx)           # commencant par l heure

   mov   ARG_START(%ebp),%al              # al=direction de depart
   mov   %al,%ah                          # ah=direction courante
   ror   $8,%eax
   mov   %al,%ah                          # direction  trouvee sur
   rol   $8,%eax                          # la partie haute de eax

   mov   $AREA_START_GRADIENT,%edx        # plus proche distance
                                          # trouvee

   movb  ARG_SENS(%ebp),%bl               # bl=sens

   cmp   $0,%bl
   jne   fdg_sens_1

/*------------------------------------------------------------------*/
/* dans le sens 0                                                   */
/*------------------------------------------------------------------*/
fdg_sens_0:
loop_fdg_sens_0:

   xor   %ebx,%ebx
   mov   %ah,%bl
   sal   $2,%ebx
   add   PLACE_MESH(%edi),%ebx        # ebx prend l adresse du maillage
   mov   MESH_LINK(%ebx),%ebx
   cmp   $0,%ebx                      # le mesh existe-t-il ???
   je    not_better_fdg_sens_0
   add   %ecx,%ebx                    # sur l instant de mise a jour
   mov   INFO_STATE(%ebx),%ebx        # ebx= distance
   cmp   %edx,%ebx                    # on compare
   jge   not_better_fdg_sens_0        # pas mieux
   mov   %ebx,%edx                    # mieux!, on met a jour
   ror   $8,%eax
   mov   %al,%ah                      # direction trouve ds eax haut
   rol   $8,%eax

not_better_fdg_sens_0:
   inc   %ah                              # direction suivante
   cmp   $12,%ah
   jne   skip_fdg_sens_0
   xor   %ah,%ah
skip_fdg_sens_0:
   cmp   %ah,%al
   jne   loop_fdg_sens_0

   movb  $1,ARG_SENS(%ebp)                # la prochaine fois on
                                          # changera de direction
   jmp   fdg_end_of_calc

/*------------------------------------------------------------------*/
/* dans le sens 1                                                   */
/*------------------------------------------------------------------*/
fdg_sens_1:
loop_fdg_sens_1:

   xor   %ebx,%ebx
   mov   %ah,%bl
   sal   $2,%ebx
   add   PLACE_MESH(%edi),%ebx        # ebx prend l adresse du maillage
   mov   MESH_LINK(%ebx),%ebx
   cmp   $0,%ebx                      # le mesh existe-t-il ???
   je    not_better_fdg_sens_1
   add   %ecx,%ebx                    # sur l instant de mise a jour
   mov   INFO_STATE(%ebx),%ebx        # ebx= distance
   cmp   %edx,%ebx                    # on compare
   jge   not_better_fdg_sens_1        # pas mieux
   mov   %ebx,%edx                    # mieux!, on met a jour
   ror   $8,%eax
   mov   %al,%ah                      # direction trouve ds eax haut
   rol   $8,%eax

not_better_fdg_sens_1:
   dec   %ah                              # direction suivante
   jge   skip_fdg_sens_1
   mov   $11,%ah
skip_fdg_sens_1:
   cmp   %ah,%al
   jne   loop_fdg_sens_1

   movb  $0,ARG_SENS(%ebp)                # la prochaine fois on
                                          # changera de direction
#   jmp   fdg_end_of_calc

/*------------------------------------------------------------------*/
/* fin du calcul, partie commune du calcul de direction principal   */
/*------------------------------------------------------------------*/
fdg_end_of_calc:
   mov   PLACE_MESH(%edi),%ebx        # ebx prend l adresse du maillage
   add   %ecx,%ebx                    # sur le param update
   ror   $8,%eax                      # ah=dir choisie
   movb  %ah,INFO_STATE(%ebx)         # mise a jour en memoire

/*------------------------------------------------------------------*/
/* la direction a ete calculee pour tout le mesh                    */
/*------------------------------------------------------------------*/
fdg_dir_ok:
   xor   %eax,%eax                        # eax a 0
   movb  INFO_STATE(%ebx),%al             #

#   movb  $0,%al

   jmp   dir_calculated
skip_1:

/*------------------------------------------------------------------*/
/* calcul "local"                                                   */
/*------------------------------------------------------------------*/

fdl_start:
   xor   %ecx,%ecx                    # ecx= resultat des tests
   movl  FIGHTER_XY(%esi),%edx        # eax contient fighter->xy

   cmp   %ax,%dx
   je    fdl_skip_x1
   jg    fdl_skip_x2
   or    $2,%cl
   jmp   fdl_skip_x1
fdl_skip_x2:
   or    $8,%cl
fdl_skip_x1:

   ror   $16,%eax
   ror   $16,%edx

   cmp   %ax,%dx
   je    fdl_skip_x3
   jg    fdl_skip_x4
   or    $4,%cl
   jmp   fdl_skip_x3
fdl_skip_x4:
   or    $1,%cl
fdl_skip_x3:

   or    %cl,%cl
   je    on_the_spot
   mov   ARG_LOCAL_DIR(%ebp),%ebx
   dec   %cl
   shl   $1,%cl
   add   ARG_SENS(%ebp),%ecx
   shl   $2,%cl
   add   %ecx,%ebx
   mov   (%ebx),%eax
   jmp   end_of_fdl
on_the_spot:
   mov   ARG_START(%ebp),%eax
end_of_fdl:

#movb  $6,%al

  # a l issue de ces calculs, eax contient la direction en question

/*------------------------------------------------------------------*/
/* on dispose ici dans eax de la direction principale de deplacement*/
/*------------------------------------------------------------------*/

dir_calculated:

   shl   $2,%eax
   mov   %eax,%ecx
   shl   $2,%eax
   add   %eax,%ecx                           #ecx=eax*20...

/*------------------------------------------------------------------*/
/* test si deplacement possible dans direction associee a p0        */
/*------------------------------------------------------------------*/

   mov   %ecx,%ebx                           #on recupere la dir
   add   ARG_FIGHTER_MOVE_OFFSET(%ebp),%ebx  #ebx=pointeur a garder!!!
   mov   OFFSET_P0(%ebx),%eax                #eax=offs ds cur_area

   mov   %eax,ARG_TEMP5(%ebp)                #offset dir p0 memorise

   shl   $3,%eax
   add   %edi,%eax                           #eax=pointeur sur place

   mov   PLACE_MESH(%eax),%edx               #edx=pointeur sur mesh
   cmp   $0,%edx                             #la place est-elle un mur?
   je    skip_3                              #oui
   mov   PLACE_FIGHTER(%eax),%edx            #edx=pointeur sur fighter
   cmp   $0,%edx                             #la place est-elle libre?
   jne   skip_3                              #non

/*------------------------------------------------------------------*/
/* deplacement dans direction associee a p0                         */
/*------------------------------------------------------------------*/

   movl  $0,%ds:PLACE_FIGHTER(%edi)          #on vide dans cur_area
   mov   %esi,%ds:PLACE_FIGHTER(%eax)        #nouvelle place

   add   ARG_FIGHTER_MOVE_XY(%ebp),%ecx      #offset ds move_xy
   mov   OFFSET_P0(%ecx),%edx                #on charge l increm
   addl  %edx,%ds:FIGHTER_XY(%esi)           #addition des coord

   mov   ARG_TEMP2(%ebp),%ecx                #y*cur_area_w+x
   mov   ARG_CURRENT_AREA_BACK(%ebp),%eax    #eax=cur_area_back->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dl                      #pixel memorise
   mov   ARG_CURRENT_AREA_DISP(%ebp),%eax    #eax=cur_area_disp->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dh                      #couleur du monstre
   movb  %dl,%ds:(%eax)                      #effacement
   add   OFFSET_P0(%ebx),%eax                #deplacement
   movb  %dh,%ds:(%eax)                      #affichage

   jmp   next_monster                        #fini avec ce monstre...

skip_3:

/*------------------------------------------------------------------*/
/* test si deplacement possible dans direction associee a p1        */
/*------------------------------------------------------------------*/

   mov   %ecx,%ebx                           #on recupere la dir
   add   ARG_FIGHTER_MOVE_OFFSET(%ebp),%ebx  #ebx=pointeur a garder!!!
   mov   OFFSET_P1(%ebx),%eax                #eax=offs ds cur_area

   mov   %eax,ARG_TEMP6(%ebp)                #offset dir p1 memorise

   shl   $3,%eax
   add   %edi,%eax                           #eax=pointeur sur place

   mov   PLACE_MESH(%eax),%edx               #edx=pointeur sur mesh
   cmp   $0,%edx                             #la place est-elle un mur?
   je    skip_4                              #oui
   mov   PLACE_FIGHTER(%eax),%edx            #edx=pointeur sur fighter
   cmp   $0,%edx                             #la place est-elle libre?
   jne   skip_4                              #non

/*------------------------------------------------------------------*/
/* deplacement dans direction associee a p1                         */
/*------------------------------------------------------------------*/

   movl  $0,%ds:PLACE_FIGHTER(%edi)          #on vide dans cur_area
   mov   %esi,%ds:PLACE_FIGHTER(%eax)        #nouvelle place

   add   ARG_FIGHTER_MOVE_XY(%ebp),%ecx      #offset ds move_xy
   mov   OFFSET_P1(%ecx),%edx                #on charge l increm
   addl  %edx,%ds:FIGHTER_XY(%esi)           #addition des coord

   mov   ARG_TEMP2(%ebp),%ecx                #y*cur_area_w+x
   mov   ARG_CURRENT_AREA_BACK(%ebp),%eax    #eax=cur_area_back->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dl                      #pixel memorise
   mov   ARG_CURRENT_AREA_DISP(%ebp),%eax    #eax=cur_area_disp->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dh                      #couleur du monstre
   movb  %dl,%ds:(%eax)                      #effacement
   add   OFFSET_P1(%ebx),%eax                #deplacement
   movb  %dh,%ds:(%eax)                      #affichage

   jmp   next_monster                        #fini avec ce monstre...

skip_4:

/*------------------------------------------------------------------*/
/* test si deplacement possible dans direction associee a p2        */
/*------------------------------------------------------------------*/

   mov   %ecx,%ebx                           #on recupere la dir
   add   ARG_FIGHTER_MOVE_OFFSET(%ebp),%ebx  #ebx=pointeur a garder!!!
   mov   OFFSET_P2(%ebx),%eax                #eax=offs ds cur_area

   mov   %eax,ARG_TEMP7(%ebp)                #offset dir p2 memorise

   shl   $3,%eax
   add   %edi,%eax                           #eax=pointeur sur place

   mov   PLACE_MESH(%eax),%edx               #edx=pointeur sur mesh
   cmp   $0,%edx                             #la place est-elle un mur?
   je    skip_5                              #oui
   mov   PLACE_FIGHTER(%eax),%edx            #edx=pointeur sur fighter
   cmp   $0,%edx                             #la place est-elle libre?
   jne   skip_5                              #non

/*------------------------------------------------------------------*/
/* deplacement dans direction associee a p2                         */
/*------------------------------------------------------------------*/

   movl  $0,%ds:PLACE_FIGHTER(%edi)          #on vide dans cur_area
   mov   %esi,%ds:PLACE_FIGHTER(%eax)        #nouvelle place

   add   ARG_FIGHTER_MOVE_XY(%ebp),%ecx      #offset ds move_xy
   mov   OFFSET_P2(%ecx),%edx                #on charge l increm
   addl  %edx,%ds:FIGHTER_XY(%esi)           #addition des coord

   mov   ARG_TEMP2(%ebp),%ecx                #y*cur_area_w+x
   mov   ARG_CURRENT_AREA_BACK(%ebp),%eax    #eax=cur_area_back->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dl                      #pixel memorise
   mov   ARG_CURRENT_AREA_DISP(%ebp),%eax    #eax=cur_area_disp->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dh                      #couleur du monstre
   movb  %dl,%ds:(%eax)                      #effacement
   add   OFFSET_P2(%ebx),%eax                #deplacement
   movb  %dh,%ds:(%eax)                      #affichage

   jmp   next_monster                        #fini avec ce monstre...

skip_5:

/*------------------------------------------------------------------*/
/* test si deplacement possible dans direction associee a p3        */
/*------------------------------------------------------------------*/

   mov   %ecx,%ebx                           #on recupere la dir
   add   ARG_FIGHTER_MOVE_OFFSET(%ebp),%ebx  #ebx=pointeur a garder!!!
   mov   OFFSET_P3(%ebx),%eax                #eax=offs ds cur_area
   shl   $3,%eax
   add   %edi,%eax                           #eax=pointeur sur place

   mov   PLACE_MESH(%eax),%edx               #edx=pointeur sur mesh
   cmp   $0,%edx                             #la place est-elle un mur?
   je    skip_6                              #oui
   mov   PLACE_FIGHTER(%eax),%edx            #edx=pointeur sur fighter
   cmp   $0,%edx                             #la place est-elle libre?
   jne   skip_6                              #non

/*------------------------------------------------------------------*/
/* deplacement dans direction associee a p3                         */
/*------------------------------------------------------------------*/

   movl  $0,%ds:PLACE_FIGHTER(%edi)          #on vide dans cur_area
   mov   %esi,%ds:PLACE_FIGHTER(%eax)        #nouvelle place

   add   ARG_FIGHTER_MOVE_XY(%ebp),%ecx      #offset ds move_xy
   mov   OFFSET_P3(%ecx),%edx                #on charge l increm
   addl  %edx,%ds:FIGHTER_XY(%esi)           #addition des coord

   mov   ARG_TEMP2(%ebp),%ecx                #y*cur_area_w+x
   mov   ARG_CURRENT_AREA_BACK(%ebp),%eax    #eax=cur_area_back->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dl                      #pixel memorise
   mov   ARG_CURRENT_AREA_DISP(%ebp),%eax    #eax=cur_area_disp->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dh                      #couleur du monstre
   movb  %dl,%ds:(%eax)                      #effacement
   add   OFFSET_P3(%ebx),%eax                #deplacement
   movb  %dh,%ds:(%eax)                      #affichage

   jmp   next_monster                        #fini avec ce monstre...

skip_6:

/*------------------------------------------------------------------*/
/* test si deplacement possible dans direction associee a p4        */
/*------------------------------------------------------------------*/

   mov   %ecx,%ebx                           #on recupere la dir
   add   ARG_FIGHTER_MOVE_OFFSET(%ebp),%ebx  #ebx=pointeur a garder!!!
   mov   OFFSET_P4(%ebx),%eax                #eax=offs ds cur_area
   shl   $3,%eax
   add   %edi,%eax                           #eax=pointeur sur place

   mov   PLACE_MESH(%eax),%edx               #edx=pointeur sur mesh
   cmp   $0,%edx                             #la place est-elle un mur?
   je    skip_7                              #oui
   mov   PLACE_FIGHTER(%eax),%edx            #edx=pointeur sur fighter
   cmp   $0,%edx                             #la place est-elle libre?
   jne   skip_7                              #non

/*------------------------------------------------------------------*/
/* deplacement dans direction associee a p4                         */
/*------------------------------------------------------------------*/

   movl  $0,%ds:PLACE_FIGHTER(%edi)          #on vide dans cur_area
   mov   %esi,%ds:PLACE_FIGHTER(%eax)        #nouvelle place

   add   ARG_FIGHTER_MOVE_XY(%ebp),%ecx      #offset ds move_xy
   mov   OFFSET_P4(%ecx),%edx                #on charge l increm
   addl  %edx,%ds:FIGHTER_XY(%esi)           #addition des coord

   mov   ARG_TEMP2(%ebp),%ecx                #y*cur_area_w+x
   mov   ARG_CURRENT_AREA_BACK(%ebp),%eax    #eax=cur_area_back->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dl                      #pixel memorise
   mov   ARG_CURRENT_AREA_DISP(%ebp),%eax    #eax=cur_area_disp->dat
   add   %ecx,%eax                           #pointe sur pixel
   movb  %ds:(%eax),%dh                      #couleur du monstre
   movb  %dl,%ds:(%eax)                      #effacement
   add   OFFSET_P4(%ebx),%eax                #deplacement
   movb  %dh,%ds:(%eax)                      #affichage

   jmp   next_monster                        #fini avec ce monstre...

skip_7:

/*------------------------------------------------------------------*/
/* test si attaque dans direction associee a p0                     */
/*------------------------------------------------------------------*/

   mov   ARG_TEMP5(%ebp),%eax                #mega astuce!!!
   shl   $3,%eax
   add   %edi,%eax                           #eax=pointeur sur place

   mov   PLACE_MESH(%eax),%edx               #edx=pointeur sur mesh
   cmp   $0,%edx                             #la place est-elle un mur?
   je    skip_8                              #oui, fini
   mov   PLACE_FIGHTER(%eax),%edx            #edx=pointeur sur fighter
   cmp   $0,%edx                             #y-a-t-il un streum?
   je    skip_8                              #non, fini
   movb  ARG_TEMP1(%ebp),%al                 #recupere team
   cmpb  FIGHTER_TEAM(%edx),%al              #compare
   je    skip_8                              #les memes...

/*------------------------------------------------------------------*/
/* attaque ds dir associee a p0                                     */
/*------------------------------------------------------------------*/

   xor   %ecx,%ecx                   # ecx a 0
   mov   %al,%cl                     # ecx=al
   shl   $2,%cl                      # ecx=team*4
   mov   ARG_ATTACK(%ebp),%ebx       # ebx=attack
   add   %ecx,%ebx                   # ebx=attack[team]
   movw  FIGHTER_HEALTH(%edx),%ax    # ax=health du voisin
   subw  (%ebx),%ax                  # attaque!!!
   jg    no_change_team_0            # change de camp???
   mov   ARG_NEW_HEALTH(%ebp),%ebx   # ebx=new_health
   add   %ecx,%ebx                   # ebx=new_health[team]
   movw  (%ebx),%bx                  # bx=valeur de n_h[team]
new_health_loop_0:
   add   %bx,%ax                       # health+=new_health
   jl    new_health_loop_0             # tant que health<0
   mov   ARG_MAX_FIGHTER_HEALTH_1(%ebp),%bx
   cmpw  %bx,%ax                       # health>max_health-1?
   jle   not_to_much_new_health_0
   mov   %bx,%ax                       # health=max_health-1
not_to_much_new_health_0:
   shr   $2,%cl
   movb  %cl,FIGHTER_TEAM(%edx)        # changement de team
   shl   $2,%cl                        # en memoire
   jmp   display_attacked_fighter_0

no_change_team_0:
   xor   %ecx,%ecx                     # ecx=0
   movb  FIGHTER_TEAM(%edx),%cl        # cl=team
   shl   $2,%cl                        # ecx=team*4

display_attacked_fighter_0:

   movw  %ax,FIGHTER_HEALTH(%edx)           # health en mem

   cwde                                     # eax=ax
   mov   ARG_COLORS_PER_TEAM(%ebp),%ebx     # bx=colors
   cdq
   mull  %ebx                                # eax*=colors
   mov   ARG_MAX_FIGHTER_HEALTH_1(%ebp),%ebx # bx=max_health
   inc   %ebx
   cdq
   divl  %ebx                                # eax/=max_health
   mov   ARG_COLOR_FIRST_ENTRY(%ebp),%ebx    # ebx=cfe
   add   %ecx,%ebx                           # ebx=cfe+team
   addb  (%ebx),%al                          # eax=color

   mov   ARG_CURRENT_AREA_DISP(%ebp),%ebx # ecx=cur_area_disp>dat
   add   ARG_TEMP2(%ebp),%ebx             # y*cur_area_w+x
   add   ARG_TEMP5(%ebp),%ebx                #on recupere la dir

   movb  %al,(%ebx)

   jmp   next_monster
skip_8:

/*------------------------------------------------------------------*/
/* test si attaque dans direction associee a p1                     */
/*------------------------------------------------------------------*/

   mov   ARG_TEMP6(%ebp),%eax                #mega astuce!!!
   shl   $3,%eax
   add   %edi,%eax                           #eax=pointeur sur place

   mov   PLACE_MESH(%eax),%edx               #edx=pointeur sur mesh
   cmp   $0,%edx                             #la place est-elle un mur?
   je    skip_9                              #oui, fini
   mov   PLACE_FIGHTER(%eax),%edx            #edx=pointeur sur fighter
   cmp   $0,%edx                             #y-a-t-il un streum?
   je    skip_9                              #non, fini
   movb  ARG_TEMP1(%ebp),%al                 #recupere team
   cmpb  FIGHTER_TEAM(%edx),%al              #compare
   je    skip_9                              #les memes...

/*------------------------------------------------------------------*/
/* attaque ds dir associee a p1                                     */
/*------------------------------------------------------------------*/

   xor   %ecx,%ecx                   # ecx a 0
   mov   %al,%cl                     # ecx=al
   shl   $2,%cl                      # ecx=team*4
   mov   ARG_ATTACK(%ebp),%ebx       # ebx=attack
   add   %ecx,%ebx                   # ebx=attack[team]
   movw  FIGHTER_HEALTH(%edx),%ax    # ax=health du voisin
   mov   (%ebx),%bx                  # bx=force d attaque
   shr   $SIDE_ATTACK_FACTOR,%bx     # force/ car
                                     # attaque sur le cote
   subw  %bx,%ax                     # attaque!!!
   jg    no_change_team_1            # change de camp???
   mov   ARG_NEW_HEALTH(%ebp),%ebx   # ebx=new_health
   add   %ecx,%ebx                   # ebx=new_health[team]
   movw  (%ebx),%bx                  # bx=valeur de n_h[team]
new_health_loop_1:
   add   %bx,%ax                       # health+=new_health
   jl    new_health_loop_1             # tant que health<0
   mov   ARG_MAX_FIGHTER_HEALTH_1(%ebp),%bx
   cmpw  %bx,%ax                       # health>max_health-1?
   jle   not_to_much_new_health_1
   mov   %bx,%ax                       # health=max_health-1
not_to_much_new_health_1:
   shr   $2,%cl
   movb  %cl,FIGHTER_TEAM(%edx)        # changement de team
   shl   $2,%cl                        # en memoire
   jmp   display_attacked_fighter_1

no_change_team_1:
   xor   %ecx,%ecx                     # ecx=0
   movb  FIGHTER_TEAM(%edx),%cl        # cl=team
   shl   $2,%cl                        # ecx=team*4

display_attacked_fighter_1:

   movw  %ax,FIGHTER_HEALTH(%edx)           # health en mem

   cwde                                     # eax=ax
   mov   ARG_COLORS_PER_TEAM(%ebp),%ebx     # bx=colors
   cdq
   mull  %ebx                                # eax*=colors
   mov   ARG_MAX_FIGHTER_HEALTH_1(%ebp),%ebx # bx=max_health
   inc   %ebx
   cdq
   divl  %ebx                                # eax/=max_health
   mov   ARG_COLOR_FIRST_ENTRY(%ebp),%ebx    # ebx=cfe
   add   %ecx,%ebx                           # ebx=cfe+team
   addb  (%ebx),%al                          # eax=color

   mov   ARG_CURRENT_AREA_DISP(%ebp),%ebx # ecx=cur_area_disp>dat
   add   ARG_TEMP2(%ebp),%ebx             # y*cur_area_w+x
   add   ARG_TEMP6(%ebp),%ebx                #on recupere la dir

   movb  %al,(%ebx)

   jmp   next_monster
skip_9:

/*------------------------------------------------------------------*/
/* test si attaque dans direction associee a p2                     */
/*------------------------------------------------------------------*/

   mov   ARG_TEMP6(%ebp),%eax                #mega astuce!!!
   shl   $3,%eax
   add   %edi,%eax                           #eax=pointeur sur place

   mov   PLACE_MESH(%eax),%edx               #edx=pointeur sur mesh
   cmp   $0,%edx                             #la place est-elle un mur?
   je    skip_10                              #oui, fini
   mov   PLACE_FIGHTER(%eax),%edx            #edx=pointeur sur fighter
   cmp   $0,%edx                             #y-a-t-il un streum?
   je    skip_10                              #non, fini
   movb  ARG_TEMP1(%ebp),%al                 #recupere team
   cmpb  FIGHTER_TEAM(%edx),%al              #compare
   je    skip_10                              #les memes...

/*------------------------------------------------------------------*/
/* attaque ds dir associee a p2                                     */
/*------------------------------------------------------------------*/

   xor   %ecx,%ecx                   # ecx a 0
   mov   %al,%cl                     # ecx=al
   shl   $2,%cl                      # ecx=team*4
   mov   ARG_ATTACK(%ebp),%ebx       # ebx=attack
   add   %ecx,%ebx                   # ebx=attack[team]
   movw  FIGHTER_HEALTH(%edx),%ax    # ax=health du voisin
   mov   (%ebx),%bx                  # bx=force d attaque
   shr   $SIDE_ATTACK_FACTOR,%bx     # force/ car
                                     # attaque sur le cote
   subw  %bx,%ax                     # attaque!!!
   jg    no_change_team_2            # change de camp???
   mov   ARG_NEW_HEALTH(%ebp),%ebx   # ebx=new_health
   add   %ecx,%ebx                   # ebx=new_health[team]
   movw  (%ebx),%bx                  # bx=valeur de n_h[team]
new_health_loop_2:
   add   %bx,%ax                       # health+=new_health
   jl    new_health_loop_2             # tant que health<0
   mov   ARG_MAX_FIGHTER_HEALTH_1(%ebp),%bx
   cmpw  %bx,%ax                       # health>max_health-1?
   jle   not_to_much_new_health_2
   mov   %bx,%ax                       # health=max_health-1
not_to_much_new_health_2:
   shr   $2,%cl
   movb  %cl,FIGHTER_TEAM(%edx)        # changement de team
   shl   $2,%cl                        # en memoire
   jmp   display_attacked_fighter_2

no_change_team_2:
   xor   %ecx,%ecx                     # ecx=0
   movb  FIGHTER_TEAM(%edx),%cl        # cl=team
   shl   $2,%cl                        # ecx=team*4

display_attacked_fighter_2:

   movw  %ax,FIGHTER_HEALTH(%edx)           # health en mem

   cwde                                     # eax=ax
   mov   ARG_COLORS_PER_TEAM(%ebp),%ebx     # bx=colors
   cdq
   mull  %ebx                                # eax*=colors
   mov   ARG_MAX_FIGHTER_HEALTH_1(%ebp),%ebx # bx=max_health
   inc   %ebx
   cdq
   divl  %ebx                                # eax/=max_health
   mov   ARG_COLOR_FIRST_ENTRY(%ebp),%ebx    # ebx=cfe
   add   %ecx,%ebx                           # ebx=cfe+team
   addb  (%ebx),%al                          # eax=color

   mov   ARG_CURRENT_AREA_DISP(%ebp),%ebx # ecx=cur_area_disp>dat
   add   ARG_TEMP2(%ebp),%ebx             # y*cur_area_w+x
   add   ARG_TEMP6(%ebp),%ebx                #on recupere la dir

   movb  %al,(%ebx)

   jmp   next_monster
skip_10:

/*------------------------------------------------------------------*/
/* test si regeneration dans direction associee a p0                */
/*------------------------------------------------------------------*/
   mov   ARG_TEMP5(%ebp),%eax                #mega astuce!!!
   shl   $3,%eax
   add   %edi,%eax                           #eax=pointeur sur place

   mov   PLACE_MESH(%eax),%edx               #edx=pointeur sur mesh
   cmp   $0,%edx                             #la place est-elle un mur?
   je    skip_11                             #oui, fini
   mov   PLACE_FIGHTER(%eax),%edx            #edx=pointeur sur fighter
   cmp   $0,%edx                             #y-a-t-il un streum?
   je    skip_11                             #non, fini
   movb  ARG_TEMP1(%ebp),%al                 #recupere team
   cmpb  FIGHTER_TEAM(%edx),%al              #compare
   jne   skip_11                             #les memes...

/*------------------------------------------------------------------*/
/* regeneration ds dir associee a p0                                     */
/*------------------------------------------------------------------*/
   xor   %ecx,%ecx                   # ecx a 0
   mov   %al,%cl                     # ecx=al
   shl   $2,%cl                      # ecx=team*4
   mov   ARG_DEFENSE(%ebp),%ebx      # ebx=defense
   add   %ecx,%ebx                   # ebx=defense[team]
   movw  FIGHTER_HEALTH(%edx),%ax    # ax=health du voisin
   addw  (%ebx),%ax                  # regeneration!!!
   mov   ARG_MAX_FIGHTER_HEALTH_1(%ebp),%bx
   cmpw  %bx,%ax                       # health>max_health?
   jle   not_to_much_defense
   mov   %bx,%ax
not_to_much_defense:
   jmp   display_attacked_fighter_0
skip_11:

/*------------------------------------------------------------------*/
/* fin de la boucle, on passe a la suite                            */
/*------------------------------------------------------------------*/
next_monster:
   add   $SIZE_OF_FIGHTER,%esi # on passe au streumon suivant

   pop  %ecx
   dec  %ecx
   je   the_end
   jmp  main_loop

the_end:

   popw %gs
   popw %fs
   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret                              # retour au c


