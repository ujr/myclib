#include "scan.h"
#include <string.h>

/** Scan prefix of s consisting only of bytes in accept */
int
scanwhile(const char *s, const char *accept)
{
  if (!s) return 0;
  if (!accept) accept = "";
  return strspn(s, accept);
}
