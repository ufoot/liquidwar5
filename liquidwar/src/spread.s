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
/* Copyright (C) 1998-2016 Christian Mauduit                                 */
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
/* nom           : spread.s                                         */
/* contenu       : etalement du gradient accelere                   */
/* date de modif : 3 mai 98                                         */
/********************************************************************/


/*typedef union                 */
/*        {                     */
/*         int time;            */
/*         struct               */
/*          {                   */
/*           short x;           */
/*           short y;           */
/*          } cursor;           */
/*        } MESH_UPDATE;        */

/*typedef struct                */
/*        {                     */
/*         int dir :8;          */
/*         int grad:24;         */
/*        } MESH_STATE;         */

/*typedef struct                  */
/*        {                       */
/*         MESH_UPDATE update;    */
/*         MESH_STATE state;      */
/*        } MESH_INFO;            */

/*typedef struct                     */
/*        {                          */
/*         char decal_for_dir :8;    */
/*         int  size          :24;   */
/*        } MESH_SIDE;               */

/*typedef struct                        */
/*        {                             */
/*         short x;                     */
/*         short y;                     */
/*         MESH_SIDE side;              */
/*         MESH_INFO info[NB_TEAMS];    */
/*         void *link[NB_DIRS];         */
/*        } MESH;                       */


.equ SIZE_OF_MESH,104

.equ OFFSET_TO_GRAD_0,12
.equ OFFSET_TO_GRAD_1,20
.equ OFFSET_TO_GRAD_2,28
.equ OFFSET_TO_GRAD_3,36
.equ OFFSET_TO_GRAD_4,44
.equ OFFSET_TO_GRAD_5,52

.equ SQUARE_SIZE_INC,4

.equ ARG_FIRST,8
.equ ARG_SIZE,12
.equ ARG_OFFSET,16

.text

/*  il y a de maniere tres bourrine 5*2 procedures attachees chacune   */
/*  a l'etalement de 2,3,4,5 ou 6 gradients, le code est le meme       */
/*  dans chacune d'elle a l'exception d'une constante.                 */
/* la moitie des fonctions etale les gradients dans un sens,           */
/*  l'autre moitie les etale dans l'autre sens                         */


/*==================================================================*/
/* premiere serie de fonctions, etalement des gradients vers le bas */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* void boost_gradient_down_2 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_down_2
.globl boost_gradient_down_2
   .align 4
_boost_gradient_down_2:
boost_gradient_down_2:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gd_2:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gd_2_1

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax     /* GPF sous win ICI */
   jae   skip_gd_2_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gd_2_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gd_2_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gd_2_1:

   addl  $SIZE_OF_MESH,%esi

   loopl main_loop_gd_2

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret

