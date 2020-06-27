#include "myfuns.h"
#include <string.h>

static const char *seps = " \t\n\r\f\v";

/** Split s into args; return number of args in s.
    Args are delimited by space unless quoted.
    There is no escaping of spaces or quotes.
*/
int argsplit(char *s, char *args[], int maxargs)
{
  int n;

  for (n=0; n<maxargs; n++) args[n] = 0;

  if (!s) return 0;

  maxargs -= 1; /* leave room for null ptr */
  n = 0;

  while (*s) {
    while (strchr(seps, *s)) ++s; /* skip space */
    if (*s == '\'' || *s == '\"') {
      char quote = *s++;
      if (args && n < maxargs) args[n] = s;
      while (*s && (*s != quote)) ++s;
    }
    else if (*s) {
      if (args && n < maxargs) args[n] = s;
      while (*s && !strchr(seps, *s)) ++s;
    }
    if (*s) {
      if (args && n < maxargs) *s++ = '\0';
      else ++s; /* don't modify s if just counting */
    }
    n += 1;
  }
  return n;
}
