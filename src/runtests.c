/* Test Suite */

#include <stdio.h>

#include "test.h"

#define UNUSED(x) (void)(x)

extern void scan_test(int *numpass, int *numfail);

int
main(int argc, char **argv)
{
  int numpass = 0;
  int numfail = 0;

  UNUSED(argc);
  UNUSED(argv);

  scan_test(&numpass, &numfail);

  printf("%d fail, %d pass\n", numfail, numpass);
  return numfail > 0 ? 1 : 0;
}
