#include "myfuns.h"
#include <string.h>

/** Compare strings for equality; args may be null.
 *    if (streq(s, "foo")) ...
 *  reads nicer than
 *    if (strcmp(s, "foo") == 0) ...
 */
int streq(const char *s, const char *t)
{
  if (!s && !t) return 1;
  if (!s || !t) return 0;
  return strcmp(s, t) == 0;
}
