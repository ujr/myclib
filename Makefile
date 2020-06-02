.POSIX:

CC = gcc -std=c89
CFLAGS = -Wall -Wextra -Os -g3
LDFLAGS = -s
LDLIBS = # -lm
PREFIX = /usr/local

all:

clean:
	rm -f *.o

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

