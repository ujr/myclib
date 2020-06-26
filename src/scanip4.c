#include "scan.h"

/** Scan an IPv4 address (dotted decimal notation) */
int
scanip4(const char *s, unsigned char ip[4])
{
  const char *p;
  unsigned int u;
  int n;

  if (!s) return 0;

  p = s;

  if ((n = scanuint(p, &u)) == 0 || (u > 255)) return 0;
  ip[0] = (unsigned char) u;
  p += n;

  if (*p != '.') return 0;
  p += 1;

  if ((n = scanuint(p, &u)) == 0 || (u > 255)) return 0;
  ip[1] = (unsigned char) u;
  p += n;

  if (*p != '.') return 0;
  p += 1;

  if ((n = scanuint(p, &u)) == 0 || (u > 255)) return 0;
  ip[2] = (unsigned char) u;
  p += n;

  if (*p != '.') return 0;
  p += 1;

  if ((n = scanuint(p, &u)) == 0 || (u > 255)) return 0;
  ip[3] = (unsigned char) u;
  p += n;

  return p - s; /* #bytes scanned */
}
