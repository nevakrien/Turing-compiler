CC = gcc
CXX = g++

# Use -g for debugging information and -Wall for compiler warnings
CFLAGS = -g2 -Wall
CXXFLAGS = -g2 -Wall

all: io.o


io.o:
	$(CC) $(CFLAGS) -c io.c

test_io.out: io.o
	$(CC) $(CFLAGS) test_io.c io.o -o test_io.out

clean_io:
	rm -f io.o test_io.out

clean: clean_io

check: all 
	rm -f io.o test_io.out


.PHONY: all clean check clean_io
