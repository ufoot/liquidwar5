#!/bin/sh
#
# Script which launches Liquid War using files in the source/build tree
# instead of system wide files, very convenient for testing.

./src/liquidwar \
	-dat ./data/liquidwar.dat \
	-map ./custom/map \
	-tex ./custom/texture \
	-mid ./custom/music

