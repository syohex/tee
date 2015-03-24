.PHONY: clean

CC ?= cc
CFLAGS ?= -Wall -g

all: c

c: tee-c

tee-c: c/tee.c
	$(CC) -o tee-c $(CFLAGS) c/tee.c

clean:
	rm -f tee-c
