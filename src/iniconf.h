#ifndef INICONF_H
#define INICONF_H

#include <stddef.h>
#include <stdio.h>

/* The callback function invoked for each config setting */
typedef int (*iniconf_handler)(const char *section, const char *name, const char *value, size_t lineno, void *userdata);

/* Signature of a custom reader function */
typedef int (*iniconf_reader)(void *readerstate);

/* All return 0, or the first non-zero returned by the handler, or -1 if nomem */
int iniconf_fn(const char *fn, iniconf_handler handler, void *userdata);
int iniconf_fp(FILE *fp, iniconf_handler handler, void *userdata);
int iniconf_sz(const char *sz, iniconf_handler handler, void *userdata);
int iniconf_rf(iniconf_reader rf, void *rfstate, iniconf_handler handler, void *userdata);

#endif
