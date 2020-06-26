/* Unit tests for scan.h API */

#include "test.h"

#include <limits.h>
#include <stdio.h>

#include "scan.h"

void
scan_test(int *pnumpass, int *pnumfail)
{
  char buf[64];
  int n;
  int i;
  unsigned long ul;

  int numpass = 0;
  int numfail = 0;

  HEADING("Testing scanint()");
  TEST("scanint -", scanint("-", &i) == 0); /* not 1 */
  TEST("scanint -0", scanint("-0", &i) == 2 && i == 0);
  TEST("scanint +123", scanint("+123", &i) == 4 && i == 123);
  TEST("scanint -99x", scanint("-99x", &i) == 3 && i == -99);
  n = snprintf(buf, sizeof buf, "%d", INT_MAX);
  TEST("scanint INT_MAX", scanint(buf, &i) == n && i == INT_MAX);
  n = snprintf(buf, sizeof buf, "%d", INT_MIN);
  TEST("scanint INT_MIN", scanint(buf, &i) == n && i == INT_MIN);

  HEADING("Testing scanulong()");
  TEST("scanulong -1", scanulong("-1", &ul) == 0);
  TEST("scanulong 12x", scanulong("12x", &ul) == 2 && ul == 12);
  n = snprintf(buf, sizeof buf, "%lu", ULONG_MAX);
  TEST("scanulong ULONG_MAX", scanulong(buf, &ul) == n && ul == ULONG_MAX);

  HEADING("Testing scanblank()");
  TEST("scanblank null", scanblank(NULL) == 0);
  TEST("scanblank empty", scanblank("") == 0);
  TEST("scanblank \"  x\"", scanblank("  x ") == 2);
  TEST("scanblank only space and tab", scanblank(" \t\nx") == 2);

  HEADING("Testing scanwhite()");
  TEST("scanwhite null", scanwhite(NULL) == 0);
  TEST("scanwhite empty", scanwhite("") == 0);
  TEST("scanwhite all white space", scanwhite(" \f\n\r\t\vx") == 6);

  *pnumpass += numpass;
  *pnumfail += numfail;
}
