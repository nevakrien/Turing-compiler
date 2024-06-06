#include "cli.h"
#include <stdio.h>

// Helper function to print metadata of the tape
void PrintMetadata(const Tape tape){
    // Using bold and cyan color for the header
    printf("\033[1;36mfull tape [%d,%d] initialized tape [%d,%d]\033[0m\n", tape.left_limit, tape.right_limit, tape.left_init, tape.right_init);
}

void PrintTape(const Tape tape){
    const int cur_id = tape.cur - tape.base;

    PrintMetadata(tape);  // Call the helper function to print the metadata

    // Using bold and cyan color for current id and initialized data
    printf("\033[1;36mcurrent id:%d initialized data:\n\033[0m", cur_id);
    printf("\033[35m[\033[0m"); // Start purple color for '[' and reset immediately

    for (int i = tape.left_init; i <= tape.right_limit; i++) {
        if (i == cur_id) {
            printf("\033[32m["); // Start green color for '['
        } else {
            printf("\033[33m"); // Start yellow color for non-key ids
        }

        if (tape.base[i]) putchar('1');
        else putchar('0');

        if (i == cur_id) {
            printf("]\033[0m"); // End green color for ']'
        } else {
            printf("\033[0m"); // Reset color after each non-key id
        }
    }

    printf("\033[35m]\033[0m\n"); // End purple color for ']'
}
