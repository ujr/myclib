# The myfuns.h API

Collection of miscellaneous utility functions.

```C
#include "myfuns.h"

int argsplit(char *s, char *args[], int maxargs);
char *basename(const char *s);
int streq(const char *s, const char *t);
```

**argsplit** splits the string *s* into individual arguments
and stores pointers to each argument in the array *args* that
is assumed to have *maxargs* entries. The last entry in *args*
is always set to null, even if there are *maxargs* or more
arguments in *s*. Note that the string *s* will be modified!

The value returned is the number of arguments found in *s*,
irrespective of *maxargs*.

To get only the number of arguments, pass `NULL` for *args*
and/or `0` for *maxargs*. In this case, the string *s* is
**not** modified.

Arguments are delimited by white space. Arguments containing
white space must be given in (single or double) quotes.
There is no escaping of spaces or quotes (as that would
require rewriting *s* – maybe another time).

Usage example:

```C
char *args[N];
...
if (argsplit(cmdline, args, N) >= N) error("too many args");
```

---

**basename** returns a pointer into *s* where the basename
starts, that is, after the last `/` in *s*, assuming there
is a '/' immediately before the start of *s*; if *s* is null,
return null.

---

**streq** compares strings *s* and *t* for equality; it is
equivalent to `strcmp(s,t)==0` except that *s* and *t* may
be null (if both are null, return true, otherwise false). Also,
`if (streq(s,t)) ...` reads nicer than `if (strcmp(s,t)==0) ...`
in my opinion.
