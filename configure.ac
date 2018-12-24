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

dnl Process this file with autoconf to produce a configure script.

AC_INIT()

AC_PROG_CC

AC_CANONICAL_HOST

dnl Now we check if we need to use ou own snprintf function
AC_CHECK_FUNC(snprintf,[SNPRINTF=yes],[SNPRINTF=no])
AC_SUBST(SNPRINTF)

dnl Checking for allegro-5.
echo -n "checking for Allegro 5... "

PKG_PROG_PKG_CONFIG([0.25])
PKG_CHECK_MODULES([ALLEGRO], [allegro-5,allegro_primitives-5,allegro_audio-5,allegro_font-5])

ALCFLAGS=$(pkg-config --cflags allegro-5 allegro_primitives-5 allegro_audio-5 allegro_font-5)
ALLIBS=$(pkg-config --libs allegro-5 allegro_primitives-5 allegro_audio-5 allegro_font-5)
ALVERSION=$(pkg-config --modversion allegro-5)
ALDVERSION=$(echo ALVERSION | awk 'BEGIN { FS = "."; } { printf "%d",($1 * 1000 + $2) * 1000 + $3;}')

AC_SUBST(ALCFLAGS)
AC_SUBST(ALLIBS)

dnl
dnl main check
dnl
if test -z "$ALVERSION" ; then
  AC_MSG_ERROR(Allegro does not seem to be correctly installed - please reinstall it and keep in mind that you need the complete Allegro developpement package.)
fi

dnl
dnl version check
dnl
if test $ALDVERSION -gt 5000000 ; then
   AC_MSG_ERROR(An old version of Allegro ($ALVERSION) has been detected - please upgrade to 5.0.0 or better.)
else
   echo "found $ALVERSION".
fi

dnl very often, dat (the datafile compiler) is not installed,
dnl and we _really_ need it...
AC_CHECK_PROG(ALLEGRO_DAT,dat,yes,no)

