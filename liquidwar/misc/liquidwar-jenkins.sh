#!/bin/sh

#############################################################################
# Liquid War is a multiplayer wargame                                       #
# Copyright (C) 1998-2013 Christian Mauduit                                 #
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

# Script used by Jenkins daemon to autobuild the program in
# continuous integration mode. http://jenkins-ci.org/

echo "******** $0 $(date) ********"
if cd liquidwar ; then
    echo "******** $0 $(date) ********"
    if ./configure --prefix=$WORKSPACE/local ; then
	echo "******** $0 $(date) ********"
	if make ; then
	    echo "******** $0 $(date) ********"
	    if make install; then
		echo "******** $0 $(date) ********"
	    else
		echo "make install failed"
		exit 4
	    fi
	else
	    echo "make failed"
	    exit 3
	fi
    else
	echo "./configure failed"
	exit 2
    fi
else
    echo "cd failed"
    exit 1
fi

echo "OK"
exit 0
