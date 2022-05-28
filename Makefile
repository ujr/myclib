.POSIX:

CC = gcc -std=c99
CFLAGS = -Wall -Wextra -pedantic -Og -g -Isrc
LDFLAGS = # -s
LDLIBS = # -lm
PREFIX = /usr/local

all: liba testsuite argparse duff endian limits match random trycurs

check: testsuite liba
	bin/runtests

clean:
	rm -f bin/* src/*.o

TESTS = src/buf_test.o src/myutils_test.o src/print_test.o src/scan_test.o \
  src/strbuf_test.o src/simpleio_test.o src/scf_test.o src/iniconf_test.o \
  src/getopt_test.o src/utf8_test.o
LIBINCS = src/myutils.h src/myunix.h src/print.h src/scan.h src/utf8.h \
  src/strbuf.h src/simpleio.h src/scf.h src/test.h src/iniconf.h
LIBOBJS = src/argsplit.o src/basename.o src/streq.o src/strbuf.o \
  src/getln.o src/getln2.o src/getln3.o src/eatln.o src/scf.o \
  src/simpleio.o src/utcscan.o src/utcstamp.o src/utcinit.o src/endian.o \
  src/daemonize.o src/fdblocking.o src/fdnonblock.o \
  src/readable.o src/writable.o src/open_read.o src/open_write.o \
  src/open_append.o src/open_trunc.o src/open_excl.o \
  src/scanint.o src/scanuint.o src/scanulong.o src/scanhex.o \
  src/scanblank.o src/scanwhite.o src/scantext.o src/scanpat.o \
  src/scanuntil.o src/scanwhile.o src/scanip4.o src/scanip4op.o \
  src/scandate.o src/scantime.o \
  src/printu.o src/print0u.o src/printx.o src/print0x.o \
  src/printd.o src/prints.o src/printsn.o src/format.o \
  src/iniconf.o src/utf8.o

liba: bin/myclib.a
bin/myclib.a: $(LIBOBJS)
	rm -f $@
	ar rcs $@ $(LIBOBJS)

testsuite: bin/runtests
bin/runtests: src/runtests.c bin/myclib.a $(TESTS) $(LIBINCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(TESTS) bin/myclib.a $(LDLIBS)

argparse: bin/argparse
bin/argparse: src/scf.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -DDEMO $< $(LDLIBS)

duff: bin/duff
bin/duff: src/duff.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

endian: bin/endian
bin/endian: src/endian.c src/myutils.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -DDEMO $< $(LDLIBS)

limits: bin/limits
bin/limits: src/limits.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

match: bin/match
bin/match: src/scanpat.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -DDEMO $< $(LDLIBS)

random: bin/random
bin/random: src/random.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

trycurs: bin/trycurs
bin/trycurs: src/trycurs.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS) -lcurses

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

