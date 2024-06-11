#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static const char *assembly_template = 
"section .text\n"
"    global _start\n"
"    extern ReadTape\n"
"    extern DumpTape\n"
"    extern exit_turing\n"
"_start:\n"
"    ;initial boiler plate\n"
"    ; Ensure there are at least 2 arguments (argc >= 3)\n"
"    mov rax, [rsp]         ; Load argc\n"
"    cmp rax, 3\n"
"    jl _exit_error          ; If less than 3 arguments, exit\n"
"\n"
"    ; Load the address of the first command-line argument (input file)\n"
"    mov rsi, [rsp+16]      ; First argument (input file)\n"
"    sub rsp, 32\n"
"    lea rdi, [rsp]         ; Return struct address\n"
"\n"
"    call ReadTape\n"
"\n"
"    mov rsi, [rsp+24+32]   ; Second argument (output file) now shifted by 32\n"
"    lea rdi, [rsp]         ; Same struct pointer\n"
"\n"
"    ;!!!ACTUAL CODE: done boiler plate\n"
"%s\n"  // Placeholder for actual code
"    ;DONE:output boilerplate and exit;\n"
"    call DumpTape\n"
"\n"
"    ; Exit the program\n"
"    mov rdi, 0\n"
"    call exit_turing\n"
"\n"
"_exit_error:\n"
"    mov rdi, 3\n"
"    call exit_turing\n";

int assemble_and_link(const char* filename, const char* code) {
    // Step 1: Generate the assembly code
    char* working_name=null_check(malloc(strlen(filename)+5));
    strcpy(working_name,filename);
    strcat(working_name,".asm");

    FILE *file = fopen(working_name, "w");
    if (file == NULL) {
        free(working_name);
        perror("Failed to open file");
        return 1;
    }

    // Custom code to be inserted into the placeholder
    //const char *custom_code = "    ; Inserted custom code";

    fprintf(file, assembly_template, code);

    fclose(file);
    printf("Assembly code generated successfully.\n");



    const char* cnasm="nasm -f elf64 -o %s.o %s";
    char* nasm=null_check(malloc(strlen(cnasm)+strlen(working_name)+strlen(filename)));
    sprintf(nasm,cnasm,filename,working_name);

    // Step 2: Assemble the generated assembly code
    int result = system(nasm);
    if (result != 0) {
        free(working_name);
        free(nasm);
        fprintf(stderr, "Failed to assemble the code.\n");
        return 1;
    }
    printf("Assembly completed successfully.\n");

    // Step 3: Link the object file with io.o to create the final executable
    const char* cld="ld -o %s.out %s.o bin/io.o -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2";
    char* ld=null_check(malloc(strlen(cld)+2*strlen(filename)));
    sprintf(ld,cld,filename,filename);
    
    result = system(ld);
    free(working_name);
    free(nasm);
    free(ld);

    if (result != 0) {
        fprintf(stderr, "Failed to link the object file.\n");
        return 1;
    }
    printf("Linking completed successfully.\n");

    return 0;
}