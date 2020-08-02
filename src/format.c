#include "print.h"

#include <stdarg.h>
#include <stddef.h>

/* A much simplified snprintf(3) */

size_t
format(char *buf, size_t size, const char *fmt, ...)
{
  va_list ap;
  size_t r;

  va_start(ap, fmt);
  r = formatv(buf, size, fmt, ap);
  va_end(ap);

  return r;
}

#define shipout(c) do { if (!buf) ++i; \
  else if (i < size) buf[i++] = (c); \
  else (void) (c); } while (0)

size_t
formatv(char *buf, size_t size, const char *fmt, va_list ap)
{
  int c;
  char *s, num[32];
  size_t i = 0, j, k;
  signed long d;
  unsigned long u;

  if (!fmt) return 0;

  while (*fmt) {
    if ((c = *fmt++) == '%') switch (c = *fmt++) {
      case 's': /* string */
        s = va_arg(ap, char *);
        if (!s) s = "(null)";
        while (*s) shipout(*s++);
        break;
      case 'd': /* signed decimal */
        d = va_arg(ap, int);
        k = printd(num, d);
        for (j=0; j<k; j++) shipout(num[j]);
        break;
      case 'u': /* unsigned decimal */
        u = va_arg(ap, int);
        k = printu(num, u);
        for (j=0; j<k; j++) shipout(num[j]);
        break;
      case 'x': /* unsigned hex */
        u = va_arg(ap, int);
        k = printx(num, u);
        for (j=0; j<k; j++) shipout(num[j]);
        break;
      case 'c': /* character */
        d = va_arg(ap, int);
        shipout((unsigned char) d);
        break;
      case '%': /* percent sign */
        shipout('%');
        break;
      case 0: /* percent at end of fmt */
        shipout('%');
        goto done;
      default: /* unknown */
        shipout('%');
        shipout(c);
        break;
    }
    else shipout(c);
  }
done:
  /* Append '\0' but don't cont it */
  if (i < size && buf) buf[i] = '\0';
  return i; /* #chars appended */
}
