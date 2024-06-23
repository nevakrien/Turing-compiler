#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "compiler.h"

// cur is at offset 0
// base is at offset 8
// left_limit is at offset 16
// right_limit is at offset 20
// left_init is at offset 24
// right_init is at offset 28

static const char *assembly_start_template = 
"section .text\n"
"    global _start\n"
"    extern ReadTape\n"
"    extern DumpTape\n"
"    extern exit_turing\n"

//for some reason nasm is SOOO fucking slow to assmble if this is at the bottom
"exit_out_of_tape:\n"
"    and rsp, -16;we need an aligned stack and dont need memory\n"
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

static const char *assembly_end_template = 
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



    const char* cnasm="nasm -g -f elf64 -o %s.o %s";
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

//this is probably going to stay frozen. no reason to change it since it works.
void O0_IR_to_ASM(FILE *file,TuringIR ir){
    
    //we are using rax rcx rdi 
    //inside of some of the loops. this means that they are NOT ALOWED to be chosen

    const char* address_register="r14";//this is also used in the assembly_end_template so dont mess with it.
    const char* bit_register="r15d";
    const char* right_limit_register="r8";
    const char* left_limit_register="r9";
    const char* right_init_register="r10";
    const char* left_init_register="r11";

    const char* small_right_init_register="r10d";
    const char* small_left_init_register="r11d";

    const int move_size=4;
    const int extend_size=256*4;//same as the interpeter //HAS to be a multiple of 4


    fprintf(file,"%smov %s,qword [rsp] ;curent\n",spaces,address_register);//load cur

    char* tmp = "rcx";
    char* tmp2 = "rax";

    //the left side of these isnt inilized properly...

    fprintf(file, "%sxor %s, %s\n", spaces, tmp2,tmp2);
    // Load base address into tmp
    fprintf(file, "%smov %s, qword [rsp+8] ;base\n", spaces, tmp);

    // right_limit
    fprintf(file, "%smovsxd rax, dword [rsp+20]\n", spaces); // Load and sign-extend the value into rax (a 64-bit register)
    fprintf(file, "%slea %s, [%s + 4*rax] ;right limit\n", spaces, right_limit_register, tmp);

    // left_limit
    fprintf(file, "%smovsxd rax, dword [rsp+16]\n", spaces); // Load and sign-extend the value into rax
    fprintf(file, "%slea %s, [%s + 4*rax] ;left limit\n", spaces, left_limit_register, tmp);

    // right_init
    fprintf(file, "%smovsxd rax, dword [rsp+24]\n", spaces); // Load and sign-extend the value into rax
    fprintf(file, "%slea %s, [%s + 4*rax] ;left initilized\n", spaces, left_init_register, tmp);

    // left_init
    fprintf(file, "%smovsxd rax, dword [rsp+28]\n", spaces); // Load and sign-extend the value into rax
    fprintf(file, "%slea %s, [%s + 4*rax] ;right initilized\n", spaces, right_init_register, tmp);




    fprintf(file,"%scld\n\n",spaces);

    for(int i=0;i<ir.len;i++){
        fprintf(file,"L%d:;%s\n",i,ir.names[i]);
        
        fprintf(file,"%s;brench based on current bit\n",spaces);
        fprintf(file,"%smov %s,dword [%s]\n",spaces,bit_register,address_register);
        fprintf(file,"%stest %s, %s\n",spaces,bit_register,bit_register);
        fprintf(file,"%sjnz L%d_1\n\n",spaces,i);    

        for(int k=0;k<2;k++){
            fprintf(file,"L%d_%d:;%s[%d]\n",i,k,ir.names[i],k);
            fprintf(file,"%smov [%s],dword %d \n",spaces,address_register,ir.states[i].trans[k].write);

            //move
            switch(ir.states[i].trans[k].move){
                case Stay:
                    break;
                case Right:
                    fprintf(file, "%slea %s, [%s+%d] \n", spaces, address_register, address_register,move_size);
                    
                    fprintf(file, "%scmp %s, %s;bounds check \n", spaces, address_register,right_init_register);
                    fprintf(file, "%sjbe Done_L%d_%d\n", spaces,i,k);

                    //!!! these 2 lines are single handedly responsible for over a 100x nasm preformance drop
                    fprintf(file, "%scmp %s, %s;check out of tape\n", spaces, address_register,right_limit_register);
                    fprintf(file, "%sja exit_out_of_tape\n", spaces);
                    //!!!!

                    tmp = "rcx";//using this to avoid a move

                    fprintf(file,"%s\n;find new bound\n",spaces);

                    fprintf(file, "%slea %s,[%s+%d]\n",spaces,tmp,right_init_register,extend_size);
                    
                    fprintf(file, "%scmp %s,%s\n",spaces,tmp,right_limit_register);
                    fprintf(file, "%sjbe Extend_L%d_%d\n", spaces,i,k);

                    fprintf(file, "%smov %s,%s\n",spaces,tmp,right_limit_register);

                    fprintf(file,"Extend_L%d_%d:\n",i,k);

                    //memset 0 
                    // Set rdi to the starting address
                    fprintf(file, "%smov rdi, %s;setting up for stosq \n", spaces, address_register);
                    fprintf(file, "%smov %s, %s\n", spaces, right_init_register, tmp); // Update the right_init_register to the new end

                    // Calculate the number of 32-bit elements to zero out
                    fprintf(file, "%ssub %s, rdi\n", spaces,tmp);
                    fprintf(file, "%sshr %s, 2;bad more effishent to do quads\n", spaces,tmp); // Divide by 8
                    //not sure about this 1fprintf(file, "%ssub %s, 1\n", spaces,tmp);//????? needed but idk why


                    // Zero out the memory
                    //MAJO BUG IN THE ORDER
                    //no need tmp is rcx fprintf(file, "%smov rcx, rax\n", spaces); // Number of 32-bit elements to store
                    fprintf(file, "%sxor rax, rax\n", spaces); // Zero value to store
                    fprintf(file, "%srep stosd\n", spaces);
                    

                   
                    
                    //when we improve the speed fprintf(file, "%smov [%s],dword 0;maybe there is a 4byte remainder\n", spaces,right_init_register);


                    fprintf(file,"Done_L%d_%d:\n",i,k);
                    break;
                case Left:
                    

                    fprintf(file, "%slea %s, [%s-%d] \n", spaces, address_register, address_register,move_size);
                    
                    fprintf(file, "%scmp %s, %s;bounds check \n", spaces, address_register,left_init_register);
                    fprintf(file, "%sjae Done_L%d_%d\n", spaces,i,k);

                    //!!! these 2 lines are single handedly responsible for over a 100x nasm preformance drop
                    fprintf(file, "%scmp %s, %s;check out of tape\n", spaces, address_register,left_limit_register);
                    fprintf(file, "%sjb exit_out_of_tape\n", spaces);
                    //!!!!

                    tmp = "rax";//rcx is used down

                    fprintf(file, "%smov rcx, %s\n", spaces, left_init_register);
                    fprintf(file, "%slea %s,[%s-%d]\n",spaces,tmp,left_init_register,extend_size);
                    
                    //tmp = max(tmp right_limit)
                    fprintf(file, "%scmp %s,%s\n",spaces,tmp,left_limit_register);
                    fprintf(file, "%sjae Extend_L%d_%d\n", spaces,i,k);

                    fprintf(file, "%smov %s,%s\n",spaces,tmp,left_limit_register);

                    fprintf(file,"Extend_L%d_%d:\n",i,k);

                    //memset 0 
                    // Set rdi to the starting address 
                    // fprintf(file, "%smov rdi, %s ;setting up for stosq\n", spaces, address_register);
                    
                    
                    fprintf(file, "%smov %s, %s\n", spaces, left_init_register, tmp); // Update the left_init_register to the new end
                    fprintf(file, "%smov rdi, %s ;setting up for stosq\n", spaces, left_init_register);
                    
                    
                    // Calculate the number of 32-bit elements to zero out
                    fprintf(file, "%ssub rcx, %s\n",spaces,tmp);
                    
                    fprintf(file, "%sshr rcx, 2;bad more effishent to do quads\n", spaces); // Divide by 8
                    //fprintf(file, "%ssub rcx, 1\n", spaces);//????? needed but idk why

                    // Zero out the memory
                    //no need we did earlier with rcx fprintf(file, "%smov rcx, rax\n", spaces); // Number of 64-bit elements to store
                    fprintf(file, "%sxor rax, rax\n", spaces); // Zero value to store
                    
                    //fprintf(file, "%sstd\n", spaces);
                    fprintf(file, "%srep stosd\n", spaces);
                    //fprintf(file,"%scld\n",spaces);

                   
                    
                    // //when we improve the speed fprintf(file, "%smov [%s],dword 0;maybe there is a 4byte remainder\n", spaces,left_init_register);


                    fprintf(file,"Done_L%d_%d:\n",i,k);
                    break;
            }

            //next
            if(ir.states[i].trans[k].nextState!=-1){
                fprintf(file,"%sjmp L%d\n\n",spaces,ir.states[i].trans[k].nextState);
            }
            else{
                fprintf(file,"%sjmp exit_good\n\n",spaces);
            }
        }
    }

    //write to the struct
    fprintf(file,"exit_good:\n");
    fprintf(file,"%smov [rsp],qword %s\n",spaces,address_register);

    tmp = "rcx";
    tmp2="rax";
    // Load base address into tmp
    fprintf(file, "%smov %s, qword [rsp+8]\n", spaces, tmp);

    //not handeling the sign right

    //right init
    fprintf(file,"%ssub %s,%s\n",spaces,right_init_register,tmp);
    fprintf(file, "%sshr %s, 2;move to int indexing like c\n", spaces,right_init_register);
    fprintf(file, "%smov [rsp+28], dword %s \n", spaces, small_right_init_register);

    fprintf(file,"%ssub %s,%s\n",spaces,left_init_register,tmp);
    fprintf(file, "%sshr %s, 2;move to int indexing like c\n", spaces,left_init_register);
    fprintf(file, "%smov [rsp+24], dword %s \n", spaces, small_left_init_register);//tmp2_short);
}


