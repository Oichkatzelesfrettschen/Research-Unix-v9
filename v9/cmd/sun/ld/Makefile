#
# @(#)Makefile	1.1 (Sun) 2/3/86 16:23:29
#
# Compiler command used for the loader
CC ?= cc
# Flags passed to the compiler
CFLAGS ?= -O2 -Dsun3
OBJS    = ld.o incl.o
PROGRAM = ld

${PROGRAM}:	${OBJS}
	${CC} -o ${PROGRAM} ${OBJS}

install:	${PROGRAM}
	install -s ${PROGRAM} ${DESTDIR}/bin/${PROGRAM}

clean:	
	rm -f ${OBJS}


