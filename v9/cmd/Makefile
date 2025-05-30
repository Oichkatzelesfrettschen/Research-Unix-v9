# Compiler command; default to the system C compiler
CC ?= cc
# Compiler flags for building utilities
CFLAGS ?= -O2
FILES=	ar cat chmod cmp cp date dd df du echo ed \
	fsck getty grep init kill ln login ls mkdir \
	mkfs mount mv newgrp nice pr printenv procmount \
	pwd ranlib rm rmdir size stty sync tar test \
	time umount update who write

all: $(FILES)

install: $(FILES)
	/bin/mv $(FILES) /n/labsun/src/sys/raw/test

clean:
	rm -f *.o $(FILES)

ar: ar.o
	$(CC) $(CFLAGS) -o $@ $@.o

cat: cat.o
	$(CC) $(CFLAGS) -o $@ $@.o

chmod: chmod.o
	$(CC) $(CFLAGS) -o $@ $@.o

cmp: cmp.o
	$(CC) $(CFLAGS) -o $@ $@.o

cp: cp.o
	$(CC) $(CFLAGS) -o $@ $@.o

date: date.o
	$(CC) $(CFLAGS) -o $@ $@.o

dd: dd.o
	$(CC) $(CFLAGS) -o $@ $@.o

df: df.o
	$(CC) $(CFLAGS) -o $@ $@.o

du: du.o
	$(CC) $(CFLAGS) -o $@ $@.o

echo: echo.o
	$(CC) $(CFLAGS) -o $@ $@.o

ed: ed.o
	$(CC) $(CFLAGS) -o $@ $@.o

fsck: fsck.o
	$(CC) $(CFLAGS) -o $@ $@.o

getty: getty.o
	$(CC) $(CFLAGS) -o $@ $@.o

grep: grep.o
	$(CC) $(CFLAGS) -o $@ $@.o

init: init.o
	$(CC) $(CFLAGS) -o $@ $@.o

kill: kill.o
	$(CC) $(CFLAGS) -o $@ $@.o

ln: ln.o
	$(CC) $(CFLAGS) -o $@ $@.o

login: login.o
	$(CC) $(CFLAGS) -o $@ $@.o

ls: ls.o
	$(CC) $(CFLAGS) -o $@ $@.o

mkdir: mkdir.o
	$(CC) $(CFLAGS) -o $@ $@.o

mkfs: mkfs.o
	$(CC) $(CFLAGS) -o $@ $@.o

mount: mount.o
	$(CC) $(CFLAGS) -o $@ $@.o

mv: mv.o
	$(CC) $(CFLAGS) -o $@ $@.o

newgrp: newgrp.o
	$(CC) $(CFLAGS) -o $@ $@.o

nice: nice.o
	$(CC) $(CFLAGS) -o $@ $@.o

pr: pr.o
	$(CC) $(CFLAGS) -o $@ $@.o

printenv: printenv.o
	$(CC) $(CFLAGS) -o $@ $@.o

procmount: procmount.o
	$(CC) $(CFLAGS) -o $@ $@.o

pwd: pwd.o
	$(CC) $(CFLAGS) -o $@ $@.o

ranlib: ranlib.o
	$(CC) $(CFLAGS) -o $@ $@.o

rm: rm.o
	$(CC) $(CFLAGS) -o $@ $@.o

rmdir: rmdir.o
	$(CC) $(CFLAGS) -o $@ $@.o

size: size.o
	$(CC) $(CFLAGS) -o $@ $@.o

stty: stty.o
	$(CC) $(CFLAGS) -o $@ $@.o

sync: sync.o
	$(CC) $(CFLAGS) -o $@ $@.o

tar: tar.o
	$(CC) $(CFLAGS) -o $@ $@.o

test: test.o
	$(CC) $(CFLAGS) -o $@ $@.o

time: time.o
	$(CC) $(CFLAGS) -o $@ $@.o

umount: umount.o
	$(CC) $(CFLAGS) -o $@ $@.o

update: update.o
	$(CC) $(CFLAGS) -o $@ $@.o

who: who.o
	$(CC) $(CFLAGS) -o $@ $@.o

write: write.o
	$(CC) $(CFLAGS) -o $@ $@.o
