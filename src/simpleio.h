#ifndef _SIMPLEIO_H_
#define _SIMPLEIO_H_

#include <stddef.h>

/* internal buffer sizes */
#define SIOBUFSIZEIN 4096
#define SIOBUFSIZEOUT 1024

/* arguments for putmode() */
#define SIOLINEBUF 0   /* line-buffered stdout */
#define SIOFULLBUF 1   /* fully buffered stdout */
#define SIOAUTOBUF 2   /* line-buf if tty, full-buf else */

extern int setin(const char *fn);   /* -1/errno on error */
extern int setout(const char *fn);  /* -1/errno on error */
extern int seterr(const char *fn);  /* -1/errno on error */

/* buffered reading from stdin */

extern int getbyte(void); /* -1 on error with errno set */
extern long getline(char *buf, size_t max, int sep, int *partial);
extern long skipline(int sep); /* skip until and incl sep */
extern void ungetbyte(int c); /* push back, at most once */

/* buffered writing to stdout */

extern int putbyte(int c);   /* -1 on error (see errno), 0 if ok */
extern int putstr(const char *s);  /* -1 on error (see errno), 0 if ok */
extern int putbuf(const char *buf, size_t len); /* ditto */
extern int putline(const char *s); /* end with \n if not in s */

extern int putmode(int mode);  /* 0 = line, 1 = fully buffered */
#define putflush() putbyte(-1) /* flush output buffer */

/* unbuffered writing to stderr
 *
 * The logging functions do not return errors, they simply
 * abort(3): we probably couldn't report the error anyway
 */
extern void logbyte(int c); /* abort on error */
extern void logstr(const char *s); /* ditto */
extern void logbuf(const char *buf, size_t len); /* ditto */
extern void logline(const char *s); /* end with \n if not in s */

#endif
