# The Standard C Library

The standard library is an integral part of the C standard.
The functionality of the ANSI C Standard Library is declared
in the following headers. Other headers, like `<stdint.h>`,
are **not** part of the ANSI C standard.

|Header|Remarks|
|------|-------|
|`<assert.h>` | assertions: the `assert()` macro|
|`<ctype.h>`  | character classification: `isalpha()` etc.|
|`<errno.h>`  | the `errno` symbol and the `EXXX` constants|
|`<float.h>`  | various float and double constants|
|`<limits.h>` | size of integral types: `XXX_MIN` and `XXX_MAX` (see [limits.c](../src/limits.c))|
|`<locale.h>` | localisation: `setlocale()` and `localeconv()`|
|`<math.h>`   | math functions: `atan2()`, `sqrt()`, etc.|
|`<setjmp.h>` | non-local jumps: `setjmp()` and `longjmp()`|
|`<signal.h>` | signal processing: `raise()` and `signal()`|
|`<stdarg.h>` | variable argument lists: `va_list` etc.|
|`<stddef.h>` | `NULL`, `size_t`, `ptrdiff_t`, `wchar_t`, `offsetof()`|
|`<stdio.h>`  | I/O streams: `fopen()`, `printf()`, etc.|
|`<stdlib.h>` | memory allocation, random numbers, and varia|
|`<string.h>` | strings and byte arrays: `strxxx()` and `memxxx()`|
|`<time.h>`   | getting and formatting time: `time()`, `strftime()`, etc.|

## Assertions

Assertions document assumptions inherent in the code
and enforce them at run-time by aborting the program
if an assertion fails. The standard `<assert.h>` header
defines the **assert** macro. If `NDEBUG` is defined
when `<assert.h>` is included, it always evaluates to
`((void) 0)`, that is, it does nothing. Otherwise, it
tests the argument expression; if true, it does nothing,
if false, it prints a diagnostic message to standard
error and aborts the program. Beware that the expression
being asserted must not have side effects.
It is recommended to leave `NDEBUG` undefined.
The header *could* be implemented as follows:

```C
#undef assert
#ifdef NDEBUG
#define assert(test) ((void) 0)
#else
void _assert(char *msg);
#define _STR(x) _VAL(x)
#define _VAL(x) #x
#define assert(test) ((test) ? (void) 0 : \
  _assert(__FILE__ ":" _STR(__LINE__) " " #test))
#endif
```

The initial `#undef` allows assertions to be turned
on and off in the same source file. The active form
of the macro evaluates to a conditional expression
resulting in `((void) 0)` if the assertion holds and
an invocation of some `_assert` function if it fails.
The `_STR` and `_VAL` trickery converts `__LINE__`
(an integer) into a literal string: when on line 13,
`_STR(__LINE__)` evaluates to `_VAL(13)`, which then
evaluates to the string `"13"`, such that the `_assert`
function is called with a literal string argument like
`"foo.c:13 bar > 0"`.

## Error Codes

The `<errno.h>` header provides the *errno* symbol
and a number of error codes like `EDOM` and `ERANGE`.
At program startup, *errno* is zero. Many library
functions may set it to a positive value to indicate
an error.

In the olden days, *errno* was an ordinary global `int`
variable, but with the advent of multi-threading, *errno*
became a macro that expands to a thread-local lvalue.
Therefore, a declaration like `extern int errno;` is an
invalid substitute for the correct `#include <errno.h>`.

Standard C requires only three error codes: `EDOM`
(domain error), `ERANGE` (result out of range), and
`EILSEQ` (a multi-byte encoding error). The vast
majority of the other, better known, error codes
are from POSIX: `EINVAL` (invalid arguments),
`ENOMEM` (out of memory), `EACCESS` (permission denied),
`EMFILE` (too many open files), etc.

Beware that you may want to preserve *errno* across library
calls, for example:

