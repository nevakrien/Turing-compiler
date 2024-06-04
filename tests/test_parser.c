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

// Helper function to print tokens for debugging
void print_tokens(CodeLines code) {
    printf("Number of lines: %d\n", code.len);
    for (int i = 0; i < code.len; i++) {
        TokenNode* current = code.lines[i];
        printf("Line %d tokens:\n", i+1);
        while (current) {
            printf("'%.*s'\n", current->tok.len, current->tok.data);
            current = current->next;
        }
    }
}

// Test functions
void test_empty_input() {
    CodeLines result = tokenize_text("");
    assert(result.len == 0);
    printf("Test Empty Input: Passed\n");
}

void test_whitespace_only() {
    CodeLines result = tokenize_text("    \t   ");
    //printf("white space only: %d\n",result.len);
    
    assert(result.len == 0);
    printf("Test Whitespace Only: Passed\n");
}

void test_single_line_input() {
    CodeLines result = tokenize_text("hello");
    assert(result.len == 1);
    assert(result.lines[0]->tok.len == 5);
    assert(strncmp(result.lines[0]->tok.data, "hello", 5) == 0);
    printf("Test Single Line Input: Passed\n");
}

void test_multiple_lines() {
    CodeLines result = tokenize_text("hello world\nsecond line\n   third line");
    assert(result.len == 3);
    assert(strncmp(result.lines[0]->tok.data, "hello", 5) == 0);
    assert(strncmp(result.lines[1]->tok.data, "second", 6) == 0);
    assert(strncmp(result.lines[2]->tok.data, "third", 5) == 0);
    printf("Test Multiple Lines: Passed\n");
}

void test_lines_with_only_whitespace() {
    CodeLines result = tokenize_text(" \n\t\n\n   \n");
    assert(result.len == 0);
    printf("Test Lines With Only Whitespace: Passed\n");
}

void test_tokenizer(){
    test_empty_input();
    test_whitespace_only();
    test_single_line_input();
    test_multiple_lines();
    test_lines_with_only_whitespace();
}

int main(){
    test_prints();
    test_tokenizer();
    

    return 0;
}