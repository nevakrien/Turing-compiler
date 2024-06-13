section .text
    global _start
    extern ReadTape
    extern DumpTape
    extern exit_turing
exit_out_of_tape:
    mov rdi, 2
    call exit_turing
_start:
    ;initial boiler plate
    ; Ensure there are at least 2 arguments (argc >= 3)
    mov rax, [rsp]         ; Load argc
    cmp rax, 3
    jl _exit_error          ; If less than 3 arguments, exit

    ; Load the address of the first command-line argument (input file)
    mov rsi, [rsp+16]      ; First argument (input file)
    sub rsp, 32
    lea rdi, [rsp]         ; Return struct address

    call ReadTape

    ;!!!ACTUAL CODE: done boiler plate
    mov r14,qword [rsp]
    xor rax, rax
    mov rcx, qword [rsp+8]
    movsxd rax, dword [rsp+20]
    lea r8, [rcx + 4*rax]
    movsxd rax, dword [rsp+16]
    lea r9, [rcx + 4*rax]
    movsxd rax, dword [rsp+24]
    lea r11, [rcx + 4*rax]
    movsxd rax, dword [rsp+28]
    lea r10, [rcx + 4*rax]
    cld
L0:;S0
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L0_1

L0_0:;S0[0]
    mov [r14],dword 0 
    jmp L1

L0_1:;S0[1]
    mov [r14],dword 0 
    jmp L1

L1:;S1
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L1_1

