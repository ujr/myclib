.POSIX:

CC = gcc -std=c89
CFLAGS = -Wall -Wextra -Os -g3
LDFLAGS = -s
LDLIBS = # -lm
PREFIX = /usr/local

all:

duff: bin/duff
bin/duff: src/duff.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

endian: bin/endian
bin/endian: src/endian.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -DDEMO $< $(LDLIBS)

clean:
	rm -f bin/* src/*.o

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

