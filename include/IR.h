#ifndef IR_H
#define IR_H

#include "parser.h"
#include <stdint.h>



//
typedef struct{
	Bit write;
	Dir move;
	int nextState; //-1 is halt -2 is not intilized
} TransIR;

typedef struct{
	int stateId;
	TransIR trans[2];

} StateIR;

typedef struct{
	StateIR* states;
	int len;
	const char** names;
}TuringIR;

static inline void free_TuringIR(TuringIR x){
	free(x.states);
	free(x.names);
}


TuringIR make_initial_ir (TuringMachineEncoding encode);
TuringIR prone_ir (TuringIR ir);




#endif // IR_H
