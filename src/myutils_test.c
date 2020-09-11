/* Unit tests for myfuns.h API */

#include "test.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "myutils.h"
#include "strbuf.h"

void
myutils_test(int *pnumpass, int *pnumfail)
{
  static const int nargs = 4;
  char *args[nargs];
  char buf[64];
  FILE *fp;
  strbuf sb = {0};
  int partial;
  long n;
  char *line;
  size_t size;
  struct tm tm;

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

  HEADING("Testing getln()");
  fp = tmpfile();
  n = getln(fp, &sb, &partial);
  TEST("getln empty", n == 0 && sblen(&sb) == 0 && partial == 1);
  fputs("one\n\nthree\ndos\r\nsomewhat longer\npartial", fp);
  rewind(fp);
  n = getln(fp, &sb, &partial);
  TEST("getln 1st", n == 4 && STREQ("one\n", sbptr(&sb)) && !partial);
  n = getln(fp, &sb, &partial);
  TEST("getln 2nd", n == 1 && STREQ("\n", sbptr(&sb)) && !partial);
  n = getln(fp, &sb, &partial);
  TEST("getln 3rd", n == 6 && STREQ("three\n", sbptr(&sb)) && !partial);
  n = getln(fp, &sb, &partial);
  TEST("getln 4th", n == 5 && STREQ("dos\r\n", sbptr(&sb)) && !partial);
  n = getln(fp, &sb, &partial);
  TEST("getln 5th", n == 16 && STREQ("somewhat longer\n", sbptr(&sb)) && !partial);
  n = getln(fp, &sb, &partial);
  TEST("getln 6th", n == 7 && STREQ("partial", sbptr(&sb)) && partial);
  n = getln(fp, &sb, &partial);
  TEST("getln eof", n == 0 && sblen(&sb) == 0 && partial);
  fclose(fp);

  HEADING("Testing getln2()");
  fp = tmpfile();
  line = 0; size = 0;
  n = getln2(fp, &line, &size);
  TEST("getln2 empty", n == 0 && STREQ("", line) && size > 0);
  fputs("one\n\nthree\ndos\r\nsomewhat longer\npartial", fp);
  rewind(fp);
  n = getln2(fp, &line, &size);
  TEST("getln2 1st", n == 4 && STREQ("one\n", line) && size > (size_t) n);
  n = getln2(fp, &line, &size);
  TEST("getln2, 2nd", n == 1 && STREQ("\n", line));
  n = getln2(fp, &line, &size);
  TEST("getln2 3rd", n == 6 && STREQ("three\n", line));
  n = getln2(fp, &line, &size);
  TEST("getln2 4th", n == 5 && STREQ("dos\r\n", line));
  n = getln2(fp, &line, &size);
  TEST("getln2 5th", n == 16 && STREQ("somewhat longer\n", line) && size > (size_t) n);
  n = getln2(fp, &line, &size);
  TEST("getln2 6th", n == 7 && STREQ("partial", line) && size > (size_t) n);
  n = getln2(fp, &line, &size);
  TEST("getln2 eof", n == 0 && STREQ("", line));
  fclose(fp);

  HEADING("Testing getln3()");
  fp = tmpfile();
  line = buf; size = 8;
  assert(sizeof buf > size);
  n = getln3(fp, line, size, &partial);
  TEST("getln3 empty", n == 0 && STREQ("", line));
  fputs("one\n\nthree\ndos\r\nsomewhat longer\npartial", fp);
  rewind(fp);
  n = getln3(fp, line, size, &partial);
  TEST("getln3 1st", n == 4 && STREQ("one\n", line) && !partial);
  n = getln3(fp, line, size, &partial);
  TEST("getln3 2nd", n == 1 && STREQ("\n", line) && !partial);
  n = getln3(fp, line, size, &partial);
  TEST("getln3 3rd", n == 6 && STREQ("three\n", line) && !partial);
  n = getln3(fp, line, size, &partial);
  TEST("getln3 4th", n == 5 && STREQ("dos\r\n", line) && !partial);
  n = getln3(fp, line, size, &partial);
  TEST("getln3 5th", n == 7 && STREQ("somewha", line) && partial);
  n = eatln(fp);
  TEST("eatln", n == 9);
  n = getln3(fp, line, size, &partial);
  TEST("getln3 6th", n == 7 && STREQ("partial", line));
  /* here's a very special case: buffer is full at end of
     partial last line, so partial==1, but next read is EOF,
     so we would like partial==0 but this we do not yet know */
  TEST("getln3 6th more", partial == 1);
  n = getln3(fp, line, size, &partial);
  TEST("getln3 eof", n == 0 && STREQ("", line) && !partial);
  fclose(fp);

  HEADING("Testing utcscan()/utcstamp()");
  n = utcscan("1977-02-25T12:34:56Z", &tm);
  TEST("utcscan", n == 20 &&
    tm.tm_year == 77 && tm.tm_mon == 1 && tm.tm_mday == 25 &&
    tm.tm_hour == 12 && tm.tm_min == 34 && tm.tm_sec == 56);
  n = utcscan(" \t 1977-02-25 \t 12:34:56Z ", &tm);
  TEST("utscan padded", n == 25);
  n = utcstamp(buf, 0);
  TEST("utcstamp", n == 20);
  buf[n] = 0;
  INFO("UTCSTAMP: %s", buf);

  HEADING("Testing signum()");
  TEST("signum 42", signum(42) == 1);
  TEST("signum -5", signum(-5) == -1);
  TEST("signum 0", signum(0) == 0);
  TEST("signum 1.2", signum(1.2) == 1);
  TEST("signum -0.5", signum(-.5) == -1);
  TEST("signum 0.0", signum(0.0) == 0);

  *pnumpass += numpass;
  *pnumfail += numfail;
}
