/* Unit tests for getopt.h header library */

#include "getopt.h"

#include "test.h"

#include <stdio.h>

#define ARGCOUNT(av) (sizeof(av)/sizeof(*av) - 1)

void
getopt_test(int *pnumpass, int *pnumfail)
{
  int numpass = 0;
  int numfail = 0;

  char *av1[] = { "test", "-a", "-b", "one", "-c", "two", "-d", "--", "fn", 0 };
  int ac1 = ARGCOUNT(av1);

  char *av2[] = { "test", "-b", "foo", "-cbar", "-abbaz", 0 };
  int ac2 = ARGCOUNT(av2);

  char *av3[] = { "test", "-x", "-a", "-b", 0 };
  int ac3 = ARGCOUNT(av3);

  char *av4[] = { "test", "-a", "-", "more", 0 };
  int ac4 = ARGCOUNT(av4);

  HEADING("Testing getopt.h");

  optind = 0; /* reset */
  TEST("-a", getopt(ac1, av1, "ab:c:d") == 'a');
  TEST("-b", getopt(ac1, av1, "ab:c:d") == 'b');
  TEST("arg", STREQ(optarg, "one"));
  TEST("-c", getopt(ac1, av1, "ab:c:d") == 'c');
  TEST("arg", STREQ(optarg, "two"));
  TEST("-d", getopt(ac1, av1, "ab:c:d") == 'd' && optopt == 'd');
  TEST("end", getopt(ac1, av1, "ab:c:d") == -1);
  TEST("optind", optind == 8 && STREQ(av1[optind], "fn"));

  optind = 0; /* reset */
  TEST("-b arg", getopt(ac2, av2, "ab:c:d") == 'b' && STREQ(optarg, "foo"));
  TEST("-carg", getopt(ac2, av2, "ab:c:d") == 'c' && STREQ(optarg, "bar"));
  TEST("-a", getopt(ac2, av2, "ab:c:d") == 'a' && optarg == 0);
  TEST("barg", getopt(ac2, av2, "ab:c:d") == 'b' && STREQ(optarg, "baz"));
  TEST("end", getopt(ac2, av2, "ab:c:d") == -1);
  TEST("optind", optind == 5);

  optind = 0; /* reset */
  TEST("-x", getopt(ac3, av3, "ab:c:d") == '?' && optopt == 'x');
  TEST("-a", getopt(ac3, av3, "ab:c:d") == 'a');
  TEST("-b", getopt(ac3, av3, "ab:c:d") == ':' && optopt == 'b');
  TEST("optind", optind == 4);

  optind = 0; /* reset */
  TEST("-a", getopt(ac4, av4, "ab:c:d") == 'a');
  TEST("-", getopt(ac4, av4, "ab:c:d") == -1); /* - is stdin */
  TEST("optind", optind == 2);

  if (pnumpass) *pnumpass += numpass;
  if (pnumfail) *pnumfail += numfail;
}