/*------------------------------------------------------------------*/
/* void boost_gradient_down_3 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_down_3
.globl boost_gradient_down_3
   .align 4
_boost_gradient_down_3:
boost_gradient_down_3:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gd_3:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gd_3_2

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax
   jae   skip_gd_3_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gd_3_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gd_3_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gd_3_1:

   movl  OFFSET_TO_GRAD_2(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_2(%edi),%eax
   jae   skip_gd_3_2
   movl  %eax,%ds:OFFSET_TO_GRAD_2(%edi)

skip_gd_3_2:

   addl  $SIZE_OF_MESH,%esi

   loopl main_loop_gd_3

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret

/*------------------------------------------------------------------*/
/* void boost_gradient_down_4 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_down_4
.globl boost_gradient_down_4
   .align 4
_boost_gradient_down_4:
boost_gradient_down_4:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gd_4:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gd_4_3

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax
   jae   skip_gd_4_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gd_4_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gd_4_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gd_4_1:

   movl  OFFSET_TO_GRAD_2(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_2(%edi),%eax
   jae   skip_gd_4_2
   movl  %eax,%ds:OFFSET_TO_GRAD_2(%edi)

skip_gd_4_2:

   movl  OFFSET_TO_GRAD_3(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_3(%edi),%eax
   jae   skip_gd_4_3
   movl  %eax,%ds:OFFSET_TO_GRAD_3(%edi)

skip_gd_4_3:

   addl  $SIZE_OF_MESH,%esi

   loopl main_loop_gd_4

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret

/*------------------------------------------------------------------*/
/* void boost_gradient_down_5 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_down_5
.globl boost_gradient_down_5
   .align 4
_boost_gradient_down_5:
boost_gradient_down_5:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gd_5:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gd_5_4

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax
   jae   skip_gd_5_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gd_5_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gd_5_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gd_5_1:

   movl  OFFSET_TO_GRAD_2(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_2(%edi),%eax
   jae   skip_gd_5_2
   movl  %eax,%ds:OFFSET_TO_GRAD_2(%edi)

skip_gd_5_2:

   movl  OFFSET_TO_GRAD_3(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_3(%edi),%eax
   jae   skip_gd_5_3
   movl  %eax,%ds:OFFSET_TO_GRAD_3(%edi)

skip_gd_5_3:

   movl  OFFSET_TO_GRAD_4(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_4(%edi),%eax
   jae   skip_gd_5_4
   movl  %eax,%ds:OFFSET_TO_GRAD_4(%edi)

skip_gd_5_4:

   addl  $SIZE_OF_MESH,%esi

   loopl main_loop_gd_5

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret


/*------------------------------------------------------------------*/
/* void boost_gradient_down_6 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_down_6
.globl boost_gradient_down_6
   .align 4
_boost_gradient_down_6:
boost_gradient_down_6:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gd_6:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gd_6_5

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax
   jae   skip_gd_6_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gd_6_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gd_6_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gd_6_1:

   movl  OFFSET_TO_GRAD_2(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_2(%edi),%eax
   jae   skip_gd_6_2
   movl  %eax,%ds:OFFSET_TO_GRAD_2(%edi)

skip_gd_6_2:

   movl  OFFSET_TO_GRAD_3(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_3(%edi),%eax
   jae   skip_gd_6_3
   movl  %eax,%ds:OFFSET_TO_GRAD_3(%edi)

skip_gd_6_3:

   movl  OFFSET_TO_GRAD_4(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_4(%edi),%eax
   jae   skip_gd_6_4
   movl  %eax,%ds:OFFSET_TO_GRAD_4(%edi)

skip_gd_6_4:

   movl  OFFSET_TO_GRAD_5(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_5(%edi),%eax
   jae   skip_gd_6_5
   movl  %eax,%ds:OFFSET_TO_GRAD_5(%edi)

skip_gd_6_5:

   addl  $SIZE_OF_MESH,%esi

   loopl main_loop_gd_6

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret


/*==================================================================*/
/* deuxieme serie de fonctions, etalement des gradients vers le haut*/
/*==================================================================*/

/*------------------------------------------------------------------*/
/* void boost_gradient_up_2 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_up_2
.globl boost_gradient_up_2
   .align 4
_boost_gradient_up_2:
boost_gradient_up_2:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gu_2:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gu_2_1

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax
   jae   skip_gu_2_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gu_2_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gu_2_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gu_2_1:

   subl  $SIZE_OF_MESH,%esi

   loopl main_loop_gu_2

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret

/*------------------------------------------------------------------*/
/* void boost_gradient_up_3 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_up_3
.globl boost_gradient_up_3
   .align 4
_boost_gradient_up_3:
boost_gradient_up_3:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gu_3:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gu_3_2

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax
   jae   skip_gu_3_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gu_3_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gu_3_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gu_3_1:

   movl  OFFSET_TO_GRAD_2(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_2(%edi),%eax
   jae   skip_gu_3_2
   movl  %eax,%ds:OFFSET_TO_GRAD_2(%edi)

skip_gu_3_2:

   subl  $SIZE_OF_MESH,%esi

   loopl main_loop_gu_3

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret

/*------------------------------------------------------------------*/
/* void boost_gradient_up_4 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_up_4
.globl boost_gradient_up_4
   .align 4
_boost_gradient_up_4:
boost_gradient_up_4:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gu_4:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gu_4_3

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax
   jae   skip_gu_4_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gu_4_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gu_4_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gu_4_1:

   movl  OFFSET_TO_GRAD_2(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_2(%edi),%eax
   jae   skip_gu_4_2
   movl  %eax,%ds:OFFSET_TO_GRAD_2(%edi)

skip_gu_4_2:

   movl  OFFSET_TO_GRAD_3(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_3(%edi),%eax
   jae   skip_gu_4_3
   movl  %eax,%ds:OFFSET_TO_GRAD_3(%edi)

skip_gu_4_3:

   subl  $SIZE_OF_MESH,%esi

   loopl main_loop_gu_4

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret

/*------------------------------------------------------------------*/
/* void boost_gradient_up_5 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_up_5
.globl boost_gradient_up_5
   .align 4
_boost_gradient_up_5:
boost_gradient_up_5:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gu_5:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gu_5_4

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax
   jae   skip_gu_5_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gu_5_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gu_5_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gu_5_1:

   movl  OFFSET_TO_GRAD_2(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_2(%edi),%eax
   jae   skip_gu_5_2
   movl  %eax,%ds:OFFSET_TO_GRAD_2(%edi)

skip_gu_5_2:

   movl  OFFSET_TO_GRAD_3(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_3(%edi),%eax
   jae   skip_gu_5_3
   movl  %eax,%ds:OFFSET_TO_GRAD_3(%edi)

skip_gu_5_3:

   movl  OFFSET_TO_GRAD_4(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_4(%edi),%eax
   jae   skip_gu_5_4
   movl  %eax,%ds:OFFSET_TO_GRAD_4(%edi)

skip_gu_5_4:

   subl  $SIZE_OF_MESH,%esi

   loopl main_loop_gu_5

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret


/*------------------------------------------------------------------*/
/* void boost_gradient_up_6 (MESH *first, int size, int offset);  */
/*------------------------------------------------------------------*/

