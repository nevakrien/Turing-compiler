L0: ;Write1_L1
    mov [r14d],dword 1
    jmp L1
L1: ;NoOp1
    jmp L2
L2: ;Write1_L2
    mov [r14d],dword 1
    jmp L3
L3: ;FlipBack1
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L3_1

    mov [r14d],dword 1
    jmp L4
L3_1:
    mov [r14d],dword 0
    jmp L5
L4: ;FlipAgain1
    jmp L5
L5: ;Write1_L3
    mov [r14d],dword 1
    jmp L6
L6: ;NoOp2
    jmp L7
L7: ;Write1_L4
    mov [r14d],dword 1
    jmp L8
L8: ;MoveBack1
    add r14, -4
    cmp r14, r11;bounds check init
    jae L8_1

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L8_2

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L8_3

L8_2:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L8_3:;joined logic

    xor rax,rax
    rep stosd
L8_1:;done bounds check

    jmp L9
L9: ;MoveForward1
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

    jmp L10
L10: ;Write1_L5
    mov [r14d],dword 1
    jmp L11
L11: ;Return_R1
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

    jmp L12
L12: ;NoOp3
    jmp L13
L13: ;Return_R2
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L13_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L13_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L13_3

L13_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L13_3:;joined logic

    xor rax,rax
    rep stosd
L13_1:;done bounds check

    jmp L14
L14: ;FlipBack2
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L14_1

    mov [r14d],dword 1
    jmp L15
L14_1:
    mov [r14d],dword 0
    jmp L16
L15: ;FlipAgain2
    jmp L16
L16: ;Return_R3
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L16_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L16_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L16_3

L16_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L16_3:;joined logic

    xor rax,rax
    rep stosd
L16_1:;done bounds check

    jmp L17
L17: ;MoveBack2
    add r14, -4
    cmp r14, r11;bounds check init
    jae L17_1

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L17_2

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L17_3

L17_2:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L17_3:;joined logic

    xor rax,rax
    rep stosd
L17_1:;done bounds check

    jmp L18
L18: ;MoveForward2
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L18_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L18_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L18_3

L18_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L18_3:;joined logic

    xor rax,rax
    rep stosd
L18_1:;done bounds check

    jmp L19
L19: ;Return_R4
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L19_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L19_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L19_3

L19_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L19_3:;joined logic

    xor rax,rax
    rep stosd
L19_1:;done bounds check

    jmp L20
L20: ;NoOp4
    jmp L21
L21: ;Return_R5
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L21_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L21_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L21_3

L21_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L21_3:;joined logic

    xor rax,rax
    rep stosd
L21_1:;done bounds check

    jmp L22
L22: ;Initialize_Right
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L22_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L22_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L22_3

L22_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L22_3:;joined logic

    xor rax,rax
    rep stosd
L22_1:;done bounds check

    jmp L23
L23: ;Start_Copy
    mov [r14d],dword 1
    jmp L24
L24: ;FlipBack3
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L24_1

    mov [r14d],dword 1
    jmp L25
L24_1:
    mov [r14d],dword 0
    jmp L26
L25: ;FlipAgain3
    jmp L26
L26: ;Decrement_Left
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L26_1

    mov [r14d],dword 0
    jmp L28
L26_1:
    mov [r14d],dword 0
    jmp L27
L28: ;Halt_Check
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L28_1

    mov [r14d],dword 0
    jmp exit_good

L28_1:
    mov [r14d],dword 1
    jmp L29
L27: ;NoOp5
    jmp L29
L29: ;Move_Left
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L29_1

    mov [r14d],dword 0
    jmp L30
L29_1:
    mov [r14d],dword 1
    jmp L29
L30: ;NoOp6
    jmp L31
L31: ;Find_Right_Tape
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L31_1

    mov [r14d],dword 0
    jmp L34
L31_1:
    mov [r14d],dword 1
    jmp L32
L34: ;Write_One
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L34_1

    mov [r14d],dword 1
    jmp L37
L34_1:
    mov [r14d],dword 1
    jmp L35
L32: ;MoveBack3
    add r14, -4
    cmp r14, r11;bounds check init
    jae L32_1

    add r11, -1;optimistic new bounds
    cmp r14, r9
    jae L32_2

    cmp r14, r9
    jae exit_out_of_tape
    mov rdi,r9
    ;rcx=prev_bound-rdi
    lea rcx,[r11+1]
    sub rcx,rdi
    shr rcx,2;we move in groups of 4
    mov r11,r9
    jmp L32_3

L32_2:;easy case no re-adjustment
    mov rcx,0
    mov rdi,r11
L32_3:;joined logic

    xor rax,rax
    rep stosd
L32_1:;done bounds check

    jmp L33
L33: ;MoveForward3
    add r14, 4
    cmp r14, r10;bounds check init
    jbe L33_1

    add r10, 1;optimistic new bounds
    cmp r14, r8
    jbe L33_2

    cmp r14, r8
    jbe exit_out_of_tape
    lea rdi,[r10-1+4]
    ;rcx=limit-prev_bound
    lea rcx,[rdi+1-4]
    sub rcx,r8
    shr rcx,2;we move in groups of 4
    mov r10,r8
    jmp L33_3

L33_2:;easy case no re-adjustment
    mov rcx,0
    lea rdi,[r10-1+4]
L33_3:;joined logic

    xor rax,rax
    rep stosd
L33_1:;done bounds check

    jmp L34
L37: ;Return_Left
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L37_1

    mov [r14d],dword 0
    jmp L26
L37_1:
    mov [r14d],dword 1
    jmp L37
L35: ;FlipBack4
    ;spliting
    mov [r15d], r14d
    test r15, r15
    jnz L35_1

    mov [r14d],dword 1
    jmp L36
L35_1:
    mov [r14d],dword 0
    jmp L34
L36: ;FlipAgain4
    jmp L34
