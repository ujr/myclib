#include "scan.h"

/** Scan given text, return #bytes scanned */
int
scantext(const char *s, const char *t)
{
  const char *anchor;
  if (!s || !t) return 0;
  anchor = t;
  while (*s && (*s == *t)) { ++s; ++t; }
  if (*t) return 0;
  return t - anchor; /* #bytes scanned */
}
