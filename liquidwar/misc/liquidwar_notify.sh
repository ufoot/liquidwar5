#!/bin/sh

#############################################################################
# Liquid War is a multiplayer wargame                                       #
# Copyright (C) 1998-2014 Christian Mauduit                                 #
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

# A basic script which might for instance be called whenever someone
# connects on your server, if you use the "-callback" option.

# For now this script simpy send a notification mail to a list of
# people. You have to parameter the list manually, or else it won't
# send anything to anyone. Be carefull to put email addresses that
# correspond to friends and/or people you know, and won't feel offensed
# by receiving everytime someone connects.

# You might also imagine to write a program that pops-up a window,
# or a windows .bat file that plays a sound or pops-up a window as well.
# In fact, you can put pretty much anything in that script. Simply put
# whatever you find a convenient way to be informed that someones is 
# ready to play.

HOSTNAME=`hostname`
DATE=`date`
SUBJECT="[liquidwar-bot] Connection"

#####################
# I M P O R T A N T #
#####################

# You need to uncomment the line bellow, and put the right email 
# addresses. Putting your address here would possibly be a good choice.

#RECEIVERS="ufoot@ufoot.org"

if test "$RECEIVERS" = "" ; then
    echo "You need to parameter this script first!"
    exit 1
fi

for r in $RECEIVERS; do
    echo "Sending notification to $r"
    mail -s "$SUBJECT" $r <<EOF
Hi,

This is the Liquid War server running on "$HOSTNAME".
Please notice that someone connected ($DATE).

He/she might be waiting for you to play with him. Ready for a game?

The Liquid War bot.

PS: please note that if you've received this mail it means that the
person running the server decided to activate the feature "send a mail 
whenever someone connects" and manually parametered it so that you are 
the receiver of the mail. Therefore the author(s) of Liquid War are 
absolutely not the authors of this mail, and also they probably do not
even know your e-mail. If you think this is spam, please try and find
out who's the sender by tracking down infos such as IP addresses. But be
sure that we (Liquid War team) are not responsible for this. Final dot.

EOF
done

