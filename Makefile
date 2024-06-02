CC = gcc
CXX = g++

# Use -g for debugging information and -Wall for compiler warnings
CFLAGS = -g -O2 -Wall
CXXFLAGS = -g -O2 -Wall

all: io.o


io.o:
	$(CC) $(CFLAGS) -c io.c

clean:
	rm -f io.o

check: all 
	rm -f io.o


.PHONY: all clean check
