#include "turing.h"
#include "utils.h"


TuringResult run_turing(Tape* tape,const TuringMachine machine,int max_steps){
	State state=machine.states[0];
	
	//c++ style iterator
	Bit* end_right=tape->base+tape->right_init;
	Bit* end_left=tape->base-tape->left_init;

	//c++ style iterator
	Bit* end_max=tape->base+tape->max;
	Bit* end_min=tape->base-tape->min;

	if(end_right==end_left) UNREACHABLE();

	int i=0;
	for(;i<max_steps;i++){
		//figure states
		Bit val= *tape->cur;
		TRANSITION trans=state[val];

		//write
		*tape->cur=trans.write;

		//move
		tape->cur+=trans.move;
		if(tape->cur==end_right){

		}

		else if(tape->cur==end_left){


		}
	}

	return (TuringResult){HAULT,i}
}