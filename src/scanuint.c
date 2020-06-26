#include "scan.h"

/** Scan an unsigned int in decimal notation */
int
scanuint(const char *s, unsigned int *pval)
{
  const char *p;
  unsigned int val;

  if (!s) return 0;

  for (p=s, val=0; '0' <= *p && *p <= '9'; p++) {
    val = 10 * val + (*p - '0');
  }

  if (pval) *pval = val;
  return p - s; /* #bytes scanned */
}
