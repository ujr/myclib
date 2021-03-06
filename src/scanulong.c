#include "scan.h"

/** Scan an unsigned long in decimal notation */
int
scanulong(const char *s, unsigned long *pval)
{
  const char *p;
  unsigned long val;

  if (!s) return 0;

  for (p=s, val=0; '0'<=*p && *p<='9'; p++) {
    val = 10 * val + (*p - '0');
  }

  if (pval) *pval = val;
  return p - s; /* #bytes scanned */
}
