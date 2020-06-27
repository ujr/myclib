#ifndef _MYFUNS_H_
#define _MYFUNS_H_

/* Miscellaneous utility functions */

extern int argsplit(char *s, char *args[], int maxargs);
extern char *basename(const char *s);
extern int streq(const char *s, const char *t); /* accept NULL */

#endif
