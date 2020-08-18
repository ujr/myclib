/* simpleio.c - a substitute for the standard I/O library for
 * simple applications that only use stdin, stdout, and stderr
 *
 * 2004-05-12 initial version
 * 2004-11-20 changed getline to count+append sep
 * 2005-07-15 corrected bug in getline: now return 0 on eof
 * 2006-08-13 removed putfmt and logfmt (use format.{h,c})
 * 2008-01-14 incorporated skipline.c
 */

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "simpleio.h"

/* input buffer */
static char inbuf[SIOBUFSIZEIN];
static long inidx = 0;
static long inlen = 0;

/* writeall: a perseverant write(), returning 0 if ok */
static int writeall(int fd, const char *buf, size_t len)
{
  while (len) { int w;
    do w = write(fd, buf, len);
    while (w < 0 && errno == EINTR);
    if (w < 0) return w; /* error, see errno */
    buf += w;
    len -= w;
  }
  return 0;
}

/* reopen: assign a new file to a given descriptor */
static int reopen(int fd, const char *path, int mode)
{
  int tmpfd = open(path, mode, 0660);
  if (tmpfd < 0) return -1;  /* see errno */

  if (dup2(tmpfd, fd) < 0) return -1;  /* see errno */
  (void) close(tmpfd);  /* don't care if this fails */
  errno = 0; return 0;
}

/* setin: associate file 'fn' with fd 0 (stdin) */
int setin(const char *fn)
{
  inidx = inlen = 0; /* flush input buffer */
  return fn ? reopen(0, fn, O_RDONLY) : 0;
}

/* setout: associate file 'fn' with fd 1 (stdout) */
int setout(const char *fn)
{
  if (putflush() < 0) return -1; /* flush output buffer */
  return fn ? reopen(1, fn, O_WRONLY|O_CREAT|O_TRUNC) : 0;
}

/* seterr: associate file 'fn' with fd 2 (stderr) */
int seterr(const char *fn)
{
  return reopen(2, fn, O_WRONLY|O_CREAT|O_APPEND);
}

/** buffered reading from stdin
 *
 * getbyte() returns the byte read or -1 on error/EOF.
 * getline() returns the number of bytes read or -1 on error; if
 *   the last byte read is not sep, then we've a partial line.
 */

static int ungotten = -1;
void ungetbyte(int c) { ungotten = c; }

int getbyte(void)
{
  if (ungotten >= 0) {
    int c = ungotten;
    ungotten = -1;
    return c;
  }

  if (inidx >= inlen) { /* buffer empty */
    do inlen = read(0, inbuf, sizeof(inbuf));
    while (inlen < 0 && errno == EINTR);
    if (inlen < 0) return -1; /* error, see errno */
    inidx = 0;
    errno = 0;
    if (inlen == 0) return -1; /* EOF */
  }
  return (unsigned char) inbuf[inidx++];
}

long getline(char *buf, size_t max, int sep, int *partial)
{
  char *p = buf;
  char *end = buf + max - 1;
  int c = -1;

  if (partial) *partial = 1;  /* so far it's a partial line! */
  while (p < end) {
    c = getbyte();
    if (c < 0) break; /* eof or error */
    *p++ = (char) (unsigned char) c;
    if (c == sep) break;  /* end-of-line */
  }
  if (max > 0) *p = '\0';  /* null-terminate string */
  if (c == sep && partial) *partial = 0;  /* full line */
  if (c < 0 && errno) return -1;
  return p - buf;  /* #chars read, incl 'sep', but without \0 */
}

long skipline(int sep)
{ /* skip to sep; return #bytes skipped, -1 on error */
  int c;
  long n = 0;

  do { c = getbyte(); ++n; }
  while (c >= 0 && c != sep);

  return (c == sep) ? n : -1;
}

/** buffered writing to stdout **/

static int buffering = 0;  /* 0 = line, 1 = fully buffered */

int putmode(int mode)
{
  if (mode == 0 || mode == 1) buffering = mode;
  else buffering = isatty(1) ? 0 : 1;

  return buffering;
}

int putbyte(int c)
{
  static char buf[SIOBUFSIZEOUT];
  static size_t i = 0;

  if (c >= 0) buf[i++] = (unsigned char) c; /* buffer c */
  if (i >= sizeof buf || (i > 0 && (c < 0 || (c=='\n' && buffering==0)))) {
    if (writeall(1, buf, i) < 0) return -1;
    i = 0; /* reset index */
  }
  return 0;
}

int putstr(const char *s)
{
  if (s) while (*s)
    if (putbyte(*s++) < 0) return -1;
  return 0;
}

int putbuf(const char *buf, size_t len)
{
  if (buf) while (len-- > 0)
    if (putbyte(*buf++) < 0) return -1;
  return 0;
}

int putline(const char *s)
{
  register const char *p = s;
  if (p) while (*p && (*p != '\n')) putbyte(*p++);
  return putbyte('\n');  /* 0 if ok, -1 otherwise */
}

/** unbuffered writing to stderr
 *
 * Routines for log output to stderr;
 * here logbuf() is the basis since this output is unbuffered.
 * Silently abort(3) on error -- we couldn't report it anyway.
 * Null pointers are simply ignored.
 */

void logbuf(const char *buf, size_t len)
{
  if (buf == 0 || len == 0) return;
  if (writeall(2, buf, len) < 0) abort();
}

void logstr(const char *s)
{
  if (s) logbuf(s, strlen(s));
}

void logbyte(int c)
{
  char cc = (char) (unsigned char) c;
  logbuf(&cc, 1);
}

void logline(const char *s)
{
  if (s) {
    size_t len = strlen(s);
    logbuf(s, len);
    if (s[len-1] == '\n') return;
  }
  logbuf("\n", 1);
}
