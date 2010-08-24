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
/* nom           : glouglou.s                                       */
/* contenu       : affichage avec vagues accelere                   */
/* date de modif : 3 mai 98                                         */
/********************************************************************/

.equ BMP_W,                 0
.equ BMP_H,                 4
.equ BMP_CLIP,              8
.equ BMP_CL,                12
.equ BMP_CR,                16
.equ BMP_CT,                20
.equ BMP_CB,                24
.equ BMP_VTABLE,            28
.equ BMP_WBANK,             32
.equ BMP_RBANK,             36
.equ BMP_DAT,               40
.equ BMP_ID,                44
.equ BMP_EXTRA,             48
.equ BMP_XOFS,              52
.equ BMP_YOFS,              56
.equ BMP_SEG,               60
.equ BMP_LINE,              64

.equ VTABLE_UNBANK,          8

.equ ARG_DISTORSION_TARGET,            8
.equ ARG_CURRENT_AREA_W,        12
.equ ARG_W,                     16
.equ ARG_Y,                     20
.equ ARG_FP_X,                  24
.equ ARG_IP_X,                  28
.equ ARG_FP_Y,                  32
.equ ARG_IP_Y,                  36
.equ ARG_LIM_W,                 40
.equ ARG_LIM_H,                 44
.equ ARG_FP_X0,                 48
.equ ARG_Y_CORRES,              52
.equ ARG_WAVE_SHAPE_WX,         56
.equ ARG_WAVE_SHAPE_X_CORRES,   60
.equ ARG_SRC,                   64
.equ ARG_TEMP1,                 68            # pointeur debut ligne
.equ ARG_TEMP2,                 72            # pointeur sur fc aff
.equ ARG_TEMP3,                 76
.equ ARG_TEMP4,                 80
.equ ARG_TEMP5,                 84

.text


/*==================================================================*/
/* affichage de jolies distorsions                                  */
/*==================================================================*/

/*------------------------------------------------------------------*/
/* void draw_distor_line (BITMAP *distorsion_target,                */
/*                          int current_area_w,                     */
/*                          int w,                                  */
/*                          int y,                                  */
/*                          int fp_x,                               */
/*                          int ip_x,                               */
/*                          int fp_y,                               */
/*                          int ip_y,                               */
/*                          int lim_w,                              */
/*                          int lim_h,                              */
/*                          int *fp_x0,                             */
/*                          int *y_corres,                          */
/*                          int *WAVE_SHAPE_WX,                     */
/*                          int **WAVE_SHAPE_X_CORRES,              */
/*                          char *src,                              */
/*                          int temp1,                              */
/*                          int temp2,                              */
/*                          int temp3,                              */
/*                          int temp4,                              */
/*                          int temp5);                             */
/*------------------------------------------------------------------*/
.globl __draw_distor_line 
.globl _draw_distor_line
.globl draw_distor_line
   .align 4
__draw_distor_line:
_draw_distor_line:
draw_distor_line:
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

   mov   ARG_SRC(%ebp),%esi
   mov   ARG_CURRENT_AREA_W(%ebp),%edi

   movl ARG_DISTORSION_TARGET(%ebp), %edx               # edx = bmp
   movl ARG_Y(%ebp), %eax                               # eax = y
   movl BMP_LINE(%edx, %eax, 4), %eax
   movl %eax,ARG_TEMP1(%ebp)               # ARG_TEMP1 = ptr debut ligne

   xor   %ecx,%ecx                # ecx va servir pour stocker x
main_loop:

   # putpixel (DISTORSION_TARGET, x,y *src)
	
   movl %ds:(%esi),%ebx              # bl = color
   movl ARG_TEMP1(%ebp), %eax
   movb %bl, %ds:(%eax, %ecx)    # store the pixel
	
   mov   ARG_WAVE_SHAPE_X_CORRES(%ebp),%eax #eax=wave_shape_x_corres
   mov   %ds:(%eax,%ecx,4),%eax   # eax=wave_shape_x_corres[x]
   mov   ARG_Y(%ebp),%ebx         # ebx=y
   mov   %ds:(%eax,%ebx,4),%eax   # eax=wave_shape_x_corres[x][y]
   mov   ARG_FP_X0(%ebp),%ebx     # ebx=arg_fp_x0
   mov   %ds:(%ebx,%ecx,4),%edx   # edx=arg_fp_x0[x]
   add   %eax,%edx                # edx+=eax
   mov   %edx,%ds:(%ebx,%ecx,4)   # on memorise en ram
   neg   %edx

   # a ce stade edx contient -reste_x

   mov   ARG_FP_X(%ebp),%eax              # eax=fp_x
   mov   ARG_WAVE_SHAPE_WX(%ebp),%ebx     # ebx=wave_shape_wx
   add   %ds:(%ebx,%ecx,4),%eax           # fp_x+=wave_shape_wx[x]

   mov   ARG_LIM_W(%ebp),%ebx             # ebx=lim_w
loop_while_1:
   cmp   %edx,%eax                        # fp_x<-reste_x
   jnl   exit_while_1
   add   %ebx,%eax                        # fp_x+=lim_w
   dec   %esi	                          # src--
   jmp   loop_while_1
exit_while_1:

   add   %ebx,%edx
loop_while_2:
   cmp   %edx,%eax                        # fp_x>=-reste_x+lim_w
   jnge  exit_while_2
   sub   %ebx,%eax                        # fp_x-=lim_w
   inc   %esi                             # src++
   jmp   loop_while_2
exit_while_2:

   mov   %eax,ARG_FP_X(%ebp)               # on sauve fp_x

   mov   ARG_FP_Y(%ebp),%eax               # eax=fp_y
   mov   ARG_Y_CORRES(%ebp),%ebx           # ebx=y_corres
   add   %ds:(%ebx,%ecx,4),%eax            # fp_y+=y_corres[x]

   mov   ARG_LIM_H(%ebp),%ebx             # ebx=lim_h
loop_while_3:
   cmp   $0,%eax                          # fp_y<0
   jnl   exit_while_3
   add   %ebx,%eax                        # fp_x+=lim_h
   sub   %edi,%esi                        # src-=current_area_w
   jmp   loop_while_3
exit_while_3:

loop_while_4:
   cmp   %ebx,%eax                        # fp_y>=lim_h
   jnge  exit_while_4
   sub   %ebx,%eax                        # fp_y-=lim_h
   add   %edi,%esi                        # src+=current_area_w
   jmp   loop_while_4
exit_while_4:

   mov   %eax,ARG_FP_Y(%ebp)               # on sauve fp_y

/*   mov   ARG_TEMP1(%ebp),%ecx    # ecx=x         */
   incl  %ecx                    # x++
   cmp   ARG_W(%ebp),%ecx        # x!=w ?
   je    the_end
   jmp   main_loop

the_end:

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

/*------------------------------------------------------------------*/
/* affichage pour bitmap lineaire, golee dans allegro               */
/*------------------------------------------------------------------*/
linear_putpixel8_for_distor_line:


   ret

