#include "myunix.h"

#include <fcntl.h>
#include <unistd.h>

#ifndef O_NONBLOCK
#error Your system headers do not define O_NONBLOCK
#endif

int open_excl(const char *fn)
{
  return open(fn, O_WRONLY | O_CREAT | O_EXCL, 0600);
}
