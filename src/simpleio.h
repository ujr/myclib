#ifndef SIMPLEIO_H
#define SIMPLEIO_H

#include <stddef.h>

/* internal buffer sizes */
#define SIOBUFSIZEIN 4096
#define SIOBUFSIZEOUT 1024

/* arguments for putmode() */
#define SIOLINEBUF 0   /* line-buffered stdout */
#define SIOFULLBUF 1   /* fully buffered stdout */
#define SIOAUTOBUF 2   /* line-buf if tty, full-buf else */

int setin(const char *fn);   /* -1/errno on error */
int setout(const char *fn);  /* -1/errno on error */
int seterr(const char *fn);  /* -1/errno on error */

/* buffered reading from stdin */

int getbyte(void); /* -1 on error with errno set */
long getline(char *buf, size_t max, int sep, int *partial);
long skipline(int sep); /* skip until and incl sep */
void ungetbyte(int c); /* push back, at most once */

/* buffered writing to stdout */

int putbyte(int c);   /* -1 on error (see errno), 0 if ok */
int putstr(const char *s);  /* -1 on error (see errno), 0 if ok */
int putbuf(const char *buf, size_t len); /* ditto */
int putline(const char *s); /* end with \n if not in s */

int putmode(int mode);  /* 0 = line, 1 = fully buffered */
#define putflush() putbyte(-1) /* flush output buffer */

/* unbuffered writing to stderr
 *
 * The logging functions do not return errors, they simply
 * abort(3): we probably couldn't report the error anyway
 */
void logbyte(int c); /* abort on error */
void logstr(const char *s); /* ditto */
void logbuf(const char *buf, size_t len); /* ditto */
void logline(const char *s); /* end with \n if not in s */

#endif
