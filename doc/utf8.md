# The utf8.h API

Encoding and decoding to and from UTF-8.

```C
#include "utf8.h"

int c;       /* code point to encode or decoded */
char *z;     /* pointer to 0-terminated string */
char *p;     /* pointer to char buffer */
const char *end;  /* one after end of buffer */

UTF8_GET(c, z, end)
UTF8_PUT(c, p)
UTF8_SKIP(z)

size_t utf8get(const char *p, int *pc);
size_t utf8put(int c, char *p);
size_t utf8len(const char *z, size_t len);
```

**utf8get** decodes one character from the buffer at `p`,
stores the result in `*pc`. Returns the number of bytes read,
usually 1 to 4, but may be more in case of invalid UTF-8 data.

**utf8put** stores the given code point `c` UTF-8 encoded into
the buffer starting at the given pointer `p`. Values for `c`
beyond the max value of 1114111 are replaced by U+FFFD, so that
at most 4 bytes are written. Returns the number of bytes written.

**utf8len** counts the number of characters in the given
buffer, up to the given length or a 0 byte, whichever limit
is reached first. (Pass `-1` for the length to turn it off).

To encode or decode entire strings the macros may be
found to be more efficient. The macros increment the
pointers. UTF8_SKIP advances the pointer by one character
without decoding it.
