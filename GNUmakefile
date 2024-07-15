CC = $(shell which gcc-14 || echo gcc) #this is to get the best preformance
CXX =$(shell which g++-14 || echo g++) #clang fails... but nicer error messages


# Compiler flags
BASE_FLAGS=-Wall -g2 -march=native -Iinclude
CFLAGS = $(BASE_FLAGS) -std=gnu99
CXXFLAGS= $(BASE_FLAGS) -std=c++17


TEST_FLAGS= #-fsanitize=address -fsanitize=undefined

##dirs:
#include
#src
#tests
#bin

# Default target
all: all_tools all_tests bin/compiler.o bin/O2.o  #bin/tmc1_bad_hop#bin/libio.so
	@echo "Compiler used: $(CC)"

all_tools: bin/tape_tool bin/run_turing bin/tmc1 bin/tmc0 bin/treemc
all_tests: bin/test_io bin/test_turing bin/test_parser bin/test_compiler bin/test_code_tree bin/test_tree_parse

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

#c++ O2
pch/O2.hpp.gch: include/O2.hpp
	$(CXX) $(CXXHEADERFLAGS) -fpermissive -x c++-header include/O2.hpp -o pch/O2.hpp.gch

bin/O2.o: src/O2.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/asm_O2.o: src/asm_O2.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/tree_asm.o: src/tree_asm.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/basic_fuse.o: src/basic_fuse.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

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

# bin/test_cpp: tests/test_cpp.cpp bin/cli.o bin/IR.o bin/parser.o bin/compiler.o
# 	$(CXX) $(CXXFLAGS) $^ -o $@ -fpermissive

bin/test_code_tree: tests/test_code_tree.cpp
	$(CXX) $(CXXFLAGS) $(TEST_FLAGS) $^ -o $@

bin/test_tree_parse: tests/test_tree_parse.cpp bin/O2.o bin/cli.o bin/IR.o bin/parser.o 
	$(CXX) $(CXXFLAGS) $(TEST_FLAGS) $^ -o $@


#tools
bin/treemc: src/treemc.cpp  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o bin/O2.o bin/asm_O2.o bin/tree_asm.o bin/basic_fuse.o
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/tmc0: src/tmc0.c  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o
	$(CC) $(CFLAGS) $^ -o $@

bin/tmc1: src/tmc1.c  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o
	$(CC) $(CFLAGS) $^ -o $@


bin/tape_tool: src/tape_tool.c bin/io.o bin/cli.o bin/IR.o bin/parser.o bin/turing.o
	$(CC) $(CFLAGS) $^ -o $@

bin/run_turing: src/interpreter.c bin/io.o bin/cli.o bin/IR.o bin/parser.o bin/turing.o
	$(CC) $(CFLAGS) $^ -o $@


bin/tmc1_bad_hop: old_versions/tmc1_bad_jump
	cp old_versions/tmc1_bad_jump bin/tmc1_bad_hop

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


test: clean
	time make all -j
	python3 test.py

bench: check
	@cd code_tests/ && \
    ./test.sh

comp_test:
	make --assume-new=bin/compiler.o -j
	python3 -c "from test import run_comp_test;run_comp_test()"

.PHONY: all clean clean_io clean_turing test check comp_test bench all_tools all_tests
