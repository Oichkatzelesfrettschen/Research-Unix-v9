# Research Unix Version 9

This repository contains the source tree for AT&T Research Unix **Version 9**. It is the historical release for Sun3 hardware and includes userland commands, libraries, and kernel sources. For additional background see `v9/README`.

## History

Version 9 Unix was developed at Bell Labs as the successor to Version 8. The code in this repository derives from a Sun3 port around 1987 and preserves the environment used internally at Bell Labs. Binaries for Sun3 are included in the tree alongside the sources.

## License

The source is distributed under the terms described in [COPYING.pdf](COPYING.pdf). Please read that document for the precise license.

## Directory overview

- **cmd** – User commands and utilities.
- **include** – System headers for building the userland.
- **ipc** – Interprocess communication libraries and dialers.
- **jerq** – Software for the AT&T 5620 terminal.
- **jtools** – Tools such as the `sam` editor and other utilities ported for X11.
- **libc** – Core C library.
- **libtermlib** – Terminal capability library.
- **netb** – Experimental network file system.
- **sys** – Kernel sources for the Sun3 port.
- **X11** – X11 Release 1 adapted for Version 9.

## Building on a modern system

The original build expects a running Version 9 environment. To experiment on a contemporary Unix-like host:

1. Ensure you have standard build tools (`make`, `cc`, `yacc`, etc.). GCC works for most components.
2. Many makefiles assume the v9 toolchain and may require edits when compiling natively. Cross‑compiling is recommended if you have access to a Version 9 system or emulator.
3. In general, build commands mirror traditional Unix builds:
   ```sh
   cd v9/cmd
   make
   ```
4. Kernel sources live in `v9/sys`. Building a kernel requires Sun3 hardware or an emulator and significant manual setup.

Due to the age of the code, expect to modify makefiles and source files for modern compilers. Refer to `v9/README` for detailed historical instructions.

