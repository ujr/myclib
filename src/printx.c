#include "print.h"

size_t /* print an unsigned long in hex, return #chars */
printx(char *s, unsigned long val)
{
  static const char *digits = "0123456789ABCDEF";
  register unsigned long v = val;
  register int len;

  for (len = 1; v > 15; len++) v /= 16; /* find #digits */
  if (s) { s += len; /* emit least significant first */
    do { *--s = digits[val%16]; val /= 16; }
    while (val > 0);
  }
  return len;
}
