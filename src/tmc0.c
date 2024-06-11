#include "cli.h"
#include  "IR.h"
#include "utils.h"
#include "compiler.h"

//#define FREE_FOR_DEBUG

void O0_code(FILE *file,void* data){
	TuringIR* ir=(TuringIR*)data;
	O0_IR_to_ASM(file,*ir);
}

int main(int argc, char* argv[]){
	if(argc!=3){
		printf("usage: %s src_file out_file\n",argv[0]);
		return 1;
	}
	CompileStepOne comp = first_compile_step(argv[1]);
	assemble_and_link(argv[2],&O0_code,&comp.ir);
	
	//no need to free anything.
	#ifdef FREE_FOR_DEBUG
	free(comp.ir.names);
	free(comp.ir.states);
	free(comp.text);
	#endif
	
	return 0;
}
