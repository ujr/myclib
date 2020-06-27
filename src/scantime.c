#include "scan.h"
#include <time.h>

/** Scan an ISO 8601 time (like 12:34:56).
 *  Update only tm_hour, tm_min, tm_sec;
 *  leave all other tm fields untouched.
 *
 *  TODO Scan optional time offset from GMT (like Z or +0200)
 *
 *  Return number of bytes scanned, zero on error.
 */
int
scantime(const char *s, struct tm *tp)
{
  const char *p;
  unsigned h, m, t, c;

  if (!s) return 0;

  p = s;

  if ((h = (unsigned char) (*p++ - '0')) > 9) return 0;
  while ((c = (unsigned char) (*p - '0')) <= 9) { h = h*10 + c; p++; }

  if (*p++ != ':') return 0;

  if ((m = (unsigned char) (*p++ - '0')) > 9) return 0;
  while ((c = (unsigned char) (*p - '0')) <= 9) { m = m*10 + c; p++; }

  if (*p != ':') t = 0;
  else { ++p;
    if ((t = (unsigned char) (*p++ - '0')) > 9) return 0;
    while ((c = (unsigned char) (*p - '0')) <= 9) { t = t*10 + c; p++; }
  }

  if (tp) {
    tp->tm_hour = h;
    tp->tm_min = m;
    tp->tm_sec = t;
  }

#if 0
  /* scan offset from GMT */
  while ((*p == ' ') || (*p == '\t')) ++p;
  if (*p == '+') sign = 1; else if (*p == '-') sign = -1; else return 0;
  ++p;
  c = (unsigned char) (*p++ - '0'); if (c > 9) return 0; z = c;
  c = (unsigned char) (*p++ - '0'); if (c > 9) return 0; z = z*10 + c;
  c = (unsigned char) (*p++ - '0'); if (c > 9) return 0; z = z*6 + c;
  c = (unsigned char) (*p++ - '0'); if (c > 9) return 0; z = z*10 + c;
  if (offset) *offset = z * sign;
#endif

  return p - s;  /* #chars scanned */
}
