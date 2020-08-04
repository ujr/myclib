# Notes on POSIX

POSIX is the “Portable Operating System Interface”,
a standard by the IEEE Computer Society that aims
at compatibility between operating systems. POSIX
is modeled after the Unix systems. It defines a
number of system calls, library routines, a shell
and command line utilities.
The name “POSIX” was coined by Richard Stallman.

POSIX is a superset of the Standard C Library.

POSIX extends the Standard C Library by many more
headers typical for Unix systems, but also with
specifications for many command line utilities
and a command line shell.

The standard can be found online on the site of
the industry consortium The Open Group:

- IEEE Std 1003.1 2004 edition  
<http://www.opengroup.org/onlinepubs/009695399/>

- IEEE Std 1003.1 2017 edition  
<http://www.opengroup.org/onlinepubs/9699919799/>

## Headers

In addition to the headers that are part of the
Standard C Library (see [C Lib Notes](./CLib.md)),
it defines, amongst others, the following headers:

|Header|Remarks|
|------|-------|
|`<dirent.h>`|reading file system directories|
|`<fnmatch.h>`|file name matching (glob patterns)|
|`<ftw.h>`|file tree walk (traversal)|
|`<poll.h>`|the `poll()` function for I/O multiplexing|
|`<regex.h>`|regular expression matching|
|`<stdint.h>`|integer types like `int8_t` and `uint32_t`|
|`<strings.h>`|`strcasecmp` and `strncasecmp` and `ffs`|
|`<syslog.h>`|the system error logger: `syslog()` etc.|
|`<unistd.h>`|Unix system functions|
|`<sys/select.h>`|the `select()` function for I/O multiplexing|
|`<sys/stat.h>`|the `stat()` function and definitions|
|`<sys/types.h>`|system types like `uid_t` and `ssize_t`|
|`<sys/wait.h>`|the `waitpid()` function and definitions|

## Make

Compared to GNU make, POSIX make is quite limited,
but very portable. Attempt to write Makefiles that
use only POSIX make features.

Internal macros: `$@`, `$%`, `$?`, `$<`, `$*` (only)  
Note that `$^` is badly missing from this set!

See separate notes: [Makefiles.md](./Makefiles.md)
