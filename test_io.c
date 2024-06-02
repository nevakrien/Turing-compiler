#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "io.h" // Assume this header file contains all necessary declarations

#define PAGE_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        return 1;
    }

    const char *output_filename = argv[1];

    // Create and initialize a Tape
    Tape tape;
    tape.base = (Bit *)allocate_all_tape(PAGE_SIZE * sizeof(Bit)); // Assuming allocate_all_tape handles memory allocation

    // Initialize the tape with some data
    for (int i = 0; i < PAGE_SIZE; i++) {
        tape.base[i] = (i % 2 == 0) ? Bit_0 : Bit_1;
    }
    tape.cur = tape.base + (PAGE_SIZE / 2); // Set current position to the middle
    tape.left_init = 0;
    tape.right_init = PAGE_SIZE - 1;

    // Open file to dump the tape
    int out_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd == -1) {
        perror("Failed to open file for writing");
        free(tape.base); // Free allocated tape
        return 1;
    }

    // Dump tape to file
    DumpTape(&tape, out_fd);
    close(out_fd);

    // Open file to read the tape
    int in_fd = open(output_filename, O_RDONLY);
    if (in_fd == -1) {
        perror("Failed to open file for reading");
        free(tape.base); // Free allocated tape
        return 1;
    }

    // Read tape from file
    Tape read_tape = ReadTape(in_fd);
    close(in_fd);

    // Check if the data read matches the data written
    int data_match = 1;
    for (int i = tape.left_init; i <= tape.right_init; i++) {
        if (tape.base[i] != read_tape.base[i]) {
            data_match = 0;
            break;
        }
    }

    if (data_match) {
        printf("Data verification successful.\n");
    } else {
        printf("Data verification failed.\n");
    }

    return (data_match) ? 0 : 1; // Return 0 if successful, 1 if failed
}
