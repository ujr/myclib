#include "myutils.h"

#include <errno.h>
#include <stdio.h>

#define SEP '\n'

long /* read line from fp into buf, return #chars read, 0 on eof, -1 on error */
getln3(FILE *fp, char *buf, size_t size, int *partial)
{
  char *p = buf;
  char *end = buf + size - 1; /* -1 for terminating \0 */
  int c = EOF;

  if (!fp || !buf) {
    errno = EINVAL;
    return -1;
  }

  while (p < end) {
    c = getc(fp);
    if (c == EOF) break; /* eof or error */
    *p++ = (char) (unsigned char) c;
    if (c == SEP) break; /* end-of-line */
  }

  if (size > 0) *p = '\0'; /* terminate string */

  /* distinguish error from end-of-file */
  if (c == EOF && ferror(fp)) return -1;

  /* flag partially read line (buffer too small) */
  if (partial) *partial = (p >= end && c != SEP) ? 1 : 0;

  return p-buf;
}
