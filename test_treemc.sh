#/bin/bash
make check 
./bin/treemc code_tests/tasks/made_bad/code.t gen
valgrind ./gen.out code_tests/tasks/made_bad/input.tape debug.tape