#include "myunix.h"

#include <fcntl.h>
#include <unistd.h>

#ifndef O_NONBLOCK
#error Your system headers do not define O_NONBLOCK.
#endif

int fdnonblock(int fd)
{
  int flags = fcntl(fd, F_GETFL, 0);
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
