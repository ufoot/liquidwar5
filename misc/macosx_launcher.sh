#!/bin/sh
# Application bundle launcher for Liquid War
# Copyright (C) 2003 Ryan Brown

#This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA. 
# prefs="$HOME/Library/Preferences/liquidwar.cfg"
resources=`dirname "$0"`
resources=`dirname "$resources"`
resources="$resources/Resources"
prefs=~/Library/Preferences/liquidwar.cfg
parameters="-cfg $prefs -dat liquidwar.dat -map map -tex texture -mid music"
liquidwar="$resources/liquidwar"
# Becuase I can't get liquidwar to find the files when they
# have a space in their filename.
cd "$resources"

if [ "x`ps ax|grep -e \\\\besd\\\\b`" = 'x' ] ; then 
    if [ -d /sw/bin ]; then
	PATH=$PATH:$d
    fi
    export PATH
    esd=`which esd`
    if [ "x$esd" = "x" ] ; then 
	:
    else
	($esd -nobeeps) &
	esdpid=$!
    fi
fi

for d in /usr/X11R6/bin /usr/bin/X11 /usr/local/bin/X11; do
	PATH=$PATH:$d
done
export PATH
case `ps ax` in
    *X11*)
	xrunning=true
	;;
    *XDarwin*)
	xrunning=true
	;;
    *)
	xrunning=0
	;;
esac
if [ $xrunning = true ] ; then
    if [ "x$DISPLAY" = "x" ]; then
	DISPLAY=":0"
	export DISPLAY
    fi
    "$liquidwar" $parameters
else
    for server in "/Applications/X11.app/Contents/MacOS/X11" "/Applications/XDarwin.app/Contents/MacOS/XDarwin" "$HOME/Applications/X11.app/Contents/MacOS/X11" "$HOME/Applications/XDarwin.app/Contents/MacOS/XDarwin" `which X` ;
      do
      if [ -x "$server" ] ; then
	  echo running \"xinit \"$liquidwar\" $parameters -- $server\"
	  xinit "$liquidwar" $parameters -- $server
	  if [ "x$esdpid" = "x" ]; then
	      exit 0
	  else
	      kill $esdpid
	  fi
	  exit 0
      else
	  echo $server isn\'t executable
      fi
    done
fi
if [ "x$esdpid" = "x" ]; then
    exit 0
else
    kill $esdpid
fi
exit 0
