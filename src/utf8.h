/* UTF-8 encoding and decoding */


#include <stddef.h>


extern size_t utf8get(const char *p, int *pc);
extern size_t utf8put(int c, char *p);
extern size_t utf8len(const char *z, size_t len);


#define UTF8_SKIP(z) do {             \
  if ((*(z++)) >= 0xC0 ){             \
    while ((*z & 0xC0) == 0x80) z++;  \
  }                                   \
} while (0)


#define UTF8_GET(c, z, end) do {                     \
  extern const unsigned char utf8tab[64];            \
  c = (unsigned char) *z++;                          \
  if (c >= 0xC0) {                                   \
    c = utf8tab[c & 0x3F];                           \
    while ((*z & 0xC0) == 0x80 && z < end) {         \
      c = (c << 6) + ((unsigned char) *z++ & 0x3F);  \
    }                                                \
    if (c < 0x80 || (0xD800 <= c && c <= 0xDFFF)) {  \
      c = 0xFFFD; /* replacement character */        \
    }                                                \
  }                                                  \
} while (0)


#define UTF8_PUT(c, p) do { int u = (c);               \
  if (u < 0x80) *p++ = (unsigned char) (u & 255);      \
  else if (u < 0x800) {                                \
    *p++ = 0xC0 + (unsigned char) ((u >>  6) & 0x1F);  \
    *p++ = 0x80 + (unsigned char) ( u        & 0x3F);  \
  }                                                    \
  else if (u < 0x10000) {                              \
    *p++ = 0xE0 + (unsigned char) ((u >> 12) & 0x0F);  \
    *p++ = 0x80 + (unsigned char) ((u >>  6) & 0x3F);  \
    *p++ = 0x80 + (unsigned char) ( u        & 0x3F);  \
  }                                                    \
  else {                                               \
    *p++ = 0xF0 + (unsigned char) ((u >> 18) & 0x07);  \
    *p++ = 0x80 + (unsigned char) ((u >> 12) & 0x3F);  \
    *p++ = 0x80 + (unsigned char) ((u >>  6) & 0x3F);  \
    *p++ = 0x80 + (unsigned char) ( u        & 0x3F);  \
  }                                                    \
} while (0)
