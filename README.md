# Research UNIX V9 Build Instructions

This repository contains the source for the Ninth Edition UNIX system. Modern
compiler toolchains such as Clang can be used to build the commands, libraries
and kernel.

## Prerequisites

Run `setup.sh` to install development dependencies. It installs Clang 14,
Tree-sitter tooling, cscope, cloc and Python utilities used for code analysis.

```sh
./setup.sh
```

## Building

Use the top-level `Makefile` to build all components. The default compiler is
`clang-14` but can be overridden via the `CC` environment variable.

```sh
make
```

## Code Analysis

The `analyze.sh` script generates cscope databases, counts lines with `cloc`,
collects complexity metrics via `lizard` and attempts to create Tree-sitter
TAGS if the C grammar is available.

```sh
./analyze.sh
```
