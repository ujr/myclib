#ifndef _MYUTILS_H_
#define _MYUTILS_H_

#include <stddef.h>
#include <stdio.h>

#include "strbuf.h"

/* Miscellaneous utility functions */

extern int argsplit(char *s, char *args[], int maxargs);
extern char *basename(const char *s);
extern int streq(const char *s, const char *t); /* accept NULL */

extern long getln(FILE *fp, strbuf *sp, int *partial);
extern long getln2(FILE *fp, char **buf, size_t *size);
extern long getln3(FILE *fp, char *buf, size_t size, int *partial);
extern size_t eatln(FILE *fp);

#endif
