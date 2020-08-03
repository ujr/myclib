#ifndef _MYUTILS_H_
#define _MYUTILS_H_

#include <stddef.h>
#include <stdio.h>

/* Miscellaneous utility functions */

extern int argsplit(char *s, char *args[], int maxargs);
extern char *basename(const char *s);
extern int streq(const char *s, const char *t); /* accept NULL */

#endif
