#include "print.h"

size_t /* print a string, return #chars */
prints(char *s, const char *z)
{
  register const char *p = z;
  if (!p) return 0;
  if (s) while (*p) *s++ = *p++;
  else while (*p) p++;
  return p - z;
}
