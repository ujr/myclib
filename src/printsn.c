#include "print.h"

size_t /* print a string, no more than max, return #chars */
printsn(char *s, const char *z, size_t max)
{
  register const char *p = z;
  if (p && max > 0)
  {
    if (s) while (*p && max--) *s++ = *p++;
    else while (*p && max--) p++;
  }
  return p - z;
}