.globl _boost_gradient_up_6
.globl boost_gradient_up_6
   .align 4
_boost_gradient_up_6:
boost_gradient_up_6:
   pushl %ebp
   movl %esp, %ebp
   pushl %eax
   pushl %ebx
   pushl %ecx
   pushl %edx
   pushl %esi
   pushl %edi
   pushw %es 

   movl  ARG_FIRST(%ebp),%esi      /* esi pointeur sur mesh courant*/
   movl  ARG_SIZE(%ebp),%ecx      /* ecx prend la taille de la boucle */
   movl  ARG_OFFSET(%ebp),%ebx    /* ebx prend l'offset de direction */

main_loop_gu_6:

   add   %ebx,%esi
   movl  (%esi),%edi
   sub   %ebx,%esi

   cmp   $0,%edi
   je    skip_gu_6_5

   movl  OFFSET_TO_GRAD_0(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_0(%edi),%eax
   jae   skip_gu_6_0
   movl  %eax,%ds:OFFSET_TO_GRAD_0(%edi)

skip_gu_6_0:

   movl  OFFSET_TO_GRAD_1(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_1(%edi),%eax
   jae   skip_gu_6_1
   movl  %eax,%ds:OFFSET_TO_GRAD_1(%edi)

skip_gu_6_1:

   movl  OFFSET_TO_GRAD_2(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_2(%edi),%eax
   jae   skip_gu_6_2
   movl  %eax,%ds:OFFSET_TO_GRAD_2(%edi)

skip_gu_6_2:

   movl  OFFSET_TO_GRAD_3(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_3(%edi),%eax
   jae   skip_gu_6_3
   movl  %eax,%ds:OFFSET_TO_GRAD_3(%edi)

skip_gu_6_3:

   movl  OFFSET_TO_GRAD_4(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_4(%edi),%eax
   jae   skip_gu_6_4
   movl  %eax,%ds:OFFSET_TO_GRAD_4(%edi)

skip_gu_6_4:

   movl  OFFSET_TO_GRAD_5(%esi),%edx
   movl  SQUARE_SIZE_INC(%esi),%eax
   add   %edx,%eax
   cmp   OFFSET_TO_GRAD_5(%edi),%eax
   jae   skip_gu_6_5
   movl  %eax,%ds:OFFSET_TO_GRAD_5(%edi)

skip_gu_6_5:

   subl  $SIZE_OF_MESH,%esi

   loopl main_loop_gu_6

   popw %es
   popl %edi
   popl %esi
   popl %edx
   popl %ecx
   popl %ebx
   popl %eax
/*   movl %ebp, %esp*/
   popl %ebp
   ret






