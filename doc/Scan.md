# The scan.h API

```C
#include "scan.h"

int scanint(const char *s, int *pval);
int scanulong(const char *s, unsigned long *pval);
int scanhex(const char *s, unsigned long *pval);

int scanblank(const char *s); /* blank or tab */
int scanwhite(const char *s); /* blank \f \n \r \t \v */
int scantext(const char *s, const char *t);
int scanpat(const char *s, const char *pat);

int scanip4(const char *s, unsigned char ip[4]); /* 192.168.1.2 */
int scanip4op(const char *s, unsigned char ip[4], unsigned *port);

#include <time.h>
int scandate(const char *s, struct tm *tp); /* 2005-07-15 */
int scantime(const char *s, struct tm *tp); /* 12:34:56 */
```

These functions look for numbers, timestamps, etc.
at the beginning of the string pointed to by *s*.
Leading white space is *not* skipped (unless noted otherwise).

They all return the number of bytes scanned.
On syntax error, zero is returned and the out
argument(s) are undefined. Zero may also indicate
that an item was not present at the beginning of *s*.
Whether this is an error or not, is up to the caller.

The item(s) scanned are placed into the locations
pointed to by the additional argument(s).

- scanint: scan int in decimal notation with optional sign (`-` or `+`)
- scanuint: scan unsigned int in decimal notation
- scanulong: scan unsigned long in decimal notation
- scanhex: scan unsigned long in hexadecimal (0-9 a-f A-F)

- scanblank: scan spaces and tabs (but no other white space)
- scanwhite: scan white space (`\f`, `\n`, `\r`, `\t`, `\v`, space)
- scantext: scan (exactly) the given text
- scanpat: match input against a simple pattern (see below)

- scanip4: scan an IP v4 address in dotted decimal notation (like 192.168.1.2)
- scanip4op: like scanip4 with an optional port (separated by
  a blank or a colon or a percent sign)

- scandate: scan an ISO 8601 date (like 2005-07-15)
- scantime: scan an ISO 8601 time (like 12:34:56)

The time and date scanners update only the relevant fields
of the `struct tm`; all others are left untouched.

## Patterns

The `scanpat(s,pat)` function matches the string input `s` against the
pattern string `pat` and returns the number of bytes matched or zero
if `s` and `pat` do not match. The pattern works like this:

1. any sequence of blanks at the beginning of pat matches any
   sequence of blanks and tabs, even the empty sequence, in s;
2. any other sequence of blanks matches any non-empty sequence
   of blanks and tabs in s;
3. an asterisk matches any character up to the first occurrence
   in s of the next character in pat;
4. every other character matches itself.

There is no pattern that requires a blank or tab at the beginning
of `s`; if needed, say: `if (isspace(*p) && (n=scanpat(p, "...")))`

## Implementation Notes

The scanners assume that the digits 0..9 are encoded
consecutively and in this order. This is the case for
both ASCII and UTF-8 and allows scanning an integer
as by this pseudo code:

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

Since 2005-10-15
