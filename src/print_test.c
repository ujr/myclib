/* Unit tests for print.h API */

#include "test.h"

#include <limits.h>
#include <stdio.h>

#include "print.h"
#include "myfuns.h"

void
print_test(int *pnumpass, int *pnumfail)
{
  char buf[1024];
  char expected[1024];
  char *p;
  size_t s;

  int numpass = 0;
  int numfail = 0;

  HEADING("Testing print.h functions");

  p = buf;
  p += printu(p, 123);
  p += printc(p, ',');
  p += printu(p, 0);
  p += printc(p, ',');
  p += printu(p, -1); /* cast to ulong, thus ULONG_MAX */
  print0(p);
  snprintf(expected, sizeof expected, "123,0,%lu", ULONG_MAX);
  TEST("printu()", streq(expected, buf));

  p = buf;
  p += print0u(p, 123, 8);
  p += printc(p, ',');
  p += print0u(p, 4321, 2);
  p += printc(p, ',');
  p += print0u(p, 0, 0);
  p += printc(p, ',');
  p += print0u(p, 0, 3);
  print0(p);
  TEST("print0u", streq("00000123,4321,0,000", buf));

  p = buf;
  p += printx(p, 123);
  p += printc(p, ',');
  p += printx(p, 0);
  p += printc(p, ',');
  p += printx(p, -1); /* cast to ulong, thus ULONG_MAX */
  print0(p);
  snprintf(expected, sizeof expected, "7B,0,%lX", ULONG_MAX);
  TEST("printx()", streq(expected, buf));

  p = buf;
  p += print0x(p, 123, 8);
  p += printc(p, ',');
  p += print0x(p, 0xABCD, 8);
  p += printc(p, ',');
  p += print0x(p, 0xDEADBEEF, 5);
  print0(p);
  TEST("print0x()", streq("0000007B,0000ABCD,DEADBEEF", buf));

  p = buf;
  p += printd(p, -123);     p += printc(p, ',');
  p += printd(p, 0);        p += printc(p, ',');
  p += printd(p, LONG_MIN); p += printc(p, ',');
  p += printd(p, LONG_MAX);
  snprintf(expected, sizeof expected, "-123,0,%ld,%ld", LONG_MIN, LONG_MAX);
  TEST("printi()", streq(expected, buf));

  p = buf;
  p += prints(p, "Hello\tString");
  p += prints(p, "|");
  p += printsn(p, "How do you do?", 3);
  print0(p);
  TEST("prints[n]()", streq("Hello\tString|How", buf));

  s = format(buf, sizeof buf, "");
  TEST("format empty", streq("", buf) && s == 0);

  s = format(buf, sizeof buf, "%% %c %d %u %x %s %",
    'x', -123, 2345, 65535, "hi");
  TEST("format misc", streq("% x -123 2345 FFFF hi %", buf) && s == 23);

  s = format(buf, sizeof buf, "str=%s", (char *) 0);
  TEST("format null str", streq("str=(null)", buf) && s == 10);

  *pnumpass += numpass;
  *pnumfail += numfail;
}
