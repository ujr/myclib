#include "scan.h"
#include <string.h>

/** Scan prefix of s consisting of bytes not in reject */
int
scanuntil(const char *s, const char *reject)
{
  if (!s) return 0;
  if (!reject) reject = "";
  return strcspn(s, reject);
}
