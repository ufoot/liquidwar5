// Persistence of Vision Ray Tracer Scene Description File for Liquid War icon.// Copyright (C) 2003  Ryan Brown

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA.
#version 3.5;#include "colors.inc"#include "textures.inc"#include "glass.inc"//#include "woods.inc"global_settings {  assumed_gamma 1.2  max_trace_level 5}// ----------------------------------------camera {//  orthographic  location  <0.0, 10, -20.0>  direction 1.59*z  right     x*image_width/image_height  look_at   1.6*<0.0, 3.0,  0.0>}#declare BGColor = Gray50;background{Gray35}light_source {  30*<1, 5, -3>            // light's position (translated below)  color rgb <1, 1, 1>  // light's color  area_light <10, 0, 0>, <0, 0, 10>, 5, 5  adaptive 1  jitter}// ----------------------------------------#declare Globe = sphere {<0,6,0>,5}#declare Ring = torus {	4.001,1.001	translate 2*y		}#declare Base = difference {	cylinder {0,2*y,4}	object { Ring }	object { Globe }}object {	Base	texture { 		DMFDarkOak		rotate 45*<1,1,0>		}	}union {object {	Globe	//material { M_Glass4 }	//material {M_NB_Old_Glass}	texture { T_Glass4 }	interior { I_Glass }}blob {	threshold .6	sphere { <-1.5,7.5,0>, 2.5, .75 pigment { Red } }	sphere { <0,7.5,1.5>, 2.5, .5 pigment { Red } }	sphere { <-1.5,6,-1.5>, 2.5, .5 pigment {Red} }	sphere { <.75, 6, -.75>, 2.5, .25 pigment {Yellow}}	sphere { <1.5, 4.5,0>, 2.5, .75 pigment { Yellow } }	sphere { <0,4.5,-1.5>, 2.5, .5 pigment { Yellow } }	sphere { <1.5,6,1.5>, 2.5, .5 pigment {Yellow} }	sphere { <-.75, 6, .75>, 2.5, .25 pigment {Red}}}}