# My C Library

This is my collection of C utility functions,
some of my own design and writing, some from
public domain code I found useful.
I also collect here some notes on C and related topics.

The stuff here may be used as a static library,
as a “copy-paste” source, or simply as a memory aid.

![MIT License](https://img.shields.io/badge/License-MIT-blue.svg)
Code and notes available under the [MIT License](LICENSE).

## Overview

- Duff's Device: [Duff.md](doc/Duff.md), [duff.c](src/duff.c)
- Endian: [Endian.md](doc/Endian.md), [endian.h](src/endian.h), [endian.c](src/endian.c)
- Scanning: [Scan.md](doc/Scan.md), [scan.h](src/scan.h)
- Style: [Notes on Programming in C](doc/PikeStyle.md) by Rob Pike
- Testing: [test.h](src/test.h), [runtests.c](test/runtests.c)

## C Standard Library

The C Programming Language is specified together
with the C Standard Library (ANSI C, ISO C).
It provides some 200 functions for I/O, dynamic
memory, mathematics, assertions, formatting, etc.
It is useful, always available, but somewhat
inconsistent.
There are several implementations, including
[glibc](https://www.gnu.org/software/libc/) (GNU)
or [musl libc](https://musl.libc.org/) (especially
for static linking).

## Literature

Classics on C programming and Unix programming are:

B.W. Kernighan, D.M. Ritchie: *The C Programming Language*.
Second edition, Prentice Hall, 1988.
[Amazon](https://www.amazon.com/dp/0131103628)

P.J. Plauger: *The Standard C Library*. Prentice Hall, 1992.
[Amazon](https://www.amazon.com/dp/0131315099)

W. Richard Stevens: *Advanced Programming in the Unix
Environment*. Addison-Wesley, 1992.
[Amazon](https://www.amazon.com/dp/0201563177)
