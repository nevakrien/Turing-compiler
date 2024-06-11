#include "turing.h"
#include "io.h"
#include "cli.h"
#include  "IR.h"
#include "parser.h"
#include "utils.h"

//#define FREE_FOR_DEBUG

int main(int argc, char* argv[]){
	if(argc<4){
		printf("usage: %s src_file tape_file out_tape_file (optionaly max_steps)\n",argv[0]);
		return 1;
	}
	CompileStepOne comp = first_compile_step(argv[1]);
	Tape tape=ReadTape(argv[2]);
	
	TuringMachine machine = finalize_unsafe(comp.ir);

	TuringResult result;
	int print_steps=0;
	if(argc==4){
	 result = run_turing_no_stop(&tape,machine,0);

	}

	else if(argc==5){
		result = run_turing(&tape,machine,0,int_of_str(argv[4]));
		print_steps=1;
	}

	else{
		printf("too many arguments\n");
		return 1;
	}

	switch(result.code){
		case HALT:
			printf("machine halted ");
			break;
		case OUT_OF_TAPE:
			printf("ran out of tape state: %s ",comp.ir.names[result.state_id]);
			break;
		case TIME_OUT:
			printf("ran out of time state: %s ",comp.ir.names[result.state_id]);
			break;
		default:
			UNREACHABLE();
	}
	if(print_steps){
		printf("steps: %d",result.steps);
	}
	putchar('\n');

	DumpTape(&tape,argv[3]);
	
	//no need to free anything.
	#ifdef FREE_FOR_DEBUG
	free(comp.ir.names);
	free(comp.ir.states);
	free(comp.text);
	free(machine.states);
	free_all_tape(&(tape.base[tape.left_limit]),sizeof(Bit)*(1+tape.right_limit-tape.left_limit));
	#endif
	
	return 0;
}
