
#include <assert.h>
#include <stddef.h>

#include "utf8.h"

/* About UTF-8
 *
 * Value Range     First Byte Continuation Bytes
 *     0..127      0xxx xxxx
 *   128..2047     110x xxxx  10xx xxxx                        (1)
 *  2048..65535    1110 xxxx  10xx xxxx  10xx xxxx             (2)
 * 65536..1114111  1111 0xxx  10xx xxxx  10xx xxxx  10xx xxxx  (3)
 *
 * (1) continuation bytes are 10xx xxxx (6 bits payload)
 * (2) values 55296..57343 (UTF-16 surrogate pairs) are not allowed
 * (3) 1114111 = 10FFFF hex is the maximum value allowed
 *
 * For details see RFC 3629 and consult Wikipedia.
 *
 * The decoder below uses a table to get the payload from the
 * first byte, instead of switching on the first few bits
 * (this idea is from SQLite). Then it reads all continuation
 * bytes that follow, even if there are more than the first
 * byte mandates. Overlong encodings of 7bit characters are
 * recognised and replaced by U+FFFD (replacement character),
 * as are surrogate pairs 0xD800..0xDFFF, which are not allowed
 * in UTF-8. However, overlong encodings of larger values are
 * not detected and bytes 0x80..0xBF are returned as-is, even
 * though they are not valid UTF-8.
 */


/* Payload of 1st byte & 0x3F given the two hi bits are 11 */
const unsigned char utf8tab[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  /* 110x xxxx */
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,  /* 110x xxxx */
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,  /* 110x xxxx */
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,  /* 110x xxxx */
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  /* 1110 xxxx */
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,  /* 1110 xxxx */
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  /* 1111 0xxx */
  0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x00, 0x00,  /* 1111 10xx, 110x, 1110, 1111 */
};


/** decode one character from p, store it in pc, return #bytes read */
size_t
utf8get(const char *p, int *pc)
{
  const int replacement = 0xFFFD;
  const char *q = p;
  int c = (unsigned char) *p++;
  if (c >= 0xC0) {
    /* get payload from low 6 bits of first byte */
    c = utf8tab[c & 0x3F];
    /* ingest continuation bytes (10xx xxxx) */
    while ((*p & 0xC0) == 0x80) {
      c = (c << 6) + ((unsigned char) *p++ & 0x3F);
    }
    /* replace overlong 7bit encodings and surrogate pairs */
    if (c < 0x80 || (0xD800 <= c && c <= 0xDFFF)) {
      c = replacement;
    }
  }
  if (pc) *pc = c;
  return p - q;
}


/** encode c as utf8 at p, return #bytes written (1..4) */
size_t
utf8put(int c, char *p) {
  unsigned char *pp = (unsigned char *) p;
  if (c < 0x80) {
    *pp = (unsigned char) (c&0xFF);
    return 1;
  }
  if (c < 0x800) {
    pp[0] = 0xC0 + (unsigned char) ((c >>  6) & 0x1F);
    pp[1] = 0x80 + (unsigned char) ( c        & 0x3F);
    return 2;
  }
  if (c < 0x10000) {
    pp[0] = 0xE0 + (unsigned char) ((c >> 12) & 0x0F);
    pp[1] = 0x80 + (unsigned char) ((c >>  6) & 0x3F);
    pp[2] = 0x80 + (unsigned char) ( c        & 0x3F);
    return 3;
  }
  if (c < 0x110000) {
    pp[0] = 0xF0 + (unsigned char) ((c >> 18) & 0x07);
    pp[1] = 0x80 + (unsigned char) ((c >> 12) & 0x3F);
    pp[2] = 0x80 + (unsigned char) ((c >>  6) & 0x3F);
    pp[3] = 0x80 + (unsigned char) ( c        & 0x3F);
    return 4;
  }
  /* c is beyond the max of 0x10FFFF; emit a replacement
     character U+FFFD, which encodes in UTF-8 as EF BF BD */
  pp[0] = (char) 0xEF;
  pp[1] = (char) 0xBF;
  pp[2] = (char) 0xBD;
  return 3;
}


/** return num of code points in z (up to first 0 or len) */
size_t
utf8len(const char *z, size_t len)
{
  const unsigned char *end;
  const unsigned char *p = (const unsigned char *) z;
  size_t n;

  if (len != (size_t)(-1)) end = &p[len];
  else end = (const unsigned char *)(-1);
  assert(p <= end);

  for (n = 0; *p && p < end; n++) {
    if (*p++ >= 0xC0) {
      while ((*p & 0xC0) == 0x80) p++;
    }
  }
  return n;
}
