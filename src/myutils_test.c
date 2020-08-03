/* Unit tests for myfuns.h API */

#include "test.h"

#include <stdio.h>
#include <string.h>

#include "myutils.h"

void
myfuns_test(int *pnumpass, int *pnumfail)
{
  static const int nargs = 4;
  char *args[nargs];
  char buf[64];

  int numpass = 0;
  int numfail = 0;

  HEADING("Testing streq()");
  TEST("streq null null is true", streq(NULL, NULL));
  TEST("streq foo foo is true", streq("foo", "foo"));
  TEST("streq x null is falls", !streq("x", NULL));
  TEST("streq null x is false", !streq(NULL, "x"));
  TEST("streq foo fox is false", !streq("foo", "fox"));

  HEADING("Testing basename()");
  TEST("basename null is null", basename(NULL) == NULL);
  TEST("basename foo.txt is foo.txt", streq(basename("foo.txt"), "foo.txt"));
  TEST("basename /foo/bar/baz is baz", streq(basename("/foo/bar/baz"), "baz"));
  TEST("basename / is /", streq(basename("/"), "/"));

  HEADING("Testing argsplit()");
  TEST("argsplit null", argsplit(NULL, NULL, 0) == 0);
  TEST("argsplit plain", argsplit(strcpy(buf, "foo bar baz"), args, 3) == 3
    && streq(args[0], "foo") && streq(args[1], "bar") && streq(args[2], 0));
  TEST("argsplit quote", argsplit(strcpy(buf, " a 'b\" c' \"d' e\""), args, nargs) == 3
    && streq(args[0], "a") && streq(args[1], "b\" c") && streq(args[2], "d' e") && streq(args[3], 0));
  TEST("argsplit count", argsplit(" a 'b\" c' \"d' e\"", 0, 0) == 3);

  *pnumpass += numpass;
  *pnumfail += numfail;
}
