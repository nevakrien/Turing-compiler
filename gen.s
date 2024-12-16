.section .text
.global main
.extern ReadTape
.extern DumpTape
.extern exit_turing

exit_out_of_tape:
    mov r0, #2              // r0 = 2 (exit code)
    bl exit_turing          // Call exit_turing function

main:
    // Initial boilerplate
    // Ensure there are at least 2 arguments (argc >= 3)
    ldr r0, [sp]            // Load argc into r0
    cmp r0, #3              // Compare argc with 3
    blt _exit_bad_args      // If argc < 3, jump to _exit_bad_args

    // Load the first command-line argument (argv[1] - input file)
    ldr r1, [sp, #16]       // r1 = argv[1] (input file)

    // Allocate stack space and align the stack
    sub sp, sp, #32         // Reserve 32 bytes on the stack
    mov r0, sp              // r0 = pointer to return struct (output buffer)

    // Call ReadTape
    bl ReadTape             // ReadTape(r0 = return struct, r1 = input file)

    // !!!ACTUAL CODE: done boilerplate
    // Placeholder for additional code below
ldr r5, [sp] // Load cur
ldr r9, [sp, #4] // Load base
ldr r2, [sp, #8] // Load right_limit (as int)
add r4, r9, r2, lsl #2 // Calculate right_limit address (base + 4 * right_limit)
ldr r2, [sp, #12] // Load left_limit (as int)
add r1, r9, r2, lsl #2 // Calculate left_limit address (base + 4 * left_limit)
ldr r2, [sp, #16] // Load right_init (as int)
add r3, r9, r2, lsl #2 // Calculate right_init address (base + 4 * right_init)
ldr r2, [sp, #20] // Load left_init (as int)
add r0, r9, r2, lsl #2 // Calculate left_init address (base + 4 * left_init)
    mov r7, #0
    mov r8, #1
L0: //Write1_L1
//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L0_1

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L0_2

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L0_3

L0_2: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L0_3: // Joined logic

mov r3, #0 // Zero out r3
loop_start_0:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_0 // Continue if not zero
L0_1: // Done with bounds check

//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L0_4

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L0_5

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L0_6

L0_5: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L0_6: // Joined logic

mov r3, #0 // Zero out r3
loop_start_0:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_0 // Continue if not zero
L0_4: // Done with bounds check

//Split
ldr r6, [r5]
ands r6, r6, r6
bne L0_8
b L0_7
L0_8:
//Write
str r7, [r5] // Write 0 to memory
// Jumping to the next state
//StateEnd
    b L5
b L0_9
L0_7:
//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//StateEnd
    b L5
L0_9:
L5: //Write1_L3
//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L5_1

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L5_2

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L5_3

L5_2: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L5_3: // Joined logic

mov r3, #0 // Zero out r3
loop_start_5:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_5 // Continue if not zero
L5_1: // Done with bounds check

//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L5_4

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L5_5

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L5_6

L5_5: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L5_6: // Joined logic

mov r3, #0 // Zero out r3
loop_start_5:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_5 // Continue if not zero
L5_4: // Done with bounds check

//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//Move
add r5, r5, #4
cmp r5, r3 // Bounds check init
b.le L5_7

add r3, r3, #1024 // Optimistic new bounds
cmp r3, r4 // Second bounds check
b.le L5_8

cmp r5, r4 // Address exceeds bounds
b.gt exit_out_of_tape
sub r0, r3, #1024 // Load limit - extend to r0
sub r2, r0, r4 // r2 = limit - prev_bound
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r3, r4 // Update init to limit
b L5_9

L5_8: // Easy case
mov r2, #256 // r2 = extend / 4
sub r0, r3, #1024 // Load adjusted init to r0
L5_9: // Joined logic

mov r3, #0 // Zero out r3
loop_start_5:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_5 // Continue if not zero
L5_7: // Done with bounds check

//Split
ldr r6, [r5]
ands r6, r6, r6
bne L5_11
b L5_10
L5_11:
//Write
str r7, [r5] // Write 0 to memory
// Jumping to the next state
//StateEnd
    b L16
b L5_12
L5_10:
//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//StateEnd
    b L16
L5_12:
L16: //Return_R3
//Move
add r5, r5, #16
cmp r5, r3 // Bounds check init
b.le L16_1

add r3, r3, #1024 // Optimistic new bounds
cmp r3, r4 // Second bounds check
b.le L16_2

cmp r5, r4 // Address exceeds bounds
b.gt exit_out_of_tape
sub r0, r3, #1024 // Load limit - extend to r0
sub r2, r0, r4 // r2 = limit - prev_bound
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r3, r4 // Update init to limit
b L16_3

L16_2: // Easy case
mov r2, #256 // r2 = extend / 4
sub r0, r3, #1024 // Load adjusted init to r0
L16_3: // Joined logic

mov r3, #0 // Zero out r3
loop_start_16:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_16 // Continue if not zero
L16_1: // Done with bounds check

//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//Move
add r5, r5, #4
cmp r5, r3 // Bounds check init
b.le L16_4

add r3, r3, #1024 // Optimistic new bounds
cmp r3, r4 // Second bounds check
b.le L16_5

cmp r5, r4 // Address exceeds bounds
b.gt exit_out_of_tape
sub r0, r3, #1024 // Load limit - extend to r0
sub r2, r0, r4 // r2 = limit - prev_bound
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r3, r4 // Update init to limit
b L16_6

L16_5: // Easy case
mov r2, #256 // r2 = extend / 4
sub r0, r3, #1024 // Load adjusted init to r0
L16_6: // Joined logic

mov r3, #0 // Zero out r3
loop_start_16:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_16 // Continue if not zero
L16_4: // Done with bounds check

//Split
ldr r6, [r5]
ands r6, r6, r6
bne L16_8
b L16_7
L16_8:
//Write
str r7, [r5] // Write 0 to memory
// Jumping to the next state
//StateEnd
    b L26
b L16_9
L16_7:
//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//StateEnd
    b L26
L16_9:
L26: //Decrement_Left
//Split
ldr r6, [r5]
ands r6, r6, r6
bne L26_2
b L26_1
L26_2:
//Write
str r7, [r5] // Write 0 to memory
// Jumping to the next state
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L26_4

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L26_5

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L26_6

L26_5: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L26_6: // Joined logic

mov r3, #0 // Zero out r3
loop_start_26:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_26 // Continue if not zero
L26_4: // Done with bounds check

//StateEnd
    b L29
b L26_3
L26_1:
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L26_7

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L26_8

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L26_9

L26_8: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L26_9: // Joined logic

mov r3, #0 // Zero out r3
loop_start_26:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_26 // Continue if not zero
L26_7: // Done with bounds check

//Split
ldr r6, [r5]
ands r6, r6, r6
bne L26_11
b L26_10
L26_11:
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L26_13

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L26_14

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L26_15

L26_14: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L26_15: // Joined logic

mov r3, #0 // Zero out r3
loop_start_26:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_26 // Continue if not zero
L26_13: // Done with bounds check

//StateEnd
    b L29
b L26_12
L26_10:
//Exit
    b exit_good

L26_12:
L26_3:
L29: //Move_Left
//Split
ldr r6, [r5]
ands r6, r6, r6
bne L29_2
b L29_1
L29_2:
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L29_4

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L29_5

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L29_6

L29_5: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L29_6: // Joined logic

mov r3, #0 // Zero out r3
loop_start_29:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_29 // Continue if not zero
L29_4: // Done with bounds check

//StateEnd
    b L29
b L29_3
L29_1:
//Move
add r5, r5, #4
cmp r5, r3 // Bounds check init
b.le L29_7

add r3, r3, #1024 // Optimistic new bounds
cmp r3, r4 // Second bounds check
b.le L29_8

cmp r5, r4 // Address exceeds bounds
b.gt exit_out_of_tape
sub r0, r3, #1024 // Load limit - extend to r0
sub r2, r0, r4 // r2 = limit - prev_bound
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r3, r4 // Update init to limit
b L29_9

L29_8: // Easy case
mov r2, #256 // r2 = extend / 4
sub r0, r3, #1024 // Load adjusted init to r0
L29_9: // Joined logic

mov r3, #0 // Zero out r3
loop_start_29:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_29 // Continue if not zero
L29_7: // Done with bounds check

//Split
ldr r6, [r5]
ands r6, r6, r6
bne L29_11
b L29_10
L29_11:
//Move
add r5, r5, #4
cmp r5, r3 // Bounds check init
b.le L29_13

add r3, r3, #1024 // Optimistic new bounds
cmp r3, r4 // Second bounds check
b.le L29_14

cmp r5, r4 // Address exceeds bounds
b.gt exit_out_of_tape
sub r0, r3, #1024 // Load limit - extend to r0
sub r2, r0, r4 // r2 = limit - prev_bound
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r3, r4 // Update init to limit
b L29_15

L29_14: // Easy case
mov r2, #256 // r2 = extend / 4
sub r0, r3, #1024 // Load adjusted init to r0
L29_15: // Joined logic

mov r3, #0 // Zero out r3
loop_start_29:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_29 // Continue if not zero
L29_13: // Done with bounds check

//StateEnd
    b L34
b L29_12
L29_10:
//Move
add r5, r5, #4
cmp r5, r3 // Bounds check init
b.le L29_16

add r3, r3, #1024 // Optimistic new bounds
cmp r3, r4 // Second bounds check
b.le L29_17

cmp r5, r4 // Address exceeds bounds
b.gt exit_out_of_tape
sub r0, r3, #1024 // Load limit - extend to r0
sub r2, r0, r4 // r2 = limit - prev_bound
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r3, r4 // Update init to limit
b L29_18

L29_17: // Easy case
mov r2, #256 // r2 = extend / 4
sub r0, r3, #1024 // Load adjusted init to r0
L29_18: // Joined logic

mov r3, #0 // Zero out r3
loop_start_29:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_29 // Continue if not zero
L29_16: // Done with bounds check

//StateEnd
    b L34
L29_12:
L29_3:
L34: //Write_One
//Split
ldr r6, [r5]
ands r6, r6, r6
bne L34_2
b L34_1
L34_2:
//Move
add r5, r5, #4
cmp r5, r3 // Bounds check init
b.le L34_4

add r3, r3, #1024 // Optimistic new bounds
cmp r3, r4 // Second bounds check
b.le L34_5

cmp r5, r4 // Address exceeds bounds
b.gt exit_out_of_tape
sub r0, r3, #1024 // Load limit - extend to r0
sub r2, r0, r4 // r2 = limit - prev_bound
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r3, r4 // Update init to limit
b L34_6

L34_5: // Easy case
mov r2, #256 // r2 = extend / 4
sub r0, r3, #1024 // Load adjusted init to r0
L34_6: // Joined logic

mov r3, #0 // Zero out r3
loop_start_34:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_34 // Continue if not zero
L34_4: // Done with bounds check

//Split
ldr r6, [r5]
ands r6, r6, r6
bne L34_8
b L34_7
L34_8:
//Write
str r7, [r5] // Write 0 to memory
// Jumping to the next state
//StateEnd
    b L34
b L34_9
L34_7:
//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//StateEnd
    b L34
L34_9:
b L34_3
L34_1:
//Write
str r8, [r5] // Write 1 to memory
// Jumping to the next state
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L34_10

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L34_11

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L34_12

L34_11: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L34_12: // Joined logic

mov r3, #0 // Zero out r3
loop_start_34:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_34 // Continue if not zero
L34_10: // Done with bounds check

//StateEnd
    b L37
L34_3:
L37: //Return_Left
//Split
ldr r6, [r5]
ands r6, r6, r6
bne L37_2
b L37_1
L37_2:
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L37_4

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L37_5

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L37_6

L37_5: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L37_6: // Joined logic

mov r3, #0 // Zero out r3
loop_start_37:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_37 // Continue if not zero
L37_4: // Done with bounds check

//StateEnd
    b L37
b L37_3
L37_1:
//Move
add r5, r5, #-4
cmp r5, r0 // Bounds check init
b.ge L37_7

add r0, r0, #-1024 // Optimistic new bounds
cmp r0, r1 // Second bounds check
b.ge L37_8

cmp r5, r1 // Address exceeds bounds
b.lt exit_out_of_tape
mov r0, r1 // Load limit to r0
add r1, r0, #1024 // Calculate previous bound + extend
sub r2, r1, r0 // r2 = prev_bound - limit
lsr r2, r2, #2 // Divide by 4 (groups of 4)
mov r0, r1 // Update init to limit
b L37_9

L37_8: // Easy case
mov r2, #256 // r2 = extend / 4
mov r0, r0 // Load init to r0
L37_9: // Joined logic

mov r3, #0 // Zero out r3
loop_start_37:
str r3, [r0], #4 // Store zero and increment pointer
subs r2, r2, #1 // Decrement counter
b.ne loop_start_37 // Continue if not zero
L37_7: // Done with bounds check

//StateEnd
    b L26
L37_3:
exit_good:
str r5, [sp] // Writing current address to stack
ldr r2, [sp, #4] // Loading base address
// Moving right_init to int index
sub r3, r3, r2 // Subtract base from right_init
lsr r3, r3, #2 // Right shift right_init by 2
str r3, [sp, #20] // Store right_init as int to stack
// Moving left_init to int index
sub r0, r0, r2 // Subtract base from left_init
lsr r0, r0, #2 // Right shift left_init by 2
str r0, [sp, #16] // Store left_init as int to stack
// DONE: Output boilerplate and exit

    ldr r1, [sp, #56]       // r1 = argv[2] (output file), offset by 32+24
    mov r0, sp              // r0 = pointer to the same return struct

    // Call DumpTape
    bl DumpTape             // DumpTape(r0 = return struct, r1 = output file)

    // Exit the program
    mov r0, #0              // r0 = 0 (exit code)
    bl exit_turing          // Call exit_turing function

_exit_bad_args:
    mov r0, #3              // r0 = 3 (exit code for bad arguments)
    bl exit_turing          // Call exit_turing function
