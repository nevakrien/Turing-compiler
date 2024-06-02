#include "io.h"
#include "utils.h"

//platform dependent bullshit
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


void* __attribute__((sysv_abi)) allocate_all_tape(size_t size) {
	#ifdef _WIN32
	    // Windows-specific memory allocation using VirtualAlloc
	    void* memory = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	    if (memory == NULL) 
	#else
	    // Unix-like systems memory allocation using mmap
	    void* memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	    if (memory == MAP_FAILED) 
	#endif
    {
        perror("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return memory;
}




void __attribute__((sysv_abi)) exit_turing(TuringDone code,int current_step){
	if(code==HAULT){
		printf("haulted at step: %d\n",current_step);
		return;
	}

	printf("didnt hault current step: %d",current_step);
	switch(code){
		case OUT_OF_TAPE:
			printf("  ERROR: out of tape");
			break;
		
		default:
			printf(" unknowen error code: %d!!!",code);
			break;
	}

	printf(" current step: %d\n",current_step);

	exit(code);
}

//read write buffer
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#endif


// Helper function to pack bits into bytes
static void pack_bits(uint8_t *dest, const Bit *source, int count) {
    for (int i = 0; i < count; ++i) {
        if (source[i] == Bit_1) {
            dest[i / 8] |= (1 << (i % 8));
        }
    }
}

// Helper function to unpack bits from bytes
static void unpack_bits(const uint8_t *source, Bit *dest, int count) {
    for (int i = 0; i < count; ++i) {
        dest[i] = (source[i / 8] & (1 << (i % 8))) ? Bit_1 : Bit_0;
    }
}

void __attribute__((sysv_abi)) DumpTape(Tape* tape, int out_fd) {
    if (!tape || !tape->base) {
        fprintf(stderr, "Invalid tape or base pointer\n");
        exit(EXIT_FAILURE);
    }

    int index = tape->cur - tape->base;
    int length = tape->right_init - tape->left_init + 1;

    // Allocate and zero-initialize the buffer for writing
    uint8_t *buffer = calloc((length + 7) / 8, sizeof(uint8_t));
    if (!buffer) {
        perror("Allocation failed");
        exit(EXIT_FAILURE);
    }
    pack_bits(buffer, &tape->base[tape->left_init], length);

#ifdef _WIN32
    DWORD written;
    if (!WriteFile((HANDLE)_get_osfhandle(out_fd), &index, sizeof(index), &written, NULL) ||
        written != sizeof(index) ||
        !WriteFile((HANDLE)_get_osfhandle(out_fd), buffer, (length + 7) / 8, &written, NULL) ||
        written != (DWORD)((length + 7) / 8)) 
#else
    if (write(out_fd, &index, sizeof(index)) != sizeof(index) ||
        write(out_fd, buffer, (length + 7) / 8) != (length + 7) / 8) 
#endif
    {
        perror("Failed to write to file");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    free(buffer);
}




Tape __attribute__((sysv_abi)) ReadTape(int in_fd) {
    // Initialize Tape struct
    Tape tape;
    memset(&tape, 0, sizeof(tape));

    // Read the metadata
    struct {
        int cur_index;
        int left_index;
        int right_index;
        int length;
    } metadata;

#ifdef _WIN32
    DWORD read_bytes;
    ReadFile((HANDLE)_get_osfhandle(in_fd), &metadata, sizeof(metadata), &read_bytes, NULL);
    if (read_bytes != sizeof(metadata))
#else
    if (read(in_fd, &metadata, sizeof(metadata)) != sizeof(metadata))
#endif
    {
        fprintf(stderr, "Failed to read full metadata\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the tape array based on the metadata
    Bit *buffer = calloc(metadata.length, sizeof(Bit));
    if (!buffer) {
        perror("Failed to allocate memory for tape data");
        exit(EXIT_FAILURE);
    }

    // Read the packed bit data
#ifdef _WIN32
    ReadFile((HANDLE)_get_osfhandle(in_fd), buffer, (metadata.length + 7) / 8, &read_bytes, NULL);
    if (read_bytes != (DWORD)((metadata.length + 7) / 8))
#else
    if (read(in_fd, buffer, (metadata.length + 7) / 8) != (metadata.length + 7) / 8)
#endif
    {
        fprintf(stderr, "Failed to read full tape data\n");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // Unpack the bit data into the tape array
    Bit *tape_data = calloc(metadata.length, sizeof(Bit));
    if (!tape_data) {
        perror("Failed to allocate memory for unpacked tape data");
        free(buffer);
        exit(EXIT_FAILURE);
    }
    unpack_bits(buffer, tape_data, metadata.length);
    free(buffer);

    // Set the remaining tape properties
    tape.base = tape_data;
    tape.cur = tape.base + metadata.cur_index;
    tape.left_init = metadata.left_index;
    tape.right_init = metadata.right_index;
    tape.min = metadata.left_index;  // Assuming min is the same as left_init
    tape.max = metadata.right_index; // Assuming max is the same as right_init

    return tape;
}
