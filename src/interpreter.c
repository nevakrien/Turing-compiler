#include "turing.h"
#include "io.h"
#include "cli.h"
#include  "IR.h"
#include "parser.h"
#include "utils.h"

//#define FREE_FOR_DEBUG

int main(int argc, char* argv[]){
	if(argc!=4){
		printf("usage: %s src_file tape_file out_tape_file\n",argv[0]);
		return 1;
	}
	CompileStepOne comp = first_compile_step(argv[1]);
	Tape tape=ReadTape(argv[2]);
	
	TuringMachine machine = finalize_unsafe(comp.ir);
	TuringResult result = run_turing_no_stop(&tape,machine,0);

	switch(result.code){
		case HAULT:
			printf("machine haulted\n");
			break;
		case OUT_OF_TAPE:
			printf("ran out of tape\n");
			break;
		case TIME_OUT:
		default:
			UNREACHABLE();
	}
	DumpTape(&tape,argv[3]);
	
	//no need to free anything.
	#ifdef FREE_FOR_DEBUG
	free(comp.ir.names);
	free(comp.ir.states);
	free(comp.text);
	free(machine.states);
	free_all_tape(&(tape.base[tape.left_limit]),sizeof(Bit)*(tape.right_limit-tape.left_limit));
	#endif
	
	return 0;
}
