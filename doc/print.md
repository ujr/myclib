# The print.h API

Formatting values to character representations.

```C
#include "print.h"

char buf[...];
char *p = buf;

unsigned long u;
signed long d;
const char *z = "string";
int k;

size_t n;

n = printc(p, c);          /* append char */
n = printd(p, d);          /* append (signed) decimal */
n = printu(p, u);          /* append unsigned decimal */
n = print0u(p, u, k);      /* ditto, left-pad to k with '0' */
n = printx(p, u);          /* append hexadecimal */
n = print0x(p, u, k);      /* ditto, left-pad to k with '0' */
n = prints(p, z);          /* append string */
n = printsn(p, z, k);      /* ditto, but at most first k chars */
n = print0(0);             /* append '\0' to terminate string */

const char *fmt;
va_list ap;
size_t size = sizeof buf;

n = format(buf, size, fmt, ...);
n = format(buf, size, fmt, ap);
```

You will usually want to use **printf**(3) or **snprintf**(3)
from the Standard C Library.

All functions here return the number of charcters written
to the buffer. If the buffer is NULL, they return how many
characters would have been written.

It is the **caller's responsibility** to **avoid buffer overrun**!

- printc: append the single character '*' to the buffer
- printd: append the (signed) `long` *d* in decimal
- printu: append the `unsigned long` *u* in decimal
- printx: append the `unsigned long` *u* in hexadecimal
- print0u: same as `printu` but `0`-padded to *n* digits
- print0x: same as `printx` but `0`-padded to *n* digits
- prints: append the zero-termianted string to the buffer
- printsn: same as `prints` but at append at most *n* chars
- print0: append `\0' to the buffer (zero-terminate the string)

A typical usage pattern might be:

```C
char buf[128], *p;

p = buf;
p += prints(p, "Error: ");
p += printsn(p, errmsg, 64);
p += prints(p, " (code=");
p += printd(p, errcode);
p += prints(p, ")");
print0(p);

puts(buf);
```

Evidently, the same can be easily achieved with a simple
call to `printf` or `snprintf`, if desired.

Since 2005-10-15

The format functions are similar:

- format: append *fmt* with place holders expanded
- formatv: same as `format` but accepting a `va_list` argument

The **format** functions assume the target buffer is at least
of the given size. They copy *fmt* to the buffer, replacing
placeholders with the 4th, 5th, etc arguments as follows:

- `%c` is replaced by a single character from the corresponding
  argument, which must be of type `int` (cast to `unsigned char`).
- `%d` is replaced by a decimal rendition of the corresponding
  argument, which must be of type `int`.
- `%u` is replaced by a decimal rendition of the corresponding
  argument, which must be of type `unsigned int`.
- `%x` is replaced by a hexadecimal renditoin of the corresponding
  argument, which must be of type `unsigned int`.
- `%s` is replaced by the verbatim string in the corresponding
  argument, which must be of type `char*`.
- `%%` is replaced by a single `%` sign.

Other than the print functions above, the format functions
do append the terminating `\0` (if there is enough room in
the buffer). The terminating `\0` is not included in the
count returned.

```C
char buf[256];
int num = 123;
const char *str = "hello";
...
putbuf(buf, format(buf, sizeof buf, "num=%d str=%s\n", num, str));
```

Since 2006-08-27
