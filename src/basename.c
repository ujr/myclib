#include "myfuns.h"

/** Return the basename of a path string, that is, a pointer
 *  to one after the last occurrence of '/' in the argument.
 *  Return the argument if there is no '/' or it is null.
 *  Special case: the basename of "/" is "/".
 */
char *basename(const char *s)
{
  static const char sep = '/';
  register const char *p;

  if (!s) return 0;

  for (p = s; *p; ) {
    if (*p++ == sep) {
      if (*p) s = p;
    }
  }

  return (char *) s;
}
