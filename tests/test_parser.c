#include "parser.h"
#include "turing.h"
#include <stdlib.h>

// Function to create a new state and directly initialize transitions
void create_and_initialize_state(State *s, Bit write1, Dir move1, State* next1, Bit write2, Dir move2, State* next2) {
    s->transitions[0].write = write1;
    s->transitions[0].move = move1;
    s->transitions[0].NextState = next1;
    s->transitions[1].write = write2;
    s->transitions[1].move = move2;
    s->transitions[1].NextState = next2;
}

int main() {
    // Allocate space for the Turing machine states within the machine
    TuringMachine tm;
    tm.size = 2;
    tm.states = malloc(tm.size * sizeof(State));

    if (tm.states == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize states directly in the allocated array
    create_and_initialize_state(&tm.states[0], Bit_0, Left, &tm.states[1], Bit_1, Stay, &tm.states[1]);
    create_and_initialize_state(&tm.states[1], Bit_1, Right, NULL, Bit_0, Left, NULL);

    // Print the Turing machine
    print_machine(tm, 0);

    // Free memory
    free(tm.states);

    return 0;
}
