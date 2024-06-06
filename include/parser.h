#ifndef PARSER_H
#define PARSER_H
#include "utils.h"
#include "turing.h"

typedef struct{
	char* NameStr;
	Bit read;
	Bit write;
	Dir move;
	char* NextStateStr;
} TransitionEncoding;

// typedef struct{
// 	TransitionEncoding state[2];
// }StateEncoding;

typedef struct TuringMachineEncoding{
	int len;
	TransitionEncoding* trans;
}TuringMachineEncoding;


char *read_file_into_buffer(const char *filename, size_t *length);

void print_trans(TuringMachine machine,Transition trans, int indent);
void print_state(TuringMachine machine,State s, int indent);
void print_machine(TuringMachine machine, int indent);


//keeps refrences to raw_text and mutates it. 
//raw_text should only be freed once we finished working with names directly
TuringMachineEncoding parse_text_with_prints(MUTATEBLE char* raw_text);

#endif // PARSER_H
