#include "myutils.h"

#include <errno.h>
#include <stdio.h>

#define SEP '\n'

long /* read line, return #chars read, 0 if eof, -1 on error */
getln(FILE *fp, strbuf *sp, int *partial)
{
  int c;

  if (!fp || !sp) {
    errno = EINVAL;
    return -1;
  }

  sbtrunc(sp, 0);
  while ((c = getc(fp)) != EOF && c != SEP) {
    sbaddc(sp, c);
  }
  if (c == SEP)
    sbaddc(sp, c);
  if (partial) *partial = c != SEP;

  if (c != EOF) return sblen(sp);
  return sbfailed(sp) || ferror(fp) ? -1 : (long) sblen(sp);
}
