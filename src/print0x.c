#include "print.h"

size_t /* print an unsigned long in hex, 0 padded, return #chars */
print0x(char *s, unsigned long val, int n)
{
  int len = printx(0, val);
  while (len < n) { /* prepent zeros */
    if (s) *s++ = '0';
    len++;
  }
  printx(s, val);
  return (size_t) len;
}
