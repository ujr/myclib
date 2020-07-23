#ifndef _MYUNIX_H_
#define _MYUNIX_H_

/* Miscellaneous Unix-related utilities */

extern int open_read(const char *fn);   /* open for reading only */
extern int open_write(const char *fn);  /* open for writing only */
extern int open_append(const char *fn); /* open for appending only */
extern int open_trunc(const char *fn);  /* truncate to length zero */
extern int open_excl(const char *fn);   /* open for exclusive writing */

extern int daemonize(void);      /* become a daemon */

extern int fdblocking(int fd);   /* make fd blocking */
extern int fdnonblock(int fd);   /* make fd non-blocking */

extern int readable(int fd);     /* check if fd is readable */
extern int writable(int fd);     /* check if fd is writable */

#endif
