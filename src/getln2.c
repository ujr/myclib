#include "myutils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define SEP '\n'

long /* read line, realloc if needed, return #chars, 0 on eof, -1 on error */
getln2(FILE *fp, char **buf, size_t *size)
{
  if (!fp || !buf || !size) {
    errno = EINVAL;
    return -1;
  }

  int c;
  char *p = *buf;
  size_t n = 0;
  size_t max = p ? *size : 0;

  for (;;) {
    if (n+1 >= max) {
      max = 3*((max+16)/2);
      p = realloc(p, max);
      if (!p) return -1; /* nomem */
      *buf = p;
      *size = max;
    }
    c = getc(fp);
    if (c == SEP || c == EOF) {
      if (c == SEP) p[n++] = c;
      p[n] = '\0';
      break;
    }
    p[n++] = (char) (unsigned char) c;
  }

  if (ferror(fp)) return -1; /* ioerr */
  return (c == EOF && n == 0) ? 0 : n;
}
