#include "myunix.h"

#include <fcntl.h>
#include <unistd.h>

#ifndef O_NONBLOCK
#error Your system headers do not define O_NONBLOCK
#endif

int open_append(const char *fn)
{
  return open(fn, O_WRONLY | O_APPEND | O_NONBLOCK | O_CREAT, 0644);
}
