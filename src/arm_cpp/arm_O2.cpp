#include "arm_O2.hpp"
#include "arm.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace ARM {

void write_asm_general(FILE *file, RegisterState &reg, const char** names, CodeTree::CodeNode* x) {
    switch(x->type()) {
        HANDLE_CASE(Split)
        HANDLE_CASE(Write)
        HANDLE_CASE(Move)
        HANDLE_CASE(StateStart)
        HANDLE_CASE(StateEnd)
        HANDLE_CASE(Exit)

        HANDLE_SCOPED_CASE(HistoryNode)
        HANDLE_SCOPED_CASE(LinearFuse)
        default:
            UNREACHABLE();
    }
}

void Tree_IR_to_ASM(FILE *file,TreeIR ir,const char** names){
	RegisterState reg=RegisterState(
        Register(R5),
        Register(R7),
		Register(R8),
		Register(R6),
		(BoundsDir){Register(R0),Register(R1)},
		(BoundsDir){Register(R3),Register(R4)}
	);

	reg.add_tmp(R13);//we need rsp for things
	
    fprintf(file,arm_start_template);

	load_tape_from_stack(file,reg);
	ASSERT(reg.tmp.size()==1);

    fprintf(file,"%smov r%d, #0\n",_,reg.zero);
    fprintf(file,"%smov r%d, #1\n",_,reg.one);

	for(auto i=0u;i<ir.size();i++){
		write_asm(file,reg,names,ir[i].get());
	}

	fprintf(file,"exit_good:\n");
	store_tape_to_stack(file,reg);

	fprintf(file,arm_end_template);
	//for deubg
	validate_tree(ir);
}

void bounds_check_asm(FILE *file, RegisterState &reg, Register address, int move) {
    if (move == 0) {
        return;
    }

    // Determine bounds and branch conditions
    BoundsDir bounds = move > 0 ? reg.right : reg.left;
    const char* cond = (move > 0 ? "le" : "ge");
    const char* tape_cond = (move > 0 ? "gt" : "lt");
    int extend = ((abs(move) + EXTEND_STEPS - 1) / EXTEND_STEPS) * EXTEND_SIZE;

    // First bounds check
    int ret_idx = ++reg.cur_split;
    fprintf(file, "cmp r%d, r%d // Bounds check init\n", address, bounds.init);
    fprintf(file, "b.%s L%d_%d\n\n", cond, reg.cur_state, ret_idx);

    // Optimistic bounds adjustment
    int step = extend * move / abs(move);
    fprintf(file, "add r%d, r%d, #%d // Optimistic new bounds\n", bounds.init, bounds.init, step);

    // Second check
    int easy_case = ++reg.cur_split;
    int joined_code = ++reg.cur_split;
    fprintf(file, "cmp r%d, r%d // Second bounds check\n", bounds.init, bounds.limit);
    fprintf(file, "b.%s L%d_%d\n\n", cond, reg.cur_state, easy_case);

    // Overflow handling (hard case)
    fprintf(file, "cmp r%d, r%d // Address exceeds bounds\n", address, bounds.limit);
    fprintf(file, "b.%s exit_out_of_tape\n", tape_cond);

    // Hard case logic
    if (move < 0) {
        fprintf(file, "mov r0, r%d // Load limit to r0\n", bounds.limit);
        fprintf(file, "add r1, r%d, #%d // Calculate previous bound + extend\n", bounds.init, extend);
        fprintf(file, "sub r2, r1, r0 // r2 = prev_bound - limit\n");
    } else {
        fprintf(file, "sub r0, r%d, #%d // Load limit - extend to r0\n", bounds.init, extend);
        fprintf(file, "sub r2, r0, r%d // r2 = limit - prev_bound\n", bounds.limit);
    }
    fprintf(file, "lsr r2, r2, #2 // Divide by 4 (groups of 4)\n");

    // Fix bounds and rejoin logic
    fprintf(file, "mov r%d, r%d // Update init to limit\n", bounds.init, bounds.limit);
    fprintf(file, "b L%d_%d\n\n", reg.cur_state, joined_code);

    // Easy case: no bounds adjustment needed
    fprintf(file, "L%d_%d: // Easy case\n", reg.cur_state, easy_case);
    fprintf(file, "mov r2, #%d // r2 = extend / 4\n", extend / 4);
    if (move < 0) {
        fprintf(file, "mov r0, r%d // Load init to r0\n", bounds.init);
    } else {
        fprintf(file, "sub r0, r%d, #%d // Load adjusted init to r0\n", bounds.init, step);
    }

    // Joined logic
    fprintf(file, "L%d_%d: // Joined logic\n\n", reg.cur_state, joined_code);
    fprintf(file, "mov r3, #0 // Zero out r3\n");
    fprintf(file, "loop_start_%d:\n", reg.cur_state);
    fprintf(file, "str r3, [r0], #4 // Store zero and increment pointer\n");
    fprintf(file, "subs r2, r2, #1 // Decrement counter\n");
    fprintf(file, "b.ne loop_start_%d // Continue if not zero\n", reg.cur_state);

    // Final return point
    fprintf(file, "L%d_%d: // Done with bounds check\n\n", reg.cur_state, ret_idx);
}



