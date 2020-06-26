/* Unit tests for scan.h API */

#include "test.h"

#include <limits.h>
#include <stdio.h>
#include <time.h>

#include "scan.h"

void
scan_test(int *pnumpass, int *pnumfail)
{
  char buf[64];
  int n;
  int i;
  unsigned int ui;
  unsigned long ul;
  unsigned char ip[4];
  unsigned int port;
  struct tm tm;

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

  HEADING("Testing scanuint()");
  TEST("scanuint 000", scanuint("000", &ui) == 3 && ui == 0);
  TEST("scanuint -12", scanuint("-12", &ui) == 0);
  TEST("scanuint 12x", scanuint("12x", &ui) == 2 && ui == 12);
  n = snprintf(buf, sizeof buf, "%u", UINT_MAX);
  TEST("scanuint UINT_MAX", scanuint(buf, &ui) == n && ui == UINT_MAX);

  HEADING("Testing scanulong()");
  TEST("scanulong -1", scanulong("-1", &ul) == 0);
  TEST("scanulong 12x", scanulong("12x", &ul) == 2 && ul == 12);
  n = snprintf(buf, sizeof buf, "%lu", ULONG_MAX);
  TEST("scanulong ULONG_MAX", scanulong(buf, &ul) == n && ul == ULONG_MAX);

  HEADING("Testing scanhex()");
  TEST("scanhex 00000000", scanhex("00000000", &ul) == 8 && ul == 0);
  TEST("scanhex DeadBeef", scanhex("DeadBeef", &ul) == 8 && ul == 0xDeadBeef);
  TEST("scanhex abcx", scanhex("abcx", &ul) == 3 && ul == 0xabc);
  n = snprintf(buf, sizeof buf, "%lx", ULONG_MAX);
  TEST("scanhex ULONG_MAX", scanhex(buf, &ul) == n && ul == ULONG_MAX);

  HEADING("Testing scanblank()");
  TEST("scanblank null", scanblank(NULL) == 0);
  TEST("scanblank empty", scanblank("") == 0);
  TEST("scanblank \"  x\"", scanblank("  x ") == 2);
  TEST("scanblank only space and tab", scanblank(" \t\nx") == 2);

  HEADING("Testing scanwhite()");
  TEST("scanwhite null", scanwhite(NULL) == 0);
  TEST("scanwhite empty", scanwhite("") == 0);
  TEST("scanwhite all white space", scanwhite(" \f\n\r\t\vx") == 6);

  HEADING("Testing scantext()");
  TEST("scantext null", scantext(NULL, NULL) == 0);
  TEST("scantext \"abc\" in \"abcd\"", scantext("abcd", "abc") == 3);
  TEST("scantext \"abc\" in \"abc\"", scantext("abc", "abc") == 3);
  TEST("scantext \"abc\" in \"ab\"", scantext("ab", "abc") == 0);

  HEADING("Testing scanpat()");
  TEST("scanpat \"abc\"", scanpat("abcd", "abc") == 3);
  TEST("scanpat \" abc \"", scanpat("\t abc \tz", " abc ") == 7);
  TEST("scanpat ditto no leading space", scanpat("abc   z", " abc ") == 6);
  TEST("scanpat \"*x*y*z*\"", scanpat("xyz", "*x*y*z*") == 3);
  TEST("scanpat ditto", scanpat("!x!!y!!!z!!!!", "*x*y*z*") == 13);
  TEST("scanpat \" foo: E* \"", scanpat("foo: EINTR x", " foo: E* ") == 11);
  TEST("scanpat ditto other input", scanpat("foo: EINTRx", "foo: E* ") == 0);
  TEST("scanpat null", scanpat(NULL, NULL) == 0);

  HEADING("Testing scanuntil()");
  TEST("scanuntil abz null", scanuntil("abz", NULL) == 3);
  TEST("scanuntil abz xyz", scanuntil("abz", "xyz") == 2);

  HEADING("Testing scanwhile()");
  TEST("scanwhile abz null", scanwhile("abz", NULL) == 0);
  TEST("scanwhile abz abc", scanwhile("abz", "abc") == 2);

  HEADING("Testing scanip4()");
  TEST("scanip4 192.168.1.2", scanip4("192.168.1.2", ip) == 11
    && ip[0] == 192 && ip[1] == 168 && ip[2] == 1 && ip[3] == 2);
  TEST("scanip4 255.255.255.255:", scanip4("255.255.255.255:", ip) == 15
    && ip[0] == 255 && ip[1] == 255 && ip[2] == 255 && ip[3] == 255);

  HEADING("Testing scanip4op()");
  TEST("scanip4op 192.168.1.2:4321", scanip4op("192.168.1.2:4321", ip, &port) == 16
    && ip[0] == 192 && ip[3] == 2 && port == 4321);
  TEST("scanip4op 10.1.2.3 (no port)", scanip4op("10.1.2.3", ip, &port) == 8
    && ip[0] == 10 && ip[3] == 3 && port == 0);
  TEST("scanip4op 10.1.2.3: (requires port!)", scanip4op("10.1.2.3:", ip, &port) == 0);

  HEADING("Testing scandate()");
  TEST("scandate 2005-07-14", scandate("2005-07-14", &tm) == 10
    && tm.tm_year == 105 && tm.tm_mon == 6 && tm.tm_mday == 14);
  TEST("scandate -5-2-03", scandate("-5-2-03", &tm) == 7
    && tm.tm_year == -1905 && tm.tm_mon == 1 && tm.tm_mday == 3);

  HEADING("Testing scantime()");
  TEST("scantime 12:34:56", (n=scantime("12:34:56", &tm)) == 8
    && tm.tm_hour == 12 && tm.tm_min == 34 && tm.tm_sec == 56);
  TEST("scantime 05:12", scantime("05:12", &tm) == 5
    && tm.tm_hour == 5 && tm.tm_min == 12 && tm.tm_sec == 0);

  *pnumpass += numpass;
  *pnumfail += numfail;
}
