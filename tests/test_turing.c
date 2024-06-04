#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "turing.h"
#include "utils.h"

// Test for continuous right movement and bit flipping, including left initialization
void test_infinite_right_flip() {
    // Create and initialize a Tape
    int test_tape_size = 1024;  // Initial tape size
    Bit *tape_memory = calloc(test_tape_size, sizeof(Bit)); // Allocate and zero-initialize the tape
    if (tape_memory == NULL) {
        printf("Memory allocation failed for tape_memory.\n");
        exit(1);
    }

    //const int offset=10;
    const int offset=0;
    Tape tape;
    tape.base = tape_memory;
    tape.cur = tape_memory + (test_tape_size / 2)-offset; // Start in the middle of the tape
    tape.left_limit = 0;
    tape.right_limit = test_tape_size - 1;
    tape.left_init = tape.left_limit + (test_tape_size / 2)-offset; // Initialize left half
    tape.right_init = tape.cur-tape.base; // Initialize right part

    //int start_id=tape.cur-tape.base;

    // Setup a simple Turing machine
    TuringMachine machine;
    machine.size = 1;
    machine.states = malloc(sizeof(State));
    if (machine.states == NULL) {
        printf("Memory allocation failed for machine.states.\n");
        free(tape_memory);
        exit(1);
    }

    // State that flips the bit and moves right indefinitely
    machine.states[0].transitions[Bit_0] = (TRANSITION){Bit_1, Right, &machine.states[0]};
    machine.states[0].transitions[Bit_1] = (TRANSITION){Bit_0, Right, &machine.states[0]};

    // Run the Turing machine
    int max_steps = 2000; // Force many steps to potentially expand tape
    TuringResult result = run_turing(&tape, machine, max_steps);

    // Check conditions after simulation
    if (result.code != OUT_OF_TAPE) {
        printf("Expected OUT_OF_TAPE, but got different result code. %d\n",result.code);
        exit(1);
    }
    if (tape.cur <= tape.base + tape.right_init) {
        printf("Cursor did not move beyond the initial right limit as expected.\n");
        exit(1);
    }
    for (int i = tape.left_init; i <= tape.right_init; i++) {
        if (tape.base[i] != Bit_1) {
            printf("Bit at index %d was not flipped to 1 as expected.\n", i);
            exit(1);
        }
    }
    for (int i = 0; i < tape.left_init; i++) {
        if (tape.base[i] != Bit_0) {
            printf("Unexpected initialization of left side bits. %d\n",i);
            exit(1);
        }
    }

    // Clean up
    free(tape.base);
    free(machine.states);
    printf("Right flip test passed!\n");
}

// Test for stationary Turing machine
void test_stationary_turing_machine() {
    int tape_size = 256;
    Bit *tape_memory = calloc(tape_size, sizeof(Bit));
    if (tape_memory == NULL) {
        printf("Memory allocation failed for tape_memory.\n");
        exit(1);
    }

    Tape tape;
    tape.base = tape_memory;
    tape.cur = tape_memory + (tape_size / 2);
    tape.left_limit = 0;
    tape.right_limit = tape_size - 1;
    tape.left_init = 0;
    tape.right_init = tape_size - 1;

    TuringMachine machine;
    machine.size = 1;
    machine.states = malloc(sizeof(State));
    if (machine.states == NULL) {
        printf("Memory allocation failed for machine.states.\n");
        free(tape_memory);
        exit(1);
    }

    // State that writes the same bit and stays in place
    machine.states[0].transitions[Bit_0] = (TRANSITION){Bit_0, Stay, &machine.states[0]};
    machine.states[0].transitions[Bit_1] = (TRANSITION){Bit_1, Stay, &machine.states[0]};

    // Run the Turing machine with enough steps to trigger a time-out
    int max_steps = 1000;
    TuringResult result = run_turing(&tape, machine, max_steps);

    if (result.code != TIME_OUT) {
        printf("Expected TIME_OUT, but got different result code.\n");
        exit(1);
    }

    free(tape.base);
    free(machine.states);
    printf("Stationary machine test passed!\n");
}



// Test for an N-state Turing machine
void test_n_state_left_movement() {
    int test_tape_size = 1024;  // Initial tape size
    Bit *tape_memory = calloc(test_tape_size, sizeof(Bit)); // Allocate and zero-initialize the tape
    if (tape_memory == NULL) {
        printf("Memory allocation failed for tape_memory.\n");
        exit(1);
    }

    const int offset = 10; // Start offset to allow space for left movement
    if (offset < 0 || offset >= test_tape_size / 2) {
        printf("Offset out of valid range.\n");
        free(tape_memory);
        UNREACHABLE();
    }

    Tape tape;
    tape.base = tape_memory;
    tape.cur = tape_memory + (test_tape_size / 2) + offset; // Start in the middle of the tape
    tape.left_limit = 0;
    tape.right_limit = test_tape_size - 1;
    tape.left_init = tape.left_limit + (test_tape_size / 2) + offset; // Initialize left half
    tape.right_init = tape.cur - tape.base; // Initialize right part

    int start_index = tape.cur - tape.base;
    
    if (tape.left_init < 0 || tape.left_init >= test_tape_size) {
        printf("Left initialization out of tape bounds.\n");
        free(tape_memory);
        UNREACHABLE();
    }
    if (tape.right_init < 0 || tape.right_init >= test_tape_size) {
        printf("Right initialization out of tape bounds.\n");
        free(tape_memory);
        UNREACHABLE();
    }

    int num_states = 10; // Define the number of states
    TuringMachine machine;
    machine.size = num_states;
    machine.states = malloc(sizeof(State) * num_states);
    if (machine.states == NULL) {
        printf("Memory allocation failed for machine.states.\n");
        free(tape_memory);
        exit(1);
    }

    // Set up states to write 1, move left, and transition to the next state
    for (int i = 0; i < num_states - 1; i++) {
        machine.states[i].transitions[Bit_0] = (TRANSITION){Bit_1, Left, &machine.states[i + 1]};
        machine.states[i].transitions[Bit_1] = (TRANSITION){Bit_1, Left, &machine.states[i + 1]};
    }
    // Last state transitions to NULL (halt)
    machine.states[num_states - 1].transitions[Bit_0] = (TRANSITION){Bit_1, Left, NULL};
    machine.states[num_states - 1].transitions[Bit_1] = (TRANSITION){Bit_1, Left, NULL};

    // Run the Turing machine
    TuringResult result = run_turing(&tape, machine, num_states*10); // Arbitrary high number of steps

    // Check conditions after simulation
    if (result.code != HAULT) {
        printf("Expected HAULT, but got different result code: %d\n", result.code);
        exit(1);
    }
    
    for (int i = start_index; i <= tape.right_init; i++) {
        if (tape.base[i] != Bit_1) {
            printf("Bit at index %d was not written as 1 as expected.\n", i);
            exit(1);
        }
    }

    // Clean up
    free(tape.base);
    free(machine.states);
    printf("N-state left movement test passed!\n");
}


int main() {
    test_stationary_turing_machine();
    test_infinite_right_flip();
    test_n_state_left_movement();

    return 0;
}
