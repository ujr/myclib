#include "myunix.h"

#include <fcntl.h>
#include <unistd.h>

int readable(int fd)
{ /* true if fd is readable */
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) return 0;
  flags &= O_ACCMODE;
  return (flags == O_RDONLY) || (flags == O_RDWR);
}
