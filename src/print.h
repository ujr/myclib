#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>
#include <stddef.h>

/* Render a string representation into the buffer pointed to by
 * the 1st argument from the object given in the 2nd argument.
 * Return the number of chars in the representation, even if the
 * buffer is NULL (useful for determining lengths beforehand).
 */

#define printc(s, c) ((s) ? *(s)=c, 1 : 1)
#define print0(s) ((s) ? *(s)='\0', 1 : 1)

size_t prints(char *s, const char *z);
size_t printsn(char *s, const char *z, size_t  max);

size_t printd(char *s, signed long val);
size_t printu(char *s, unsigned long val);
size_t print0u(char *s, unsigned long val, int n);
size_t printx(char *s, unsigned long val);
size_t print0x(char *s, unsigned long val, int n);

size_t format(char *s, size_t n, const char *fmt, ...);
size_t formatv(char *s, size_t n, const char *fmt, va_list ap);

#if 0
#include <time.h>
size_t printstm(char *s, struct tm *tp);
#endif

#endif
