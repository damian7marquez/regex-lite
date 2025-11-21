CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: regex-lite

regex-lite: main.o file_search.o matcher.o
	$(CC) $(CFLAGS) -o regex-lite main.o file_search.o matcher.o

main.o: main.c file_search.h matcher.h
	$(CC) $(CFLAGS) -c main.c

file_search.o: file_search.c file_search.h matcher.h
	$(CC) $(CFLAGS) -c file_search.c

matcher.o: matcher.c matcher.h
	$(CC) $(CFLAGS) -c matcher.c

clean:
	rm -f *.o regex-lite
