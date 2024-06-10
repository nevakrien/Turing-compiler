#include <stdio.h>
#include <stdlib.h>

const char* assembly_template = "section .text\n"
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
                                "%s\n" // Placeholder for actual code
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

int main()
{
    // Step 1: Generate the assembly code
    FILE* file = fopen("generated.asm", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // Custom code to be inserted into the placeholder
    const char* custom_code = "    ; Inserted custom code";

    fprintf(file, assembly_template, custom_code);

    fclose(file);
    printf("Assembly code generated successfully.\n");

    // Step 2: Assemble the generated assembly code
    int result = system("nasm -f elf64 -o generated.o generated.asm");
    if (result != 0) {
        fprintf(stderr, "Failed to assemble the code.\n");
        return 1;
    }
    printf("Assembly completed successfully.\n");

    // Step 3: Link the object file with io.o to create the final executable
    result = system("ld -o generated.out generated.o bin/io.o -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2");
    if (result != 0) {
        fprintf(stderr, "Failed to link the object file.\n");
        return 1;
    }
    printf("Linking completed successfully.\n");

    return 0;
}
