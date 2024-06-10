section .text
    global _start
    extern ReadTape
    extern DumpTape
    extern exit_turing
_start:
    ; Ensure there are at least 2 arguments (argc >= 3)
    mov rax, [rsp]         ; Load argc
    cmp rax, 3
    jl _exit_error          ; If less than 3 arguments, exit

    ; Load the address of the first command-line argument (input file)
    mov rsi, [rsp+16]; First argument (input file)
    ;return for struct adress
    sub rsp, 32
    lea rdi, [rsp]       
    

    call ReadTape


    mov rsi, [rsp+24+32]    ; Second argument (output file) now shifted by 32
    lea rdi, [rsp]          ;same struct pointer
    call DumpTape          


    ; Exit the program
    mov rdi, 0
    call exit_turing

_exit_error:
    mov rdi, 3
    call exit_turing