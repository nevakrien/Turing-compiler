CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -g2 -Wall -Iinclude

##dirs:
#include
#src
#tests
#bin

#Object files
IO_OBJ = bin/io.o
TURING_OBJ = bin/turing.o

# Test executables
TEST_IO = bin/test_io
TEST_TURING = bin/test_turing

# Default target
all: $(TEST_IO) $(TEST_TURING)

# Compile source files to object files
$(IO_OBJ): src/io.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

$(TURING_OBJ): src/turing.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

# Build test executables
$(TEST_IO): tests/test_io.c $(IO_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_TURING): tests/test_turing.c $(TURING_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Cleanup
clean:
	rm -rf bin

clean_io:
	rm -f $(IO_OBJ) $(TEST_IO)

clean_turing:
	rm -f $(TURING_OBJ) $(TEST_TURING)

# Check
check: all
	rm -rf bin
test: clean
	python3 test.py
	rm -rf bin

.PHONY: all clean clean_io clean_turing test check
