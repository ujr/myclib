#include "print.h"

size_t /* print a signed long, return #chars */
printd(char *s, signed long val)
{
  unsigned long u;
  size_t len;
  /* beware that -i == i if i is LONG_MIN, so `if (i<0) i*=-1` is a bug */
  if (val >= 0) { u = val; len = 0; }
  else { u = -val; len = 1; if (s) *s++ = '-'; }
  len += printu(s, u);
  return len;
}