L1_0:;S1[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L1_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L1_0
    mov rcx,r8
Extend_L1_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L1_0:
    jmp L2

L1_1:;S1[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L1_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L1_1
    mov rcx,r8
Extend_L1_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L1_1:
    jmp L2

L2:;S2
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L2_1

L2_0:;S2[0]
    mov [r14],dword 0 
    jmp L3

L2_1:;S2[1]
    mov [r14],dword 0 
    jmp L3

L3:;S3
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L3_1

L3_0:;S3[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L3_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L3_0
    mov rcx,r8
Extend_L3_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L3_0:
    jmp L4

L3_1:;S3[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L3_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L3_1
    mov rcx,r8
Extend_L3_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L3_1:
    jmp L4

L4:;S4
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L4_1

L4_0:;S4[0]
    mov [r14],dword 0 
    jmp L5

L4_1:;S4[1]
    mov [r14],dword 0 
    jmp L5

L5:;S5
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L5_1

L5_0:;S5[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L5_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L5_0
    mov rcx,r8
Extend_L5_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L5_0:
    jmp L6

L5_1:;S5[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L5_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L5_1
    mov rcx,r8
Extend_L5_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L5_1:
    jmp L6

L6:;S6
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L6_1

L6_0:;S6[0]
    mov [r14],dword 0 
    jmp L7

L6_1:;S6[1]
    mov [r14],dword 0 
    jmp L7

L7:;S7
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L7_1

L7_0:;S7[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L7_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L7_0
    mov rcx,r8
Extend_L7_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L7_0:
    jmp L8

L7_1:;S7[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L7_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L7_1
    mov rcx,r8
Extend_L7_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L7_1:
    jmp L8

L8:;S8
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L8_1

L8_0:;S8[0]
    mov [r14],dword 0 
    jmp L9

L8_1:;S8[1]
    mov [r14],dword 0 
    jmp L9

L9:;S9
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L9_1

L9_0:;S9[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L9_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L9_0
    mov rcx,r8
Extend_L9_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L9_0:
    jmp L10

L9_1:;S9[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L9_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L9_1
    mov rcx,r8
Extend_L9_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L9_1:
    jmp L10

L10:;S10
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L10_1

L10_0:;S10[0]
    mov [r14],dword 0 
    jmp L11

L10_1:;S10[1]
    mov [r14],dword 0 
    jmp L11

L11:;S11
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L11_1

L11_0:;S11[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L11_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L11_0
    mov rcx,r8
Extend_L11_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L11_0:
    jmp L12

L11_1:;S11[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L11_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L11_1
    mov rcx,r8
Extend_L11_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L11_1:
    jmp L12

L12:;S12
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L12_1

L12_0:;S12[0]
    mov [r14],dword 0 
    jmp L13

L12_1:;S12[1]
    mov [r14],dword 0 
    jmp L13

L13:;S13
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L13_1

L13_0:;S13[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L13_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L13_0
    mov rcx,r8
Extend_L13_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L13_0:
    jmp L14

L13_1:;S13[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L13_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L13_1
    mov rcx,r8
Extend_L13_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L13_1:
    jmp L14

L14:;S14
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L14_1

L14_0:;S14[0]
    mov [r14],dword 0 
    jmp L15

L14_1:;S14[1]
    mov [r14],dword 0 
    jmp L15

L15:;S15
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L15_1

L15_0:;S15[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L15_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L15_0
    mov rcx,r8
Extend_L15_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L15_0:
    jmp L16

L15_1:;S15[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L15_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L15_1
    mov rcx,r8
Extend_L15_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L15_1:
    jmp L16

L16:;S16
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L16_1

L16_0:;S16[0]
    mov [r14],dword 0 
    jmp L17

L16_1:;S16[1]
    mov [r14],dword 0 
    jmp L17

L17:;S17
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L17_1

L17_0:;S17[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L17_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L17_0
    mov rcx,r8
Extend_L17_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L17_0:
    jmp L18

L17_1:;S17[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L17_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L17_1
    mov rcx,r8
Extend_L17_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L17_1:
    jmp L18

L18:;S18
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L18_1

L18_0:;S18[0]
    mov [r14],dword 0 
    jmp L19

L18_1:;S18[1]
    mov [r14],dword 0 
    jmp L19

L19:;S19
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L19_1

L19_0:;S19[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L19_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L19_0
    mov rcx,r8
Extend_L19_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L19_0:
    jmp L20

L19_1:;S19[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L19_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L19_1
    mov rcx,r8
Extend_L19_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L19_1:
    jmp L20

L20:;S20
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L20_1

L20_0:;S20[0]
    mov [r14],dword 0 
    jmp L21

L20_1:;S20[1]
    mov [r14],dword 0 
    jmp L21

L21:;S21
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L21_1

L21_0:;S21[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L21_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L21_0
    mov rcx,r8
Extend_L21_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L21_0:
    jmp L22

L21_1:;S21[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L21_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L21_1
    mov rcx,r8
Extend_L21_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L21_1:
    jmp L22

L22:;S22
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L22_1

L22_0:;S22[0]
    mov [r14],dword 0 
    jmp L23

L22_1:;S22[1]
    mov [r14],dword 0 
    jmp L23

L23:;S23
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L23_1

L23_0:;S23[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L23_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L23_0
    mov rcx,r8
Extend_L23_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L23_0:
    jmp L24

L23_1:;S23[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L23_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L23_1
    mov rcx,r8
Extend_L23_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L23_1:
    jmp L24

L24:;S24
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L24_1

L24_0:;S24[0]
    mov [r14],dword 0 
    jmp L25

L24_1:;S24[1]
    mov [r14],dword 0 
    jmp L25

L25:;S25
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L25_1

L25_0:;S25[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L25_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L25_0
    mov rcx,r8
Extend_L25_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L25_0:
    jmp L26

L25_1:;S25[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L25_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L25_1
    mov rcx,r8
Extend_L25_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L25_1:
    jmp L26

L26:;S26
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L26_1

L26_0:;S26[0]
    mov [r14],dword 0 
    jmp L27

L26_1:;S26[1]
    mov [r14],dword 0 
    jmp L27

L27:;S27
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L27_1

L27_0:;S27[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L27_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L27_0
    mov rcx,r8
Extend_L27_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L27_0:
    jmp L28

L27_1:;S27[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L27_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L27_1
    mov rcx,r8
Extend_L27_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L27_1:
    jmp L28

L28:;S28
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L28_1

L28_0:;S28[0]
    mov [r14],dword 0 
    jmp L29

L28_1:;S28[1]
    mov [r14],dword 0 
    jmp L29

L29:;S29
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L29_1

L29_0:;S29[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L29_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L29_0
    mov rcx,r8
Extend_L29_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L29_0:
    jmp L30

L29_1:;S29[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L29_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L29_1
    mov rcx,r8
Extend_L29_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L29_1:
    jmp L30

L30:;S30
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L30_1

L30_0:;S30[0]
    mov [r14],dword 0 
    jmp L31

L30_1:;S30[1]
    mov [r14],dword 0 
    jmp L31

L31:;S31
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L31_1

L31_0:;S31[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L31_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L31_0
    mov rcx,r8
Extend_L31_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L31_0:
    jmp L32

L31_1:;S31[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L31_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L31_1
    mov rcx,r8
Extend_L31_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L31_1:
    jmp L32

L32:;S32
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L32_1

L32_0:;S32[0]
    mov [r14],dword 0 
    jmp L33

L32_1:;S32[1]
    mov [r14],dword 0 
    jmp L33

L33:;S33
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L33_1

L33_0:;S33[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L33_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L33_0
    mov rcx,r8
Extend_L33_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L33_0:
    jmp L34

L33_1:;S33[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L33_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L33_1
    mov rcx,r8
Extend_L33_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L33_1:
    jmp L34

L34:;S34
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L34_1

L34_0:;S34[0]
    mov [r14],dword 0 
    jmp L35

L34_1:;S34[1]
    mov [r14],dword 0 
    jmp L35

L35:;S35
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L35_1

L35_0:;S35[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L35_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L35_0
    mov rcx,r8
Extend_L35_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L35_0:
    jmp L36

L35_1:;S35[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L35_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L35_1
    mov rcx,r8
Extend_L35_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L35_1:
    jmp L36

L36:;S36
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L36_1

L36_0:;S36[0]
    mov [r14],dword 0 
    jmp L37

L36_1:;S36[1]
    mov [r14],dword 0 
    jmp L37

L37:;S37
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L37_1

L37_0:;S37[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L37_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L37_0
    mov rcx,r8
Extend_L37_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L37_0:
    jmp L38

L37_1:;S37[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L37_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L37_1
    mov rcx,r8
Extend_L37_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L37_1:
    jmp L38

L38:;S38
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L38_1

L38_0:;S38[0]
    mov [r14],dword 0 
    jmp L39

L38_1:;S38[1]
    mov [r14],dword 0 
    jmp L39

L39:;S39
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L39_1

L39_0:;S39[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L39_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L39_0
    mov rcx,r8
Extend_L39_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L39_0:
    jmp L40

L39_1:;S39[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L39_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L39_1
    mov rcx,r8
Extend_L39_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L39_1:
    jmp L40

L40:;S40
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L40_1

L40_0:;S40[0]
    mov [r14],dword 0 
    jmp L41

L40_1:;S40[1]
    mov [r14],dword 0 
    jmp L41

L41:;S41
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L41_1

L41_0:;S41[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L41_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L41_0
    mov rcx,r8
Extend_L41_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L41_0:
    jmp L42

L41_1:;S41[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L41_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L41_1
    mov rcx,r8
Extend_L41_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L41_1:
    jmp L42

L42:;S42
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L42_1

L42_0:;S42[0]
    mov [r14],dword 0 
    jmp L43

L42_1:;S42[1]
    mov [r14],dword 0 
    jmp L43

L43:;S43
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L43_1

L43_0:;S43[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L43_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L43_0
    mov rcx,r8
Extend_L43_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L43_0:
    jmp L44

L43_1:;S43[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L43_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L43_1
    mov rcx,r8
Extend_L43_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L43_1:
    jmp L44

L44:;S44
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L44_1

L44_0:;S44[0]
    mov [r14],dword 0 
    jmp L45

L44_1:;S44[1]
    mov [r14],dword 0 
    jmp L45

L45:;S45
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L45_1

L45_0:;S45[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L45_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L45_0
    mov rcx,r8
Extend_L45_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L45_0:
    jmp L46

L45_1:;S45[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L45_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L45_1
    mov rcx,r8
Extend_L45_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L45_1:
    jmp L46

L46:;S46
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L46_1

L46_0:;S46[0]
    mov [r14],dword 0 
    jmp L47

L46_1:;S46[1]
    mov [r14],dword 0 
    jmp L47

L47:;S47
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L47_1

L47_0:;S47[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L47_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L47_0
    mov rcx,r8
Extend_L47_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L47_0:
    jmp L48

L47_1:;S47[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L47_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L47_1
    mov rcx,r8
Extend_L47_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L47_1:
    jmp L48

L48:;S48
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L48_1

L48_0:;S48[0]
    mov [r14],dword 0 
    jmp L49

L48_1:;S48[1]
    mov [r14],dword 0 
    jmp L49

L49:;S49
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L49_1

L49_0:;S49[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L49_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L49_0
    mov rcx,r8
Extend_L49_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L49_0:
    jmp L50

L49_1:;S49[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L49_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L49_1
    mov rcx,r8
Extend_L49_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L49_1:
    jmp L50

L50:;S50
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L50_1

L50_0:;S50[0]
    mov [r14],dword 0 
    jmp L51

L50_1:;S50[1]
    mov [r14],dword 0 
    jmp L51

L51:;S51
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L51_1

L51_0:;S51[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L51_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L51_0
    mov rcx,r8
Extend_L51_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L51_0:
    jmp L52

L51_1:;S51[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L51_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L51_1
    mov rcx,r8
Extend_L51_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L51_1:
    jmp L52

L52:;S52
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L52_1

L52_0:;S52[0]
    mov [r14],dword 0 
    jmp L53

L52_1:;S52[1]
    mov [r14],dword 0 
    jmp L53

L53:;S53
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L53_1

L53_0:;S53[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L53_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L53_0
    mov rcx,r8
Extend_L53_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L53_0:
    jmp L54

L53_1:;S53[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L53_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L53_1
    mov rcx,r8
Extend_L53_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L53_1:
    jmp L54

L54:;S54
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L54_1

L54_0:;S54[0]
    mov [r14],dword 0 
    jmp L55

L54_1:;S54[1]
    mov [r14],dword 0 
    jmp L55

L55:;S55
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L55_1

L55_0:;S55[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L55_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L55_0
    mov rcx,r8
Extend_L55_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L55_0:
    jmp L56

L55_1:;S55[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L55_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L55_1
    mov rcx,r8
Extend_L55_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L55_1:
    jmp L56

L56:;S56
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L56_1

L56_0:;S56[0]
    mov [r14],dword 0 
    jmp L57

L56_1:;S56[1]
    mov [r14],dword 0 
    jmp L57

L57:;S57
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L57_1

L57_0:;S57[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L57_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L57_0
    mov rcx,r8
Extend_L57_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L57_0:
    jmp L58

L57_1:;S57[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L57_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L57_1
    mov rcx,r8
Extend_L57_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L57_1:
    jmp L58

L58:;S58
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L58_1

L58_0:;S58[0]
    mov [r14],dword 0 
    jmp L59

L58_1:;S58[1]
    mov [r14],dword 0 
    jmp L59

L59:;S59
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L59_1

L59_0:;S59[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L59_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L59_0
    mov rcx,r8
Extend_L59_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L59_0:
    jmp L60

L59_1:;S59[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L59_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L59_1
    mov rcx,r8
Extend_L59_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L59_1:
    jmp L60

L60:;S60
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L60_1

L60_0:;S60[0]
    mov [r14],dword 0 
    jmp L61

L60_1:;S60[1]
    mov [r14],dword 0 
    jmp L61

L61:;S61
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L61_1

L61_0:;S61[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L61_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L61_0
    mov rcx,r8
Extend_L61_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L61_0:
    jmp L62

L61_1:;S61[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L61_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L61_1
    mov rcx,r8
Extend_L61_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L61_1:
    jmp L62

L62:;S62
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L62_1

L62_0:;S62[0]
    mov [r14],dword 0 
    jmp L63

L62_1:;S62[1]
    mov [r14],dword 0 
    jmp L63

L63:;S63
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L63_1

L63_0:;S63[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L63_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L63_0
    mov rcx,r8
Extend_L63_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L63_0:
    jmp L64

L63_1:;S63[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L63_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L63_1
    mov rcx,r8
Extend_L63_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L63_1:
    jmp L64

L64:;S64
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L64_1

L64_0:;S64[0]
    mov [r14],dword 0 
    jmp L65

L64_1:;S64[1]
    mov [r14],dword 0 
    jmp L65

L65:;S65
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L65_1

L65_0:;S65[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L65_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L65_0
    mov rcx,r8
Extend_L65_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L65_0:
    jmp L66

L65_1:;S65[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L65_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L65_1
    mov rcx,r8
Extend_L65_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L65_1:
    jmp L66

L66:;S66
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L66_1

L66_0:;S66[0]
    mov [r14],dword 0 
    jmp L67

L66_1:;S66[1]
    mov [r14],dword 0 
    jmp L67

L67:;S67
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L67_1

L67_0:;S67[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L67_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L67_0
    mov rcx,r8
Extend_L67_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L67_0:
    jmp L68

L67_1:;S67[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L67_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L67_1
    mov rcx,r8
Extend_L67_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L67_1:
    jmp L68

L68:;S68
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L68_1

L68_0:;S68[0]
    mov [r14],dword 0 
    jmp L69

L68_1:;S68[1]
    mov [r14],dword 0 
    jmp L69

L69:;S69
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L69_1

L69_0:;S69[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L69_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L69_0
    mov rcx,r8
Extend_L69_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L69_0:
    jmp L70

L69_1:;S69[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L69_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L69_1
    mov rcx,r8
Extend_L69_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L69_1:
    jmp L70

L70:;S70
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L70_1

L70_0:;S70[0]
    mov [r14],dword 0 
    jmp L71

L70_1:;S70[1]
    mov [r14],dword 0 
    jmp L71

L71:;S71
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L71_1

L71_0:;S71[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L71_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L71_0
    mov rcx,r8
Extend_L71_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L71_0:
    jmp L72

L71_1:;S71[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L71_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L71_1
    mov rcx,r8
Extend_L71_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L71_1:
    jmp L72

L72:;S72
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L72_1

L72_0:;S72[0]
    mov [r14],dword 0 
    jmp L73

L72_1:;S72[1]
    mov [r14],dword 0 
    jmp L73

L73:;S73
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L73_1

L73_0:;S73[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L73_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L73_0
    mov rcx,r8
Extend_L73_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L73_0:
    jmp L74

L73_1:;S73[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L73_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L73_1
    mov rcx,r8
Extend_L73_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L73_1:
    jmp L74

L74:;S74
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L74_1

L74_0:;S74[0]
    mov [r14],dword 0 
    jmp L75

L74_1:;S74[1]
    mov [r14],dword 0 
    jmp L75

L75:;S75
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L75_1

L75_0:;S75[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L75_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L75_0
    mov rcx,r8
Extend_L75_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L75_0:
    jmp L76

L75_1:;S75[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L75_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L75_1
    mov rcx,r8
Extend_L75_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L75_1:
    jmp L76

L76:;S76
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L76_1

L76_0:;S76[0]
    mov [r14],dword 0 
    jmp L77

L76_1:;S76[1]
    mov [r14],dword 0 
    jmp L77

L77:;S77
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L77_1

L77_0:;S77[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L77_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L77_0
    mov rcx,r8
Extend_L77_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L77_0:
    jmp L78

L77_1:;S77[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L77_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L77_1
    mov rcx,r8
Extend_L77_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L77_1:
    jmp L78

L78:;S78
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L78_1

L78_0:;S78[0]
    mov [r14],dword 0 
    jmp L79

L78_1:;S78[1]
    mov [r14],dword 0 
    jmp L79

L79:;S79
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L79_1

L79_0:;S79[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L79_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L79_0
    mov rcx,r8
Extend_L79_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L79_0:
    jmp L80

L79_1:;S79[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L79_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L79_1
    mov rcx,r8
Extend_L79_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L79_1:
    jmp L80

L80:;S80
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L80_1

L80_0:;S80[0]
    mov [r14],dword 0 
    jmp L81

L80_1:;S80[1]
    mov [r14],dword 0 
    jmp L81

L81:;S81
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L81_1

L81_0:;S81[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L81_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L81_0
    mov rcx,r8
Extend_L81_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L81_0:
    jmp L82

L81_1:;S81[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L81_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L81_1
    mov rcx,r8
Extend_L81_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L81_1:
    jmp L82

L82:;S82
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L82_1

L82_0:;S82[0]
    mov [r14],dword 0 
    jmp L83

L82_1:;S82[1]
    mov [r14],dword 0 
    jmp L83

L83:;S83
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L83_1

L83_0:;S83[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L83_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L83_0
    mov rcx,r8
Extend_L83_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L83_0:
    jmp L84

L83_1:;S83[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L83_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L83_1
    mov rcx,r8
Extend_L83_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L83_1:
    jmp L84

L84:;S84
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L84_1

L84_0:;S84[0]
    mov [r14],dword 0 
    jmp L85

L84_1:;S84[1]
    mov [r14],dword 0 
    jmp L85

L85:;S85
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L85_1

L85_0:;S85[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L85_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L85_0
    mov rcx,r8
Extend_L85_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L85_0:
    jmp L86

L85_1:;S85[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L85_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L85_1
    mov rcx,r8
Extend_L85_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L85_1:
    jmp L86

L86:;S86
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L86_1

L86_0:;S86[0]
    mov [r14],dword 0 
    jmp L87

L86_1:;S86[1]
    mov [r14],dword 0 
    jmp L87

L87:;S87
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L87_1

L87_0:;S87[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L87_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L87_0
    mov rcx,r8
Extend_L87_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L87_0:
    jmp L88

L87_1:;S87[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L87_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L87_1
    mov rcx,r8
Extend_L87_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L87_1:
    jmp L88

L88:;S88
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L88_1

L88_0:;S88[0]
    mov [r14],dword 0 
    jmp L89

L88_1:;S88[1]
    mov [r14],dword 0 
    jmp L89

L89:;S89
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L89_1

L89_0:;S89[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L89_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L89_0
    mov rcx,r8
Extend_L89_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L89_0:
    jmp L90

L89_1:;S89[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L89_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L89_1
    mov rcx,r8
Extend_L89_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L89_1:
    jmp L90

L90:;S90
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L90_1

L90_0:;S90[0]
    mov [r14],dword 0 
    jmp L91

L90_1:;S90[1]
    mov [r14],dword 0 
    jmp L91

L91:;S91
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L91_1

L91_0:;S91[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L91_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L91_0
    mov rcx,r8
Extend_L91_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L91_0:
    jmp L92

L91_1:;S91[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L91_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L91_1
    mov rcx,r8
Extend_L91_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L91_1:
    jmp L92

L92:;S92
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L92_1

L92_0:;S92[0]
    mov [r14],dword 0 
    jmp L93

L92_1:;S92[1]
    mov [r14],dword 0 
    jmp L93

L93:;S93
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L93_1

L93_0:;S93[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L93_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L93_0
    mov rcx,r8
Extend_L93_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L93_0:
    jmp L94

L93_1:;S93[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L93_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L93_1
    mov rcx,r8
Extend_L93_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L93_1:
    jmp L94

L94:;S94
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L94_1

L94_0:;S94[0]
    mov [r14],dword 0 
    jmp L95

L94_1:;S94[1]
    mov [r14],dword 0 
    jmp L95

L95:;S95
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L95_1

L95_0:;S95[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L95_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L95_0
    mov rcx,r8
Extend_L95_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L95_0:
    jmp L96

L95_1:;S95[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L95_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L95_1
    mov rcx,r8
Extend_L95_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L95_1:
    jmp L96

L96:;S96
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L96_1

L96_0:;S96[0]
    mov [r14],dword 0 
    jmp L97

L96_1:;S96[1]
    mov [r14],dword 0 
    jmp L97

L97:;S97
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L97_1

L97_0:;S97[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L97_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L97_0
    mov rcx,r8
Extend_L97_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L97_0:
    jmp L98

L97_1:;S97[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L97_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L97_1
    mov rcx,r8
Extend_L97_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L97_1:
    jmp L98

L98:;S98
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L98_1

L98_0:;S98[0]
    mov [r14],dword 0 
    jmp L99

L98_1:;S98[1]
    mov [r14],dword 0 
    jmp L99

L99:;S99
    mov r15d,dword [r14]
    test r15d, r15d
    jnz L99_1

L99_0:;S99[0]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L99_0
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L99_0
    mov rcx,r8
Extend_L99_0:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L99_0:
    jmp exit_good

L99_1:;S99[1]
    mov [r14],dword 1 
    lea r14, [r14+4] 
    cmp r14, r10;bounds check 
    jbe Done_L99_1
    cmp r14, r8;check out of tape
    ja exit_out_of_tape
    
;find new bound
    lea rcx,[r10+1024]
    cmp rcx,r8
    jbe Extend_L99_1
    mov rcx,r8
Extend_L99_1:
    mov rdi, r14;setting up for stosq 
    mov r10, rcx
    sub rcx, rdi
    shr rcx, 2;bad more effishent to do quads
    sub rcx, 1
    xor rax, rax
    rep stosd
Done_L99_1:
    jmp exit_good

exit_good:
    mov [rsp],qword r14
    mov rcx, qword [rsp+8]
    sub r10,rcx
    sar r10, 2;move to int indexing like c
    mov eax, dword r10d;sign handeling 
    mov [rsp+28], dword eax 
    sub r11,rcx
    sar r11, 2;move to int indexing like c
    mov eax, dword r11d;sign handeling 
    mov [rsp+24], dword eax 
    ;DONE:output boilerplate and exit;

    mov rsi, [rsp+32+24]   ; Second argument (output file) now shifted by 32
    lea rdi, [rsp]         ; Same struct pointer

    call DumpTape

    ; Exit the program
    mov rdi, 0
    call exit_turing


_exit_error:
    mov rdi, 3
    call exit_turing
