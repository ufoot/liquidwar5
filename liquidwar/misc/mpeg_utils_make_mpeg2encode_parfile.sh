#!/bin/sh
#
# This shell script generates a parameter file suitable for use
# with mpeg2encode from http://www.mpeg.org/MSSG/.
#
# ./mpeg_utils_make_mpeg2encode_parfile.sh is part of a group of unix shell scripts to aid in converting a group
# of still-frame images to an mpeg-1 animation.
# More information can be found at http://marc.me.utexas.edu/mpeg_tools.html.
#
# There is no error checking. It is basic but functional.
# It relies upon:
#       /bin/sh
#       awk
#       identify  (from ImageMagick, http://www.imagemagick.org)
#
# Usage:
#         ./mpeg_utils_make_mpeg2encode_parfile.sh filelist
#
# where required arguemnts are:
#
#         filelist - filenames, including directory, where the still frame images reside
#
#
# Examples:
#         ./mpeg_utils_make_mpeg2encode_parfile.sh /tmp/images/*
#         ./mpeg_utils_make_mpeg2encode_parfile.sh /tmp/images/scenarioA*.tif
#
# where the /tmp/images directory contains files something like:
#
#	frame.00001.tif
#	frame.00002.tif
#	frame.00003.tif
#	...
#	frame.00486.tif
#
# The individual frames do not have to be tiff's; they can be any format 'convert' handles.
# See notes within the './mpeg_utils_imagetoppm.sh' or './mpeg_utils_imagetoyuv.sh' scripts at
# http://marc.me.utexas.edu/mpeg_tools.html.
#
# Result: mpeg2encode.par left in current working directory
#
#
# Next possible steps to make a movie:
#   (1) ensure the images are converted from whatever format into either ppm or yuv with:
#         ./mpeg_utils_imagetoppm.sh ./images/*.tif
#       or
#         ./mpeg_utils_imagetoyuv.sh ./images/*.gif
#
#   (2) run the encoder with:
#         mpeg2encode mpeg2encode.par movie.mpg
#
# For tips on how to combine 2 or more sequences of images into one mpeg, see the
# 'renumber_sequence' script at http://marc.me.utexas.edu/mpeg_tools.html.
# or use the 'rename' command and see http://www.tux.org/~mayer/linux/book.pdf.
#
#
# Marc Compere
# CompereM@asme.org
# created : 18 September 2001
# modified: 20 January 2002

# development arg setup:
#    filelist=(`ls still_frames/test*ppm`) ; echo ${filelist[*]}


parameter_set=2 # the PAL (-->2) parameter set is ImageMagick's default, but NTSC (-->1) seems to make
                # smaller mpeg's with similar playback quality

if [ "$1" = "" ];
then 
   echo " Usage:"
   echo "         ./mpeg_utils_make_mpeg2encode_parfile.sh <ppm|yuv> filelist"
   echo
   echo " where the 'ppm' or 'yuv' string indicates which intermediate format to specify for mpeg2encode to use,"
   echo " and where filelist is the list of filenames, including the directory, where the still frame images reside."
   echo
   echo " Examples:"
   echo "         ./mpeg_utils_make_mpeg2encode_parfile.sh /tmp/images/*"
   echo "         ./mpeg_utils_make_mpeg2encode_parfile.sh /tmp/images/scenarioA*.ppm"
   echo " or"
   echo "         ./mpeg_utils_make_mpeg2encode_parfile.sh yuv /tmp/images/*"
   echo "         ./mpeg_utils_make_mpeg2encode_parfile.sh ppm /tmp/images/scenarioA*.tif"
   echo
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

filelist=(${*}) # assign input filename arglist

if [ "$parameter_set" = "1" ];
then
   # set variables for NTSC
   fps=30         # specifying frames per second (this actually comes from frc spec and is only used in the comment string)
   Ngop=15        # 15 for NTSC
   frc=5          # 5 for NTSC
   vidfmt=2       # video format = 1->PAL, 2->NTSC
   Nmatcoef=4     # 5->PAL, 4->NTSC
elif [ "$parameter_set" = "2" ];
then
   # set variables for PAL
   fps=25         # specifying frames per second (this actually comes from frc spec and is only used in the comment string)
   Ngop=12        # 12 for NTSC
   frc=3          # 3 for NTSC
   vidfmt=1       # video format = 1->PAL, 2->NTSC
   Nmatcoef=5     # 5->PAL, 4->NTSC
else
 echo "mpeg2encode_preproc: invalid parameter_set number",$parameter_set
 echo "mpeg2encode_preproc: parameter_set should be 1 (for NTSC) or 2 (for PAL)"
 echo "mpeg2encode_preproc: exiting...." $'\n\n\n'
fi
   

# Set the rest of input parameters to write_parfile()
parfile="mpeg2encode.par"     # output parameter file
statfile="/dev/null"       # may be '-' for stdout or /dev/null (default) 
ipff=1             # input picture file format (only 1->yuv3 or 2->ppm used here)
                   # note: using ipff=2 for ppm's seems to create skewed animations that appear to have the aspect
                   # ratio way off, but changing the aspect ratio in the parameter file seems to do nothing...
                   # --> use yuv's as the input file format to mpeg2encode (yuv is the 'convert' default)
