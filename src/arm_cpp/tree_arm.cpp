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

void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Write* x){
	TODO
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