# The myfuns.h API

Collection of miscellaneous utility functions.

```C
#include "myutils.h"

int argsplit(char *s, char *args[], int maxargs);
char *basename(const char *s);
int streq(const char *s, const char *t);

long getln(FILE* fp, strbuf *sp, int *partial);
long getln2(FILE *fp, char **buf, size_t *size);
long getln3(FILE *fp, char *buf, size_t *size, int *more);
size_t eatln(FILE *fp);

#define ENDIAN_LITTLE 1
#define ENDIAN_BIG    2
int getendian(void);

size_t utcscan(const char *s, struct tm *tp);
size_t utcstamp(char buf[], char sep);
#define UTCSTAMPLEN 20

int signum(number); /* the sign function */
```

**argsplit** splits the string *s* into individual arguments
and stores pointers to each argument in the array *args* that
is assumed to have *maxargs* entries. The last entry in *args*
is always set to null, even if there are *maxargs* or more
arguments in *s*. Note that the string *s* will be modified!

The value returned is the number of arguments found in *s*,
irrespective of *maxargs*.

To get only the number of arguments, pass `NULL` for *args*
and/or `0` for *maxargs*. In this case, the string *s* is
**not** modified.

Arguments are delimited by white space. Arguments containing
white space must be given in (single or double) quotes.
There is no escaping of spaces or quotes (as that would
require rewriting *s* – maybe another time).

Usage example:

```C
char *args[N];
...
if (argsplit(cmdline, args, N) >= N) error("too many args");
```

---

**basename** returns a pointer into *s* where the basename
starts, that is, after the last `/` in *s*, assuming there
is a '/' immediately before the start of *s*; if *s* is null,
return null.

---

**streq** compares strings *s* and *t* for equality; it is
equivalent to `strcmp(s,t)==0` except that *s* and *t* may
be null (if both are null, return true, otherwise false). Also,
`if (streq(s,t)) ...` reads nicer than `if (strcmp(s,t)==0) ...`
in my opinion.

---

**getln:** read from *fp* a line of characters, terminated
by a `\n` character. Store the line including the `\n` into
the `strbuf` pointed to by *sp*; set _*partial_ to `0` if
`\n` occurs before end-of-input and set it to `1` otherwise.
The `strbuf` is truncated to length zero before storing
the line. Return the number of characters read, `0` on
end-of-input, or `-1` on error.

**getln2:** read from *fp* a line of characters, terminated
by a `\n` character. Store the line including the `\n` and
the string terminator `\0` into the buffer _*buf_ of size
_*size_; if necessary, reallocate a larger buffer and
update _*buf_ and _*size_. Return the number of characters
read, `0` on end-of-input, or `-1` on error.

**getln3:** read from *fp* a line of characters, terminated
by a `\n` character. Store the line including the `\n` and
the string terminator `\0` into the buffer *buf* of the
given *size* and set _*partial_ to `0`. If the line does not
fit into the buffer, store only a prefix, the string terminator
`\0`, and set _*partial_ to `1`. Return the number of characters
read, `0` on end-of-input, or `-1` on error.

**eatln:** skip current line by reading characters from *fp*
up to and including the first `\n` character. Return the
number of characters skipped.

Usage pattern:

```C
FILE *fp;
long n;

while ((n = getln(fp, ...)) > 0) {
  /* process line */
}
if (n < 0) error("getln failed");
```

Note: **getline**(3) is a GNU extension that became part of
the POSIX standard in 2008. Therefore, the name `getline` is
probably already in use (unless you `#define _POSIX_C_SOURCE`
to a value smaller than 2008). To avoid conflict, our functions
are called `getln`.

Note: our functions return `long` to accomodate for a negative
value to indicate an error. POSIX has the `ssize_t` for this
purpose, but it is not part of ANSI C. We could return `size_t`
(which is unsigned) and reserve a value like `(size_t)-1` to signal
an error, but then the idiomatic `while ((n=getln(...)) > 0) ...`
would become more cumbersome.

---

**getendian:** determine the “endian” of the machine;
return `ENDIAN_BIG` if big endian (most significant
byte stored at lowest memory address) or `ENDIAN_LITTLE`
if little endian (least significant byte stored at
lowest memory address).

---

**utcscan:** scan a UTC stamp in ISO 8601 format
(`yyyy-mm-ddThh:mm:ssZ`) and return the number of
chars scanned, usually 20, but may be different in
case of leading blanks or multiple blanks separating
the date from the time (instead of the single `T`)
or an year less than 1000 or greater than 9999.

**utcstamp:** write the current system time (UTC)
in ISO 8601 format into the buffer provided. Separate
the date from the time with the character in *sep*
(if *sep* is `0`, write the `T` required by ISO 8601).
The buffer must be at least `UTCSTAMPLEN` chars long.
If the current year is less than 1000 or greater than
9999, it will be written as the string `xxxx`.
Return the number of characters written.

---

**signum:** returns 1 if its argument is positive,
0 if it is zero, or -1 if it is negative. Works with
integers and floating-point numbers. However, NaN
(not-a-number) returns 0, not NaN (because we always
return an integer). The function is not called `sign`
so that this useful name remains available.
