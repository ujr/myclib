# Makefiles

Consider sticking to POSIX make features only.
This trades compatibility for features.
If doing so, state `.POSIX:` in the first non-comment line
to (1) document your intention and (2) request POSIX compliant
behaviour (vendor extensions are still available).

Do not use recursive Makefiles (breaks the dependency tree and results
in fragile builds); instead, have one Makefile at the root of the project.
To refer to files in subdirectories, include the subdirectory in the name,
for example: `obj/main.o: src/main.c`.

Beware that out-of-source builds (putting .o files in a separate
directory from the .c files) is not compatible with inference rules.

The special target `.SUFFIXES:` (without a value) clears the list
of built-in inference rules. Further `.SUFFIXES` (with a value)
will add to the now empty list:

```Makefile
.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
    $(CC) $(CFLAGS) -c $<
```

Conventional phony targets:

- `all` (the default target, build everything)
- `clean` (delete all generated files)
- `install` (install built artifacts)
- `distclean` (delete even more than *clean*)
- `test` or `check` (run the test suite)
- `dist` (create a package for distribution)

The `install` target, by convention, should use PREFIX and DESTDIR:

- PREFIX: should default to */usr/local*
- DESTDIR: for staged builds (install in a fake root)

## References

[A Tutorial on Portable Makefiles](https://nullprogram.com/blog/2017/08/20/)
from Chris Wellons's blog (20 Aug 2017); it refers to the [specification for
make](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/make.html)
at the Open Group (the POSIX standard).

Oram, Andrew and Talbott, Steve:
*Managing Projects with make*,
O'Reilly & Associates, Inc., 1991.

Kernighan, Brian W. and Pike, Rob:
*The UNIX Programming Environment*,
Prentice Hall.

The [POSIX standards](http://pubs.opengroup.org/onlinepubs/9699919799/)
(IEEE Std 1003.1-2017), volume about *Shell & Utilities*.
