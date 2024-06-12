#ifndef COMPILER_H
#define COMPILER_H

#include "IR.h"

typedef void (*printer_func_t)(FILE *file,void* data);
int assemble_and_link(const char* filename,const char* dirname, printer_func_t code,void* data);
//this function checks for write errors by itself. 
//this means that the interface calle can just ignore the possibilety

void O0_IR_to_ASM(FILE *file,TuringIR ir); 
//bad name but it basically a way to take IR down to assembly.
//specifcly this is all the parts that are actually definble by the user

#endif // COMPILER_H
