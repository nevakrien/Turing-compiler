#include "arm_O2.hpp"
#include "arm.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace ARM {

void write_asm_general(FILE *file, RegisterState &reg, const char** names, CodeTree::CodeNode* x) {
    switch(x->type()) {
        HANDLE_CASE(Split)
        HANDLE_CASE(Write)
        HANDLE_CASE(Move)
        HANDLE_CASE(StateStart)
        HANDLE_CASE(StateEnd)
        HANDLE_CASE(Exit)

        HANDLE_SCOPED_CASE(HistoryNode)
        HANDLE_SCOPED_CASE(LinearFuse)
        default:
            UNREACHABLE();
    }
}

void Tree_IR_to_ASM(FILE *file,TreeIR ir,const char** names){
	RegisterState reg=RegisterState(
        Register(R5),
        Register(R7),
		Register(R8),
		Register(R6),
		(BoundsDir){Register(R0),Register(R1)},
		(BoundsDir){Register(R3),Register(R4)}
	);

	reg.add_tmp(R13);//we need rsp for things
	
	fprintf(file,O0_assembly_start_template);

	load_tape_from_stack(file,reg);
	ASSERT(reg.tmp.size()==1);

	//TODO: add missing global functions

	for(auto i=0u;i<ir.size();i++){
		write_asm(file,reg,names,ir[i].get());
	}

	fprintf(file,"exit_good:\n");
	store_tape_to_stack(file,reg);

	fprintf(file,O0_assembly_end_template);
	//for deubg
	validate_tree(ir);
}

//assumes RAX RDI RCX are saved...
void unsafe_bounds_check_asm(FILE *file, RegisterState &reg, Register address, int move) {
    if(move==0){
    	return;
    }
    // Useful globals
    BoundsDir bounds = move > 0 ? reg.right : reg.left;
    const char* jump = (move > 0 ? "bls" : "bcs");
    const char* tape_jump = (move > 0 ? "bl" : "bc");
    int extend = ((abs(move) + EXTEND_STEPS - 1) / EXTEND_STEPS) * EXTEND_SIZE;
    //HERE WE HAVE X64

    TODO
}

// Function to save registers and handle collisions
void save_registers(FILE *file, RegisterState &reg, std::vector<Register> &registers_to_save, std::vector<Register> &temp_registers) {
    for (const auto &reg_needed : registers_to_save) {
        Register temp_reg;
        if (reg.contains(reg_needed)) {
            temp_reg = reg.add_tmp();
            fprintf(file, "%smov r%d, r%d ; we need r%d\n", _, temp_reg, reg_needed, reg_needed);
        } else {
            temp_reg = reg.add_tmp(reg_needed);
        }
        temp_registers.push_back(temp_reg);
    }
}

// Function to restore registers in reverse order
void restore_registers(FILE *file, const std::vector<Register> &registers_to_save, const std::vector<Register> &temp_registers) {
    for (int i = registers_to_save.size() - 1; i >= 0; --i) {
        if (temp_registers[i] != registers_to_save[i]) {
            fprintf(file, "%smov r%d, r%d ; restoring r%d\n", _, registers_to_save[i], temp_registers[i], registers_to_save[i]);
        }
    }
}

void load_tape_from_stack(FILE *file,RegisterState reg){
    TODO
}

void store_tape_to_stack(FILE *file,RegisterState reg){
    TODO
}

} // namespace ARM