#include "parser.h"
#include "turing.h"

// Function to print the details of a transition with specified indentation
void print_trans(TuringMachine machine,TRANSITION trans, int indent) {
    const char *dir_str;
    switch (trans.move) {
        case Right:
            dir_str = "R";
            break;
        case Left:
            dir_str = "L";
            break;
        case Stay:
            dir_str = "S";
            break;
        default:
            dir_str = "?";
    }

    for (int i = 0; i < indent; i++) {
        printf(" ");
    }

    int next_state_id;
    if(trans.NextState==NULL){next_state_id=-1;}
    else{next_state_id=trans.NextState -machine.states;}

    printf("Write: %d, Move: %s, Next State: %d\n", trans.write, dir_str, next_state_id);
}

// Function to print the states and their transitions with specified indentation
void print_state(TuringMachine machine,State s, int indent) {
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }
    printf("State:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < indent; j++) {
            printf(" ");
        }
        printf("  Transition %d:\n", i + 1);
        print_trans(machine,s.transitions[i], indent + 4);
    }
}

// Function to print the entire Turing machine's states and their details with specified indentation
void print_machine(TuringMachine machine, int indent) {
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }
    printf("Turing Machine Size: %d\n", machine.size);
    for (int i = 0; i < machine.size; i++) {
        print_state(machine,machine.states[i], indent + 2);
    }
}

