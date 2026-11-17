CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

all: vehicules

vehicules: main.c ex1.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f vehicules
