/* Unit tests for scf.h API */

#include "test.h"

#include "scf.h"

void
scf_test(int *pnumpass, int *pnumfail)
{
  char *cmd1[] = {
    "./prog", "-ab", "-c", "required", "-doptional",
    "-d", "-x", "-b", "--", "-arg", "-", "out", NULL };

  char *cmd2[] = { "prog", "-abcd", NULL };

  const char *opts = "abc:d::";
  struct scfargs args;

  int numpass = 0;
  int numfail = 0;

  HEADING("Testing scf");
  TEST("scfname ./prog", STREQ("prog", scfname(cmd1)));
  TEST("scfname prog", STREQ("prog", scfname(cmd2)));
  INFO("opts=\"%s\"", opts);

  scfinit(&args, cmd1);
  TEST("scfopt 1.1", scfopt(&args, opts) == 'a' && args.optarg == 0);
  TEST("scfopt 1.2", scfopt(&args, opts) == 'b' && args.optarg == 0);
  TEST("scfopt 1.3", scfopt(&args, opts) == 'c' && STREQ("required", args.optarg));
  TEST("scfopt 1.4", scfopt(&args, opts) == 'd' && STREQ("optional", args.optarg));
  TEST("scfopt 1.5", scfopt(&args, opts) == 'd' && args.optarg == 0);
  TEST("scfopt 1.6", scfopt(&args, opts) == '?' && args.optopt == 'x' && args.optarg == 0);
  TEST("scfopt 1.7", scfopt(&args, opts) == 'b' && args.optarg == 0);
  TEST("scfopt 1.8", scfopt(&args, opts) == 0);
  TEST("scfarg 1.1", STREQ("-arg", scfarg(&args)));
  TEST("scfarg 1.2", STREQ("-", scfarg(&args)));
  TEST("scfarg 1.3", STREQ("out", scfarg(&args)));
  TEST("scfarg 1.4", scfarg(&args) == 0);

  scfinit(&args, cmd2);
  TEST("scfopt 2.1", scfopt(&args, opts) == 'a' && args.optarg == 0);
  TEST("scfopt 2.2", scfopt(&args, opts) == 'b' && args.optarg == 0);
  TEST("scfopt 2.3", scfopt(&args, opts) == 'c' && STREQ("d", args.optarg));
  TEST("scfopt 2.4", scfopt(&args, opts) == 0);
  TEST("scfopt 2.4", scfopt(&args, opts) == 0); /* idempotent */
  TEST("scfarg 2.1", scfarg(&args) == 0);
  TEST("scfarg 2.2", scfarg(&args) == 0); /* idempotent */

  *pnumpass += numpass;
  *pnumfail += numfail;
}
