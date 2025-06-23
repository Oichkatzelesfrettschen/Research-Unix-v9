# Root build rules for Research UNIX V9
# Set default tools so the tree can be built with a modern toolchain
CC ?= cc
CFLAGS ?= -std=gnu89 -Oz
AS ?= as

.PHONY: all cmd libc kernel clean

all: cmd libc kernel

# Build user commands
cmd:
	$(MAKE) -C v9/cmd CC="$(CC)" CFLAGS="$(CFLAGS)" AS="$(AS)"

# Build libraries
libc:
	$(MAKE) -C v9/libc/stdio CC="$(CC)" CFLAGS="$(CFLAGS)" AS="$(AS)"
	$(MAKE) -C v9/libc/math CC="$(CC)" CFLAGS="$(CFLAGS)" AS="$(AS)"
	$(MAKE) -C v9/libc/sun CC="$(CC)" CFLAGS="$(CFLAGS)" AS="$(AS)"

# Build the kernel
kernel:
	$(MAKE) -f v9/sys/conf/mkfile CC="$(CC)" CFLAGS="$(CFLAGS)" AS="$(AS)"

clean:
	$(MAKE) -C v9/cmd clean
	$(MAKE) -C v9/libc/stdio clean
	$(MAKE) -C v9/libc/math clean
	$(MAKE) -C v9/libc/sun clean
	$(MAKE) -f v9/sys/conf/mkfile clean
