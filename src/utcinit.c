/* required for setenv & tzset */
#define _POSIX_C_SOURCE 200112L

#include "myutils.h"

#include <stdlib.h>
#include <time.h>

int /* prep library routines to assume UTC */
utcinit(void)
{ /* an empty TZ means UTC to POSIX (glibc) */
  int r = setenv("TZ", "", 1);
  tzset(); /* reinit since we changed TZ env var */
  return r; /* 0 if ok, -1 on error */
}
