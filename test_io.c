#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h" // Assume this header file contains all necessary declarations

//#define PAGE_SIZE 4096
#define TestTapeBitCount 4096

//this is autogen code and it shows. 
//still went over it manualy and even the assembly dump its fine
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        return 1;
    }

    const char *output_filename = argv[1];

    // Create and initialize a Tape
    Tape tape;
    tape.base = (Bit *)allocate_all_tape(TestTapeBitCount * sizeof(Bit)); // Assuming allocate_all_tape handles memory allocation

    // Initialize the tape with some data
    for (int i = 0; i < TestTapeBitCount; i++) {
        tape.base[i] = (i % 2 == 0) ? Bit_0 : Bit_1;
    }
    tape.cur = tape.base + (TestTapeBitCount / 2); // Set current position to the middle
    tape.left_init = 0;
    tape.right_init = TestTapeBitCount - 1;

    tape.left_limit = 0;
    tape.right_limit = TestTapeBitCount - 1;

    // Dump tape to file
    DumpTape(&tape, output_filename);

    // Read tape from file
    Tape read_tape = ReadTape(output_filename);
    //printf("left_init: %d\n",read_tape.left_init);

    // Check if the data read matches the data written
    int data_match = 1;
    for (int i = tape.left_init; i <= tape.right_init; i++) {
        if (tape.base[i] != read_tape.base[i]) {
            printf("Data verification failed. at index%d\n",i);
            exit(1);
            data_match = 0;

            break;
        }
    }

    if (data_match) {
        printf("Data verification successful.\n");
    } else {
        printf("Data verification failed.\n");
    }

    // No explicit free for tape.base as per user instructions
    return (data_match) ? 0 : 1; // Return 0 if successful, 1 if failed
}
