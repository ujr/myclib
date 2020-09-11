#ifndef MYUTILS_H
#define MYUTILS_H

#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "strbuf.h"

/* Miscellaneous utility functions */

int argsplit(char *s, char *args[], int maxargs);
char *basename(const char *s);
int streq(const char *s, const char *t); /* accept NULL */

long getln(FILE *fp, strbuf *sp, int *partial);
long getln2(FILE *fp, char **buf, size_t *size);
long getln3(FILE *fp, char *buf, size_t size, int *partial);
size_t eatln(FILE *fp);

#define ENDIAN_LITTLE  1    /* little endian: lsb at lowest mem addr */
#define ENDIAN_BIG     2    /* big endian: msb at lowest mem addr */
int getendian(void); /* return one of the costants above */

#define UTCSTAMPLEN 20 /* #bytes in a UTCSTAMP */

size_t utcscan(const char *s, struct tm *tp);
size_t utcstamp(char buf[], char sep);
int utcinit(void);

#define signum(x) (((x) > 0) - ((x) < 0))

#endif
