#include "turing.h"
#include "io.h"
#include "cli.h"
#include  "IR.h"
#include "parser.h"



int main(int argc, char* argv[]){
	if(argc!=2){
		printf("usage: %s src_file tape_file\n",argv[0]);
	}
	CompileStepOne comp = first_compile_step(argv[1]);
	Tape tape=ReadTape(argv[2]);
	
	return 0;
}
