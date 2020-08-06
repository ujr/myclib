#include "myutils.h"

#include <errno.h>
#include <stdio.h>

#define SEP '\n'

size_t /* skip current line, return #chars skipped */
eatln(FILE *fp)
{
  size_t n = 0;

  for (;;) {
    size_t m;
    int c = getc(fp);
    if (c == EOF) break;
    m = n+1;
    if (m < n) errno = ERANGE;
    n = m;
    if (c == SEP) break;
  }

  return n;
}
