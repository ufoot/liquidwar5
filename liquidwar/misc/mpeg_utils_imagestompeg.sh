#!/bin/bash
#
# This is a script to convert a group of still-frame images to an mpeg animation.
# The still frame images may be in any format ImageMagick's 'convert' utility handles.
#
# ./mpeg_utils_imagetompeg.sh is part of a group of unix shell scripts to aid in converting a group
# of still-frame images to an mpeg-1 animation.
# More information can be found at http://marc.me.utexas.edu/mpeg_tools.html.
#
# There is very little error handling.  Incorrect inputs are likley to cause errors.
#
# This script acts as a main script that calls other scripts to automate the following
# typical animation steps:
#  (1) convert images from their native format to ppm or yuv with:
#         ./mpeg_utils_imagetoppm.sh ./images/scenario1*.tif
#         ./mpeg_utils_imagetoyuv.sh ./images/scenario1*.tif
#  (2) make the encoder parameter file with:
#         ./mpeg_utils_make_mpeg2encode_parfile.sh ./images/scenario1*.ppm (use ppm images here to get image info properly in case padding was required)
#         ./mpeg_utils_make_mpeg2encode_parfile.sh ./images/scenario1*.tif (use tif images here if using yuv's to get image info properly)
#  (3) run the encoder with:
#         mpeg2encode mpeg2encode.par scenario1.mpg 
#
#
# This script relies upon:
#       convert                  (from ImageMagick, http://www.imagemagick.org)
#       identify                 (from ImageMagick, http://www.imagemagick.org)
#	mpeg2encode              (from http://www.mpeg.org/MSSG/)
#	./mpeg_utils_imagetoppm.sh               (from http://marc.me.utexas.edu/tmp/mpeg_encoding_solution/)
#       pnmpad                   (from the netpbm-progs rpm)
#	./mpeg_utils_make_mpeg2encode_parfile.sh (from http://marc.me.utexas.edu/tmp/mpeg_encoding_solution/)
#	awk
#	bash
#
# usage:
#          ./mpeg_utils_imagetompeg.sh mpegname imagenames
#
# examples: ./mpeg_utils_imagetompeg.sh sequence1.mpg /tmp/sequence_1*.tif
#           ./mpeg_utils_imagetompeg.sh test.mpg      /tmp/frames*.bmp  
#           ./mpeg_utils_imagetompeg.sh my_movie.mpg /tmp/images/movie_frame000*.gif
#	
# limitations:
#  - Valid imput image formats are limited by ImageMajick's convert utility.
#  - Input files must end with an extemsion seprated by a '.', however awk's split() function
#    allows them to contain the '.' character elsewhere in the filename.
#
# Marc Compere
# CompereM@asme.org
# created : 01 October 2001
# modified: 05 April 2002

#echo $*


# pick apart the first filename to make sure it's the name of an mpeg
# -----------------------------------------------------
   # this awk script provides the capability to correctly determine the filename extension 
   # including the possibile case where there are multiple occurrences of the "." character
   # in the filename before the extension.
   names=(`echo $1 | awk '{num_fields=split($1,fields,".")}
                                     {basename=fields[1]}
                                     {for (i=2; i<num_fields; i++)
                                        {basename=basename "." fields[i]}
                                     }
                                     {exten=fields[i]}
                                     {print basename}{print exten}'`)

   # note:
   #   awk's array indicies are 1-based
   #   bash's array indicies are 0-based
   basename=${names[0]}
   exten=${names[1]}
# -----------------------------------------------------

# set ext_ok=1 if any of the following conditions are matched
ext_ok=`[ "$exten" = "mpg" -o "$exten" = "mpeg" -o "$exten" = "MPG" -o "$exten" = "MPEG" ] && echo 1`
#echo $ext_ok
#echo $exten

