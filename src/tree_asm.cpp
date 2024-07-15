#include "O2.hpp"
#include "asm.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

void write_genral(FILE *file, RegisterState &reg, const char** names, CodeTree::CodeNode* x) {
    switch(x->type()) {
        HANDLE_CASE(Split)
        HANDLE_CASE(Write)
        HANDLE_CASE(Move)
        HANDLE_CASE(StateStart)
        HANDLE_CASE(StateEnd)
        HANDLE_CASE(Exit)
        default:
            UNREACHABLE();
    }
}

void Tree_IR_to_ASM(FILE *file,TreeIR ir,const char** names){
	RegisterState reg=RegisterState(
		Register(R14),
		Register(R15),
		(BoundsDir){Register(R11),Register(R9)},
		(BoundsDir){Register(R10),Register(R8)}
	);

	reg.add_tmp(RSP);//we need rsp for things
	
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
}