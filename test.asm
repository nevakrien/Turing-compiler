section .data
    ; No data section needed for this simple test

section .bss
    ; No uninitialized data needed for this simple test

section .text
    global _start

_start:
    ; Initialize rax and rdi with some test values
    mov rax, 10
    mov rdi, 3

    ; Perform the LEA operation: rcx = rax - rdi + 4
    lea rcx, [rax + rdi+4+2]

    ; Exit the program
    mov rax, 60         ; syscall: exit
    xor rdi, rdi        ; status: 0
    syscall
