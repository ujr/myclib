#include "myutils.h"
#include "scan.h"

size_t /* scan a UTC timestamp in ISO8601 format */
utcscan(const char *s, struct tm *tp)
{
  register const char *p;
  struct tm tm;
  int n;

  if (!s) return 0;

  p = s;
  /* skip leading blanks */
  p += scanwhile(p, " \t");

  if ((n = scandate(p, &tm)) == 0) return 0;
  p += n;

  /* ISO8601 requires 'T' between date and time,
     here we are a bit more tolerant */
  if (*p == ' ' || *p == '\t') {
    ++p; while (*p == ' ' || *p == '\t') ++p;
  }
  else if (*p == 'T' || *p == 't') ++p;
  else return 0;

  if ((n = scantime(p, &tm)) == 0) return 0;
  p += n;

  if (*p++ != 'Z') return 0; /* not UTC */

  tm.tm_wday = tm.tm_yday = tm.tm_isdst = 0;
  if (tp) *tp = tm;

  return p - s;
}
