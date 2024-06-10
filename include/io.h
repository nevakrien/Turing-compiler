// this file contains all the syscalls we will need
#ifndef IO_H
#define IO_H

#include "turing.h"

#include <stddef.h>

#define PAGE_SIZE 4096

// this can be implemented the same everywhere regardless of compiler packing
typedef struct __attribute__((packed)) {
    int cur_index;
    int left_index;
    int right_index;
    // int length;

    int left_limit;
    int right_limit;
} MetaData;

void* allocate_all_tape(size_t size) __attribute__((sysv_abi));
void exit_turing(TuringDone code) __attribute__((sysv_abi));

void DumpTape(Tape* tape, const char* out_filename) __attribute__((sysv_abi));
Tape ReadTape(const char* out_filename) __attribute__((sysv_abi));

void free_all_tape(void* memory, size_t size) __attribute__((sysv_abi));

#endif // IO_H
