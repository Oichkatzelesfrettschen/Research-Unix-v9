# Building Research UNIX V9

This project contains the historical Research UNIX Version 9 source tree. The
code targets Sun3 workstations and expects a m68k cross toolchain.

## Dependencies

Run `setup.sh` to install the required packages. It installs:

- `clang` and `gcc-m68k-linux-gnu` for cross compiling
- `cscope`, `tree-sitter-cli` and utilities for code navigation
- Python packages `tree-sitter`, `cloc`, and `lizard`

## Building

```
./setup.sh
make
```

To test individual files you can call Clang directly. The makefiles build with
the following flags:

```sh
clang -Oz -Iv9/include \
  -Wno-implicit-int -Wno-implicit-function-declaration \
  -c v9/cmd/cp.c
```

The makefiles default to the `-Oz` optimization level which produces smaller executables suitable for constrained systems. You can override this via the `CFLAGS`
environment variable if necessary.

The build is not guaranteed to succeed on modern toolchains but this will
invoke the top level Makefile.

