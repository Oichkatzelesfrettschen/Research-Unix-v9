# Research UNIX V9 Build Instructions

This repository contains the source for the Ninth Edition UNIX system. Modern
compiler toolchains such as Clang can be used to build the commands, libraries
and kernel.

## Prerequisites

Run `setup.sh` to install development dependencies. It installs Clang and
additional utilities such as Tree-sitter tooling, cscope, cloc and the Python
packages used for code analysis.

```sh
./setup.sh
```

## Building

Use the top-level `Makefile` to build all components. The default compiler is
`cc` (typically `clang` after running `setup.sh`) but can be overridden via the
`CC` environment variable. All makefiles default to the `-Oz` optimization flag
which reduces binary size. Adjust `CFLAGS` for different trade-offs.

```sh
make
```

### Manual compilation example

To experiment with individual files outside the build system you can invoke
Clang directly with the same flags used by the makefiles:

```sh
clang -Oz -Iv9/include \
  -Wno-implicit-int -Wno-implicit-function-declaration \
  -c v9/cmd/cp.c
```

## Code Analysis

The `analyze.sh` script generates cscope databases, counts lines with `cloc`,
collects complexity metrics via `lizard` and attempts to create Tree-sitter
TAGS if the C grammar is available.

```sh
./analyze.sh
```
