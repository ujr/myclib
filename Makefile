.POSIX:

CC = gcc -std=c99
CFLAGS = -Wall -Wextra -pedantic -Os -g3 -Isrc
LDFLAGS = -s
LDLIBS = # -lm
PREFIX = /usr/local

all: liba testsuite duff endian

check: testsuite
	bin/runtests

clean:
	rm -f bin/* src/*.o

TESTS = src/scan_test.o
LIBINCS = src/scan.h
LIBOBJS = src/scanint.o src/scanulong.o src/scanblank.o src/scanwhite.o

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

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

