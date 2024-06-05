#include "parser.h"
#include "turing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Function to create a new state and directly initialize transitions
void create_and_initialize_state(State *s, Bit write1, Dir move1, State* next1, Bit write2, Dir move2, State* next2) {
    s->transitions[0].write = write1;
    s->transitions[0].move = move1;
    s->transitions[0].NextState = next1;
    s->transitions[1].write = write2;
    s->transitions[1].move = move2;
    s->transitions[1].NextState = next2;
}

void test_prints() {
    // Allocate space for the Turing machine states within the machine
    TuringMachine tm;
    tm.size = 2;
    tm.states = malloc(tm.size * sizeof(State));

    if (tm.states == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Initialize states directly in the allocated array
    create_and_initialize_state(&tm.states[0], Bit_0, Left, &tm.states[1], Bit_1, Stay, &tm.states[1]);
    create_and_initialize_state(&tm.states[1], Bit_1, Right, NULL, Bit_0, Left, NULL);

    // Print the Turing machine
    print_machine(tm, 0);
    fflush(stdout);

    // Free memory
    free(tm.states);
}

void test_parse_text_with_prints(char* filename){
    printf("got file name:%s\n",filename);
    size_t len;
    char* text=read_file_into_buffer(filename,&len);
    TuringMachineEncoding ans=parse_text_with_prints(text);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // If the filename is not provided, output the correct usage
        fprintf(stderr, "Usage: %s <turing.t>\n", argv[0]);
        return 1; // Exit with a non-zero value to indicate an error
    }

    test_prints();
    test_parse_text_with_prints(argv[1]);
    

    return 0;
}