static void inilize_registers(FILE *file,RegisterData rd){
    fprintf(file,"%smov %s,qword [rsp] ;curent\n",spaces,rd.address_register);//load cur

    //the left side of these isnt inilized properly...

    fprintf(file, "%sxor %s, %s\n", spaces, rd.tmp2,rd.tmp2);
    // Load base address into rd.tmp
    fprintf(file, "%smov %s, qword [rsp+8] ;base\n", spaces, rd.tmp);

    // right_limit
    fprintf(file, "%smovsxd rax, dword [rsp+20]\n", spaces); // Load and sign-extend the value into rax (a 64-bit register)
    fprintf(file, "%slea %s, [%s + 4*rax] ;right limit\n", spaces, rd.right_limit_register, rd.tmp);

    // left_limit
    fprintf(file, "%smovsxd rax, dword [rsp+16]\n", spaces); // Load and sign-extend the value into rax
    fprintf(file, "%slea %s, [%s + 4*rax] ;left limit\n", spaces, rd.left_limit_register, rd.tmp);

    // right_init
    fprintf(file, "%smovsxd rax, dword [rsp+24]\n", spaces); // Load and sign-extend the value into rax
    fprintf(file, "%slea %s, [%s + 4*rax] ;left initilized\n", spaces, rd.left_init_register, rd.tmp);

    // left_init
    fprintf(file, "%smovsxd rax, dword [rsp+28]\n", spaces); // Load and sign-extend the value into rax
    fprintf(file, "%slea %s, [%s + 4*rax] ;right initilized\n", spaces, rd.right_init_register, rd.tmp);


    fprintf(file,"%scld\n",spaces);
}

