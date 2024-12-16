#include "arm_O2.hpp"
#include "arm.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace ARM {

void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Exit* x){
	switch(x->code){
		case HALT:
			fprintf(file,"%sb exit_good\n\n",_);
			break;
		case TIME_OUT:
			fprintf(file,"%sb exit_time_out\n\n",_);
			break;
		case OUT_OF_TAPE:
			fprintf(file,"%sb exit_out_of_tape\n\n",_);
			break;
		default:
			UNREACHABLE();
	}
	
}

void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::StateStart* x){
	reg.update_state(x->StateID);
	fprintf(file,"L%d:",reg.cur_state);
	if(names){
		fprintf(file," //%s",names[reg.cur_state]);
	}
	fprintf(file,"\n");
	write_asm_general(file,reg,names,x->next.get());
}

void write_asm(FILE *file, RegisterState &reg, const char** names, CodeTree::Write* x) {
    // Handle cases based on the TapeVal type
    switch (x->val) {
        case TapeVal::Unchanged:
            // Do nothing
            break;

        case TapeVal::Allways1:
            // Write the value 1 to the tape at the current address
            fprintf(file, "str r%d, [r%d] // Write 1 to memory\n", reg.one, reg.address);
            break;

        case TapeVal::Allways0:
            // Write the value 0 to the tape at the current address
            fprintf(file, "str r%d, [r%d] // Write 0 to memory\n", reg.zero, reg.address);
            break;

        case TapeVal::Flip:
            // Read the value at the current address into the `read` register
            fprintf(file, "ldr r%d, [r%d] // Read value from memory\n", reg.read, reg.address);

            // XOR the value with 1 (flip operation)
            fprintf(file, "eor r%d, r%d, #1 // Flip value\n", reg.read, reg.read);

            // Write the flipped value back to the memory
            fprintf(file, "str r%d, [r%d] // Write flipped value back to memory\n", reg.read, reg.address);
            break;

        default:
            UNREACHABLE(); // This should never happen
    }

    // Handle the next part of the `CodeTree`
    if (x->next) {
        // Safe jump to the next state: Use a long jump if needed
        fprintf(file, "// Jumping to the next state\n");
        write_asm_general(file, reg, names, x->next.get());
    }
}


void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::StateEnd* x){
	int next_id=x->next->StateID;
	fprintf(file,"%sb L%d\n",_,next_id);
}

void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Split* x){
	TODO

}

void write_asm(FILE *file, RegisterState &reg, const char** names, CodeTree::Move* x) {
    TODO
}

} //ARM