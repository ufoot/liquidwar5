#!/bin/sh

mapsize="0 1 2 3 4 5"
gridsize="0 1 2 3 4 5 6 7 8 9"


testfunc="rand_func big_quad boxes bubbles circles circuit hole lines \
rand_box rand_poly rand_poly_cut street worms"

#testfunc=big_quad


make cleanmaps
if [ ! -x ./liquidwar-mapgen ]; then
     make
fi


for func in $testfunc; do
     for s in $mapsize; do
          for g in $gridsize; do
               echo "******* testing: $func / $i *******"
               ./liquidwar-mapgen -f $func -s $s -g $g -o ./$func.$s.$g.bmp
               sleep 2
          done
     done
done


###############################
# arrange the maps...

if [ ! -d ./maps ]; then
     mkdir ./maps
fi

mv -f *.bmp ./maps
cd ./maps


###############################
# view the maps...

viewer=`which ee xv 2> /dev/null`
if [ -z "$viewer" ]; then
     echo "Can't find image viewer."
     exit 0
fi

if [ -z "$DISPLAY" ]; then
     echo "\$DISPLAY is not set."
     exit 0
fi

for file in *.bmp; do
     $viewer $file
done
