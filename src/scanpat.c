#include "scan.h"

/** Match string s against pattern pat, return #bytes matches.
 *  Pattern:
 *  1. any sequence of blanks at the beginning of pat matches any
 *     sequence of blanks and tabs, even the empty sequence, in s;
 *  2. any other sequence of blanks matches any non-empty sequence
 *     of blanks and tabs in s;
 *  3. an asterisk matches any character up to the first occurrence
 *     in s of the next character in pat;
 *  4. every other character matches itself.
 *  There is no pattern that requires a blank or tab at the beginning
 *  of s; if needed, say: if (isspace(*p) && (n=scanpat(p, "...")))
 */
int
scanpat(const char *s, const char *pat)
{
  const char *sp; /* source pointer */
  const char *pp; /* pattern pointer */

  if (!s || !pat) return 0;

  for (sp=s, pp=pat; ; ) {
    register int c = *pp++;
    if (c == '\0') break;
    if (c == ' ') { /* blank: sequence of blanks and tabs */
      const char *q = sp;
      while ((*sp == ' ') || (*sp == '\t')) ++sp;
      if ((sp == q) && (pp > pat+1)) return 0;
      while (*pp == ' ' || *pp == '\t') ++pp;
      continue;
    }
    if (c == '*') { /* star: anything up to next char in pat */
      c = *pp;
      while (*sp && *sp != c) ++sp;
      if (c == '\0') break;
      continue;
    }
    if (c != *sp++) return 0; /* mismatch */
  }

  return sp - s; /* #bytes matched */
}

#ifdef DEMO
#include <stdio.h>
int main(int argc, char *argv[])
{
  int i, n;
  char *pat;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <pattern> <candidates>\n", argv[0]);
    fprintf(stderr, "Test driver for scanpat(s,pat) routine\n");
    return 127;
  }

  pat = argv[1];
  for (i=2; i < argc; i++) {
    n = scanpat(argv[i], pat);
    printf("%3d  %s\n", n, argv[i]);
  }
  return 0;
}
#endif
