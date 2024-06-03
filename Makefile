CC = gcc
CXX = g++

# Use -g for debugging information and -Wall for compiler warnings
CFLAGS = -g2 -Wall
CXXFLAGS = -g2 -Wall

all: io.o turing.o


#IO
io.o:
	$(CC) $(CFLAGS) -c io.c

test_io: test_io.out

test_io.out: io.o
	$(CC) $(CFLAGS) test_io.c io.o -o test_io.out

clean_io:
	rm -f io.o test_io.out

#turing
turing.o:
	$(CC) $(CFLAGS) -c turing.c

test_turing: test_turing.out


test_turing.out: turing.o
	$(CC) $(CFLAGS) test_turing.c turing.o -o test_turing.out


clean_turing: 
	rm -rf turing.o test_turing.out

#clean
clean: clean_io clean_turing

check: all 
	rm -f io.o test_io.out


.PHONY: all clean check clean_io test_io test_turing clean_turing