```C
if (f() == NULL) {
  int saverr = errno;
  printf("Oops, f() failed\n"); /* may set errno! */
  if (saverr == EAGAIN) ...
}
```

## Standard Type Definitions

The `<stddef.h>` header provides a few standard type definitions:

- `NULL` – the null pointer constant, a macro usually
  defined to be `(void *) 0`
- `offsetof(type, member)` – integer offset of the field
  *member* from the beginning of the structure *type*.
- `size_t` – an *unsigned integer*,
  the type of the result of the `sizeof` operator.
- `ptrdiff_t` – a *signed integer*,
  the type of the result of subtracting two pointers.
- `wchar_t` – an integer type to represent wide characters, large
  enough for the largest character set among the supported locales.

Notes:

- **Prefer UTF-8** over wide characters; do not use `wchar_t`.
  See also: [utf8everywhere.org](https://utf8everywhere.org)
- Dereferencing a NULL pointer is **undefined in C**;
  therefore, an implementation may assume that any pointer
  that is dereferenced is not NULL.
- `ssize_t` is **not** part of the C standard (it is part
  of POSIX and defined in the `<sys/types.h>` header).

## Character Types

The `<ctype.h>` header is for character class testing.

```C
isalnum(c)   // isalpha(c) or isdigit(c)
isalpha(c)   // isupper(c) or islower(c)
iscntrl(c)   // control character
isdigit(c)   // decimal digit
isgraph(c)   // printing character except space
islower(c)   // lower-case letter
isprint(c)   // printing character including space
ispunct(c)   // punctuation
isspace(c)   // space, \n, \r, \t, formfeed, vertical tab
isupper(c)   // upper-case letter
isxdigit(c)  // hexadecimal digit: 0-9, a-f, A-F
```

In all these functions (typically implemented as macros),
the argument is of type `int` but must hold a value
representable as an `unsigned char` or the special value
`EOF` (usually `-1`). The return value is of type `int`;
it is non-zero if the condition is satisified, and zero
if not.

In the ASCII character set, the printing characters are
0x20 (space) through 0x7E (tilde); the control characters
are 0 to 0x1F, and 0x7F (DEL).

The function `tolower(c)` returns the corresponding
lower-case letter if `isupper(c)` and `c` if not.
Similarly, `toupper(c)` returns the corresponding
upper-case letter if `islower(c)` and `c` if not.

## Mathematics

The `<math.h>` header provides mathematical functions.
If an argument is outside the domain over which the
function is defined, *errno* will be set to `EDOM`.
If the result of a function overflows (cannot be
represented in a `double`), *errno* will be set to
`ERANGE` and the function returns `HUGE_VAL` with
the proper sign; if the function result underflows,
zero will be returned (and *errno* **may** be set
to `ERANGE`, but that is implementation-dependent).

```C
fabs(x)    // absolute value: |x|
ceil(x)    // least integer not leass than x
floor(x)   // largest integer not greater than x
sqrt(x)    // square root of x, x >= 0
exp(x)     // exponential function: e^x
log(x)     // natural logarithm: ln(x), x > 0
log10(x)   // base 10 logarithm of x, x > 0
pow(x,y)   // x to the power of y
sin(x)     // sine of x, x in radians
cos(x)     // cosine of x, x in radians
tan(x)     // tangent of x, x in radians
asin(x)    // arc sine of x, x in [-1, 1], range [-π/2, π/2]
acos(x)    // arc cosine of x, x in [-1, 1], range [0, π]
atan(x)    // arc tangent of x, range [-π/2, π/2]
atan2(y,x) // arc tan y/x in range [-π, π]
sinh(x)    // hyperbolic sine of x
cosh(x)    // hyperbolic cosine of x
tanh(x)    // hyperbolic tangent of x
ldexp(x,n) // x times 2^n
frexp(x, int *exp) // split x into a normalized fraction in [1/2, 1)
           // and an exponent stored into *exp
modf(x, *ip) // split x into a fractional part (returned)
           // and an integral part (stored into *ip)
fmod(x,y)  // floating-point remainder of x/y with same sign as x
```

Note: A program using any of the math functions above
must be linked with `-lm` to include the math library.

## Variable Arguments

The `<stdarg.h>` header provides the facility to step through
the arguments of a function with a variable number of arguments.

```C
#include <stdarg.h>

va_list ap;

void va_start(va_list ap, lastarg);
type va_arg(va_list ap, type);
void va_end(va_list ap);
void va_copy(va_list dst, va_list src); // C99
```

Usage is as follows: in a variable arguments function,
declare a variable *ap* of type `va_list` and initialize
it with `va_start(ap, lastarg)` where *lastarg* is the
function's last named argument. Then, each call to
`va_arg(ap, type)` returns the next unnamed argument,
assuming it is of tye given *type*. When done, `va_end(ap)`
must be called.

C99 and later also provide `va_copy(aq,ap)` to copy a
previously initialized argument list *ap* to *aq*.
A plain `va_list aq = ap;` may work on some systems;
`va_copy` will work on all systems. The copy must be
released with `va_end(aq)` as well.

Note: the historic `<varargs.h>` may be available
but is not part of the C standard and obsolete.

## Long Jumps

The `<setjmp.h>` header allows for directly exiting
from a deeply nested function call, for example, because
of an error condition or an asynchronous signal. It is
remotely similar to the `try`/`catch` and `throw` mechanism
of other languages.

```C
#include <setjmp.h>

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);
```

The `setjmp` macro saves state into *env* for a later return
by a call to `longjmp`. It returns `0` when it was called
directly, and it returns the non-zero *val* passed to `longjmp`
if it returns from “far away”.

## String Functions

The `<string.h>` header provides functions character strings and
byte arrays. In the following signatures, `s` and `t` have type `char*`,
`cs` and `ct` have type `const char*`, and `n` has type `size_t`.
The `str...` functions operate on zero-terminated char strings:

```C
char *strcpy(s, ct)      // copy ct incl \0 to s, return s
char *strncpy(s, ct, n)  // same as above but copy no more than n chars

char *strcat(s, ct)      // append ct incl \0 to s, return s
char *strncat(s, ct, n)  // same as above but copy no more than n chars

int strcmp(cs, ct)       // return <0 if cs < ct, 0 if cs = ct, >0 if cs > ct
int strncmp(cs, ct, n)   // same as above for no more than n chars

char *strchr(cs, c)      // ptr to first occurrence of c, NULL if none
char *strrchr(cs, c)     // ptr to last occurrence of c, NULL if none

size_t strlen(cs)        // return length of cs, excluding \0
```

Some functions are more esoteric (see see [Collate.md](./Collate.md)
about the even more exotic `strcoll` and `strxfrm`):

```C
size_t strspn(cs, ct)    // len of prefix consisting of chars in ct
size_t strcspn(cs, ct)   // len of prefix consisting of chars not in ct
char *strpbrk(cs, ct)    // ptr to 1st occurrence of any char of ct in cs
char *strstr(cs, ct)     // ptr to 1st occurrence of ct in cs or NULL
char *strerror(int n)    // ptr to description of error n (EXXX)
char *strtok(s, ct)      // tokenize s, see man page
```

The `mem...` functions manipulate byte arrays:

```C
void *memcpy(s, ct, n)   // copy n chars from ct to s, return s
void *memmove(s, ct, n)  // same as above, works with overlapping objects
int memcmp(cs, ct, n)    // compare n bytes, return same as strcmp
void *memchr(cs, c, n)   // ptr to 1st occurrence of c in cs or NULL
void *memset(s, c, n)    // place c in 1st n bytes of s, return s
```

Except for `memmove`, the behavior is undefined if source and target
memory regions overlap. These functions are in principle simple to
implement, but the idea is that the library provides efficient
routines optimised for the particular runtime environment.

## Signals

The `<signal.h>` header provides facilities for handling exceptional
conditions such as an interrupt signal or floating point error.

```C
#include <signal.h>

void (*signal(int sig, void (*handler)(int)))(int);
int raise(int sig);

typedef ... sig_atomic_t;
```

The C standard requires 6 signals:

- `SIGABRT` abnormal termination, usually from a call to `abort()`
- `SIGFPE` arithmetic error, e.g., divison by zero
- `SIGILL` illegal executable code, e.g., illegal instruction
- `SIGINT` interactive attention, e.g., a user pressing the
   interrupt key on a UNIX console
- `SIGSEGV` invalid storage access, e.g., misaligned or
   outside memory limits
- `SIGTERM` termination request

An implementation may have many others. This is particularly true
of implementatoins on UNIX, where the concpet of signals comes from.

Use `raise(sig)` to send signal *sig* to the calling program; the
return value is 0 on success and non-zero if the signal cannot be sent.

Use `signal()` to determine how signal *sig* is to be handled:

- `SIG_DFL`: implementation-defined default behaviour
- `SIG_IGN`: ignore signal *sig*
- All other values for *handler* are interpreted to be a
  pointer to a signal handling function, returning `void` and
  taking an `int` (the signal number) as the only parameter.

The value returned is the previous handler, or `SIG_ERR` if
an error occurs.

When a signal occurs, it's handler is reset to its default.

The nasty function prototype for `signal()` can be rewritten
in a more readable way:

```C
typedef void sigfunc(int);
sigfunc *signal(int, sigfunc *);
```

### Using signals

Avoid using signals: doing so improves portability.

Signal handlers can be grouped as

1. those that must not return and simply report an arithmetic
   exception (`SIGFPE`) or a fatal error (`SIGABRT`), and

2. those that must return, such as in response to a `SIGINT`
   asking for interactive attention; these signals usually
   occur asynchronously and may have interrupted a library
   operation.

A signal handler that must not return should end in a call to
`abort()`, `exit()`, or `longjmp()` and should not reestablish
itself. Do not call abort() in a signal handler for `SIGABRT`.

A signal handler that must return ends in a `return` statement.
If it is to reestablish itself, this should happen as the first
action upon entry. Of course, there remains a race condition:
namely if the signal occurs again, prior to reestablishing the
handler. Since these handlers usually are called asynchronously
to the program flow, they should do little but set a global flag,
which is tested outside the signal handler.

Typical code looks like that:

```C
#include <signal.h>

static volatile sig_atomic_t intflag = 0;

static void sigint(int sig)
{
  signal(SIGINT, &sigint);  /* re-establish interrupt handler */
  intflag = 1;
}
```

The type `sig_atomic_t` is defined in `<signal.h>` and is
an integer type small enough that it can be accessed atomically,
even in the presence of asynchronous interrupts (signals).
Declaring *intflag* above as `volatile` forces the compiler
to actually access the memory and not to keep this variable
in a register. All objects shared between a (asynchronous)
signal handler and the rest of the code should be of type
`volatile sig_atomic_t`.

### Implementing Signals

How would signals be implemented? The code shown below is
taken from *The Standard C Library* (Plauger 1992).

The header `<signal.h>` defines just the six signals of
the C standard using numbers typical in Unix systems.
The implementation of `signal()` and `raise()` is simple
and generic in that it does make use of the signals that
the underlying operating system might provide.

#### signal.h

```C
/* signal.h standard header */
#ifndef _SIGNAL
#define _SIGNAL
   /* type definitions */
typedef int sig_atomic_t;
typedef void _Sigfun(int);
   /* signal codes */
#define SIGABRT 6
#define SIGINT  2
#define SIGILL  4
#define SIGFPE  8
#define SIGSEGV 11
#define SIGTERM 15
#define _NSIG   32 /* one more than last code */
   /* signal return values */
#define SIG_DFL (_Sigfun *) 0
#define SIG_ERR (_Sigfun *) -1
#define SIG_IGN (_Sigfun *) 1
   /* declarations */
int raise(int);
_Sigfun *signal(int, _Sigfun *);
#endif
```

#### signal.c

```C
/* signal function -- simple version */
#include <signal.h>

   /* external declarations */
extern _Sigfun *_Sigtable[_NSIG];

_Sigfun *(signal)(int sig, _Sigfun *fun)
{
  _Sigfun *s;

  if (sig <= 0 || _NSIG <= sig || fun == SIG_ERR)
        return (SIG_ERR);  /* bad signal */
  /* add machine-dependent handling here */
  s = _Sigtable[sig], _Sigtable[sig] = fun;
  return s;
}
```

#### raise.c

```C
/* raise function -- simple version */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

   /* static data */
_Sigfun *_Sigtable[_NSIG] = {0};  /* handler table */

int (raise)(int sig)  /* raise a signal */
{
  _Sigfun *s;

  if (sig <= 0 || _NSIG <= sig)
        return -1;  /* bad signal */
  if ((s = _Sigtable[sig]) != SIG_IGN && s != SIG_DFL) {
        _Sigtable[sig] = SIG_DFL;  /* revert handler */
        (*s)(sig);  /* call handler */
  }
  else if (s == SIG_DFL) {  /* default handling */
        char ac[10], *p;

        switch (sig) {  /* print known signals by name */
        case SIGABRT:
                p = "abort";
                break;
        case SIGFPE:
                p = "arithmetic error";
                break;
        case SIGILL:
                p = "invalid executable code";
                break;
        case SIGINT:
                p = "interruption";
                break;
        case SIGSEGV:
                p = "invalid storage access";
                break;
        case SIGTERM:
                p = "termination request";
                break;
        default:
                *(p = &ac[(sizeof ac) - 1]) = '\0';
                do *--p = sig % 10 + '0';
                while ((sig /= 10) != 0);
                fputs("signal #", stderr);
        }
        fputs(p, stderr);
        fputs(" -- terminating\n", stderr);
        exit(EXIT_FAILURE);
  }
  return 0;
}
```

### Implementation on Unix

Implementing signals on Unix is particularly simple because
signals in C are modeled after the signals in Unix.

```C
#include <signal.h>

_Sigfun *_Signal(int, _Sigfun *);
int _Getpid(void);
int _Kill(int pid, int sig);

_Sigfun *(signal)(int sig, _Sigfun *fun)
{
  return _Signal(sig, fun);
}

int (raise)(int sig)
{
  return _Kill(_Getpid(), sig);
}
```

### Testing <signal.h>

```C
/* test signal functions */
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/* see if required types and constants are defined and
   properly typed (signal and raise are exercised below) */
static int sigs[] = {
  SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM };
static void (*rets[])(int) = {SIG_DFL, SIG_ERR, SIG_IGN};
static sig_atomic_t atomic;

static void field_fpe(int sig)  /* handle SIGFPE */
{
  assert(sig == SIGFPE);
  puts("SUCCESS testing <signal.h>");
  exit(EXIT_SUCCESS);
}

int main()  /* test basic workings of signal functions */
{
  printf("sizeof (sig_atomic_t) = %u\n", sizeof(sig_atomic_t));
  assert(signal(SIGFPE, &field_fpe) == SIG_DFL);
  assert(signal(SIGFPE, &field_fpe) == &field_fpe);
  raise(SIGFPE);
  puts("FAILURE testing <signal.h>");
  return EXIT_FAILURE;
}
```

## Input and Output

The `<stdio.h>` header provides input, output, and formatting
facilities. Files and other data sources/sinks are abstracted
as *streams* by `FILE` objects. Three such streams are already
open at program startup: the standard input, output, and error
streams, named `stdin`, `stdout`, and `stderr`. For brevity,
the descriptions below use *fn* for filenames and *fp* for
streams (FILE pointer). For details consult the manual pages.

### File operations

```C
FILE *fopen(const char *fn, const char *mode);
FILE *freopen(const char *fn, const char *mode, FILE *fp);
FILE *tmpfile(void);         /* create temporary file */
int fileno(FILE *fp);        /* file descriptor for stream, POSIX */
int setvbuf(FILE *fp, char *buf, int mode, size_t size);
void setbuf(FILE *fp, char *buf);
int fflush(FILE *fp);        /* write buffered data */
int fclose(FILE *fp);        /* flush buffer close stream */

int remove(const char *fn);
int rename(const char *oldname, const char *newname);
char *tmpnam(char s[]);      /* DO NOT USE */

size_t fread(void *ptr, size_t size, size_t nobj, FILE *fp);
size_t fwrite(const void *ptr, size_t size, size_t nobj, FILE *fp);

int getchar(void);           /* getc(stdin) */
int getc(FILE *fp);          /* may be a macro */
int fgetc(FILE *fp);         /* always a function */
char *fgets(char *buf, int size, FILE *fp);
char *gets(char *buf);       /* DO NOT USE */
int ungetc(int c, FILE *fp); /* put c back onto stream */

int putchar(int c);          /* putc(c, stdout) */
int putc(int c, FILE *fp);   /* may be a macro */
int fputc(int c, FILE *fp);  /* always a function */
int puts(const char *s);     /* write s and a '\n' to stdout */
int fputs(const char *s, FILE *fp); /* write s to fp */

int printf(const char *fmt, ...);  /* to stdout */
int fprintf(FILE *fp, const char *fmt, ...);
int sprintf(char *buf, const char *fmt, ...);
int snprintf(char *buf, size_t size, const char *fmt, ...); /* C99 */

int vprintf(const char *fmt, va_list ap);
int vfprintf(FILE *fp, const char *fmt, va_list ap);
int vsprintf(char *buf, const char *fmt, va_list ap);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap); /* C99 */

int scanf(const char *fmt, ...);  /* from stdin */
int fscanf(FILE *fp, const char *fmt, ...);
int sscanf(char *s, const char *fmt, ...);

long ftell(FILE *fp);        /* get current file position */
int fseek(FILE *fp, long offset, int origin); /* set file position */
void rewind(FILE *fp);       /* seek to beginning of file, clearerr */
int fgetpos(FILE *fp, fpos_t *ptr);
int fsetpos(FILE *fp, const fpos_t *ptr);

int feof(FILE *fp);          /* non-zero iff EOF flag set */
int ferror(FILE *fp);        /* non-zero iff error flag set */
void clearerr(FILE *fp);     /* clear eof and error flags */
void perror(const char *s);  /* system error message to stderr */
```

Notes:

- printf: the `l` length modifier indicates the argument is
  of type `long` or instead of `int`; the `h` length modifier
  indicates the argument is of type `short` instead of `int`.
- printf: the `z` length modifier is for `size_t` or `ssize_t`
  arguments, and `t` is for `ptrdiff_t` arguments
- `snprintf` and `vsnprintf` appeared with C99.
- `fileno` is from POSIX, not from the C standard library.

### Implementation

K&R provide an implementation of `fopen` and `getc` on top
of a Unix system as an example of C in use. The stream type
`FILE` is a structure that holds the Unix file descriptor,
a buffer, and some flags. By convention, names beginning
with an underscore are meant for library-internal use only.

Header stuff:

```C
#define NULL      0
#define EOF       (-1)
#define BUFSIZ    1024
#define OPEN_MAX  20   /* max #files open at once */

typedef struct _iobuf {
    int  cnt;          /* characters left */
    char *ptr;         /* next character position */
    char *base;        /* location of buffer */
    int  flag;         /* mode of file access */
    int  fd;           /* file descriptor */
} FILE;
extern FILE _iob[OPEN_MAX];

#define stdin   (&_iob[0])
#define stdout  (&_iob[1])
#define stderr  (&_iob[2])

enum _flags {
    _READ   = 01,      /* file open for reading */
    _WRITE  = 02,      /* file open for writing */
    _UNBUF  = 04,      /* file is unbuffered */
    _EOF    = 010,     /* EOF has occurred on this file */
    _ERR    = 020      /* error occurred on this file */
};

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);

#define feof(p)     (((p)->flag & _EOF) != 0)
#define ferror(p)   (((p)->flag & _ERR) != 0)
#define fileno(p)   ((p)->fd)

#define getc(p)    (--(p)->cnt >= 0 \
    ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x,p)  (--(p)->cnt >= 0 \
    ? *(p)->ptr++ = (x) : _flushbuf((x),p))

#define getchar()   getc(stdin)
#define putchar(x)  putc((x), stdout)
```

The implementation maintains a list of `FILE` structures
and calls upon Unix file services:

```C
#include <fcntl.h>
#include "syscalls.h"
#define PERMS 0666   /* RW for owner, group, others */

FILE _iob[OPEN_MAX] = {  /* stdin, stdout, stderr: */
    { 0, (char *) 0, (char *) 0, _READ, 0 },
    { 0, (char *) 0, (char *) 0, _WRITE, 1 },
    { 0, (char *) 0, (char *) 0, _WRITE | _UNBUF, 2 }
};

/* fopen: open file, return file ptr */
FILE *fopen(char *name, char *mode)
{
  int fd;
  FILE *fp;

  if (*mode != 'r' && *mode != 'w' && *mode != 'a')
    return NULL;
  for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
    if ((fp->flag & (_READ | _WRITE)) == 0)
      break;  /* found free slot */
  if (fp >= _iob + OPEN_MAX)  /* no free slots */
    return NULL;
  
  if (*mode == 'w')
    fd = creat(name, PERMS);
  else if (*mode == 'a') {
    if ((fd = open(name, O_WRONLY, 0)) == -1)
      fd = creat(name, PERMS);
    lseek(fd, 0L, 2);
  } else
    fd = open(name, O_RDONLY, 0);
  if (fd == -1)  /* couldn't access name */
    return NULL;
  fp->fd = fd;
  fp->cnt = 0;
  fp->base = NULL;
  fp>flag = (*mode == 'r') ? _READ : _WRITE;
  return fp;
}

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
  int bufsize;

  if ((fp->flag & (_READ|_EOF|_ERR)) != _READ)
    return EOF;
  bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
  if (fp->base == NULL)  /* no buffer yet */
    if ((fp->base = (char *) malloc(bufsize)) == NULL)
      return EOF;
  fp->ptr = fp->base;
  fp->cnt = read(fp->fd, fp->ptr, bufsize);
  if (--fp->cnt < 0) {
    if (fp->cnt == -1)
      fp->flag |= _EOF;
    else
      fp->flag |= _ERR;
    fp->cnt = 0;
    return EOF;
  }
  return (unsigned char) *fp->ptr++;
}
```

## Miscellaneous Utilities

The `<stdlib.h>` header provides functions for many different
purposes.

### Convert string to number

```C
double atof(const char *s)
int atoi(const char *s)
long atol(const char *s)

double strtod(const char *s, char **endp)
long strtol(const char *s, char **endp, int base)
unsigned long strtoul(const char *s, char **endp, int base)
```

**atof** is equivalent to `strtod(s, (char **) NULL)`.  
**atoi** is equivalent to `(int) strtol(s, (char **) NULL, 10)`.  
**atol** is equivalent to `strtol(s, (char **) NULL, 10)`.

**strtod** converts the prefix of *s* to `double`, ignoring
leading white space; it stores a pointer to any unconverted
sufix into the memory pointed to by *endp* (unless it is NULL).
If the answer would overflow, `HUGE_VAL` is returned with the
proper sign; if the answer would underflow, zero is returned.
In either case, *errno* is set to `ERANGE`.

**strtol** converts the prefix of *s* to `long`, ignoring
leading white space; it stores a pointer to any unconverted
suffix into the memory pointed ty by *endp* (unless it is NULL).
If *base* is between 2 and 36, conversion is done assuming that
the input is written in that base. If *base* is zero, the base
is 8, 10, or 16; leading `0` implies octal and leading `0x` or
`0X` hexadecimal. Letters in either case represent digits from
10 to *base*-1; a leading `0x` or `0X` is permitted in base 16.
If the answer would overflow, `LONG_MAX` or `LONG_MIN` is returned,
depending on the sign of the result, and *errno* is set to `ERANGE`.

**strtoul** is the same except that the result is `unsigned long`
and the error value is `ULONG_MAX`.

### Random numbers

```C
int rand(void)
void srand(unsigned int seed)
```

**rand** returns a pseudo-random integer in the range
`0` to `RAND_MAX`, which is at least 32767; **srand**
installs *seed* as the seed for a new sequence of numbers.
The initial seed is 1. This random number generator may be
quite weak and should not be used for cryptographic purposes.

### Heap memory

```C
void *calloc(size_t nobj, size_t size)
void *malloc(size_t size)
void *realloc(void *p, size_t size)
void free(void *p)
```

The first three functions allocate memory and return a pointer
to the first byte allocated or NULL if the request fails.

**calloc** allocates memory for an array of *nobj* objects,
each of *size* bytes. The allocated memory is initialised
to all zeros.

**malloc** allocates *size* bytes of memory.
The memory allocated is *not* initialised.

**realloc** changes the size of a memory block previously
allocated using `malloc` or `calloc`, and returns a pointer
to the re-allocated memory block, which may be different
from *p*. If *p* is NULL, `realloc` is the same as `malloc`.
If *size* is zero, `realloc` is the same as `free`.

**free** returns the memory block pointed to by *p* to the pool
of unused heap blocks. The behaviour is undefined if *p* was
not allocated by one of the above routines or if it is freed
more than once.

### Program exit

```C
void abort(void)
void exit(int status)
int atexit(void (*f)(void))
```

**abort** causes the program to terminate abnormally,
as if by `raise(SIGABRT)`.

**exit** causes normal program termination. `atexit` functions
are called in reverse order of registration, open files are
flushed, open streams are closed, and control is returned to
the environment. How *status* is returned to the environment
is implementation-dependent, but zero is taken as successful
termination. The values `EXIT_SUCCESS` and `EXIT_FAILURE` may
also be used.

**atexit** registers the function *f* to be called when the
program terminates normally (with `exit` or by returning from
`main`); it returns non-zero if the registration cannot be made.

### Environment

```C
char *getenv(const char *name)
```

**getenv** returns the environment string associated with *name*,
or NULL if no string exists. Details are implementation-dependent.
(On most Unix systems there are also functions to modify
the environment, typically called putenv or the like.)

### System

```C
int system(const char *s)
```

**system** passes the string *s* to the environment for execution.
If *s* is NULL, `system` returns non-zero if there is a command processor.
If *s* is not NULL, the return value is implementation-dependent.

### Varia

```C
void *bsearch(const void *key, const void *base, size_t n, size_t size,
  int (*cmp)(const void *keyval, const void *datum))
void qsort(void *base, size_t n, size_t size,
  int (*cmp)(const void *, const void *))
int abs(int n)
long labs(long n)
div_t div(int numer, int denom)
ldiv_t ldiv(long numer, long denom)
```

The first two functions implement binary search and quick sort.
**abs** and **labs** compute the absolute value of their arguments,
and **div** and **ldiv** compute quotient and remainder of *numer*/*denom*.
