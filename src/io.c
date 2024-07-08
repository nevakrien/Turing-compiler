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
        perror("Memory allocation failed [allocate_all_tape]\n");
        printf("size=%ld\n",size);
        exit(EXIT_FAILURE);
    }
    return memory;
}



void __attribute__((sysv_abi)) free_all_tape(void* memory, size_t size) {
    #ifdef _WIN32
        // Windows-specific memory deallocation using VirtualFree
        if (!VirtualFree(memory, 0, MEM_RELEASE)) {
            perror("Memory deallocation failed [free_all_tape]\n");
            exit(EXIT_FAILURE);
        }
    #else
        // Unix-like systems memory deallocation using munmap
        if (munmap(memory, size) == -1) {
            perror("Memory deallocation failed [free_all_tape]\n");
            exit(EXIT_FAILURE);
        }
    #endif
}


void __attribute__((sysv_abi)) exit_turing(TuringDone code){
	if(code==HALT){
		printf("halted\n");
		exit(code);
	}

	printf("didnt halt");
	switch(code){
		case OUT_OF_TAPE:
			printf("  ERROR: out of tape");
			break;
		case TIME_OUT:
			printf("  ERROR: out of tape");
			break;
		case BAD_ARGS:
            printf("  ERROR: bad args");
            break;
		default:
			printf(" unknowen error code: %d!!!",code);
			break;
	}
    putchar('\n');
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


//#define HACKY_TEST
#ifdef HACKY_TEST
MetaData m;
static void DEBUG_METADATA_WRITE(MetaData x){
	m=x;
}
static void DEBUG_METADATA_READ(MetaData x){
	assert(memcmp(&x, &m, sizeof(MetaData)) == 0);
}
#else
#define DEBUG_METADATA_READ(m)
#define DEBUG_METADATA_WRITE(m)
#endif

void __attribute__((sysv_abi)) DumpTape(const Tape* tape, const char *out_filename) {
    if (!tape || !tape->base) {
        fprintf(stderr, "Invalid tape or base pointer\n");
        exit(EXIT_FAILURE);
    }

    int index = tape->cur - tape->base;
    int length = tape->right_init - tape->left_init+ 1;
    MetaData metadata = {index, tape->left_init, tape->right_init,// length,
    tape->left_limit,tape->right_limit};

    DEBUG_METADATA_WRITE(metadata);


    uint8_t *buffer = calloc((length + 7) / 8, sizeof(uint8_t));
    if (!buffer) {
        perror("Allocation failed [DumpTape]");
        exit(EXIT_FAILURE);
    }
    pack_bits(buffer, &tape->base[tape->left_init], length);

    FILE *out_file = fopen(out_filename, "wb");
    if (!out_file) {
        perror("Failed to open output file");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    if (fwrite(&metadata, sizeof(metadata), 1, out_file) != 1 ||
        fwrite(buffer, (length + 7) / 8, 1, out_file) != 1) {
        perror("Failed to write to file");
        fclose(out_file);
        free(buffer);
        exit(EXIT_FAILURE);
    }

    fclose(out_file);
    free(buffer);
}

Tape __attribute__((sysv_abi)) ReadTape(const char *in_filename) {
    Tape tape;
    memset(&tape, 0, sizeof(tape));
    MetaData metadata;

    FILE *in_file = fopen(in_filename, "rb");
    if (!in_file) {
        fprintf(stderr, "Failed to open input file\n");
        exit(EXIT_FAILURE);
    }

    if (fread(&metadata, sizeof(metadata), 1, in_file) != 1) {
        fprintf(stderr, "Failed to read full metadata\n");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    DEBUG_METADATA_READ(metadata);
    int length=metadata.right_index-metadata.left_index+1;


    uint8_t *buffer = calloc((length + 7) / 8, sizeof(uint8_t));
    if (!buffer) {
        perror("Failed to allocate memory for tape data");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    if (fread(buffer, (length + 7) / 8, 1, in_file) != 1) {
        fprintf(stderr, "Failed to read full tape data\n");
        free(buffer);
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    fclose(in_file);

    int total_tape_length=metadata.right_limit-metadata.left_limit+1;
    
    Bit *start=allocate_all_tape(sizeof(Bit)*total_tape_length);
    Bit *base=start-metadata.left_limit;
    Bit *tape_data=&base[metadata.left_index];
    
    if(&base[metadata.right_index]-&base[metadata.left_index]+1!=length){
    	UNREACHABLE();
    }

    memset(tape_data, 0,length); // Initialize the relevant section of the tape
	unpack_bits(buffer, tape_data, length); // Unpack the buffer into the initialized section

    

    // Bit *tape_data = calloc(metadata.length, sizeof(Bit));
    // if (!tape_data) {
    //     perror("Failed to allocate memory for unpacked tape data");
    //     free(buffer);
    //     exit(EXIT_FAILURE);
    // }

    //unpack_bits(buffer, tape_data, total_tape_length);
    free(buffer);

    tape.base = base;
    tape.cur = tape.base + metadata.cur_index;
    tape.left_init = metadata.left_index;
    tape.right_init = metadata.right_index;
    
    // tape.left_limit = metadata.left_index;
    // tape.right_limit = metadata.right_index	;
    tape.left_limit = metadata.left_limit;
    tape.right_limit = metadata.right_limit;

    return tape;
}