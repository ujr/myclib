#ifndef _SCAN_H_
#define _SCAN_H_

/* All scanners here return the number of bytes scanned */

extern int scanint(const char *s, int *pv);
extern int scanulong(const char *s, unsigned long *pv);

extern int scanblank(const char *s); /* space and tab */
extern int scanwhite(const char *s); /* space \f \n \r \t \v */

#endif
