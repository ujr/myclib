# My C Utils

This is my collection of C utility functions,
some of my own design and writing, some from
public domain code I found useful.
I also collect here notes on C and related topics.

The stuff here may be used as a static library,
as a “copy-paste” source, or simply as a memory aid.

![MIT License](https://img.shields.io/badge/License-MIT-blue.svg)  
Code and notes available under the [MIT License](LICENSE).

## Overview

Notes on C and related tools:

- [Notes on C](doc/C.md) and its [Standard Library](doc/CLib.md)
- [Notes on POSIX](doc/POSIX.md), a standard for portable programs
- Integer limits: compile and run [limits.c](src/limits.c)
- Style: [Notes on Programming in C](doc/PikeStyle.md) by Rob Pike
- Duff's Device: [Duff.md](doc/Duff.md), [duff.c](src/duff.c)
- The syslog facility: [syslog.md](doc/syslog.md)
- The curses screen handling library: [curses.md](doc/curses.md)

Stuff in the library:

- Buffer: [buf.h](src/buf.h) (header only)
- Command line parsing: [scf.md](doc/scf.md), [scf.h](src/scf.h)
- Config (INI) file parsing: [iniconf.md](doc/iniconf.md), [iniconf.h](src/iniconf.h)
- Endian: [Endian.md](doc/Endian.md), [endian.c](src/endian.c)
- Formatting: [print.md](doc/print.md), [print.h](src/print.h)
- Getopt: [getopt.h](src/getopt.h) (header only) (cf scf.c/h)
- Scanning: [scan.md](doc/scan.md), [scan.h](src/scan.h)
- Growable string: [strbuf.md](doc/strbuf.md), [strbuf.h](src/strbuf.h)
- Simple I/O: [simpleio.md](doc/simpleio.md), [simpleio.h](src/simpleio.h)
- Testing: [test.h](src/test.h), [runtests.c](src/runtests.c)
- Unix: [Unix.md](doc/Unix.md), [myunix.h](src/myunix.h)
- UTF-8: [utf8.md](doc/utf8.md), [utf8.h](src/utf8.h)
- Utils: [Utils.md](doc/Utils.md), [myutils.h](src/myutils.h)

Tools

- Make, [Makefiles](doc/Makefiles.md) in general,
  and our [Makefile](./Makefile) as an example
- [GCC](doc/GCC.md) is the GNU Compiler Collection,
  and [GDB](doc/GDB.md) is the GNU Debugger; together with
  GNU Make they form the “GNU Toolchain”, to which you may
  add Binutils (as, ld, nm, strip, etc.), Autotools, and Bison.
- [Valgrind](https://www.valgrind.org) is a free memory debugging tool:
  compile with `-g` and `-O0` or `-O1`, then run the executable
  under Valgrind control with `valgrind --leak-check=full myprog`.
- [EditorConfig](https://editorconfig.org) is a recent *de facto*
  standard for consistent formatting, supported by ever more editors.
  A file named *.editorconfig* in the project root (or a subdirectory)
  provides preferences for encoding, line ending, and indentation.

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

## Conventions

Short variable names:

- `c` for characters
- `i`, `j`, `k` for indices
- `n`, `m` for counts and counters
- `p`, `q` for pointers
- `s`, `t`, `z` for zero-terminated strings

- `fn` for a filename (`const char *`)
- `fp` for a `FILE` pointer

## Literature

Classics on C and Unix programming include:

B.W. Kernighan, D.M. Ritchie: *The C Programming Language*.
Second edition, Prentice Hall, 1988.
[Amazon](https://www.amazon.com/dp/0131103628)

P.J. Plauger: *The Standard C Library*. Prentice Hall, 1992.
[Amazon](https://www.amazon.com/dp/0131315099)

W. Richard Stevens: *Advanced Programming in the Unix
Environment*. Addison-Wesley, 1992.
[Amazon](https://www.amazon.com/dp/0201563177) and
<http://www.kohala.com/start/>

Steve Summit:
*C Programming FAQs*. Addison-Wesley, 1995.
[Amazon](https://www.amazon.com/dp/0201845199)
and online at [c-faq.com](http://c-faq.com/)
