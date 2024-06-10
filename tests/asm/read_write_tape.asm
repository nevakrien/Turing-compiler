section .text
    global _start         ; Entry point for the program
    extern ReadTape       ; Declare the external ReadTape function
    extern DumpTape       ; Declare the external DumpTape function

_start:
    ; Ensure there are at least 2 arguments (argc >= 3)
    mov rax, [rsp]         ; Load argc
    cmp rax, 3
    jl _exit               ; If less than 3 arguments, exit

    ; Load the address of the first command-line argument (input file)
    mov rsi, [rsp+16]; First argument (input file) is at [rsp+8]
    ;return for struct adress
    sub rsp, 32
    lea rdi, [rsp]       
    

    call ReadTape          ;


    mov rsi, [rsp+24+32]      ; Second argument (output file)
    lea rdi, [rsp]          ;same struct pointer
    call DumpTape          
    ; Exit the program
    mov rdi, 0             ; Exit code 0
    call _exit

_exit:
    mov rax, 60            ; syscall: exit
    syscall
