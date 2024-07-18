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
    mov r12, qword [rsp+8] ;base
    movsxd r13, dword [rsp+20]
    lea r8, [r12 + 4*r13] ;right limit
    movsxd r13, dword [rsp+16]
    lea r9, [r12 + 4*r13] ;left limit
    movsxd r13, dword [rsp+24]
    lea r11, [r12 + 4*r13] ;left initilized
    movsxd r13, dword [rsp+28]
    lea r10, [r12 + 4*r13] ;right initilized
L0: ;Write1_L1
;LinearFuse
    mov r13, r14
    add r13, 8
    cmp r13, r10;bounds check init
    jbe L0_1

    add r10, 1024;optimistic new bounds
    cmp r10, r8
    jbe L0_2

    cmp r13, r8
    ja exit_out_of_tape
    lea rdi,[r10-1024+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1024-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L0_3

L0_2:;easy case no re-adjustment
    mov rcx,1024
    lea rdi,[r10-1024+4]
L0_3:;joined logic

    xor rax,rax
    rep stosd
L0_1:;done bounds check

    mov r13, r14
    add r13, 8
    cmp r13, r10;bounds check init
    jbe L0_4

    add r10, 1024;optimistic new bounds
    cmp r10, r8
    jbe L0_5

    cmp r13, r8
    ja exit_out_of_tape
    lea rdi,[r10-1024+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1024-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L0_6

L0_5:;easy case no re-adjustment
    mov rcx,1024
    lea rdi,[r10-1024+4]
L0_6:;joined logic

    xor rax,rax
    rep stosd
L0_4:;done bounds check

    add r14, 8
    ;write ops
    mov [r14 + -8], dword 1
    

;HistoryNode
;StateEnd
    jmp L12
L12: ;Decrement_Left
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L12_1

;LinearFuse
    add r14, -4
    cmp r14, r11;bounds check init
    jae L12_2

    add r11, -1024;optimistic new bounds
    cmp r11, r9
    jae L12_3

    cmp r14, r9
    jb exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1024]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L12_4

L12_3:;easy case no re-adjustment
    mov rcx,1024
    mov rdi,r11
L12_4:;joined logic

    xor rax,rax
    rep stosd
L12_2:;done bounds check

    ;write ops
    mov [r14 + 4], dword 0
    

;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L12_5

;LinearFuse
    add r14, 0
    ;write ops
    mov [r14 + 0], dword 0
    

;HistoryNode
;Exit
    jmp exit_good

L12_5:
;LinearFuse
    add r14, -4
    cmp r14, r11;bounds check init
    jae L12_6

    add r11, -1024;optimistic new bounds
    cmp r11, r9
    jae L12_7

    cmp r14, r9
    jb exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1024]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L12_8

L12_7:;easy case no re-adjustment
    mov rcx,1024
    mov rdi,r11
L12_8:;joined logic

    xor rax,rax
    rep stosd
L12_6:;done bounds check

    ;write ops
    mov [r14 + 4], dword 1
    

;HistoryNode
;StateEnd
    jmp L14
L12_1:
;LinearFuse
    add r14, -4
    cmp r14, r11;bounds check init
    jae L12_9

    add r11, -1024;optimistic new bounds
    cmp r11, r9
    jae L12_10

    cmp r14, r9
    jb exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1024]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L12_11

L12_10:;easy case no re-adjustment
    mov rcx,1024
    mov rdi,r11
L12_11:;joined logic

    xor rax,rax
    rep stosd
L12_9:;done bounds check

    ;write ops
    mov [r14 + 4], dword 0
    

;HistoryNode
;StateEnd
    jmp L14
L14: ;Move_Left
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L14_1

;LinearFuse
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L14_2

    add r10, 1024;optimistic new bounds
    cmp r10, r8
    jbe L14_3

    cmp r14, r8
    ja exit_out_of_tape
    lea rdi,[r10-1024+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1024-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L14_4

L14_3:;easy case no re-adjustment
    mov rcx,1024
    lea rdi,[r10-1024+4]
L14_4:;joined logic

    xor rax,rax
    rep stosd
L14_2:;done bounds check

    ;write ops
    mov [r14 + -4], dword 0
    

;HistoryNode
;StateEnd
    jmp L15
L14_1:
;LinearFuse
    add r14, -4
    cmp r14, r11;bounds check init
    jae L14_5

    add r11, -1024;optimistic new bounds
    cmp r11, r9
    jae L14_6

    cmp r14, r9
    jb exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1024]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L14_7