static void O1_extend_left(FILE *file,RegisterData rd,int i,int k){

    //!!! these 2 lines are single handedly responsible for over a 100x nasm preformance drop
    fprintf(file, "%scmp %s, %s;check out of tape\n", spaces, rd.address_register,rd.left_limit_register);
    fprintf(file, "%sjb exit_out_of_tape\n", spaces);
    //!!!!

    
    if(strcmp(rd.tmp,"rcx")==0){
        //rcx is used down
        UNREACHABLE();
    }

    fprintf(file, "%smov rcx, %s\n", spaces, rd.left_init_register);
    fprintf(file, "%slea %s,[%s-%d]\n",spaces,rd.tmp,rd.left_init_register,rd.extend_size);
    
    //rd.tmp = max(rd.tmp right_limit)
    fprintf(file, "%scmp %s,%s\n",spaces,rd.tmp,rd.left_limit_register);
    if(i<0){
        fprintf(file, "%sjbe Extend_Left_internal\n", spaces);

    }
    else{
        fprintf(file, "%sjbe Extend_Left_internal_%d_%d\n", spaces,i,k);

    }

    fprintf(file, "%smov %s,%s\n",spaces,rd.tmp,rd.left_limit_register);

    if(i<0){
        fprintf(file, "Extend_Left_internal:\n");

    }
    else{
        fprintf(file, "Extend_Left_internal_%d_%d:\n",i,k);

    }

    //memset 0 
    // Set rdi to the starting address 
    // fprintf(file, "%smov rdi, %s ;setting up for stosq\n", spaces, rd.address_register);
    
    
    fprintf(file, "%smov %s, %s\n", spaces, rd.left_init_register, rd.tmp); // Update the rd.left_init_register to the new end
    fprintf(file, "%smov rdi, %s ;setting up for stosq\n", spaces, rd.left_init_register);
    
    
    // Calculate the number of 32-bit elements to zero out
    fprintf(file, "%ssub rcx, %s\n",spaces,rd.tmp);
    
    fprintf(file, "%sshr rcx, 2;bad more effishent to do quads\n", spaces); // Divide by 8
    //fprintf(file, "%ssub rcx, 1\n", spaces);//????? needed but idk why

    // Zero out the memory
    //no need we did earlier with rcx fprintf(file, "%smov rcx, rax\n", spaces); // Number of 64-bit elements to store
    fprintf(file, "%sxor rax, rax\n", spaces); // Zero value to store
    
    //fprintf(file, "%sstd\n", spaces);
    fprintf(file, "%srep stosd\n", spaces);
    //fprintf(file,"%scld\n",spaces);
    // //when we improve the speed fprintf(file, "%smov [%s],dword 0;maybe there is a 4byte remainder\n", spaces,rd.left_init_register);
}


