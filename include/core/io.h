//this file contains all the syscalls we will need
#ifndef IO_H
#define IO_H

#include "turing.h"

#include <stddef.h>

#define PAGE_SIZE 4096

//this can be implemented the same everywhere regardless of compiler packing
typedef struct  __attribute__((packed)) {
    int cur_index;
    int left_index;
    int right_index;

    int left_limit;
    int right_limit;
} MetaData;

#if defined(__x86_64__) || defined(_M_X64)
    #define ABI_ATTRIBUTE __attribute__((sysv_abi))
#elif defined(__arm__) || defined(__aarch64__)
    #define ABI_ATTRIBUTE /* Add ARM-specific attributes if needed */
#else
    #define ABI_ATTRIBUTE
#endif

void* allocate_all_tape(size_t size) ABI_ATTRIBUTE;
void exit_turing(TuringDone code) ABI_ATTRIBUTE;

void DumpTape(const Tape* tape, const char *out_filename) ABI_ATTRIBUTE;
Tape ReadTape(const char *out_filename) ABI_ATTRIBUTE;

void free_all_tape(void* memory, size_t size) ABI_ATTRIBUTE;

#endif //IO_H
