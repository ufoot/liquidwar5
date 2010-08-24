#!/bin/sh
#
# This script has been ripped from the Allegro game library.
# It handles the CR/LF in the text files so that there are
# no conflicts between DOS/Windows and UNIX text readers.

proc_help()
{
   echo "Usage: ./fix.sh {unix|dos}"
   exit 1
}

proc_fix_unix()
{
   proc_filelist
   FILELIST="$FILELIST `find . -type f "(" \
   	-name "*.sh" -o \
        -name "*.in" -o \
        -name "config.guess" -o \
        -name "config.sub" -o \
        -name "install-sh" -o \
        -name "liquidward" \
	")"`" 
   proc_dtou
   proc_chmod
}

proc_fix_dos()
{
   proc_filelist
   FILELIST="$FILELIST `find . -type f "(" \
   	-name "*.bat" -o \
        -name "*.js" \
	")"`" 
   proc_utod
}

proc_filelist()
{
   # common files.
   FILELIST=`find . -type f "(" \
      -name "*.c" -o \
      -name "*.h" -o \
      -name "*.s" -o \
      -name "*.txt" -o \
      -name "*.xml" -o \
      -name "*.html" -o \
      -name "*.tex" -o \
      -name "*.texi" -o \
      -name "*.man" -o \
      -name "*.py" -o \
      -name "*.win" -o \
      -name "*.dos" -o \
      -name "ALLEGRO" -o \
      -name "COPYING" -o \
      -name "INSTALL" -o \
      -name "README" -o \
      -name "README.*" -o \
      -name "ChangeLog" \
   ")"`
}

proc_chmod()
{
   echo "Changing file attributes..."
   find . -type d | xargs chmod 755
   find . -type f | xargs chmod 644
   find . -type f "(" \
   	-name "configure" -o \
	-name "config.status" -o \
	-name "config.guess" -o \
	-name "config.sub" -o \
	-name "install-sh" -o \
	-name "*.sh" -o \
	-name "liquidwar" -o \
	-name "liquidwar-server" -o \
	-name "liquidward" \
   ")" | xargs chmod 755
}

proc_utod()
{
   echo "Converting files from Unix to DOS/Win32..."
   for file in $FILELIST; do
      #echo "$file"
      cp $file _tmpfile
      perl -p -i -e "s/([^\r]|^)\n/\1\r\n/" _tmpfile
      touch -r $file _tmpfile
      mv _tmpfile $file
   done
}

proc_dtou()
{
   echo "Converting files from DOS/Win32 to Unix..."
   for file in $FILELIST; do
      #echo "$file"
      mv $file _tmpfile
      tr -d '\015' < _tmpfile > $file
      touch -r _tmpfile $file
      rm _tmpfile
   done
}

# prepare allegro for the given platform.

case "$1" in
   "unix"    ) proc_fix_unix;;
   "dos"     ) proc_fix_dos;;
   *         ) proc_help;;
esac

echo "Done!"

exit 0

