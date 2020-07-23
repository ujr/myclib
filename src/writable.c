#include "myunix.h"

#include <fcntl.h>
#include <unistd.h>

int writable(int fd)  /* true if fd is writeable */
{
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) return 0;
  flags &= O_ACCMODE;
  return (flags == O_WRONLY) || (flags == O_RDWR);
}
