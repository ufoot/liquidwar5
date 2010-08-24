# Copyright (C) 2003, David Redick
# Released under the GNU General Public License (v2)

CC=gcc
CFLAGS=-ggdb -O3 -Wall -W -ansi `allegro-config --cflags`
LDFLAGS=`allegro-config --libs` -lm
EXE=liquidwar-mapgen

OBJ=big_quad.o boxes.o bubbles.o circles.o circuit.o cut.o func.o hole.o \
	lines.o map.o map_generator.o misc.o rand_box.o rand_func.o rand_poly.o \
	rand_poly_cut.o street.o worms.o


all: $(EXE)

$(EXE): core main.c
	$(CC) $(CFLAGS) main.c $(OBJ) $(LDFLAGS) -o $(EXE)
	#strip $(EXE)

core: $(OBJ)
	@echo ""
	@echo "Done compiling core functions..."
	@echo ""

clean: cleanmap
	rm -rf $(EXE) *~ *.o

cleanmaps: cleanmap

cleanmap:
	rm -rf *.bmp ./maps/*

###############################################################################
# core files... keep func.o, map.o, map_generator.o and misc.o at top

misc.o: misc.c misc.h
	$(CC) $(CFLAGS) -c misc.c

func.o: func.c func.h func_list.h
	$(CC) $(CFLAGS) -c func.c

map.o: map.c map.h func.o misc.o
	$(CC) $(CFLAGS) -c map.c

map_generator.o: map_generator.c map_generator.h map.o
	$(CC) $(CFLAGS) -c map_generator.c


big_quad.o: big_quad.c map.o
	$(CC) $(CFLAGS) -c big_quad.c

boxes.o: boxes.c map.o
	$(CC) $(CFLAGS) -c boxes.c

bubbles.o: bubbles.c map.o
	$(CC) $(CFLAGS) -c bubbles.c

circles.o: circles.c map.o
	$(CC) $(CFLAGS) -c circles.c

circuit.o: circuit.c map.o
	$(CC) $(CFLAGS) -c circuit.c

cut.o: cut.c cut.h map.o
	$(CC) $(CFLAGS) -c cut.c

hole.o: hole.c rand_poly.o
	$(CC) $(CFLAGS) -c hole.c

lines.o: lines.c cut.o
	$(CC) $(CFLAGS) -c lines.c

rand_box.o: rand_box.c map.o
	$(CC) $(CFLAGS) -c rand_box.c

rand_func.o: rand_func.c map.o
	$(CC) $(CFLAGS) -c rand_func.c

rand_poly.o: rand_poly.c map.o
	$(CC) $(CFLAGS) -c rand_poly.c

rand_poly_cut.o: rand_poly_cut.c rand_poly.o cut.o
	$(CC) $(CFLAGS) -c rand_poly_cut.c

street.o: street.c map.o
	$(CC) $(CFLAGS) -c street.c

worms.o: worms.c map.o
	$(CC) $(CFLAGS) -c worms.c
