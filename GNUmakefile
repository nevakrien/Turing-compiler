CC = $(shell which gcc-14 || echo gcc) #this is to get the best preformance
CXX =$(shell which g++-14 || echo g++) #clang fails... but nicer error messages

ARM_CC = arm-linux-gnueabihf-gcc -march=armv7-a #note that this is assumed to exist by the compiler later


# Compiler flags
BASE_FLAGS= -march=native -Wall -g2 -Iinclude/core -Wno-attributes
CFLAGS = $(BASE_FLAGS) -std=gnu99
CXXFLAGS= $(BASE_FLAGS) -Iinclude/cpp -std=c++17 

ARM_FLAGS = -Iinclude/core -std=gnu99 -Wno-attributes



TEST_FLAGS= #-fsanitize=address -fsanitize=undefined

##dirs:
#include
#src
#tests
#bin

# Default target
all: all_tools all_tests all_io bin/compiler.o bin/O2.o  #bin/tmc1_bad_hop#bin/libio.so
	@echo "Compiler used: $(CC)"

all_io: bin/io.o bin/arm_io.o
all_tools: bin/tape_tool bin/run_turing bin/tmc1 bin/tmc0 bin/treemc bin/tmc2 bin/arm bin/arm_treemc
all_tests: bin/test_io bin/test_turing bin/test_parser bin/test_compiler bin/test_code_tree bin/test_tree_parse

# Compile source files to object files
bin/io.o: src/core/io.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -static -c $< -o $@

# Compile source files to object files
bin/arm_io.o: src/core/io.c
	@mkdir -p bin
	arm-linux-gnueabihf-gcc -static -c -o $@ $< $(ARM_FLAGS)

bin/cli.o: src/core/cli.c  
	@mkdir -p bin
	$(CC) $(CFLAGS) $<  -c -o $@

bin/turing.o: src/core/turing.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/IR.o: src/core/IR.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/parser.o: src/core/parser.c #bin/turing.o
	$(CC) $(CFLAGS) -c $^ -o $@

bin/compiler.o: src/core/compiler.c
	$(CC) $(CFLAGS) -c $^ -o $@

bin/O2.o: src/cpp/O2.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/asm_O2.o: src/cpp/asm_O2.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/tree_asm.o: src/cpp/tree_asm.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/linear_asm.o: src/cpp/linear_asm.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/basic_fuse.o: src/cpp/basic_fuse.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/linear_fuse.o: src/cpp/linear_fuse.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/history_maps.o: src/cpp/history_maps.cpp
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
bin/treemc: src/tools/treemc.cpp  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o bin/O2.o bin/asm_O2.o bin/tree_asm.o bin/linear_asm.o bin/basic_fuse.o bin/history_maps.o
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/tmc2: src/tools/tmc2.cpp  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o bin/O2.o bin/asm_O2.o bin/tree_asm.o bin/linear_asm.o bin/basic_fuse.o bin/linear_fuse.o bin/history_maps.o
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/arm: src/tools/arm.c  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o
	$(CC) $(CFLAGS) $^ -o $@

bin/tmc0: src/tools/tmc0.c  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o
	$(CC) $(CFLAGS) $^ -o $@

bin/tmc1: src/tools/tmc1.c  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o
	$(CC) $(CFLAGS) $^ -o $@


bin/tape_tool: src/tools/tape_tool.c bin/io.o bin/cli.o bin/IR.o bin/parser.o bin/turing.o
	$(CC) $(CFLAGS) $^ -o $@

bin/run_turing: src/tools/interpreter.c bin/io.o bin/cli.o bin/IR.o bin/parser.o bin/turing.o
	$(CC) $(CFLAGS) $^ -o $@


bin/tmc1_bad_hop: old_versions/tmc1_bad_jump
	cp old_versions/tmc1_bad_jump bin/tmc1_bad_hop

#arm specific
bin/arm_O2.o: src/arm_cpp/arm_O2.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/linear_arm.o: src/arm_cpp/linear_arm.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/tree_arm.o: src/arm_cpp/tree_arm.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

bin/arm_treemc: src/tools/arm_treemc.cpp  bin/cli.o bin/IR.o bin/parser.o bin/compiler.o bin/O2.o bin/arm_O2.o bin/tree_arm.o bin/linear_arm.o bin/basic_fuse.o bin/history_maps.o
	$(CXX) $(CXXFLAGS) $^ -o $@


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
