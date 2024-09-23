#include "O2.hpp"

extern "C"{
	#include "compiler.h"
	#include "cli.h"
	#include  "IR.h"
	#include "utils.h"
	#include <libgen.h>
}

const char *casm = "nasm -g -f elf64 -o %s.o %s";

//#define FREE_FOR_DEBUG

struct CompData{
	TreeIR* tree;
	const char** names;

	//CompData(TreeIR* tree,const char** names) : tree(tree),names(names) {}
};

void O2_code(FILE *file,void* data){
	CompData* d=(CompData*)data;
	Tree_IR_to_ASM(file,std::move(*(d->tree)),d->names);
}



int main(int argc, char* argv[]){
	if(argc!=3){
		printf("usage: %s src_file out_file\n",argv[0]);
		return 1;
	}
	CompileStepOne comp = first_compile_step(argv[1]);
	TreeIR tree=make_inital_tree(comp.ir);
	// tree=basic_fuse(std::move(tree));
	// warn_unreachble(tree,comp.ir);

	tree=linear_fuse(std::move(tree));
	validate_tree(tree);

	CompData data={&tree,comp.ir.names};

	assemble_and_link(argv[2],dirname(argv[0]),&O2_code,casm,".asm",&data);
	
	//no need to free anything.
	#ifdef FREE_FOR_DEBUG
	free(comp.ir.names);
	free(comp.ir.states);
	free(comp.text);
	#endif
	
	return 0;
}
