#!/bin/bash
#
# This is a script to convert a group of still-frame images to .ppm files suitable
# for use with mpeg2encode (via Imagemagick's "convert" front-end).
#
# imagestoyuv is part of a group of unix shell scripts to aid in converting a group
# of still-frame images to an mpeg-1 animation.
# More information can be found at http://marc.me.utexas.edu/mpeg_tools.html.
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
   echo "Usage:   ./mpeg_utils_imagetoyuv.sh images"
   echo "example: ./mpeg_utils_imagetoyuv.sh /tmp/sequence_1*.tif" $'\n\n'
   echo
   echo "TYPICAL ANIMATION STEPS:"
   echo " (1) convert images from their native format to ppm or yuv with:"
   echo "        ./mpeg_utils_imagetoyuv.sh ./images/scenario1*.tif"
   echo " (2) make the encoder parameter file with:"
   echo "        ./mpeg_utils_make_mpeg2encode_parfile.sh ./images/scenario1*.tif"
   echo "     note: provide this script with the *source* filenames (not the yuv filenames)"
   echo "     for 'identify' to work properly"
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

      #echo ${name[*]}
      #echo "name[0]=${name[0]}" # basename
      #echo "name[1]=${name[1]}" # exten

      # determine if either size is an odd number...
	 # (fortunately, conversion to the yuv format automagically includes
	 #  a conversion to even horizontal and vertical dimensions so there's
         #  no need for optional padding or size adjustment if the hsize or vsize
         #  is an odd number the way ./mpeg_utils_imagetoppm.sh does.)

      #echo "name[0]=${name[0]}"
      #echo "name[1]=${name[1]}"  $'\n\n\n'
      echo "./mpeg_utils_imagetoppm.sh: converting ${name[0]}.${name[1]} --> ${name[0]}.yuv..."
      convert ${name[0]}.${name[1]} ${name[0]}.yuv
   done

fi




















