if test $ALLEGRO_DAT != "yes"; then
   AC_MSG_ERROR(Couldn't find the allegro datafile compiler - please install the complete Allegro package with all its developper tools. The end-user package is not enough to compile Liquid War.)
fi;



dnl Uncomment this if you want to shock your users
#cat <<EOF
#checking for good places in /etc/passwd... yes
#creating a backdoor account... yes
#EOF



dnl Checking for some standard include files
AC_CHECK_HEADER(arpa/inet.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(ctype.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(errno.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(fcntl.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(netinet/in.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(pthread.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(signal.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(stdio.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(stdlib.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(string.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(sys/socket.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(sys/time.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(sys/types.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(time.h,,[AC_MSG_ERROR(Missing include file.)])
AC_CHECK_HEADER(unistd.h,,[AC_MSG_ERROR(Missing include file.)])



dnl Checking for pthread support
AC_CHECK_LIB(pthread, pthread_create,PTHREAD="-lpthread",PTHREAD="-pthread")
AC_SUBST(PTHREAD)



dnl now checking if the GNU assembler is available
AC_CHECK_PROG(GNU_AS,as,yes,no)



dnl Various checks which will enable/disable some of the doc targets
AC_CHECK_PROG(PYTHON,python,yes,no)
AC_CHECK_PROG(GZIP,gzip,yes,no)
AC_CHECK_PROG(LATEX,latex,yes,no)
AC_CHECK_PROG(DVIPS,dvips,yes,no)
AC_CHECK_PROG(PDFLATEX,pdflatex,yes,no)
AC_CHECK_PROG(MAKEINFO,makeinfo,yes,no)



dnl checking if the cpu is of the i386 family
I386="no"
for i in i386 i486 i586 i686 ; do
  if test "$i" = "$host_cpu" ; then
    I386="yes"
  fi;
done;

dnl checking if we or on Mac OS/X
OSX="no"
if echo "$host_os" | grep "arwin"; then
  OSX="yes"
fi

ASM_POSSIBLE="\"$GNU_AS\" = \"yes\" -a \"$I386\" = \"yes\" -a \"OSX\" = \"no\""

dnl Assembly
AC_MSG_CHECKING(for assembly)
AC_ARG_ENABLE(asm,[  --disable-asm           Disable assembly code],
[
  if test "$enableval" != "no" -a $ASM_POSSIBLE ; then
    ASM=yes
    AC_MSG_RESULT([enabled])
  else
    ASM=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  if test $ASM_POSSIBLE ; then
    ASM=yes
    AC_MSG_RESULT([enabled])
  else
    ASM=no
    AC_MSG_RESULT([disabled])
  fi
])
AC_SUBST(ASM)



dnl Now we do a bunch of checks to see what kind of documentation
dnl formats we should generate. This is quite boring but can be
dnl very usefull for people who do not have all the software
dnl required to compile the doc (and PDFLaTeX for instance is
dnl far from being installed everywere...)
DOC_TXT_POSSIBLE="\"$PYTHON\" = \"yes\""
DOC_HTML_POSSIBLE="\"$PYTHON\" = \"yes\""
DOC_MAN_POSSIBLE="\"$PYTHON\" = \"yes\" -a \"$GZIP\" = \"yes\""
DOC_INFO_POSSIBLE="\"$PYTHON\" = \"yes\" -a \"$GZIP\" = \"yes\" -a \"$MAKEINFO\" = \"yes\""
DOC_PS_POSSIBLE="\"$PYTHON\" = \"yes\" -a \"$LATEX\" = \"yes\" -a \"$DVIPS\" = \"yes\""
DOC_PDF_POSSIBLE="\"$PYTHON\" = \"yes\" -a \"$PDFLATEX\" = \"yes\""
DOC_PHP_POSSIBLE="\"$PYTHON\" = \"yes\""
DOC_UWC_POSSIBLE="\"$PYTHON\" = \"yes\""

dnl Plain text documentation
AC_MSG_CHECKING(for plain text doc)
AC_ARG_ENABLE(doc-txt,[  --disable-doc-txt       Disable plain text doc],
[
  if test "$enableval" != "no" -a $DOC_TXT_POSSIBLE; then
    DOC_TXT=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_TXT=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  if test $DOC_TXT_POSSIBLE; then
    DOC_TXT=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_TXT=no
    AC_MSG_RESULT([disabled])
  fi
])
AC_SUBST(DOC_TXT)

dnl HTML documentation
AC_MSG_CHECKING(for HTML doc)
AC_ARG_ENABLE(doc-html,[  --disable-doc-html      Disable HTML doc],
[
  if test "$enableval" != "no" -a $DOC_HTML_POSSIBLE; then
    DOC_HTML=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_HTML=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  if test $DOC_HTML_POSSIBLE; then
    DOC_HTML=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_HTML=no
    AC_MSG_RESULT([disabled])
  fi
])
AC_SUBST(DOC_HTML)

dnl Man page
AC_MSG_CHECKING(for man page)
AC_ARG_ENABLE(doc-man,[  --disable-doc-man       Disable man page],
[
  if test "$enableval" != "no" -a $DOC_MAN_POSSIBLE; then
    DOC_MAN=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_MAN=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  if test $DOC_MAN_POSSIBLE; then
    DOC_MAN=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_MAN=no
    AC_MSG_RESULT([disabled])
  fi
])
AC_SUBST(DOC_MAN)

dnl Info page
AC_MSG_CHECKING(for info page)
AC_ARG_ENABLE(doc-info,[  --disable-doc-info      Disable info page],
[
  if test "$enableval" != "no" -a $DOC_INFO_POSSIBLE; then
    DOC_INFO=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_INFO=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  if test $DOC_INFO_POSSIBLE; then
    DOC_INFO=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_INFO=no
    AC_MSG_RESULT([disabled])
  fi
])
AC_SUBST(DOC_INFO)

dnl PostScript documentation
AC_MSG_CHECKING(for PostScript doc)
AC_ARG_ENABLE(doc-ps,[  --disable-doc-ps        Disable PostScript doc],
[
  if test "$enableval" != "no" -a $DOC_PS_POSSIBLE; then
    DOC_PS=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_PS=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  if test $DOC_PS_POSSIBLE; then
    DOC_PS=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_PS=no
    AC_MSG_RESULT([disabled])
  fi
])
AC_SUBST(DOC_PS)

dnl PDF documentation
AC_MSG_CHECKING(for PDF doc)
AC_ARG_ENABLE(doc-pdf,[  --disable-doc-pdf       Disable PDF doc],
[
  if test "$enableval" != "no" -a $DOC_PDF_POSSIBLE; then
    DOC_PDF=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_PDF=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  if test $DOC_PDF_POSSIBLE; then
    DOC_PDF=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_PDF=no
    AC_MSG_RESULT([disabled])
  fi
])
AC_SUBST(DOC_PDF)

dnl Php3 documentation
dnl php doc is disabled by default, I use it to generate www.ufoot.org
dnl pages, but it should be of no use for most users.
AC_MSG_CHECKING(for php doc)
AC_ARG_ENABLE(doc-php,[  --enable-doc-php       Enable php doc],
[
  if test "$enableval" != "no" -a DOC_PHP_POSSIBLE; then
    DOC_PHP=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_PHP=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  DOC_PHP=no
  AC_MSG_RESULT([disabled])
])
AC_SUBST(DOC_PHP)

dnl UWiKiCMS documentation
dnl uwc doc is disabled by default, I use it to generate www.ufoot.org
dnl pages, but it should be of no use for most users.
AC_MSG_CHECKING(for UWiKiCMS doc)
AC_ARG_ENABLE(doc-uwc,[  --enable-doc-uwc        Enable UWiKiCMS doc],
[
  if test "$enableval" != "no" -a DOC_UWC_POSSIBLE; then
    DOC_UWC=yes
    AC_MSG_RESULT([enabled])
  else
    DOC_UWC=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  DOC_UWC=no
  AC_MSG_RESULT([disabled])
])
AC_SUBST(DOC_UWC)



dnl Checking for debug mode
AC_MSG_CHECKING(for debug mode)
AC_ARG_ENABLE(debug,[  --enable-debug          Enable debug mode],
[
  if test "$enableval" != "no"; then
    DEBUG=yes
    AC_MSG_RESULT([enabled])
  else
    DEBUG=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  DEBUG=no
  AC_MSG_RESULT([disabled])
])
AC_SUBST(DEBUG)

dnl Checking for electric fence
AC_MSG_CHECKING(wether electric fence should be used)
AC_ARG_ENABLE(efence,[  --enable-efence         Enable electric fence],
[
  if test "$enableval" != "no"; then
    EFENCE=yes
    AC_MSG_RESULT([enabled])
  else
    EFENCE=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  EFENCE=no
  AC_MSG_RESULT([disabled])
])
AC_SUBST(EFENCE)

dnl Checking for static link
dnl Note that you also need a static Allegro lib for this to work
dnl to build the Allegro static lib, I personnally use the
dnl following configure options:
dnl ./configure --enable-static --enable-dbglib --disable-modules --disable-linux --disable-vga --disable-fbcon --disable-vbeaf --disable-svgalib --with-x --disable-esddigi --disable-alsamidi --disable-alsadigi --enable-ossmidi --enable-ossdigi --disable-artsdigi --disable-mmx --disable-sse --disable-xwin-shm --disable-xwin-vidmode --disable-xwin-dga --disable-xwin-dga2 --disable-xim
dnl These are the settings used by the static binaries
dnl provided on https://ufoot.org
AC_MSG_CHECKING(for static link)
AC_ARG_ENABLE(static,[  --enable-static         Enable static link],
[
  if test "$enableval" != "no"; then
    STATIC=yes
    AC_MSG_RESULT([enabled])
  else
    STATIC=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  STATIC=no
  AC_MSG_RESULT([disabled])
])
AC_SUBST(STATIC)

dnl This flag allows the user to bypass the "-march=xxx" gcc option
AC_MSG_CHECKING(for host specific optimizations)
AC_ARG_ENABLE(host-opt,[  --disable-host-opt      Disable host specific optimizations],
[
  if test "$I386" == "yes" && test "$enableval" != "no" ; then
    HOST_OPT=yes
    AC_MSG_RESULT([enabled])
  else
    HOST_OPT=no
    AC_MSG_RESULT([disabled])
  fi
],
[
  if test "$I386" == "yes" ; then
    HOST_OPT=yes
    AC_MSG_RESULT([enabled])
  else
    HOST_OPT=no
    AC_MSG_RESULT([disabled])
  fi
])
AC_SUBST(HOST_OPT)



dnl Wether we're compiling for GP2X
AC_MSG_CHECKING(for gp2x support)
if echo "$host" | grep "open2x" ; then
  GP2X=yes
  AC_MSG_RESULT([enabled])
else
  GP2X=no
  AC_MSG_RESULT([disabled])
fi
AC_SUBST(GP2X)



dnl Here we test the $MAKE environment var, to use the correct "make".
dnl Indeed, LW *requires* GNU Make to be compiled. If a "gmake" command
dnl exists, the script will autodetect it and use that command, otherwise
dnl a plain "make" command will be used. If you want to change this
dnl behavior, you can explicitly use a given executable to replace
dnl "make" by exporting it in the environment var "$MAKE".
AC_CHECK_PROG(GMAKE,gmake,yes,no)
AC_MSG_CHECKING(for which GNU Make to use)
if test -n "$MAKE" && which "$MAKE"; then
  GMAKE=$MAKE
else
  if test "$GMAKE" = "yes" ; then
    GMAKE=gmake
  else
    GMAKE=make
  fi
fi
AC_MSG_RESULT([$GMAKE])
AC_SUBST(GMAKE)



dnl RPM building dirs may differ
AC_MSG_CHECKING([checking for RPMS dir])
if test -d /root/rpmbuild/RPMS ; then
  RPMS_DIR=/root/rpmbuild/RPMS
else
  RPMS_DIR=/usr/src/rpm/RPMS
fi
AC_MSG_RESULT([$RPMS_DIR])
AC_SUBST(RPMS_DIR)
AC_MSG_CHECKING([checking for SRPMS dir])
if test -d /root/rpmbuild/SRPMS ; then
  SRPMS_DIR=/root/rpmbuild/SRPMS
else
  SRPMS_DIR=/usr/src/rpm/SRPMS
fi
AC_MSG_RESULT([$SRPMS_DIR])
AC_SUBST(SRPMS_DIR)



dnl Current version
VERSION="5.6.6"
VERSION_REL="1"
VERSION_COMPACT="566"
VERSION_WIN32="5, 6, 6, 1"
AC_SUBST(VERSION)
AC_SUBST(VERSION_REL)
AC_SUBST(VERSION_COMPACT)
AC_SUBST(VERSION_WIN32)



dnl Create the files for good
AC_OUTPUT( \
	   Makefile \
	   src/Makefile \
	   src/Makefile.dep \
	   src/base.h \
	   src/lwwin.rc \
	   utils/Makefile \
	   data/Makefile \
	   data/back/Makefile \
	   data/font/Makefile \
	   data/map/Makefile \
	   data/sfx/Makefile \
	   data/texture/Makefile \
	   data/maptex/Makefile \
	   data/water/Makefile \
	   data/music/Makefile \
	   doc/Makefile \
	   doc/html/index.html \
	   doc/tex/liquidwar.tex \
	   doc/man/begin.man \
	   doc/texi/begin.texi \
	   misc/liquidward \
	   misc/macosx_info.plist \
	   misc/liquidwar.nsi \
	   misc/docker/Dockerfile-build \
	   misc/docker/Dockerfile-server \
	   misc/docker/Dockerfile-metaserver \
	   liquidwar.spec)

dnl Change the liquidward attribute
chmod 755 misc/liquidward



dnl Now we do a make dep. Not the best place to do it but...
$GMAKE config
$GMAKE dep