// Function to save registers and handle collisions
void save_registers(FILE *file, RegisterState &reg, std::vector<Register> &registers_to_save, std::vector<Register> &temp_registers) {
    for (const auto &reg_needed : registers_to_save) {
        Register temp_reg;
        if (reg.contains(reg_needed)) {
            temp_reg = reg.add_tmp();
            fprintf(file, "%smov r%d, r%d ; we need r%d\n", _, temp_reg, reg_needed, reg_needed);
        } else {
            temp_reg = reg.add_tmp(reg_needed);
        }
        temp_registers.push_back(temp_reg);
    }
}

// Function to restore registers in reverse order
void restore_registers(FILE *file, const std::vector<Register> &registers_to_save, const std::vector<Register> &temp_registers) {
    for (int i = registers_to_save.size() - 1; i >= 0; --i) {
        if (temp_registers[i] != registers_to_save[i]) {
            fprintf(file, "%smov r%d, r%d ; restoring r%d\n", _, registers_to_save[i], temp_registers[i], registers_to_save[i]);
        }
    }
}

void load_tape_from_stack(FILE *file, RegisterState reg) {
    ASSERT(reg.contains(R13)); // Ensure SP (R13) is being tracked

    int checkpoint = reg.get_tmp_count();
    Register tmp = reg.add_tmp();  // Temporary register for intermediate calculations
    Register base = reg.add_tmp(); // Temporary register for the base pointer

    // Load `cur` (Bit* cur) into `address`
    fprintf(file, "ldr r%d, [sp] // Load cur\n", reg.address);

    // Load `base` (Bit* base) into `base`
    fprintf(file, "ldr r%d, [sp, #4] // Load base\n", base);

    // Load `right_limit` and calculate effective address
    fprintf(file, "ldr r%d, [sp, #8] // Load right_limit (as int)\n", tmp);
    fprintf(file, "add r%d, r%d, r%d, lsl #2 // Calculate right_limit address (base + 4 * right_limit)\n",
            reg.right.limit, base, tmp);

    // Load `left_limit` and calculate effective address
    fprintf(file, "ldr r%d, [sp, #12] // Load left_limit (as int)\n", tmp);
    fprintf(file, "add r%d, r%d, r%d, lsl #2 // Calculate left_limit address (base + 4 * left_limit)\n",
            reg.left.limit, base, tmp);

    // Load `right_init` and calculate effective address
    fprintf(file, "ldr r%d, [sp, #16] // Load right_init (as int)\n", tmp);
    fprintf(file, "add r%d, r%d, r%d, lsl #2 // Calculate right_init address (base + 4 * right_init)\n",
            reg.right.init, base, tmp);

    // Load `left_init` and calculate effective address
    fprintf(file, "ldr r%d, [sp, #20] // Load left_init (as int)\n", tmp);
    fprintf(file, "add r%d, r%d, r%d, lsl #2 // Calculate left_init address (base + 4 * left_init)\n",
            reg.left.init, base, tmp);

    // Clean up temporary registers
    reg.tmp_back_to(checkpoint);  // Reset temporary register state to avoid collisions
}



void store_tape_to_stack(FILE *file, RegisterState reg) {
    ASSERT(reg.contains(R13)); // Ensure SP (R13) is being tracked

    int checkpoint = reg.get_tmp_count();
    Register base = reg.add_tmp(); // Temporary register for the base address

    // Store `cur` (current address) to the stack
    fprintf(file, "str r%d, [sp] // Writing current address to stack\n", reg.address);

    // Load `base` (base address) into a temporary register
    fprintf(file, "ldr r%d, [sp, #4] // Loading base address\n", base);

    // Handle `right_init`
    fprintf(file, "// Moving right_init to int index\n");
    fprintf(file, "sub r%d, r%d, r%d // Subtract base from right_init\n", reg.right.init, reg.right.init, base);
    fprintf(file, "lsr r%d, r%d, #2 // Right shift right_init by 2\n", reg.right.init, reg.right.init);
    fprintf(file, "str r%d, [sp, #20] // Store right_init as int to stack\n", reg.right.init);

    // Handle `left_init`
    fprintf(file, "// Moving left_init to int index\n");
    fprintf(file, "sub r%d, r%d, r%d // Subtract base from left_init\n", reg.left.init, reg.left.init, base);
    fprintf(file, "lsr r%d, r%d, #2 // Right shift left_init by 2\n", reg.left.init, reg.left.init);
    fprintf(file, "str r%d, [sp, #16] // Store left_init as int to stack\n", reg.left.init);

    reg.tmp_back_to(checkpoint);  // Reset temporary register state to avoid collisions

}



} // namespace ARM