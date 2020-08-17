#include "myutils.h"

#include <stdlib.h>
#include <time.h>

/* UTCSTAMP: 2005-07-15T12:34:56Z */
/* TAISTAMP: @4000000042d77da2 */

/* If year < 1000 or year > 9999 we do not print the
 * year, but the string "xxxx" to mark the problem.
 */
#define xxxx 0x78787878 /* ASCII */
#define DEFAULT_SEP 'T' /* by ISO8601 */

size_t /* format UTC timestamp into buf */
utcstamp(char buf[], char sep)
{
  register char *p;
  struct tm *tp;
  time_t now;
  long x;

  now = time((time_t *) 0);
  tp = gmtime(&now);
  if (!tp) abort();

  p = buf;

  x = tp->tm_year + 1900;
  if ((x < 1000) || (x > 9999)) { *((long *) p) = xxxx; p += 4; }
  else { p += 4;
    do { *--p = '0' + (x % 10); x /= 10; } while (x);
    p += 4;
  }

  x = tp->tm_mon + 1;
  p[0] = '-'; p[2] = '0' + (x % 10); x /= 10; p[1] = '0' + (x % 10);
  p += 3;

  x = tp->tm_mday;
  p[0] = '-'; p[2] = '0' + (x % 10); x /= 10; p[1] = '0' + (x % 10);
  p += 3;

  x = tp->tm_hour;
  p[0] = sep ? sep : DEFAULT_SEP;
  p[2] = '0' + (x % 10); x /= 10; p[1] = '0' + (x % 10);
  p += 3;

  x = tp->tm_min;
  p[0] = ':'; p[2] = '0' + (x % 10); x /= 10; p[1] = '0' + (x % 10);
  p += 3;

  x = tp->tm_sec;
  p[0] = ':'; p[2] = '0' + (x % 10); x /= 10; p[1] = '0' + (x % 10);
  p += 3;

  *p++ = 'Z'; /* means UTC */
  return p - buf; /* #chars printed */
}

#ifdef UTCSTAMP
#include <stdio.h>
#include <time.h>
int main(int argc, char **argv)
{ /* output utcstamp, followed by args */
  char buf[64];

  putbuf(buf, utcstamp(buf));

  if (*argv) argv++; /* skip arg0 */
  while (*argv) putfmt(" %s", *argv++);
  putbyte('\n');

  return 0;
}
#endif /* UTCSTAMP */
