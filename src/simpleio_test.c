/* Unit tests for simpleio.{c.h} */

#define _POSIX_C_SOURCE 2001L

#include "test.h"

#include <stdio.h>
#include <unistd.h>

#include "simpleio.h"

static int redirect(int fd1, int fd2)
{
  int fd = dup(fd2);
  dup2(fd1, fd2);
  return fd;
}

static void restore(int fd1, int fd2)
{
  dup2(fd1, fd2);
}

void
simpleio_test(int *pnumpass, int *pnumfail)
{
  FILE *fp;
  int oldfd;
  char buf[256];
  int partial;
  long n;

  int numpass = 0;
  int numfail = 0;

  HEADING("Testing simpleio.{c,h}");

  fp = tmpfile();
  fputs("food for input\n\nand longer than our buffer", fp);
  rewind(fp);

  oldfd = redirect(fileno(fp), 0);

  TEST("getbyte", getbyte() == 'f');
  TEST("getbyte", getbyte() == 'o');

  n = getline(buf, 16, '\n', &partial);
  TEST("getline", n == 13 && STREQ("od for input\n", buf) && !partial);

  n = getline(buf, 16, '\n', &partial);
  TEST("getline empty", n == 1 && STREQ("\n", buf) && !partial);

  n = getline(buf, 16, '\n', &partial);
  TEST("getline long", n == 15 && STREQ("and longer than", buf) && partial);

  n = getline(buf, 16, '\n', &partial);
  TEST("getline rest", n == 11 && STREQ(" our buffer", buf) && partial);

  n = getline(buf, 16, '\n', &partial);
  TEST("getline eof", n == 0 && STREQ("", buf) && partial);

  restore(oldfd, 0);
  fclose(fp);

  fp = tmpfile();
  oldfd = redirect(fileno(fp), 1);
  putbyte('h');
  putbyte('i');
  putline(" there");
  putstr("welcome");
  putbuf("\ncrap", 1);
  putflush();
  rewind(fp);
  n = fread(buf, 1, sizeof(buf) - 1, fp);
  if (n < 0) buf[0] = 0; else buf[n] = 0;
  fclose(fp);
  restore(oldfd, 1);
  TEST("putxxx", STREQ("hi there\nwelcome\n", buf));

  fp = tmpfile();
  oldfd = redirect(fileno(fp), 2);
  logbyte('h');
  logbyte('i');
  logline(" there");
  logstr("welcome");
  logbuf("\ncrap", 1);
  rewind(fp);
  n = fread(buf, 1, sizeof(buf) - 1, fp);
  if (n < 0) buf[0] = 0; else buf[n] = 0;
  fclose(fp);
  restore(oldfd, 2);
  TEST("logxxx", STREQ("hi there\nwelcome\n", buf));

  *pnumpass += numpass;
  *pnumfail += numfail;
}
