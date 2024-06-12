#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "compiler.h"

static const char *assembly_start_template = 
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
"    ;!!!ACTUAL CODE: done boiler plate\n";
//"%s\n"  // Placeholder for actual code

static const char *assembly_end_template = 
"    ;DONE:output boilerplate and exit;\n"
"exit_good:"
"    mov rsi, [rsp+24+32]   ; Second argument (output file) now shifted by 32\n"
"    lea rdi, [rsp]         ; Same struct pointer\n"
"\n"
"    call DumpTape\n"
"\n"
"    ; Exit the program\n"
"    mov rdi, 0\n"
"    call exit_turing\n"
"\n"
"_exit_error:\n"
"    mov rdi, 3\n"
"    call exit_turing\n";

int assemble_and_link(const char* filename,const char* dirname, printer_func_t codefunc,void* data) {
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

    fprintf(file,"%s",assembly_start_template);
    codefunc(file,data);
    fprintf(file,"%s",assembly_end_template);

    if (ferror(file)) {
        perror("Error occurred during Assembly generation\n");
        exit(1);
    }

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
    const char* cld="ld -o %s.out %s.o %s/io.o -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2";
    char* ld=null_check(malloc(strlen(cld)+2*strlen(filename)));
    sprintf(ld,cld,filename,filename,dirname);
    
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

const char* spaces="    ";

//does not handle hault properly yet. other issues with register size specifications on the ops
void O0_IR_to_ASM(FILE *file,TuringIR ir){
    const char* address_register="rax";
    const char* bit_register="ecx";
    const char* right_limit_register="r8d";
    const char* left_limit_register="r9d";
    int move_size=4;

    fprintf(file,"%smov %s,qword [rsp]\n",spaces,address_register);//load cur

    fprintf(file,"%smov %s,dword [rsp+20]\n",spaces,right_limit_register);
    fprintf(file,"%sadd %s,dword [rsp+8]\n",spaces,right_limit_register);//add base
    
    fprintf(file,"%smov %s,dword [rsp+16]\n",spaces,left_limit_register);
    fprintf(file,"%sadd %s,dword [rsp+8]\n",spaces,left_limit_register);//add base



    for(int i=0;i<ir.len;i++){
        fprintf(file,"L%d:;%s\n",i,ir.names[i]);
        
        //brench based on bit
        fprintf(file,"%smov %s,dword [%s]\n",spaces,bit_register,address_register);
        fprintf(file,"%stest %s, %s\n",spaces,bit_register,bit_register);
        fprintf(file,"%sjnz L%d_1\n",spaces,i);    

        //read 0
        fprintf(file,"L%d_0:;%s\n",i,ir.names[i]);
        fprintf(file,"%smov [%s],dword %d \n",spaces,address_register,ir.states[i].trans[0].write);


        if(ir.states[i].trans[0].move!=Stay){
            char sign=ir.states[i].trans[0].move > 0 ? '+' : '-';
            fprintf(file, "%slea %s, [%s%c%d] \n", spaces, address_register, address_register, sign,move_size);

        }

        if(ir.states[i].trans[0].nextState!=-1){
            fprintf(file,"%sjmp L%d\n",spaces,ir.states[i].trans[0].nextState);
        }
        else{
            fprintf(file,"%sjmp exit_good\n",spaces);
        }


        //read 1
        fprintf(file,"L%d_1:;%s\n",i,ir.names[i]);   
        fprintf(file,"%smov [%s],dword %d \n",spaces,address_register,ir.states[i].trans[1].write);
        
        if(ir.states[i].trans[1].move!=Stay){
            char sign=ir.states[i].trans[1].move > 0 ? '+' : '-';
            fprintf(file, "%slea %s, [%s%c%d] \n", spaces, address_register, address_register, sign,move_size);

        }

        if(ir.states[i].trans[1].nextState!=-1){
            fprintf(file,"%sjmp L%d\n",spaces,ir.states[i].trans[1].nextState);
        }
        else{
            fprintf(file,"%sjmp exit_good\n",spaces);
        }

    }
}