CC = $(shell which gcc-14 || echo gcc) #this is to get the best preformance
#CXX = g++

# Compiler flags
CFLAGS = -g2 -march=native -Wall -Iinclude

##dirs:
#include
#src
#tests
#bin

# Default target
all:bin/tmc1 bin/tmc0 bin/test_io bin/test_turing bin/test_parser bin/tape_tool bin/run_turing bin/compiler.o bin/test_compiler #bin/libio.so
	@echo "Compiler used: $(CC)"

# Compile source files to object files
bin/io.o: src/io.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@


bin/cli.o: src/cli.c  
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/turing.o: src/turing.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/IR.o: src/IR.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/parser.o: src/parser.c #bin/turing.o
	$(CC) $(CFLAGS) -c $^ -o $@

bin/compiler.o: src/compiler.c
	$(CC) $(CFLAGS) -c $^ -o $@


# Build test executables
# bin/test_io: tests/test_io.c bin/io.o #bin/cli.o bin/IR.o bin/parser.o
# 	$(CC) $(CFLAGS) $^ -o $@

bin/test_io: tests/test_io.c bin/io.o
	$(CC) $(CFLAGS) $^ -o $@ -fno-inline -fno-inline-functions 

bin/test_turing: tests/test_turing.c bin/turing.o
	$(CC) $(CFLAGS) $^ -o $@


bin/test_parser: tests/test_parser.c bin/parser.o bin/IR.o
	$(CC) $(CFLAGS) $^ -o $@

bin/test_compiler:tests/test_compiler.c bin/compiler.o 
	$(CC) $(CFLAGS) $^ -o $@

#tools
bin/tmc0: src/tmc0.c  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o
	$(CC) $(CFLAGS) $^ -o $@

#tools
bin/tmc1: src/tmc1.c  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o
	$(CC) $(CFLAGS) $^ -o $@


bin/tape_tool: src/tape_tool.c bin/io.o bin/cli.o bin/IR.o bin/parser.o
	$(CC) $(CFLAGS) $^ -o $@

bin/run_turing: src/interpreter.c bin/io.o bin/cli.o bin/IR.o bin/parser.o bin/turing.o
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
	make all -j
	#rm -rf bin/*

test: clean
	time make all -j
	python3 test.py

bench: clean
	time make all -j
	@cd code_tests/ && \
    ./test.sh

comp_test:
	make --assume-new=bin/compiler.o -j
	python3 -c "from test import run_comp_test;run_comp_test()"

.PHONY: all clean clean_io clean_turing test check comp_test bench
