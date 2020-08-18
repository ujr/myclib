# The strbuf.h API

Provides growable character buffers that are always
zero-terminated and therefore valid C string.

```C
#include "strbuf.h"

strbuf sb = {0};  /* mandatory initialization */
strbuf *sp = &sb;

char *z;
size_t n;
int r;

z = strbuf_ptr(sp);
n = strbuf_len(sp);
n = strbuf_size(sp);
c = strbuf_char(sp, n);
strbuf_char(sp, n) = c;
if (strbuf_failed(sp)) error();

r = strbuf_add(sp, strbuf *sq);
r = strbuf_addc(sp, int c);
r = strbuf_addz(sp, const char *z);
r = strbuf_addb(sp, const char *buf, size_t len);
r = strbuf_addf(sp, const char *fmt, ...);
r = strbuf_addfv(sp, const char *fmt, va_list ap);

r = strbuf_ready(sp, size_t dlen);
strbuf_trunc(sp, size_t len);
strbuf_free(sp);
```

A `strbuf` is in one of three states: unallocated
(buf==0, the initial state), normal (buf!=0), or
failed (after a memory allocation failed).

A strbuf MUST be initialized as in `strbuf sb = {0};`
to start in the unallocated state!

All long names `strbuf_xxx` have corresponding short
names `sbxxx` unless `STRBUF_NO_SHORT_NAMES` is
defined before including *strbuf.h*.

**sbptr** returns a pointer to the zero-terminated
string in the buffer; it can be used with functions
that read zero-terminated strings like **puts**(3)
or **strcmp**(3).

**sblen** returns the length of the string, excluding
the terminating zero, and **sbsize** returns the current
size (capacity) of the buffer.

**sbchar** is a character at position *n* for reading
or writing; strbuf must be allocated and *n* in range,
which is not checked!

**sbfailed** returns true after a memory allocation
has failed.

The **sbadd** functions append to the end of the string:
**add** the contents of another strbuf,
**addc** a character (`int` cast to `unsigned char`),
**addz** a zero-terminated string,
**addb** a buffer of the given length,
**addf** and **addfv** a formatted string (see **sprintf**(3)).
They all return true if successful and false if not;
the only reason for not being successful is a failed
memory allocation, so *errno* can be expected to be
`ENOMEM` in this case.

**sbready** ensures the strbuf has enough capacity
the given number of additional string bytes; return
true (non-zero) if successful and false (zero) if not.

**sbtrunc** truncates the string in the buffer to the
given length, which must be no more than the current
length of the string. String length is the number of
bytes, not counting the terminating zero-byte.

**sbfree** releases the memory of the buffer and returns
the strbuf to the unallocated state (which also clears
the failed flag).

Since 2006-06-09

---

Many similar dynamic string libraries exist. Somewhat
known is the “Simple Dynamic String” library (SDS),
available from [github.com/antirez/sds][sds].
It “hides” the housekeeping stuff and returns
a pointer directly to the char buffer, in the same
way as does the [buf.h](../src/buf.h) API.

[sds]: https://github.com/antirez/sds
