#ifndef MYUNIX_H
#define MYUNIX_H

/* Miscellaneous Unix-related utilities */

int open_read(const char *fn);   /* open for reading only */
int open_write(const char *fn);  /* open for writing only */
int open_append(const char *fn); /* open for appending only */
int open_trunc(const char *fn);  /* truncate to length zero */
int open_excl(const char *fn);   /* open for exclusive writing */

int daemonize(void);      /* become a daemon */

int fdblocking(int fd);   /* make fd blocking */
int fdnonblock(int fd);   /* make fd non-blocking */

int readable(int fd);     /* check if fd is readable */
int writable(int fd);     /* check if fd is writable */

#endif
