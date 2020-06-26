#ifndef _SCAN_H_
#define _SCAN_H_

/* All scanners here return the number of bytes scanned.
 * The comments after the prototypes hint at what the
 * function scans; see the documentation for details.
 */

extern int scanint(const char *s, int *pval);
extern int scanuint(const char *s, unsigned int *pval);
extern int scanulong(const char *s, unsigned long *pval);
extern int scanhex(const char *s, unsigned long *pval);

extern int scanblank(const char *s); /* space and tab */
extern int scanwhite(const char *s); /* space \f \n \r \t \v */
extern int scantext(const char *s, const char *t);
extern int scanpat(const char *s, const char *pat); /* foo: E* */
extern int scanuntil(const char *s, const char *reject); /* strcspn */
extern int scanwhile(const char *s, const char *accept); /* strspn */

extern int scanip4(const char *s, unsigned char ip[4]); /* 192.168.1.2 */
extern int scanip4op(const char *s, unsigned char ip[4], unsigned *port); /* 10.1.2.3:4321 */

#include <time.h> /* struct tm */
extern int scandate(const char *s, struct tm *tp); /* 2005-07-15 */
extern int scantime(const char *s, struct tm *tp); /* 12:34:45 */

#endif
