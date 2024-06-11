#ifndef COMPILER_H
#define COMPILER_H

#include "IR.h"
int assemble_and_link(const char* filename, const char* code);

char* compile(TuringIR ir); 
//bad name but it basically a way to take IR down to assembly.
//specifcly this is all the parts that are actually definble by the user

#endif // COMPILER_H
