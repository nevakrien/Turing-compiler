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

void O1_IR_to_ASM(FILE *file,TuringIR ir); 

typedef struct {
    const char* address_register;
    const char* bit_register;
    const char* right_limit_register;
    const char* left_limit_register;
    const char* right_init_register;
    const char* left_init_register;
    const char* small_right_init_register;
    const char* small_left_init_register;
    
    char* tmp;
    char* tmp2;

    const int move_size;
    const int extend_size;
} RegisterData;

#endif // COMPILER_H
