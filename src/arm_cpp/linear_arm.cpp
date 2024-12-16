#include "O2.hpp"
#include "arm.hpp"

#include <iostream>
#include <vector>

namespace ARM {

void write_asm(FILE *file, RegisterState &reg, const char** names, HistoryNode* x){
	write_asm_general(file,reg,names,x->next.get());
}

void write_asm(FILE *file, RegisterState &reg, const char** names, LinearFuse* x){	
	TODO
}

}//ARM