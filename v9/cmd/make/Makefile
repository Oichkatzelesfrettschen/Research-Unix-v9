# Description file for the Make command
# Makefile 4.1 81/02/28

P = ipr
T =
# Compiler command for building make
CC ?= cc
FILES = Makefile ident.c defs main.c doname.c misc.c files.c dosys.c gram.y
OBJECTS = ident.o main.o doname.o dosys.o gram.o misc.o files.o
POBJECTS = Pident.o Pmain.o Pdoname.o Pdosys.o Pgram.o Pmisc.o Pfiles.o
LIBES= 
LINT = lint -ps -DASCARCH
# Default compilation flags
CFLAGS ?= -O -DASCARCH -DVERSION8
PCFLAGS = -p -g -DASCARCH -DVERSION8

GCOSFILES = defs ident.c main.c doname.c misc.c gram.c gcos.c

a.out:  $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBES) 
all:	make

cmp:	make
	cmp make /bin/make
	rm -f *.o gram.c make

cp:	make
	cp make /bin/make
	rm -f *.o gram.c make


make:  $(OBJECTS)
	$(CC) -s $(CFLAGS) $(OBJECTS) $(LIBES) -o make

profmake: $(POBJECTS)
	cc $(PCFLAGS) $(POBJECTS) -o profmake

$(OBJECTS):  defs
$(POBJECTS): defs

clean:
	-rm -f *.o P*.c gram.c make a.out core

install:	make
	mv make $(DESTDIR)/bin/make

printall: # Print files off line.
	-pr $(FILES) | $P
	touch print

print:  $(FILES)	# print recently changed files
	-pr $? | $P
	touch print

test: 1zap 2zap
	diff 1zap 2zap
	rm -f 1zap 2zap

1zap:
	./make -dp | grep -v TIME >1zap

2zap:
	/bin/make -dp | grep -v TIME >2zap

time:  time1 time2 ;

time1 time2 :
	time ./make $T
	time /bin/make $T

lint :  dosys.c doname.c files.c main.c misc.c ident.c gram.c
	$(LINT) dosys.c doname.c files.c main.c misc.c ident.c gram.c
	rm -f gram.c

src:
	cp $(FILES) /usr/src/cmd/make

makearch:	$(FILES)
	ar uv makearch $?

v8:
	CFLAGS := $(CFLAGS) -DVERSION8
debug:
	CFLAGS := $(CFLAGS) -g

gcos: $(GCOSFILES)
	fsend -c -u sif $?
	touch gcos