//MUST BE CALLED WITH tmp=rcx
static void O1_extend_right(FILE *file,RegisterData rd,int i,int k){
//!!! these 2 lines are single handedly responsible for over a 100x nasm preformance drop
                    fprintf(file, "%scmp %s, %s;check out of tape\n", spaces, rd.address_register,rd.right_limit_register);
                    fprintf(file, "%sja exit_out_of_tape\n", spaces);
                    //!!!!

                    
                    if(strcmp("rcx",rd.tmp)!=0){//using this to avoid a move
                        UNREACHABLE();
                    }

                    fprintf(file,"%s\n;find new bound\n",spaces);

                    fprintf(file, "%slea %s,[%s+%d]\n",spaces,rd.tmp,rd.right_init_register,rd.extend_size);
                    
                    fprintf(file, "%scmp %s,%s\n",spaces,rd.tmp,rd.right_limit_register);

                    if(i<0){
                        fprintf(file, "%sjbe Extend_Right_internal\n", spaces);

                    }
                    else{
                        fprintf(file, "%sjbe Extend_Right_internal_%d_%d\n", spaces,i,k);

                    }
                    fprintf(file, "%smov %s,%s\n",spaces,rd.tmp,rd.right_limit_register);

                    if(i<0){
                        fprintf(file, "Extend_Right_internal:\n");

                    }
                    else{
                        fprintf(file, "Extend_Right_internal_%d_%d:\n",i,k);

                    }


                    //memset 0 
                    // Set rdi to the starting address
                    fprintf(file, "%smov rdi, %s;setting up for stosq \n", spaces, rd.address_register);
                    fprintf(file, "%smov %s, %s\n", spaces, rd.right_init_register, rd.tmp); // Update the rd.right_init_register to the new end

                    // Calculate the number of 32-bit elements to zero out
                    fprintf(file, "%ssub %s, rdi\n", spaces,rd.tmp);
                    fprintf(file, "%sshr %s, 2;bad more effishent to do quads\n", spaces,rd.tmp); // Divide by 8
                    //not sure about this 1fprintf(file, "%ssub %s, 1\n", spaces,rd.tmp);//????? needed but idk why


                    // Zero out the memory
                    //MAJO BUG IN THE ORDER
                    //no need rd.tmp is rcx fprintf(file, "%smov rcx, rax\n", spaces); // Number of 32-bit elements to store
                    fprintf(file, "%sxor rax, rax\n", spaces); // Zero value to store
                    fprintf(file, "%srep stosd\n", spaces);
                    //when we improve the speed fprintf(file, "%smov [%s],dword 0;maybe there is a 4byte remainder\n", spaces,rd.right_init_register);
}

