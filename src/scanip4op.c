#include "scan.h"

/** Scan an IPv4 address (dotted decimal notation),
 *  optionally with a port appended as ' port' or
 *  ':port' or '%port'. */
int
scanip4op(const char *s, unsigned char ip[4], unsigned *port)
{
  const char *p;
  unsigned u;
  int n;

  if (!s) return 0;

  p = s;
  if ((n = scanip4(p, ip)) > 0) p += n;
  else return 0; /* invalid IP addr */

  if (port) {
    int gotsep = 0;
    *port = 0;
    while (*p == ' ' || *p == '\t') ++p;
    if (*p == ':' || *p == '%') gotsep=1, ++p;
    while (*p == ' ' || *p == '\t') ++p;
    n = scanuint(p, &u);
    if (n > 0) {
      if (u > 65535) return 0; /* invalid port number */
      *port = u;
      p += n;
    }
    else if (gotsep) return 0; /* port required after non-blank separator! */
  }

  return p - s; /* #bytes scanned */
}