# note: ipff was set at 2 which means using the .ppm image format, but now that option
#       fails and causes mpeg2encode to crash.  (?) Go figure.  I'm now using the .yuv image format.
#       10 January 2002


#bitrate=1152000.0  # default was 5000000.0 (bits/sec), but 1152000.0 is mo'bettah
#bitrate=576000.0  # default was 5000000.0 (bits/sec), but 1152000.0 is mo'bettah
bitrate=480000.0  # default was 5000000.0 (bits/sec), but 1152000.0 is mo'bettah


# find Nframes
Nframes=0
for i in ${filelist[*]}
do
   let Nframes=Nframes+1
done
#echo $Nframes


# find the common file (and directory name) prefix string to all image files:
filenamelength=`echo ${filelist[0]} | awk '{print length}'`
all_match=1
char_cnt=0
str1=(${filelist[0]})
str2=(${filelist[${Nframes}-1]})
while [ "${all_match}" = "1" -a "${char_cnt}" -le "${filenamelength}" ] ; do
   if [ "${str1:$char_cnt:1}" = "${str2:$char_cnt:1}" ] ; # compare the characters in the first and last filenames
   then
      #echo "characters in element $char_cnt match" ;
      let char_cnt=char_cnt+1
   else
      all_match=0
      #echo "characters in element $char_cnt DO NOT match, exiting..." ;
   fi
done
basefilename=${str1:0:$char_cnt}
restofthefilename=${str1:$char_cnt}

first_num=`echo ${restofthefilename} | awk -F. '{print $1}'` # this assumes the filename has an extension, ".something"
maxnumchars=`echo $first_num | awk '{print length()}'`

#  note:
#    awk's array indicies are 1-based
#    bash's array indicies are 0-based


# place hsize and vsize in ${sizes[0]} and ${sizes[1]}
info=(`identify -ping ${filelist[0]}`) # use ImageMagick's identify command
sizes=(`echo ${info[2]} | awk -Fx '{print $1}{print $2}'`)
hsize=${sizes[0]}
vsize=${sizes[1]}

# determine if hsize is an even number or not...
hsize_odd=$(( $(( hsize % 2)) ? 1 : 0 ))
vsize_odd=$(( $(( vsize % 2)) ? 1 : 0 ))

# ... if so, correct it by adding one (mpeg2encode seems to complain otherwise)
# note: this is *only* useful if you are trying to convert a seqence of odd horizonal- or
#       vertical-sized yuv images.  A mismatch in specified yuv image sizes and the actual size
#       used to create the yuv (i.e. from the original tif or gif or whatever) will generate a reasonably
#       acceptable mpeg.  However, if ppm images require size-adjustment in the mpeg2encode.par
#       file then you're in touble --> this means the ppm image has been 'identify'ed as an odd-sized image
#       in either it's horizonal or vertical dimension and specifying a different number in the parameter
#       file will generate a bad mpeg.  Use ./mpeg_utils_imagetoppm.sh to convert the original file into a ppm and, in the
#       process, if hsize or vsize is not even-numbered, ./mpeg_utils_imagetoppm.sh will pad the image with 'pnmpad' such
#       that this script will not require adjustment to get even-numbered hzize and vsizes.
if [ "$hsize_odd" = "1" ];
then
   let hsize=hsize+1
fi
if [ "$vsize_odd" = "1" ];
then
   let vsize=vsize+1
fi

# variable descriptions:
#    fps		- frame rate code, specifying frames per second
#    parfile		- output parameter file
#    basefilename	- text-string common to each filename (not including and leading directory names)
#    maxnumchars	- number of characters in the largest number in the sequence, e.g. 2 for 10-99 frames, 3 for 100-999 frames, etc.
#    statfile		- may be '-' for stdout (default=/dev/null)
#    ipff		- input picture file format (only 1->yuv3 or 2->ppm used here)
#    Nframes		- total number of frames (image files) within the sequence
#    Ngop		- 12 or 15, number of frames within a Group Of Frames (GOP)
#    hsize		- horizonal images size, from ImageMagick's identify command
#    vsize		- horizonal images size, from ImageMagick's identify command
#    frc		- framerate code, 1,2,3,4, or 5, see mpeg2encode documentation
#    bitrate		- bits/sec, see mpeg2encode documentation
#    vidfmt		- video format = 1->PAL, 2->NTSC, see mpeg2encode documentation
#    Nmatcoef		- 5->PAL, 4->NTSC, see mpeg2encode documentation

