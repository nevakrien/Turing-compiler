//this file contains all the syscalls we will need
#ifndef IO_H
#define IO_H

#include "turing.h"

#include <stddef.h>

#define PAGE_SIZE 4096


void* allocate_all_tape(size_t size) __attribute__((sysv_abi));
void exit_turing(TuringDone code,int current_step) __attribute__((sysv_abi));

void DumpTape(Tape* tape, const char *out_filename)__attribute__((sysv_abi));
Tape ReadTape(const char *out_filename)__attribute__((sysv_abi));

#endif //IO_H