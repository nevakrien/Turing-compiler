#include "turing.h"
#include "utils.h"
#include <string.h>

static inline int get_state_id(const TuringMachine m, const State* s){
	if(s==NULL){
		return -1;
	}
	return s-m.states;
}

static const int expand_size=256; //single avx2 register
static inline void expand_tape_right(Tape* tape){
	int new_init=tape->right_init+expand_size;
	int expand_length=expand_size;

	if(new_init>tape->right_limit){
		//printf("reached the very end on the last valid byte\n");
		new_init=tape->right_limit;
		expand_length=new_init-tape->right_init;
	}
	

	memset(&tape->base[tape->right_init+1],0,expand_length*sizeof(Bit));
	tape->right_init=new_init;

	//printf("updagted init to: %d\n",new_init);
}

static inline void expand_tape_left(Tape* tape){
	int new_init=tape->left_init-expand_size;
	int expand_length=expand_size;
	
	if(new_init<tape->left_limit){
		new_init=tape->left_limit;
		expand_length=tape->left_init-new_init;
	}

	memset(&tape->base[new_init],0,expand_length*sizeof(Bit));
	tape->left_init=new_init;
}

TuringResult run_turing(Tape* tape,const TuringMachine machine,int start,int max_steps){
	if(tape->cur-tape->base > tape->right_init) {UNREACHABLE();}
	if(tape->cur-tape->base < tape->left_init) {UNREACHABLE();}

	const State* state=&machine.states[start];
	
	//c++ style iterator
	Bit* end_right=tape->base+tape->right_init;
	Bit* end_left=tape->base-tape->left_init;

	//c++ style iterator
	const Bit* end_max=tape->base+tape->right_limit;
	const Bit* end_min=tape->base-tape->left_limit;

	//printf("limits right:%d left:%d \n",tape->right_limit,tape->left_limit);

	if(end_right==end_left) UNREACHABLE();

	int i=0;
	for(;i<max_steps;i++){
		//figure states
		//printf("cur index:%d\n", tape->cur-tape->base);
		Bit val= *tape->cur;
		const Transition trans=state->transitions[val];

		//write
		*tape->cur=trans.write;

		//move
		tape->cur+=trans.move;
		
		//printf("current dist from right_end:%d\n",end_right-tape->cur);
		//bounds checks
		if(tape->cur>end_right){
			//printf("\ncurrent dist from end_max:%d\n",end_max-tape->cur);
			if(tape->cur==end_max+1){
				return (TuringResult){OUT_OF_TAPE,i+1,get_state_id(machine,trans.NextState)};
			}

			expand_tape_right(tape);
			end_right=tape->base+tape->right_init;
			
			if(end_right>end_max){
				UNREACHABLE();
			}
		}

		else if(tape->cur<end_left){
			if(tape->cur==end_min-1	){
				return (TuringResult){OUT_OF_TAPE,i+1,get_state_id(machine,trans.NextState)};
			}

			expand_tape_left(tape);
			end_left=tape->base-tape->left_init;

			if(end_left<end_min){
				UNREACHABLE();
			}
		}

		if(trans.NextState==NULL){
			return (TuringResult){HAULT,i+1,-1};
		}
		//state update
		state=trans.NextState;
	}

	return (TuringResult){TIME_OUT,i,get_state_id(machine,state)};
}

TuringResult run_turing_no_stop(Tape* tape,const TuringMachine machine,int start){
	if(tape->cur-tape->base > tape->right_init) {UNREACHABLE();}
	if(tape->cur-tape->base < tape->left_init) {UNREACHABLE();}

	const State* state=&machine.states[start];
	
	//c++ style iterator
	Bit* end_right=tape->base+tape->right_init;
	Bit* end_left=tape->base-tape->left_init;

	//c++ style iterator
	const Bit* end_max=tape->base+tape->right_limit;
	const Bit* end_min=tape->base-tape->left_limit;

	//printf("limits right:%d left:%d \n",tape->right_limit,tape->left_limit);

	if(end_right==end_left) UNREACHABLE();

	for(;;){
		//figure states
		//printf("cur index:%d\n", tape->cur-tape->base);
		Bit val= *tape->cur;
		const Transition trans=state->transitions[val];

		//write
		*tape->cur=trans.write;

		//move
		tape->cur+=trans.move;
		
		//printf("current dist from right_end:%d\n",end_right-tape->cur);
		//bounds checks
		if(tape->cur>end_right){
			//printf("\ncurrent dist from end_max:%d\n",end_max-tape->cur);
			if(tape->cur==end_max+1){
				return (TuringResult){OUT_OF_TAPE,-1,get_state_id(machine,trans.NextState)};
			}

			expand_tape_right(tape);
			end_right=tape->base+tape->right_init;
			
			if(end_right>end_max){
				UNREACHABLE();
			}
		}

		else if(tape->cur<end_left){
			if(tape->cur==end_min-1	){
				return (TuringResult){OUT_OF_TAPE,-1,get_state_id(machine,trans.NextState)};
			}

			expand_tape_left(tape);
			end_left=tape->base-tape->left_init;

			if(end_left<end_min){
				UNREACHABLE();
			}
		}

		if(trans.NextState==NULL){
			return (TuringResult){HAULT,-1,-1};
		}
		//state update
		state=trans.NextState;
	}
}
