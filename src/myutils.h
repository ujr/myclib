#ifndef _MYUTILS_H_
#define _MYUTILS_H_

#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "strbuf.h"

/* Miscellaneous utility functions */

extern int argsplit(char *s, char *args[], int maxargs);
extern char *basename(const char *s);
extern int streq(const char *s, const char *t); /* accept NULL */

extern long getln(FILE *fp, strbuf *sp, int *partial);
extern long getln2(FILE *fp, char **buf, size_t *size);
extern long getln3(FILE *fp, char *buf, size_t size, int *partial);
extern size_t eatln(FILE *fp);

#define ENDIAN_LITTLE  1    /* little endian: lsb at lowest mem addr */
#define ENDIAN_BIG     2    /* big endian: msb at lowest mem addr */
extern int getendian(void); /* return one of the costants above */

#define UTCSTAMPLEN 20 /* #bytes in a UTCSTAMP */

extern size_t utcscan(const char *s, struct tm *tp);
extern size_t utcstamp(char buf[], char sep);
extern int utcinit(void);

#endif
