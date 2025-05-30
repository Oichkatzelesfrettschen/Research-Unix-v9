# C23 Refactoring Candidate Files

This document lists potential source files that require modernization to C23
standards. It was generated using `find` and `grep` on the `v9` directory.

## Source file counts
- C files: `2395`
- Header files: `876`

## Directories with the most C sources

```
672  X11
469  cmd
406  jerq
354  sys
218  jtools
170  libc
92   ipc
8    netb
6    libtermlib
```

## Files likely using K&R style functions
A quick heuristic search for function definitions whose opening brace appears on
a separate line from the parameter list found **619** C files.  These files are
prime candidates for refactoring to modern prototypes.  The directories with the
highest counts are:

```
v9/jerq   : 167 files
v9/X11    : 117 files
v9/cmd    : 110 files
v9/jtools : 98 files
v9/sys    : 82 files
v9/ipc    : 22 files
v9/libc   : 20 files
v9/netb   : 3 files
```

Below are a few representative examples showing the K&R style:

```
main(argc, argv)
char **argv;
{
        int errs;
```
(from `v9/jerq/sgs/3nm.c`)

```
char *
dofile(file)
        char *file;
{
        int fd;
```
(from `v9/jerq/sgs/3nm.c`)
```

These patterns suggest missing function prototypes and parameter type
declarations on separate lines, typical of pre-ANSI C.

Modernizing these files will involve converting the function headers to
prototype style and ensuring proper headers for declarations.
