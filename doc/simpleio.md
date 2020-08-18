# The simpleio.h API

The SimpleIO library is a very simple replacement for
`<stdio.h>`, allowing only reading from stdin, writing
to stdout, and (unbuffered) logging to stderr.

## Buffered reading from stdin

```C
int getbyte(void);
long getline(char *buf, size_t max, int sep, int *partial);
long skipline(int sep);
void ungetbyte(int c);
```

**getbyte:** return the next byte from stdin; return `-1`
on end-of-input (*errno* zero) or if there is a problem
(*errno* non-zero).

**getline:** read from stdin at most *max*-1 bytes or until
the first occurrence of *sep* into *buf* and terminate with
a null byte. Typically *sep* is `'\n'`; try `'\0'` to read
null-terminated strings, and set it to `-1` to read until
end-of-input. If *partial* is non-null, store `0` into the
integer pointed to if there was a *sep* in the first *max*-1
bytes read (we have a complete line), or store `1` if there is
no *sep* before reading *max*-1 bytes (we have a partial line).
Return the number of bytes read, including *sep* (but not
counting the terminating `'\0'`); return `-1` if there was a
problem, with *errno* set appropriately.

**skipline:** read and discard bytes from stdin until the
first occurrence of *sep* or the end of input. Return the
number of bytes skipped, including *sep*; or `-1` if there
was an error.

**ungetbyte:** push back the given byte onto stdin.
A pushed back byte will be the next byte returned by `getbyte`.
Can push back at most one byte. If pushing back more than one
byte, later push-backs replace earlier ones.

## Buffered writing to stdout

```C
int putbyte(int c);
int putstr(const char *s);
int putbuf(const char *buf, size_t len);
int putline(const char *s);

int putmode(int mode);
int putflush(void);
```

These functions write to stdout:
**putbyte** a byte at a time,
**putstr** a null-terminated string at a time, and
**putbuf** an arbitrary buffer at a time;
**putline** writes the string *s* up to the first newline,
if there is no newline in *s*, then it appends one.

They all return `0` if they are successful and `-1` otherwise.

Use **putmode** to determine how SimpleIO buffers output:
`SIOLINEBUF` means line-buffered (each `\n` forces the
output buffer to be flushed) and is the default;
`SIOFULLBUF` means fully buffered (flush when buffer full);
finally, `SIOAUTOBUF` means to set line-buffering if stdout
is associated with a terminal and full buffering if not
(see isatty(3)).

Use **putflush** to explicitly flush the output buffer.

## Unbuffered logging to stderr

```C
void logchar(int c);
void logstr(const char *s);
void logbuf(const char *buf, size_t len);
void logline(const char *s);
```

These functions are for logging to stderr. They do not return
errors, but simply call abort(3) if there is an error: we probably
couldn't report the error anyway.

**logbuf** writes an arbitrary buffer,
**logstr** a null-terminated string, and
**logchar** a single character to stderr.
**logline** is like logstr, but appends a newline if *s*
does not end in a newline.

## Miscellaneous

```C
int setin(const char *fn);
int setout(const char *fn);
int seterr(const char *fn);
```

**setin:** discard the contents of the input buffer and
associate the file identified by *fn* with standard input
(where the get-functions read from). If *fn* is null, only
the input buffer is discarded.

**setout:** flush the output buffer and associate the file
identified by *fn* with standard output (where the put-functions
write to). If *fn* already exists, it will be truncated to length
zero, otherwise it will be created. If *fn* is null, only the
output buffer will be flushed.

**seterr:** associate the file identified by *fn* with standard
error (where the log-functions write to). If *fn* already exists,
open it for *appending*; otherwise, create the file. (Since
output to stderr is unbuffered, no buffer needs to be flushed.)

All three functions return `0` if successful and `-1` on error,
with an error code in *errno*.

Since 2004-05-12
