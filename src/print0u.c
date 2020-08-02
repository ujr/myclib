#include "print.h"

size_t /* print an unsigned long, 0 padded, return #chars */
print0u(char *s, unsigned long val, int n)
{
  int len = printu(0, val);
  while (len < n) { /* prepend zeros */
    if (s) *s++ = '0';
    len++;
  }
  printu(s, val);
  return (size_t) len;
}
