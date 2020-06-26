#include "scan.h"

/** Scan an unsigned long in hex notation, return #bytes scanned */
int
scanhex(const char *s, unsigned long *pval)
{
  unsigned long val;
  int i;

  if (!s) return 0;

  for (i=0, val=0; ;i++) {
    int c = s[i];
    if ('0' <= c && c <= '9')
      c = c - '0';
    else if ('A' <= c && c <= 'F')
      c = c - 'A' + 10;
    else if ('a' <= c && c <= 'f')
      c = c - 'a' + 10;
    else break;
    val = val * 16 + (unsigned long) c;
  }

  // for (i=0, val=0; isxdigit((c = s[i])); i++) {
  //   if (isupper(c)) c = c - 'A' + 10; else
  //   if (islower(c)) c = c - 'a' + 10; else
  //   /* isdigit! */  c = c - '0';
  //   val = val * 16 + (unsigned long) c;
  // }

  if (pval) *pval = val;

  return i; /* #bytes scanned */
}
