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

    for (int i = tape.left_init; i <= tape.right_init; i++) {
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

CompileStepOne first_compile_step(const char* filename){
	size_t length;
	char* raw_text=read_file_into_buffer(filename,&length);
	if(raw_text==NULL){
		return(CompileStepOne){0};
	}
	TuringMachineEncoding encoding = parse_text_with_prints(raw_text);
	if(encoding.trans==NULL){
		free(raw_text);
		return(CompileStepOne){0};
	}

	TuringIR ir= make_initial_ir (encoding);
	if(ir.states==NULL){
		free(raw_text);
		free(encoding.trans);
		return(CompileStepOne){0};
	}
	free(encoding.trans);
	return (CompileStepOne){ir,raw_text};
}
