#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h" 
//#include "cli.h"//this is there for a print function if we need it
//but since this modules main use is the compiled

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
    Bit *start=(Bit *)allocate_all_tape(TestTapeBitCount * sizeof(Bit));
    

    // Initialize the tape with some data
    for (int i = 0; i < TestTapeBitCount; i++) {
        start[i] = (i % 2 == 0) ? Bit_0 : Bit_1;
    }

    Tape tape;
    tape.cur = start + (TestTapeBitCount / 2); // Set current position to the middle
    tape.base=tape.cur;

    tape.left_init = -TestTapeBitCount / 2;
    tape.right_init = +(TestTapeBitCount /2 -1);

    tape.left_limit = tape.left_init;
    tape.right_limit = tape.right_init;

    //PrintTape(tape);
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
