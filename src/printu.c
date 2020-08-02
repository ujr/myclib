#include "print.h"

size_t /* print an unsigned long, return #chars */
printu(char *s, unsigned long val)
{
  static const char *digits = "0123456789";
  register unsigned long v = val;
  register size_t len;

  for (len = 1; v > 9; len++) v /= 10; /* find #digits */
  if (s) { s += len; /* start at end of buffer */
    do { *--s = digits[val%10]; val /= 10; }
    while (val > 0);
  }
  return len;
}
