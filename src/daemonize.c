#include "myunix.h"

#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/** Become a Unix daemon. For a discussion,
 *  see: Stevens 1993, section 13.3 */
int daemonize(void)
{
  pid_t pid;

  /* cd to root so not to inadvertantly block a mounted file system */
  if (chdir("/") < 0) return -1;

  if ((pid = fork()) < 0) return -1;  /* error */
  if (pid != 0) exit(0);  /* parent quits */

  setsid();  /* become session leader */
  umask(0);  /* so we don't interfere */

  return 0;  /* ok */
}
