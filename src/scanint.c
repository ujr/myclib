#include "scan.h"

/** Scan an int in decimal notation with optional sign */
int
scanint(const char *s, int *pval)
{
  const char *p;
  int sign;
  int val;

  if (!s) return 0;

  sign = 0;

  switch (*s) {
    case '-': sign=-1; ++s; break;
    case '+': sign=+1; ++s; break;
  }

  /* compute -val to get INT_MIN without overflow, but
     input beyond INT_MIN..INT_MAX silently overflows */
  for (p=s, val=0; '0' <= *p && *p <= '9'; p++) {
    val = 10 * val - (*p - '0');
  }

  if (p == s) return 0; /* no digits */

  if (pval) *pval = sign<0 ? val : -val;
  return (p - s) + (sign ? 1 : 0); /* #bytes scanned */
}
