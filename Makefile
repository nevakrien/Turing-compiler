CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -g2 -Wall -Iinclude

##dirs:
#include
#src
#tests
#bin

# Default target
all: bin/test_io bin/test_turing bin/parser.o

# Compile source files to object files
bin/io.o: src/io.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/turing.o: src/turing.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

# Build test executables
bin/test_io: tests/test_io.c bin/io.o
	$(CC) $(CFLAGS) $^ -o $@

bin/test_turing: tests/test_turing.c bin/turing.o
	$(CC) $(CFLAGS) $^ -o $@

bin/parser.o: src/parser.c #bin/turing.o
	$(CC) $(CFLAGS) -c $^ -o $@

bin/test_parser: tests/test_parser.c bin/parser.o
	$(CC) $(CFLAGS) $^ -o $@
# Cleanup
clean:
	rm -rf bin/*


clean_io:
	rm -f bin/io.o bin/test_io

clean_turing:
	rm -f bin/turing.o bin/test_turing

# Check
check: clean
	make all
	#rm -rf bin/*

test: clean
	python3 test.py
	#rm -rf bin/*

.PHONY: all clean clean_io clean_turing test check