static void print_label(FILE *file,int label){
    if(label!=-1){
         fprintf(file,"L%d",label);
    }
    else{
        fprintf(file,"exit_good");
    }
}


static void O1_move(FILE *file,RegisterData rd,TuringIR ir,int i,int k){
//move
    switch(ir.states[i].trans[k].move){
        case Stay:
            break;
        case Right:
            fprintf(file, "%slea %s, [%s+%d] \n", spaces, rd.address_register, rd.address_register,rd.move_size);
            
            fprintf(file, "%scmp %s, %s;bounds check \n", spaces, rd.address_register,rd.right_init_register);
            fprintf(file, "%sjbe ", spaces); print_label(file,ir.states[i].trans[k].nextState); fprintf(file,"\n");

            fprintf(file, "%scall function_extend_right\n", spaces);
            // rd.tmp = "rcx";
            // O1_extend_right(file,rd,i,k);

            break;
        case Left:
            

            fprintf(file, "%slea %s, [%s-%d] \n", spaces, rd.address_register, rd.address_register,rd.move_size);
            
            fprintf(file, "%scmp %s, %s;bounds check \n", spaces, rd.address_register,rd.left_init_register);
            fprintf(file, "%sjae ", spaces); print_label(file,ir.states[i].trans[k].nextState); fprintf(file,"\n");

            fprintf(file, "%scall function_extend_left\n", spaces);
            // rd.tmp = "rax";
            // O1_extend_left(file,rd,i,k);


            break;
    }
    }