if [ "$1" = "" -o "$ext_ok" = "" ]; # bad input arguments
then
   echo
   echo "You must specifiy the resulting mpeg filename and a sequence of image files"
   echo
   echo "Usage:    ./mpeg_utils_imagetompeg.sh animation.mpg images"
   echo
   echo "examples: ./mpeg_utils_imagetompeg.sh sequence1.mpg /tmp/sequence_1*.tif"
   echo "          ./mpeg_utils_imagetompeg.sh test.mpg      /tmp/frames*.bmp"
   echo "          ./mpeg_utils_imagetompeg.sh my_movie.mpg /tmp/images/movie_frame000*.gif" $'\n\n'
else

mpeg_filename=$1
sequence_name=(${*:2})
#echo ${sequence_name[*]}


# pick apart the filename from the sequence:
# -----------------------------------------------------
   # this awk script provides the capability to correctly determine the filename extension 
   # including the possibile case where there are multiple occurrences of the "." character
   # in the filename before the extension.
   names=(`echo ${sequence_name[0]} | awk '{num_fields=split($1,fields,".")}
                                     {basename=fields[1]}
                                     {for (i=2; i<num_fields; i++)
                                        {basename=basename "." fields[i]}
                                     }
                                     {exten=fields[i]}
                                     {print basename}{print exten}'`)

   # note:
   #   awk's array indicies are 1-based
   #   bash's array indicies are 0-based
   basename=${names[0]}
   exten=${names[1]}
   basename_length=`echo $basename | awk '{print length}'`
   # pick off all the numeric characters at the end of $basename
   let cnt=basename_length-1
   while [[ "${basename:$cnt:1}" == [0-9] ]] ; do
    #echo ${basename:$cnt:1}
    let cnt=cnt-1
    #echo $cnt ;
   done
   let numeric_string_length=$basename_length-$cnt-1 #; echo $numeric_string_length
   
   seq_numeric_portion=${basename:$cnt+1:$numeric_string_length} #; echo $seq_numeric_portion
   seq_string_portion=${basename:0:$cnt+1} #; echo $seq_string_portion

   # convert from a string to a number
   seq_starting_number=`builtin printf "%0.f" "$seq1_numeric_portion"`
   #echo $seq_starting_number
# -----------------------------------------------------


maxnumchars=`echo $seq_numeric_portion | awk '{print length()}'`
#echo "maxnumchars = " $maxnumchars


# make the ppm filename array
#frame_num=0
#for i in ${sequence_name[*]}
#do
#   char_frame_number=`builtin printf "%0$maxnumchars""i" "$frame_num"` #; echo $char_frame_number
#   let frame_num=frame_num+1
#   ppm_sequence_name[frame_num]=$seq_string_portion$char_frame_number.ppm # ; echo ${ppm_sequence_name[frame_num]}
#done
#echo ${ppm_sequence_name[*]}


# Do the schmack:

# (1) convert images from their native format to ppm or yuv with:
intermediate_format=yuv
# be sure to set ipff in './mpeg_utils_make_mpeg2encode_parfile.sh' to match this setting.
# ipff=1 for yuv
# ipff=2 for ppm (the ppm option is mysteriously broken on my machine as of 10 January 2002 --> now using yuv)
 if [ "$intermediate_format" = "yuv" ]; # use './mpeg_utils_imagetoyuv.sh'
 then
    echo $0: "going to use ./mpeg_utils_imagetoyuv.sh"
    ./mpeg_utils_imagetoyuv.sh ${sequence_name[*]}
 else # use './mpeg_utils_imagetoppm.sh'
    echo $0: "going to use ./mpeg_utils_imagetoppm.sh"

    echo $0: " The ppm option is broken for some reason...(?)"
    echo $0: " It produces a core dump."
    echo $0: " MDC, 10 January 2002"

    ./mpeg_utils_imagetoppm.sh ${sequence_name[*]}
 fi

# (2) make the encoder parameter file with:
./mpeg_utils_make_mpeg2encode_parfile.sh ${sequence_name[*]}

# (3) run the encoder with:
mpeg2encode mpeg2encode.par $mpeg_filename


# intentionally leave the mpeg encoder parameter file
#rm mpeg2encode.par

fi