L14_6:;easy case no re-adjustment
    mov rcx,1024
    mov rdi,r11
L14_7:;joined logic

    xor rax,rax
    rep stosd
L14_5:;done bounds check

    ;write ops
    mov [r14 + 4], dword 1
    

;HistoryNode
;StateEnd
    jmp L14
L15: ;Find_Right_Tape
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L15_1

;LinearFuse
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L15_2

    add r10, 1024;optimistic new bounds
    cmp r10, r8
    jbe L15_3

    cmp r14, r8
    ja exit_out_of_tape
    lea rdi,[r10-1024+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1024-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L15_4

L15_3:;easy case no re-adjustment
    mov rcx,1024
    lea rdi,[r10-1024+4]
L15_4:;joined logic

    xor rax,rax
    rep stosd
L15_2:;done bounds check

    ;write ops
    mov [r14 + -4], dword 0
    

;HistoryNode
;StateEnd
    jmp L16
L15_1:
;LinearFuse
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L15_5

    add r10, 1024;optimistic new bounds
    cmp r10, r8
    jbe L15_6

    cmp r14, r8
    ja exit_out_of_tape
    lea rdi,[r10-1024+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1024-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L15_7

L15_6:;easy case no re-adjustment
    mov rcx,1024
    lea rdi,[r10-1024+4]
L15_7:;joined logic

    xor rax,rax
    rep stosd
L15_5:;done bounds check

    ;write ops
    mov [r14 + -4], dword 1
    

;HistoryNode
;StateEnd
    jmp L15
L16: ;Write_One
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L16_1

;LinearFuse
    add r14, -4
    cmp r14, r11;bounds check init
    jae L16_2

    add r11, -1024;optimistic new bounds
    cmp r11, r9
    jae L16_3

    cmp r14, r9
    jb exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1024]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L16_4

L16_3:;easy case no re-adjustment
    mov rcx,1024
    mov rdi,r11
L16_4:;joined logic

    xor rax,rax
    rep stosd
L16_2:;done bounds check

    ;write ops
    mov [r14 + 4], dword 1
    

;HistoryNode
;StateEnd
    jmp L17
L16_1:
;LinearFuse
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L16_5

    add r10, 1024;optimistic new bounds
    cmp r10, r8
    jbe L16_6

    cmp r14, r8
    ja exit_out_of_tape
    lea rdi,[r10-1024+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1024-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L16_7

L16_6:;easy case no re-adjustment
    mov rcx,1024
    lea rdi,[r10-1024+4]
L16_7:;joined logic

    xor rax,rax
    rep stosd
L16_5:;done bounds check

    ;write ops
    mov [r14 + -4], dword 1
    

;HistoryNode
;StateEnd
    jmp L16
L17: ;Return_Left
;Split
    ;spliting
    mov r15d, [r14]
    test r15, r15
    jnz L17_1

;LinearFuse
    add r14, -4
    cmp r14, r11;bounds check init
    jae L17_2

    add r11, -1024;optimistic new bounds
    cmp r11, r9
    jae L17_3

    cmp r14, r9
    jb exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1024]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L17_4

L17_3:;easy case no re-adjustment
    mov rcx,1024
    mov rdi,r11
L17_4:;joined logic

    xor rax,rax
    rep stosd
L17_2:;done bounds check

    ;write ops
    mov [r14 + 4], dword 0
    

;HistoryNode
;StateEnd
    jmp L12
L17_1:
;LinearFuse
    add r14, -4
    cmp r14, r11;bounds check init
    jae L17_5

    add r11, -1024;optimistic new bounds
    cmp r11, r9
    jae L17_6

    cmp r14, r9
    jb exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1024]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L17_7

L17_6:;easy case no re-adjustment
    mov rcx,1024
    mov rdi,r11
L17_7:;joined logic

    xor rax,rax
    rep stosd
L17_5:;done bounds check

    ;write ops
    mov [r14 + 4], dword 1
    

;HistoryNode
;StateEnd
    jmp L17
exit_good:
    mov [rsp],qword r14;writing current adress
    mov r13, qword [rsp+8];loading base
    ;moving right_init to int index
    sub r10,r13
    shr r10, 2
    mov [rsp+28], dword r10d ;storing it
    ;moving left_init to int index
    sub r11,r13
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