void O1_IR_to_ASM(FILE *file,TuringIR ir){
    
    RegisterData rd = {
        .address_register = "r14",             // this is also used in the assembly_end_template so dont mess with it.
        .bit_register = "r15d",
        .right_limit_register = "r8",
        .left_limit_register = "r9",
        .right_init_register = "r10",
        .left_init_register = "r11",
        .small_right_init_register = "r10d",
        .small_left_init_register = "r11d",
        .move_size = 4,
        .extend_size = 256 * 4                 // same as the interpreter, HAS to be a multiple of 4
    };

    rd.tmp = "rcx";
    rd.tmp2 = "rax";

    inilize_registers(file,rd);
    
    fprintf(file,"%sjmp main_loop\n\n",spaces);

    rd.tmp = "rax";
    fprintf(file,"align 16\n");
    fprintf(file,"function_extend_left:\n");

    O1_extend_left(file,rd,-1,-1);
                   
    fprintf(file,"%sret\n\n",spaces);

    rd.tmp = "rcx";//must be
    fprintf(file,"align 16\n");
    fprintf(file,"function_extend_right:\n");
    
    O1_extend_right(file,rd,-1,-1);

    fprintf(file,"%sret\n\n",spaces);
    
    fprintf(file,"align 16\n");
    fprintf(file,"main_loop:\n");
    for(int i=0;i<ir.len;i++){

        fprintf(file,"L%d:;%s\n",i,ir.names[i]);
        
        
        //check if we need to branch
        int k_start;
        int skip_write;
        //int align=4;

        if(eq_TransIR(ir.states[i].trans[0],ir.states[i].trans[1])){
            fprintf(file,";identical transitions\n");
            fprintf(file,"L%d_0:;%s[0]\n",i,ir.names[i]);
            k_start=1;
            skip_write=0;
        }

        else if(SemiEq_noWrite_TransIR(ir.states[i].trans[0],ir.states[i].trans[1])){
            fprintf(file,";similar transitions\n");
            fprintf(file,"L%d_0:;%s[0]\n",i,ir.names[i]);
            k_start=1;
            skip_write=1;

            if(ir.states[i].trans[0].write!=Bit_0){
                fprintf(file,"%smov %s,dword [%s]\n",spaces,rd.bit_register,rd.address_register);
                fprintf(file,"%sxor %s,dword 1;simple filp \n",spaces,rd.bit_register);
                fprintf(file,"%smov dword [%s],dword %s \n",spaces,rd.address_register,rd.bit_register);
            }
            
            else{
                fprintf(file,"%s;write is a no op\n",spaces);
            }

            
        }

        else{
            fprintf(file,"%s;brench based on current bit\n",spaces);
            fprintf(file,"%smov %s,dword [%s]\n",spaces,rd.bit_register,rd.address_register);
            fprintf(file,"%stest %s, %s\n",spaces,rd.bit_register,rd.bit_register);
            fprintf(file,"%sjnz L%d_1\n\n",spaces,i);    

            k_start=0;
            skip_write=0;
            
        }
        

        for(int k=k_start;k<2;k++){
            fprintf(file,"L%d_%d:;%s[%d]\n",i,k,ir.names[i],k);

            //potentialy skip useless write
            if(!k_start && ir.states[i].trans[k].write==k){
            //if(0){
                fprintf(file,"%s;skiping useless write \n",spaces);
            }
            else if(!skip_write){
                fprintf(file,"%smov [%s],dword %d \n",spaces,rd.address_register,ir.states[i].trans[k].write);
            }
            

            O1_move(file,rd,ir,i,k);

            //next
            if(ir.states[i].trans[k].nextState!=-1){
                //we can potentialy skip the jump
                if(k==1&&ir.states[i].trans[k].nextState==i+1){
                //if(0){
                    fprintf(file,"%s;no need to jump already here\n\n",spaces);
                }
                else{
                    fprintf(file,"%sjmp L%d\n\n",spaces,ir.states[i].trans[k].nextState);
                    //fprintf(file,"align %d\n",align);
                }
                
                
            }
            else{
                if(i==ir.len-1){
                //if(0){
                    fprintf(file,"%s;no need to jump already here\n\n",spaces);
                }
                else{
                    fprintf(file,"%sjmp exit_good\n\n",spaces);
                    //fprintf(file,"align %d\n",align);
                }
                
            }
        }
    }

    //write to the struct
    fprintf(file,"exit_good:\n");
    fprintf(file,"%smov [rsp],qword %s\n",spaces,rd.address_register);

    rd.tmp = "rcx";
    rd.tmp2="rax";
    // Load base address into rd.tmp
    fprintf(file, "%smov %s, qword [rsp+8]\n", spaces, rd.tmp);

    //not handeling the sign right

    //right init
    fprintf(file,"%ssub %s,%s\n",spaces,rd.right_init_register,rd.tmp);
    fprintf(file, "%sshr %s, 2;move to int indexing like c\n", spaces,rd.right_init_register);
    fprintf(file, "%smov [rsp+28], dword %s \n", spaces, rd.small_right_init_register);

    fprintf(file,"%ssub %s,%s\n",spaces,rd.left_init_register,rd.tmp);
    fprintf(file, "%sshr %s, 2;move to int indexing like c\n", spaces,rd.left_init_register);
    fprintf(file, "%smov [rsp+24], dword %s \n", spaces, rd.small_left_init_register);//rd.tmp2_short);
}