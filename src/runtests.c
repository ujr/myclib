/* Test Runner */

#include <stdio.h>

#include "test.h"

#define UNUSED(x) (void)(x)

extern void scan_test(int *pnumpass, int *pnumfail);
extern void print_test(int *pnumpass, int *pnumfail);
extern void buf_test(int *pnumpass, int *pnumfail);
extern void strbuf_test(int *pnumpass, int *pnumfail);
extern void myutils_test(int *pnumpass, int *pnumfail);
extern void simpleio_test(int *pnumpass, int *pnumfail);
extern void scf_test(int *pnumpass, int *pnumfail);
extern void iniconf_test(int *pnumpass, int *pnumfail);

int
main(int argc, char **argv)
{
  int numpass = 0;
  int numfail = 0;

  UNUSED(argc);
  UNUSED(argv);

  scan_test(&numpass, &numfail);
  print_test(&numpass, &numfail);
  buf_test(&numpass, &numfail);
  strbuf_test(&numpass, &numfail);
  myutils_test(&numpass, &numfail);
  simpleio_test(&numpass, &numfail);
  scf_test(&numpass, &numfail);
  iniconf_test(&numpass, &numfail);

  SUMMARY(numpass, numfail);
  return numfail > 0 ? 1 : 0;
}