echo $'\t' "./mpeg_utils_make_mpeg2encode_parfile.sh: writing $parfile."
      echo "MPEG-1, $fps frames/sec, `date`, parameter file generated by ./mpeg_utils_make_mpeg2encode_parfile.sh" > $parfile
      echo "$basefilename%0$maxnumchars""d    /* name of source files */" >> $parfile
      echo "-         /* name of reconstructed images (\"-\": dont store) */" >> $parfile
      echo "-         /* name of intra quant matrix file     (\"-\": default matrix) */" >> $parfile
      echo "-         /* name of non intra quant matrix file (\"-\": default matrix) */" >> $parfile
      echo "$statfile         /* name of statistics file (\"-\": stdout ) */" >> $parfile
      echo "$ipff         /* input picture file format: 0=*.Y,*.U,*.V, 1=*.yuv, 2=*.ppm */" >> $parfile
      echo "$Nframes       /* number of frames */" >> $parfile
      echo "0         /* number of first frame */" >> $parfile
      echo "00:00:00:00 /* timecode of first frame */" >> $parfile
      echo "$Ngop        /* N (# of frames in GOP) */" >> $parfile
      echo "3         /* M (I/P frame distance) */" >> $parfile
      echo "1         /* ISO/IEC 11172-2 stream (0=MPEG-2, 1=MPEG-1)*/" >> $parfile
      echo "0         /* 0:frame pictures, 1:field pictures */" >> $parfile
      echo "${hsize}   /* horizontal_size */" >> $parfile
      echo "${vsize}  /* vertical_size */" >> $parfile
      echo "8         /* aspect_ratio_information 1=square pel, 2=4:3, 3=16:9, 4=2.11:1 */" >> $parfile
      echo "$frc         /* frame_rate_code 1=23.976, 2=24, 3=25, 4=29.97, 5=30 frames/sec. */" >> $parfile
      echo "$bitrate /* bit_rate (bits/s) */" >> $parfile
      echo "20       /* vbv_buffer_size (in multiples of 16 kbit) */" >> $parfile
      echo "0         /* low_delay  */" >> $parfile
      echo "1         /* constrained_parameters_flag */" >> $parfile
      echo "4         /* Profile ID: Simple = 5, Main = 4, SNR = 3, Spatial = 2, High = 1 */" >> $parfile
      echo "8         /* Level ID:   Low = 10, Main = 8, High 1440 = 6, High = 4	  */" >> $parfile
      echo "1         /* progressive_sequence */" >> $parfile
      echo "1         /* chroma_format: 1=4:2:0, 2=4:2:2, 3=4:4:4 */" >> $parfile
      echo "$vidfmt         /* video_format: 0=comp., 1=PAL, 2=NTSC, 3=SECAM, 4=MAC, 5=unspec. */" >> $parfile
      echo "5         /* color_primaries */" >> $parfile
      echo "5         /* transfer_characteristics */" >> $parfile
      echo "$Nmatcoef         /* matrix_coefficients */" >> $parfile
      echo "$hsize       /* display_horizontal_size */" >> $parfile
      echo "$vsize       /* display_vertical_size */" >> $parfile
      echo "0         /* intra_dc_precision (0: 8 bit, 1: 9 bit, 2: 10 bit, 3: 11 bit */" >> $parfile
      echo "0         /* top_field_first */" >> $parfile
      echo "1 1 1     /* frame_pred_frame_dct (I P B) */" >> $parfile
      echo "0 0 0     /* concealment_motion_vectors (I P B) */" >> $parfile
      echo "0 0 0     /* q_scale_type  (I P B) */" >> $parfile
      echo "0 0 0     /* intra_vlc_format (I P B)*/" >> $parfile
      echo "0 0 0     /* alternate_scan (I P B) */" >> $parfile
      echo "0         /* repeat_first_field */" >> $parfile
      echo "1         /* progressive_frame */" >> $parfile
      echo "0         /* P distance between complete intra slice refresh */" >> $parfile
      echo "0         /* rate control: r (reaction parameter) */" >> $parfile
      echo "0         /* rate control: avg_act (initial average activity) */" >> $parfile
      echo "0         /* rate control: Xi (initial I frame global complexity measure) */" >> $parfile
      echo "0         /* rate control: Xp (initial P frame global complexity measure) */" >> $parfile
      echo "0         /* rate control: Xb (initial B frame global complexity measure) */" >> $parfile
      echo "0         /* rate control: d0i (initial I frame virtual buffer fullness) */" >> $parfile
      echo "0         /* rate control: d0p (initial P frame virtual buffer fullness) */" >> $parfile
      echo "0         /* rate control: d0b (initial B frame virtual buffer fullness) */" >> $parfile
      echo "2 2 11 11 /* P:  forw_hor_f_code forw_vert_f_code search_width/height */" >> $parfile
      echo "1 1 3  3  /* B1: forw_hor_f_code forw_vert_f_code search_width/height */" >> $parfile
      echo "1 1 7  7  /* B1: back_hor_f_code back_vert_f_code search_width/height */" >> $parfile
      echo "1 1 7  7  /* B2: forw_hor_f_code forw_vert_f_code search_width/height */" >> $parfile
      echo "1 1 3  3  /* B2: back_hor_f_code back_vert_f_code search_width/height */"  >> $parfile ;



fi # end "if [ "$1" = "" ];" approximately on line 69


