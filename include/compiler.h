#ifndef COMPILER_H
#define COMPILER_H

#include "IR.h"

typedef void (*printer_func_t)(FILE *file,void* data);
int assemble_and_link(const char* filename,const char* dirname, printer_func_t code,const char *casm,const char *ext,void* data);
//this function checks for write errors by itself. 
//this means that the interface calle can just ignore the possibilety

void O0_IR_to_ASM(FILE *file,TuringIR ir); 
//bad name but it basically a way to take IR down to assembly.
//specifcly this is all the parts that are actually definble by the user

void O1_IR_to_ASM(FILE *file,TuringIR ir);

void ARM_IR_to_ASM(FILE *file, TuringIR ir);

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

[[maybe_unused]] static const char *O0_assembly_start_template = 
"section .text\n"
"    global _start\n"
"    extern ReadTape\n"
"    extern DumpTape\n"
"    extern exit_turing\n"

//for some reason nasm is SOOO fucking slow to assmble if this is at the bottom
"exit_out_of_tape:\n"
//"    and rsp, -16;we need an aligned stack and dont need memory\n"
"    mov rdi, 2\n"
"    call exit_turing\n"

"_start:\n"
"    ;initial boiler plate\n"
"    ; Ensure there are at least 2 arguments (argc >= 3)\n"
"    mov rax, [rsp]         ; Load argc\n"
"    cmp rax, 3\n"
"    jl _exit_bad_args          ; If less than 3 arguments, exit\n"
"\n"
"    ; Load the address of the first command-line argument (input file)\n"
"    mov rsi, [rsp+16]      ; First argument (input file)\n"
"    sub rsp, 32\n"
"    lea rdi, [rsp]         ; Return struct address\n"
"\n"
"    call ReadTape\n"
"\n"
"    ;!!!ACTUAL CODE: done boiler plate\n";
//"%s\n"  // Placeholder for actual code

[[maybe_unused]] static const char *O0_assembly_end_template = 
"    ;DONE:output boilerplate and exit;\n\n"

"    mov rsi, [rsp+32+24]   ; Second argument (output file) now shifted by 32\n"
"    lea rdi, [rsp]         ; Same struct pointer\n"
"\n"
"    call DumpTape\n"
"\n"
"    ; Exit the program\n"
"    mov rdi, 0\n"
"    call exit_turing\n"
"\n"

"\n"
"_exit_bad_args:\n"
"    mov rdi, 3\n"
"    call exit_turing\n";

#endif // COMPILER_H
