.POSIX:

CC = gcc -std=c99
CFLAGS = -Wall -Wextra -pedantic -Os -g3 -Isrc
LDFLAGS = -s
LDLIBS = # -lm
PREFIX = /usr/local

all: liba testsuite duff endian limits match

check: testsuite
	bin/runtests

clean:
	rm -f bin/* src/*.o

TESTS = src/buf_test.o src/myfuns_test.o src/strbuf_test.o src/scan_test.o
LIBINCS = src/myfuns.h src/myunix.h src/scan.h src/strbuf.h src/test.h
LIBOBJS = src/argsplit.o src/basename.o src/streq.o src/strbuf.o \
  src/daemonize.o src/fdblocking.o src/fdnonblock.o \
  src/readable.o src/writable.o src/open_read.o src/open_write.o \
  src/open_append.o src/open_trunc.o src/open_excl.o \
  src/scanint.o src/scanuint.o src/scanulong.o src/scanhex.o \
  src/scanblank.o src/scanwhite.o src/scantext.o src/scanpat.o \
  src/scanuntil.o src/scanwhile.o src/scanip4.o src/scanip4op.o \
  src/scandate.o src/scantime.o

liba: bin/myclib.a
bin/myclib.a: $(LIBOBJS)
	rm -f $@
	ar cr $@ $(LIBOBJS)
	-ranlib $@

testsuite: bin/runtests
bin/runtests: src/runtests.c bin/myclib.a $(TESTS) $(LIBINCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(TESTS) bin/myclib.a $(LDLIBS)

duff: bin/duff
bin/duff: src/duff.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

endian: bin/endian
bin/endian: src/endian.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -DDEMO $< $(LDLIBS)

limits: bin/limits
bin/limits: src/limits.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

match: bin/match
bin/match: src/scanpat.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -DDEMO $< $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

