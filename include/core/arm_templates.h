#ifndef ARM_TEMPLATES_H
#define ARM_TEMPLATES_H

[[maybe_unused]] static const char *arm_start_template = 
".section .text\n"
".global main\n"
".extern ReadTape\n"
".extern DumpTape\n"
".extern exit_turing\n"
"\n"
"exit_out_of_tape:\n"
"    mov r0, #2              // r0 = 2 (exit code)\n"
"    bl exit_turing          // Call exit_turing function\n"
"\n"
"main:\n"
"    // Initial boilerplate\n"
"    // Ensure there are at least 2 arguments (argc >= 3)\n"
"    ldr r0, [sp]            // Load argc into r0\n"
"    cmp r0, #3              // Compare argc with 3\n"
"    blt _exit_bad_args      // If argc < 3, jump to _exit_bad_args\n"
"\n"
"    // Load the first command-line argument (argv[1] - input file)\n"
"    ldr r1, [sp, #16]       // r1 = argv[1] (input file)\n"
"\n"
"    // Allocate stack space and align the stack\n"
"    sub sp, sp, #32         // Reserve 32 bytes on the stack\n"
"    mov r0, sp              // r0 = pointer to return struct (output buffer)\n"
"\n"
"    // Call ReadTape\n"
"    bl ReadTape             // ReadTape(r0 = return struct, r1 = input file)\n"
"\n"
"    // !!!ACTUAL CODE: done boilerplate\n"
"    // Placeholder for additional code below\n";

[[maybe_unused]] static const char *arm_end_template = 
"// DONE: Output boilerplate and exit\n"
"\n"
"    ldr r1, [sp, #56]       // r1 = argv[2] (output file), offset by 32+24\n"
"    mov r0, sp              // r0 = pointer to the same return struct\n"
"\n"
"    // Call DumpTape\n"
"    bl DumpTape             // DumpTape(r0 = return struct, r1 = output file)\n"
"\n"
"    // Exit the program\n"
"    mov r0, #0              // r0 = 0 (exit code)\n"
"    bl exit_turing          // Call exit_turing function\n"
"\n"
"_exit_bad_args:\n"
"    mov r0, #3              // r0 = 3 (exit code for bad arguments)\n"
"    bl exit_turing          // Call exit_turing function\n";

#endif
