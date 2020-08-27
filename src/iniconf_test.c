/* Unit tests for iniconf.h API */

#include "test.h"

#include <stdio.h>

#include "iniconf.h"

#define BOM "\xEF\xBB\xBF"  /* UTF-8 encoded BOM */

typedef struct {
  int index;
  int count;
  const char **sections;
  const char **names;
  const char **values;
} expected;

static int
handler(const char *sect, const char *name, const char *value, size_t lineno, void *userdata)
{
  expected *exp = (expected *) userdata;

  if (exp) {
    if (exp->index >= exp->count) return 997;
    if (!STREQ(exp->names[exp->index], name)) return 998;
    if (!STREQ(exp->values[exp->index], value)) return 999;
    exp->index += 1;
    return 0;
  }

  fprintf(stderr, "[%s:%s=%s] at line %zd\n", sect, name, value, lineno);
  return 0;
}

void
iniconf_test(int *pnumpass, int *pnumfail)
{
  int numpass = 0;
  int numfail = 0;

  FILE *fp;
  int r;

  HEADING("Testing iniconf");

  expected exp0 = { 0, 0, 0, 0, 0 };
  r = iniconf_sz("", handler, &exp0);
  TEST("empty", r == 0);
  r = iniconf_sz(" \t \n  \r \t \r\n  \t ", handler, &exp0);
  TEST("blank", r == 0);

  const char *s1[] = { "", "", "", "" };
  const char *n1[] = { "foo", "bar", "baz", "quux" };
  const char *v1[] = { "", "2", "hi there!", "" };
  expected exp1 = { 0, 4, s1, n1, v1 };
  r = iniconf_sz("  foo  \nbar=2\r\r\nbaz = hi\\\n there! \rquux\n=omitted", handler, &exp1);
  TEST("flags", r == 0);

  const char *s2[] = { "", "S1", "S2", "S2", "S3", "S4" };
  const char *n2[] = { "one", "two", "three", "four", "five", "implicitly" };
  const char *v2[] = { "global", "foo", "bar", "baz", "quux", "closed" };
  expected exp2 = { 0, 6, s2, n2, v2 };
  r = iniconf_sz("one=global\n[S1]\ntwo=foo\n [ S2 ] three = bar \nfour=baz\r[S3]\nfive=quux\n[S4 \nimplicitly=closed", handler, &exp2);
  TEST("sections", r == 0);

  const char *s3[] = { "", "", "", "" };
  const char *n3[] = { "foo", "bar#c3", "baz ; c4", "quux" };
  const char *v3[] = { "", "", "", "" };
  expected exp3 = { 0, 4, s3, n3, v3 };
  r = iniconf_sz("#c1\nfoo\n  ; c2  \nbar#c3\nbaz ; c4 \nquux", handler, &exp3);
  TEST("comments", r == 0);

  const char *s4[] = { ""};
  const char *n4[] = { "a long key on two lines" };
  const char *v4[] = { "multilinevalue" };
  expected exp4 = { 0, 1, s4, n4, v4 };
  r = iniconf_sz(" a long key \\\non two lines = multi\\\nline\\\nvalue\\", handler, &exp4);
  TEST("continuations", r == 0);

  const char *s5[] = { "", "user", "user", "user", "user", "user" };
  const char *n5[] = { "global", "name", "mail", "hash", "description", "oops" };
  const char *v5[] = { "true", "\"John Smith\"", "john@smith.com", "#tag",
    "Lines can be continued by escaping the newline with a backslash.",
    "42 # this is not a comment!" };
  expected exp5 = { 0, 6, s5, n5, v5 };
  fp = tmpfile();
  fputs("# comment line\r\n\r\n", fp);
  fputs("global = true\n\n", fp);
  fputs("[user]\n", fp);
  fputs("name = \"John Smith\"\n", fp);
  fputs("mail = john@smith.com\n", fp);
  fputs("hash = #tag\n", fp);
  fputs("description = Lines can be continued\\\n", fp);
  fputs(" by escaping the newline with a backslash.\n", fp);
  fputs("; The backslash and the newline are discarded, the\n", fp);
  fputs("; two physical lines joined into one logical line.\n", fp);
  fputs("oops = 42 # this is not a comment!\n", fp);
  rewind(fp);
  r = iniconf_fp(fp, handler, &exp5);
  TEST("sample ini", r == 0);

  const char *s6[] = { "" };
  const char *n6[] = { "foo" }; /* strip if at start of input */
  const char *v6[] = { "bar" BOM }; /* retain if not at start of input */
  expected exp6 = { 0, 1, s6, n6, v6 };
  r = iniconf_sz(BOM "foo=bar" BOM, handler, &exp6);
  TEST("BOM", r == 0);

  *pnumpass += numpass;
  *pnumfail += numfail;
}
