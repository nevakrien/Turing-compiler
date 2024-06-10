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



static inline TuringResult process_step(Tape* tape, const TuringMachine machine, const State** state, const Bit* end_max, const Bit* end_min, Bit** end_right, Bit** end_left, int step_count) {
    Bit val = *tape->cur;
    const Transition trans = (*state)->transitions[val];

    // Write
    *tape->cur = trans.write;

    // Move
    tape->cur += trans.move;

    // Boundary checks
    if (tape->cur > *end_right) {
        if (tape->cur == end_max + 1) {
            return (TuringResult){OUT_OF_TAPE, step_count, get_state_id(machine, trans.NextState)};
        }

        expand_tape_right(tape);
        *end_right = tape->base + tape->right_init;

        if (*end_right > end_max) {
            UNREACHABLE();
        }
    } else if (tape->cur < *end_left) {
        if (tape->cur == end_min - 1) {
            return (TuringResult){OUT_OF_TAPE, step_count, get_state_id(machine, trans.NextState)};
        }

        expand_tape_left(tape);
        *end_left = tape->base + tape->left_init;

        if (*end_left < end_min) {
            UNREACHABLE();
        }
    }

    if (trans.NextState == NULL) {
        return (TuringResult){HALT, step_count, -1};
    }

    // State update
    *state = trans.NextState;

    return (TuringResult){TIME_OUT, step_count, get_state_id(machine, *state)};
}

TuringResult run_turing(Tape* tape, const TuringMachine machine, int start, int max_steps) {
    if (tape->cur - tape->base > tape->right_init) { UNREACHABLE(); }
    if (tape->cur - tape->base < tape->left_init) { UNREACHABLE(); }

    const State* state = &machine.states[start];

    Bit* end_right = tape->base + tape->right_init;
    Bit* end_left = tape->base + tape->left_init;

    const Bit* end_max = tape->base + tape->right_limit;
    const Bit* end_min = tape->base + tape->left_limit;

    if (end_right < end_left) UNREACHABLE();

    for (int i = 0; i < max_steps; i++) {
        TuringResult result = process_step(tape, machine, &state, end_max, end_min, &end_right, &end_left, i + 1);
        if (result.code != TIME_OUT) {
            return result;
        }
    }

    return (TuringResult){TIME_OUT, max_steps, get_state_id(machine, state)};
}

TuringResult run_turing_no_stop(Tape* tape, const TuringMachine machine, int start) {
    if (tape->cur - tape->base > tape->right_init) { UNREACHABLE(); }
    if (tape->cur - tape->base < tape->left_init) { UNREACHABLE(); }

    const State* state = &machine.states[start];

    Bit* end_right = tape->base + tape->right_init;
    Bit* end_left = tape->base + tape->left_init;

    const Bit* end_max = tape->base + tape->right_limit;
    const Bit* end_min = tape->base + tape->left_limit;

    if (end_right < end_left) UNREACHABLE();

    for (;;) {
        TuringResult result = process_step(tape, machine, &state, end_max, end_min, &end_right, &end_left, -1);
        if (result.code != TIME_OUT) {
            return result;
        }
    }
}
