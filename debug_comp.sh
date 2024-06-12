#!/bin/bash
make test_compiler 

file_path="code_tests/tasks/42/"
gdb -x debug.gdb --args ./"${file_path}tmc0.out" "${file_path}input.tape" "output.tape"
