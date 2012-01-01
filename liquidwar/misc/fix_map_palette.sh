#!/bin/sh

#############################################################################
# Liquid War is a multiplayer wargame                                       #
# Copyright (C) 1998-2012 Christian Mauduit                                 #
#                                                                           #
# This program is free software; you can redistribute it and/or modify      #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation; either version 2 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with this program; if not, write to the Free Software               #
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA #
#                                                                           #
# Liquid War homepage : http://www.ufoot.org/liquidwar/v5                   #
# Contact author      : ufoot@ufoot.org                                     #
#############################################################################

# Dirty hack based on Image Magick to make maps more "liquidwarmap"
# friendly. Indeed, we want for instance _all_ the black zones to
# have index 0. Using Image Magick can help achieving this.
#
# The script processes .bmp files only since processing .pcx files
# seems to wreck them up - from Gimp's point of view at least.
#
# Use this script with caution.

#for f in ../data/map/*.bmp ; do
for f in $* ; do
    f="../data/map/$f"
    echo $f
    mogrify -colors 256 $f
    #mogrify -map ../data/map/world1.bmp $f
    mogrify -map $f $f
done

