# Building Research UNIX V9

This project contains the historical Research UNIX Version 9 source tree. The
code targets Sun3 workstations and expects a m68k cross toolchain.

## Dependencies

Run `setup.sh` to install the required packages. It installs:

- `clang-14` and `gcc-m68k-linux-gnu` for cross compiling
- `cscope`, `tree-sitter-cli` and utilities for code navigation
- Python packages `tree-sitter`, `cloc`, and `lizard`

## Building

```
./setup.sh
make CC=clang-14
```

The build is not guaranteed to succeed on modern toolchains but this will
invoke the top level Makefile.

