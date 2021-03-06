#ifndef SCAN_H
#define SCAN_H

/* All scanners here return the number of bytes scanned.
 * The comments after the prototypes hint at what the
 * function scans; see the documentation for details.
 */

int scanint(const char *s, int *pval);
int scanuint(const char *s, unsigned int *pval);
int scanulong(const char *s, unsigned long *pval);
int scanhex(const char *s, unsigned long *pval);

int scanblank(const char *s); /* space and tab */
int scanwhite(const char *s); /* space \f \n \r \t \v */
int scantext(const char *s, const char *t);
int scanpat(const char *s, const char *pat); /* foo: E* */
int scanuntil(const char *s, const char *reject); /* strcspn */
int scanwhile(const char *s, const char *accept); /* strspn */

int scanip4(const char *s, unsigned char ip[4]); /* 192.168.1.2 */
int scanip4op(const char *s, unsigned char ip[4], unsigned *port); /* 10.1.2.3:4321 */

#include <time.h> /* struct tm */
int scandate(const char *s, struct tm *tp); /* 2005-07-15 */
int scantime(const char *s, struct tm *tp); /* 12:34:45 */

#endif
