# Compiler command for diff
CC ?= cc
DIFF=	/bin/diff
DIFFH=	/usr/lib/diffh
PR=	/bin/pr
CFLAGS=	-O -DDIFF='"${DIFF}"' -DDIFFH='"${DIFFH}"' -DPR='"${PR}"'

SRCS=	diff.c diffdir.c diffreg.c
HDRS=	diff.h
OBJS=	diff.o diffdir.o diffreg.o

all: diff diffh

diff: ${OBJS}
	$(CC) ${CFLAGS} -o diff ${OBJS} ${LDFLAGS}

diffh: diffh.o
	$(CC) -o diffh diffh.o ${LDFLAGS}

clean:
	rm -f diff diffh ${OBJS} diffh.o

install: all
	strip diff diffh
	cp diff ${DIFF}
	cp diffh ${DIFFH}
