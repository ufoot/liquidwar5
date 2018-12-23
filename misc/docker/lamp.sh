# Liquid War is a multiplayer wargame
# Copyright (C) 1998-2018 Christian Mauduit
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# Liquid War homepage : https://ufoot.org/liquidwar/v5
# Contact author      : ufoot@ufoot.org

#!/bin/sh
if /etc/init.d/mysql start ; then
    if /etc/init.d/apache2 start ; then
        tail -f /var/log/mysql.log &
        tail -f /var/log/apache2/access-metaserver.log &
        while [ $(ps auxwww | grep mysqld | grep -v grep | wc -l) -ge 1 ] && [ $(ps auxwww | grep apache2 | grep -v grep | wc -l) -ge 1 ] ; do
            sleep 1
        done
    fi
fi

exit 1

