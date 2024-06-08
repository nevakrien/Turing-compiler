#ifndef CLI_H
#define CLI_H

#include "io.h"
#include "turing.h"
#include "IR.h"

void PrintTape(const Tape tape);
void PrintMetadata(const Tape tape);

typedef struct 
{
	TuringIR ir;
	char* text;//mostly for freeing
}CompileStepOne;

CompileStepOne first_compile_step(const char* filename);

TuringMachine finalize_unsafe(TuringIR ir);

#endif // CLI_H
