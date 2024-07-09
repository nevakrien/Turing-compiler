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
    jl _exit_bad_args          ; If less than 3 arguments, exit

    ; Load the address of the first command-line argument (input file)
    mov rsi, [rsp+16]      ; First argument (input file)
    sub rsp, 32
    lea rdi, [rsp]         ; Return struct address

    call ReadTape

    ;!!!ACTUAL CODE: done boiler plate
    mov r14, qword [rsp] ;cur
    mov rbx, qword [rsp+8] ;base
    movsxd rax, dword [rsp+20]
    lea r8, [rbx + 4*rax] ;right limit
    movsxd rax, dword [rsp+16]
    lea r9, [rbx + 4*rax] ;left limit
    movsxd rax, dword [rsp+24]
    lea r11, [rbx + 4*rax] ;left initilized
    movsxd rax, dword [rsp+28]
    lea r10, [rbx + 4*rax] ;right initilized
L0: ;Write1_L1
;Write
    mov [r14],dword 1
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L0_1

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L0_2

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L0_3

L0_2:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L0_3:;joined logic

    xor rax,rax
    rep stosd
L0_1:;done bounds check

;StateEnd
    jmp L1
L1: ;Write1_L2
;Write
    mov [r14],dword 1
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L1_1

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L1_2

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L1_3

L1_2:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L1_3:;joined logic

    xor rax,rax
    rep stosd
L1_1:;done bounds check

;StateEnd
    jmp L2
L2: ;Write1_L3
;Write
    mov [r14],dword 1
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L2_1

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L2_2

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L2_3

L2_2:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L2_3:;joined logic

    xor rax,rax
    rep stosd
L2_1:;done bounds check

;StateEnd
    jmp L3
L3: ;Write1_L4
;Write
    mov [r14],dword 1
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L3_1

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L3_2

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L3_3

L3_2:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L3_3:;joined logic

    xor rax,rax
    rep stosd
L3_1:;done bounds check

;StateEnd
    jmp L4
L4: ;Write1_L5
;Write
    mov [r14],dword 1
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L4_1

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L4_2

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L4_3

L4_2:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L4_3:;joined logic

    xor rax,rax
    rep stosd
L4_1:;done bounds check

;StateEnd
    jmp L5
L5: ;Return_R1
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L5_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L5_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L5_3

L5_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L5_3:;joined logic

    xor rax,rax
    rep stosd
L5_1:;done bounds check

;StateEnd
    jmp L6
L6: ;Return_R2
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L6_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L6_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L6_3

L6_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L6_3:;joined logic

    xor rax,rax
    rep stosd
L6_1:;done bounds check

;StateEnd
    jmp L7
L7: ;Return_R3
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L7_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L7_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L7_3

L7_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L7_3:;joined logic

    xor rax,rax
    rep stosd
L7_1:;done bounds check

;StateEnd
    jmp L8
L8: ;Return_R4
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L8_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L8_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L8_3

L8_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L8_3:;joined logic

    xor rax,rax
    rep stosd
L8_1:;done bounds check

;StateEnd
    jmp L9
L9: ;Return_R5
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L9_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L9_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L9_3

L9_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L9_3:;joined logic

    xor rax,rax
    rep stosd
L9_1:;done bounds check

;StateEnd
    jmp L10
L10: ;Initialize_Right
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L10_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L10_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L10_3

L10_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L10_3:;joined logic

    xor rax,rax
    rep stosd
L10_1:;done bounds check

;StateEnd
    jmp L11
L11: ;Start_Copy
;Write
    mov [r14],dword 1
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L11_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L11_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L11_3

L11_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L11_3:;joined logic

    xor rax,rax
    rep stosd
L11_1:;done bounds check

;StateEnd
    jmp L12
L12: ;Decrement_Left
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L12_1

;Write
    mov [r14],dword 0
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L12_2

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L12_3

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L12_4

L12_3:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L12_4:;joined logic

    xor rax,rax
    rep stosd
L12_2:;done bounds check

;StateEnd
    jmp L13
L12_1:
;Write
    mov [r14],dword 0
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L12_5

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L12_6

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L12_7

L12_6:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L12_7:;joined logic

    xor rax,rax
    rep stosd
L12_5:;done bounds check

;StateEnd
    jmp L14
L13: ;Halt_Check
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L13_1

;Write
    mov [r14],dword 0
;Exit
    jmp exit_good

L13_1:
;Write
    mov [r14],dword 1
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L13_2

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L13_3

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L13_4

L13_3:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L13_4:;joined logic

    xor rax,rax
    rep stosd
L13_2:;done bounds check

;StateEnd
    jmp L14
L14: ;Move_Left
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L14_1

;Write
    mov [r14],dword 0
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L14_2

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L14_3

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L14_4

L14_3:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L14_4:;joined logic

    xor rax,rax
    rep stosd
L14_2:;done bounds check

;StateEnd
    jmp L15
L14_1:
;Write
    mov [r14],dword 1
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L14_5

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L14_6

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L14_7

L14_6:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L14_7:;joined logic

    xor rax,rax
    rep stosd
L14_5:;done bounds check

;StateEnd
    jmp L14
L15: ;Find_Right_Tape
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L15_1

;Write
    mov [r14],dword 0
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L15_2

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L15_3

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L15_4

L15_3:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L15_4:;joined logic

    xor rax,rax
    rep stosd
L15_2:;done bounds check

;StateEnd
    jmp L16
L15_1:
;Write
    mov [r14],dword 1
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L15_5

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L15_6

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L15_7

L15_6:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L15_7:;joined logic

    xor rax,rax
    rep stosd
L15_5:;done bounds check

;StateEnd
    jmp L15
L16: ;Write_One
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L16_1

;Write
    mov [r14],dword 1
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L16_2

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L16_3

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L16_4

L16_3:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L16_4:;joined logic

    xor rax,rax
    rep stosd
L16_2:;done bounds check

;StateEnd
    jmp L17
L16_1:
;Write
    mov [r14],dword 1
;Move
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L16_5

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L16_6

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L16_7

L16_6:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L16_7:;joined logic

    xor rax,rax
    rep stosd
L16_5:;done bounds check

;StateEnd
    jmp L16
L17: ;Return_Left
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L17_1

;Write
    mov [r14],dword 0
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L17_2

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L17_3

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L17_4

L17_3:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L17_4:;joined logic

    xor rax,rax
    rep stosd
L17_2:;done bounds check

;StateEnd
    jmp L12
L17_1:
;Write
    mov [r14],dword 1
;Move
    add r14, -4
    cmp r14, r11;bounds check init
    jae L17_5

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L17_6

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L17_7

L17_6:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L17_7:;joined logic

    xor rax,rax
    rep stosd
L17_5:;done bounds check

;StateEnd
    jmp L17
exit_good:
    mov [rsp],qword r14;writing current adress
    mov rax, qword [rsp+8];loading base
    ;moving right_init to int index
    sub r10,rax
    shr r10, 2
    mov [rsp+28], dword r10d ;storing it
    ;moving left_init to int index
    sub r11,rax
    shr r11, 2
    mov [rsp+24], dword r11d 
    ;DONE:output boilerplate and exit;

    mov rsi, [rsp+32+24]   ; Second argument (output file) now shifted by 32
    lea rdi, [rsp]         ; Same struct pointer

    call DumpTape

    ; Exit the program
    mov rdi, 0
    call exit_turing


_exit_bad_args:
    mov rdi, 3
    call exit_turing
