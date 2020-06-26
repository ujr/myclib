# The scan.h API

```C
#include "scan.h"

int scanint(const char *s, int *pv);
int scanulong(const char *s, unsigned long *pv);

int scanblank(const char *s); /* blank or tab */
int scanwhite(const char *s); /* blank \f \n \r \t \v */
```

These functions look for numbers, timestamps, etc.
at the beginning of the string pointed to by *s*.
Leading white space is *not* skipped.

They all return the number of bytes scanned.
On syntax error, zero is returned and the out
argument(s) are undefined. Zero may also indicate
that an item was not present at the beginning of *s*.
Whether this is an error or not, is up to the caller.

The item(s) scanned are placed into the locations
pointed to by the additional argument(s).

- scanint: scan int in decimal notation with optional sign (`-` or `+`)
- scanulong: scan unsigned long in decimal notation
- scanblank: scan spaces and tabs (but no other white space)
- scanwhite: scan white space (`\f`, `\n`, `\r`, `\t`, `\v`, space)

## Implementation

The scanners generally assume that the digits 0..9
are encoded consecutively and in this order. This
is the case for both ASCII and UTF-8 and allows
scanning an integer as by this pseudo code:

```C
while ('0' <= *p && *p <= '9') {
  num = 10 * num + (*p - '0'); p += 1;
}
```

Using `isdigit(*p)` from `ctype.h` in the loop test
would make it independent of encoding, but the loop
body would still depend on the above assumption.
To be independent of the character encoding, we
would have to branch on each digit like this:

```C
switch (*p) {
  case '0': num = 10*num; break;
  case '1': num = 10*num + 1; break;
  case '2': num = 10*num + 2; break;
  // etc.
}
```

But it is not worth the effort because I am perfectly
happy assuming input is ASCII or UTF-8 encoded.
