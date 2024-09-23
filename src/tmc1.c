#include "cli.h"
#include  "IR.h"
#include "utils.h"
#include "compiler.h"
#include <libgen.h>

//#define FREE_FOR_DEBUG
const char *casm = "nasm -g -f elf64 -o %s.o %s";

void O1_code(FILE *file,void* data){
	TuringIR* ir=(TuringIR*)data;
	O1_IR_to_ASM(file,*ir);
}

int main(int argc, char* argv[]){
	if(argc!=3){
		printf("usage: %s src_file out_file\n",argv[0]);
		return 1;
	}
	CompileStepOne comp = first_compile_step(argv[1]);
	TuringIR code=prone_ir(comp.ir,1);
	
	//code=comp.ir; //can be used to test things


	assemble_and_link(argv[2],dirname(argv[0]),&O1_code,casm,".asm",&code);
	
	//no need to free anything.
	#ifdef FREE_FOR_DEBUG
	free_TuringIR(code);

	free_TuringIR(comp.ir);
	free(comp.text);
	#endif
	
	return 0;
}
