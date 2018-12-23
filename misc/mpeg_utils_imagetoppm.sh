#!/bin/bash
#
# This is a script to convert a group of still-frame images to .ppm files suitable
# for use with mpeg2encode (via Imagemagick's "convert" front-end).
#
# ./mpeg_utils_./mpeg_utils_imagetoppm.sh.sh is part of a group of unix shell scripts to aid in converting a group
# of still-frame images to an mpeg-1 animation.
# More information can be found at http://marc.me.utexas.edu/mpeg_tools.html.
#
# It ensures both horizontal and vertical dimensions are even-numbered by adding a 1-pixel
# black border if necessary with 'pbmpad'.
#
# ImageMagick's 'convert' utility along with mpeg2encode is able to convert
# a bunch of still-frames directly to an mpeg (or mpg) movie.  However, for many frames
# (i.e. over 200 or 300) you may run out of RAM or swapfile space for the conversion directly
# from tif to yuv3 or ppm, then into the mpg.  This script acts as an intermediate step to individually
# convert images in a sequence to a ppm sequence that avoids running out of RAM or swapfile space.
#
# It relies upon:
#       /bin/sh
#       awk
#       convert  (from ImageMagick, http://www.imagemagick.org)
#       identify  (from ImageMagick, http://www.imagemagick.org)
#       (possibly) pnmpad (from the netpbm-progs rpm)
#
#
# usage:
#          ./mpeg_utils_imagetoppm.sh images
#
# where 'images' is both the directory and image names of the original images to convert.
#
# examples: ./mpeg_utils_imagetoppm.sh ./tmp/*gif
#           ./mpeg_utils_imagetoppm.sh ./sequence1_*.jpg
#           ./mpeg_utils_imagetoppm.sh /tmp/images/movie_frame000*.tif
#	
# limitations:
#  - Valid imput image formats are limited by ImageMajick's convert utility.
#  - Input files must end with an extemsion seprated by a '.', however awk's split() function
#    allows them to contain the '.' character elsewhere in the filename.
#
# Script comments:
# Loop variable i successively becomes the name of image files in the specified directory.
# awk -Ftif says to separate text fields with the string '.' and that is
# how it extracts the base filename minus the extension.
#
# Marc Compere
# CompereM@asme.org
# created : 17 September 2001
# mofidied: 05 April 2002

#echo $*


if [ "$1" = "" ]; # no input arguments
then
   echo
   echo "You must specifiy a directory name."
   echo
   echo "Usage:   ./mpeg_utils_imagetoppm.sh images"
   echo "example: ./mpeg_utils_imagetoppm.sh /tmp/sequence_1*.tif" $'\n\n'
   echo
   echo "TYPICAL ANIMATION STEPS:"
   echo " (1) convert images from their native format to ppm or yuv with:"
   echo "        ./mpeg_utils_imagetoppm.sh ./images/scenario1*.tif"
   echo "        ./mpeg_utils_imagetoyuv.sh ./images/scenario1*.tif"
   echo " (2) make the encoder parameter file with:"
   echo "        ./mpeg_utils_make_mpeg2encode_parfile.sh ./images/scenario1*.ppm (use ppm images here to get image info properly in case padding was required)"
   echo "        ./mpeg_utils_make_mpeg2encode_parfile.sh ./images/scenario1*.tif (use tif images here if using yuv's to get image info properly)" 
   echo " (3) run the encoder with:"
   echo "        mpeg2encode mpeg2encode.par scenario1.mpg" $'\n\n'
else

   filelist=($*)
   #echo ${filelist[*]}

   for i in ${filelist[*]};
   do
      # this awk script provides the capability to correctly determine the filename extension 
      # including the possibile case where there are multiple occurrences of the "." character
      # in the filename before the extension.
      name=(`echo $i | awk '{num_fields=split($1,fields,".")}
                     {basename=fields[1]}
                     {for (i=2; i<num_fields; i++)
                        {basename=basename "." fields[i]}
                     }
                     {exten=fields[i]}
                     {print basename}{print exten}'`)
      # note:
      #   awk's array indicies are 1-based
      #   bash's array indicies are 0-based

      #echo "name[0]=${name[0]}" # basename
      #echo "name[1]=${name[1]}" # exten
      echo "./mpeg_utils_imagetoppm.sh: converting ${name[0]}.${name[1]} --> ${name[0]}.ppm..."
      convert ${name[0]}.${name[1]} ${name[0]}.ppm

      # place hsize and vsize in ${sizes[0]} and ${sizes[1]}
      info=(`identify -ping ${name[0]}.ppm`) # use ImageMagick's identify command
      #echo ${info[*]}
      sizes=(`echo ${info[1]} | awk -Fx '{print $1}{print $2}'`)
      hsize=${sizes[0]}
      vsize=${sizes[1]}

      # determine if either size is an even or odd number...
      hsize_odd=$(( $(( hsize % 2)) ? 1 : 0 ))
      vsize_odd=$(( $(( vsize % 2)) ? 1 : 0 ))

      # ... if so, correct it by adding a 1-pixel pad to prevent mpeg2encode from complaining.
      # (actally, add a border, but don't use 'pnmmargin' because it's a script that appears to
      #  be quite inefficient by using several temporary files)
      # (see notes in ./mpeg_utils_make_mpeg2encode_parfile.sh for more info)
      if [ "$hsize_odd" = "1" ];
      then
         echo "./mpeg_utils_imagetoppm.sh: padding horizontal dimension..."
	 pnmpad -black -r1 ${name[0]}.ppm > ${name[0]}_tmp.ppm
         mv ${name[0]}_tmp.ppm ${name[0]}.ppm
      fi
       
      if [ "$vsize_odd" = "1" ];
      then
         echo "./mpeg_utils_imagetoppm.sh: padding vertical dimension..."
	 pnmpad -black -b1 ${name[0]}.ppm > ${name[0]}_tmp.ppm
         mv ${name[0]}_tmp.ppm ${name[0]}.ppm
      fi

   done

fi











