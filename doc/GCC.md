# The GNU Compiler GCC

The *GNU Compiler Collection*, formerly the *GNU C Compiler*,
or GCC was written by Richard Stallman as the compiler for
the GNU project and is now a widely used compiler for C, C++,
and a number of other languages. It is a key component in
what is knwon as the *GNU Toolchain*, together with GDB, Make,
Binutils, Autotools, Bison.

Do I have gcc and what version?
What features are included?
What options are available?

```sh
gcc --version     # show version number
gcc -v            # show more details (target, configure options)
gcc --help        # show options of driver program (gcc)
gcc -v --help     # show options, also of subprocesses
man gcc           # manual page lists all options
```

## Usage

```sh
gcc [options] file...
```

Here are some real-world examples:

```Makefile
gcc file1.c file2.c                      # (1)
gcc -o myprog file1.c file2.c            # (2)
gcc -c file.c                            # (3)
gcc -Wall -o prog main.c libfoo.a -lbar  # (4)
$(CC) $(CFLAGS) -c -o $@ $<              # (5)
$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@   # (6)
$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS) $(LDLIBS) # (7)
```

1. Compile and assemble the two files, and link them
   into the executable file *a.out* (default name).
2. Compile *file1.c* and *file2.c* and link them into
   the executable *myprog*.
3. Compile and assemble *file.c* but do not link;
   the result will be a file *file.o*.
4. Compile *main.c* with ample warnings, link against *libfoo.a*
   and *libbar* from the library search path, and write the result
   to *prog*; note that libraries come after source files.
5. Typical invocation from a *Makefile* to compile a source
   file into an object file (`-c`) named after the target (`$@`).
6. Typical invocation from a *Makefile* to compile and link
   source files into an executable named after the rule target.
7. Typical invocation from a *Makefile* to link one or more
   object files against some libraries. `CC`, `CFLAGS`, and
   `LDLIBS` are conventional *Makefile* variables.

The linker searches object files (.o) and libraries (libX.a or -lX)
in the order they are specified. For example, if *foo.c* references
*sin* from the math library, `gcc -lm foo.c` will fail, whereas
`gcc foo.c -lm` will work because gcc will find the unresolved
symbol *sin* from *foo.c* in the following library *libm.a*.
Note that this behaviour may be used to override functionality.

Also, the search paths for libraries and include files are built
in the order specified by the `-L` and `-I` options.

## Common options

`-c` compile and assemble (but do not link)  
`-o file` place output into *file*

`-Wall` enable “all” warnings (highly recommended)  
`-Wextra` enable even more warnings (also recommended)  
`-std=c89` request conformance to the C89 standard  
`-std=c90` same as `-std=c89` (ISO ratified the same standard in 1990)  
`-std=c99` request conformance to the C99 standard  
`-ansi` equivalent to `-std=c90` or `-std=c++98`  
`-pedantic` reject programs that use exensions beyond
the specified standard

`-g` embed symbol information for debugging  
`-O0` disable optimization (for debugging)  
`-Og` enable optimizations that do not disturb debugging  
`-Os` optimize for space  

`-lm` link to the math library, required for functions in `<math.h>`  
`-lfoo` link against library `libfoo` (place *after* source files)

`-Idir` append *dir* to the include file search path  
`-Ldir` append *dir* to the library search path

## A few other options

`-E` preprocess only  
`-S` compile only (do not assemble or link)  
`-s` remove symbol table and relocation information
  (what the **strip**(1) utility does): gives smaller
  executable but cannot debug  
`-Dname` define macro name  
`-Dname=value` define macro name to have the given value  
`-x lang` specify language of the following input files
          (normally determined from the file extension)  
`-shared` create a shared library (also say `-fPIC` or `-fpic`)  
`-static` do not link with shared libraries

## Building a static library

Static libaries have the extension `.a` and are archives
of object files built by the **ar**(1) tool. When linking
against a static library, required object files are copied
from the library into the executable being built.

To create a static library, just create an archive that
contains the desired object files and create an object
index. The latter is done with the `s` option to **ar**(1).

```sh
ar rcs mylibrary.a file1.o file2.o
```

- Command `r` inserts files into the archive, replacing
  existing content of the same name.
- Option `c` silences the warning that `r` issues if the archive
  does not exist; `r` implicitly creates a missing archive.
- Option `s` creates or updates an object-file index,
  which is used by the linker.
- GNU comes with a separate tool **ranlib**(1) which creates
  the symbol index on an **ar** archive; it is equivalent to
  **ar s** but not part of POSIX. Both tools are part of
  [GNU binutils][binutils].

## Building a shared library

Shared libraries have the extension `.so` (shared object)
(or `.dll` for dynamic link library). When linking against
a shared library, no code is copied from the library into
the program, but when the program starts, it is “dynamically
linked” against the shared library (which must be in a location
found by the startup code).
By default, GCC links against a shared library (if available).

To create a shared library, compile with the `-fPIC` option
to create “position independent code” and then link with the
`-shared` option.

```sh
gcc -fPIC -Wall -c file1.c
gcc -fPIC -Wall -c file2.c
gcc -shared -o libfoo.so file1.o file2.o
```

About installing a shared library, see elsewhere.

## Compilation phases

1. Preprocessor: file inclusion and macro expansion (`.i`)
2. Compiler: from source to assembly (`.s`)
3. Assembler: from assembly to object (`.o`)
4. Linker: from object to executable or library

Normally, the driver `gcc` pushes the sources through all these
steps to create the final executable. In non-trivial projects,
`-c` is used to separate object code generation from linking.
It is instructionaly to push a source file through all steps
individually:

```sh
cat << EOT > hello.c
#include <stdio.h>
int main() { printf("Hello world!\n"); return 0; }
EOT
gcc -E -o hello.i hello.c    # preprocess hello.c into hello.i
gcc -S hello.i               # compile hello.i to assembly hello.s
as -o hello.o hello.s        # assemble hello.s into object code
ld -o hello /lib/crt0.o hello.o -lc  # link hello.o with libc.a and runtime
```

## References

Homepage of the GNU Compiler Collection:  
<http://www.gnu.org/software/gcc/>

The Linux Program Library HOWTO:  
<https://tldp.org/HOWTO/Program-Library-HOWTO/index.html>

[binutils]: https://www.gnu.org/software/binutils/
