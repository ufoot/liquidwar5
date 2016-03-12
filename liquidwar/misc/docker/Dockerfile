# Liquid War is a multiplayer wargame
# Copyright (C) 1998-2016 Christian Mauduit
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
# Liquid War homepage : http://www.ufoot.org/liquidwar/v5
# Contact author      : ufoot@ufoot.org

FROM debian
MAINTAINER Christian Mauduit "ufoot@ufoot.org"

ENV DEBIAN_FRONTEND noninteractive

# Depots, mises a jour et installs de Apache/PHP5

RUN (apt-get update && apt-get upgrade -y -q && apt-get dist-upgrade -y -q && apt-get -y -q autoclean && apt-get -y -q autoremove)
RUN apt-get install -y -q gcc make liballegro4-dev python texinfo texlive

# Installation et configuration de MariaDB

RUN install -d /usr/local/src
COPY liquidwar-5.6.5.tar.gz /usr/local/src/liquidwar-5.6.5.tar.gz
RUN cd /usr/local/src && tar xzf liquidwar-5.6.5.tar.gz
RUN cd /usr/local/src/liquidwar-5.6.5 && ./configure && make && make install

