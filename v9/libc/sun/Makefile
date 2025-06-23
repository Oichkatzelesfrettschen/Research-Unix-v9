# Compiler command for Sun-specific library
CC ?= cc
AS ?= as
# Base optimization flags
CFLAGS ?= -std=gnu89 -Oz

OFILES=	Faddd.o Fadds.o Faintd.o Faints.o Fdtos.o Ffloat.o Ffltd.o \
	Fflts.o Fmuld.o Fmuls.o Fstod.o fp_globals.o frexp.o ldexp.o \
	modf.o is68020.o ffs.o valloc.o

all: $(OFILES)

install:
	mv $(OFILES) ../OBJ

.s.o:
	/lib/cpp $< > $*.i
	$(AS) -o $*.o $*.i
	rm $*.i
