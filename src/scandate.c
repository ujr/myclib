#include "scan.h"
#include <time.h>

/** Scan an ISO 8601 date (like 2005-07-14).
 *  Update only tm_year, tm_mon, tm_mday;
 *  leave all other tm fields untouched!
 *
 *  Return number of bytes scanned, zero on error.
 *
 * Notes on struct tm:
 * tm_year is number of years since 1900.
 * tm_mon (0..11) is the number of months since January.
 * tm_mday (1..31) is the day of month (starting with 1).
 */
int
scandate(const char *s, struct tm *tp)
{
  const char *p;
  int c, y, m, d, sign;

  if (!s) return 0;

  p = s;
  sign = 1;

  if (*p == '-') { ++p; sign = -1; } /* year may be negative! */

  if ((y = (unsigned char) (*p++ - '0')) > 9) return 0;
  while ((c = (unsigned char) (*p - '0')) <= 9) { y = y*10 + c; p++; }

  if (*p++ != '-') return 0;

  if ((m = (unsigned char) (*p++ - '0')) > 9) return 0;
  while ((c = (unsigned char) (*p - '0')) <= 9) { m = m*10 + c; p++; }

  if (*p++ != '-') return 0;

  if (( d = (unsigned char) (*p++ - '0')) > 9) return 0;
  while ((c = (unsigned char) (*p - '0')) <= 9) { d = d*10 + c; p++; }

  if (tp) {
    tp->tm_year = y * sign - 1900;
    tp->tm_mon = m - 1;
    tp->tm_mday = d;
  }

  return p - s; /* #bytes scanned */
}
