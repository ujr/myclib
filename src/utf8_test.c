/* Unit tests for utf8.{c,h} */

#include "test.h"
#include "utf8.h"


static void
encode(const int *pu, char *buf, size_t buflen)
{
  char *p = buf;
  char *end = buf + buflen - 5;

  while (*pu && p < end) {
    UTF8_PUT(*pu++, p);
  }

  *p = 0;
}

static void
decode(const char *u, int *buf, int buflen)
{
  size_t n = strlen(u);
  const char *end = u+n;
  const char *p = u;
  int c, i = 0;

  while (p < end && i < buflen-1) {
    UTF8_GET(c, p, end);
    buf[i++] = c;
  }

  buf[i] = 0;
}

void
utf8_test(int *pnumpass, int *pnumfail)
{
  int codes[8] = {0};
  char buf[256];
  int c;

  int numpass = 0;
  int numfail = 0;

  HEADING("Testing utf8.{c,h}");

  buf[utf8put(0xFFFD, buf)] = 0;
  TEST("encode U+FFFD", buf[0]==(char)0xEF && buf[1]==(char)0xBF &&
                        buf[2]==(char)0xBD && buf[3]==(char)0);
  utf8get(buf, &c);
  TEST("decode EF BF BD", c==0xFFFD);

  utf8put(0x1D11E, buf);
  utf8get(buf, &c);
  TEST("U+1D11E 𝄞", c == 0x1D11E);

  char kalimero[] = "Καλημέρα κόσμε";
  TEST("utf8len", 14 == utf8len(kalimero, -1));
  //INFO("sizeof(%s)==%zu, len=%zu", kalimero, sizeof(kalimero), utf8len(kalimero, -1));
  TEST("utf8len (limit)", 1 == utf8len(kalimero, 2));
  TEST("utf8len (limit)", 8 == utf8len(kalimero, 16));

  buf[0] = 0; /* empty string */
  TEST("utf8len (empty)", 0 == utf8len(buf, -1));

  codes[0] = 0x2022;  // •    8226
  codes[1] = 0x041F;  // П    1055
  codes[2] = 0x0451;  // ё    1105
  codes[3] = 0x0442;  // т    1090
  codes[4] = 0x0440;  // р    1088
  codes[5] = 0x1D11E; // 𝄞  119070
  codes[6] = 0;

  encode(codes, buf, sizeof(buf));
  TEST("encode", strcmp(buf, "•Пётр𝄞") == 0);

  decode(buf, codes, 8);
  TEST("decode", codes[0]==0x2022 && codes[1]==0x41F && codes[2]==0x451 &&
    codes[3]==0x442 && codes[4]==0x440 && codes[5]==0x1D11E && codes[6]==0);

  if (pnumpass) *pnumpass += numpass;
  if (pnumfail) *pnumfail += numfail;
}
