#include "O2.hpp"
#include "asm.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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
		Register(R14),
		Register(R15),
		(BoundsDir){Register(R11),Register(R9)},
		(BoundsDir){Register(R10),Register(R8)}
	);

	reg.add_tmp(RSP);//we need rsp for things
	
	fprintf(file,O0_assembly_start_template);

	load_tape_from_stack(file,reg);
	ASSERT(reg.tmp.size()==1);

	for(auto i=0u;i<ir.size();i++){
		write_asm(file,reg,names,ir[i].get());
	}

	fprintf(file,"exit_good:\n");
	store_tape_to_stack(file,reg);

	fprintf(file,O0_assembly_end_template);
	//for deubg
	validate_tree(ir);
}

//assumes RAX RDI RCX are saved...
void unsafe_bounds_check_asm(FILE *file, RegisterState &reg, Register address, int move) {
    if(move==0){
    	return;
    }
    // Useful globals
    BoundsDir bounds = move > 0 ? reg.right : reg.left;
    const char* jump = (move > 0 ? "jbe" : "jae");
    const char* tape_jump = (move > 0 ? "ja" : "jb");
    int extend = ((abs(move) + EXTEND_STEPS - 1) / EXTEND_STEPS) * EXTEND_SIZE;

    // First check
    int ret_idx = ++reg.cur_split;
    fprintf(file, "%scmp %s, %s;bounds check init\n", _, address.Quad(), bounds.init.Quad());
    fprintf(file, "%s%s L%d_%d\n\n", _, jump, reg.cur_state, ret_idx);

    // Moving the bounds
    int step = extend * move / abs(move);
    fprintf(file, "%sadd %s, %d;optimistic new bounds\n", _, bounds.init.Quad(), step);

    // Second check (maybe easy case)
    int easy_case = ++reg.cur_split;
    int joined_code = ++reg.cur_split;
    fprintf(file, "%scmp %s, %s\n", _, bounds.init.Quad(), bounds.limit.Quad());
    fprintf(file, "%s%s L%d_%d\n\n", _, jump, reg.cur_state, easy_case);

    // Hard case: checking for exit
    fprintf(file, "%scmp %s, %s\n", _, address.Quad(), bounds.limit.Quad());
    fprintf(file, "%s%s exit_out_of_tape\n", _, tape_jump);

    // Hard case body
    if (move < 0) {
        fprintf(file, "%smov rdi,%s\n", _, bounds.limit.Quad());
        fprintf(file, "%s;rcx=prev_bound-rdi\n", _);
        fprintf(file, "%slea rcx,[%s+%d]\n", _, bounds.init.Quad(), extend);
        fprintf(file, "%ssub rcx,rdi\n", _);
    } else {
        fprintf(file, "%slea rdi,[%s-%d+4]\n", _, bounds.init.Quad(), extend);
        fprintf(file, "%s;rcx=limit-prev_bound\n", _);
        fprintf(file, "%slea rcx,[rdi+%d-4]\n", _, extend);
        fprintf(file, "%ssub rcx,%s\n", _, bounds.limit.Quad());
    }

    fprintf(file, "%sshr rcx,2;we move in groups of 4\n", _);

    // Fix init and bounds
    fprintf(file, "%smov %s,%s\n", _, bounds.init.Quad(), bounds.limit.Quad());
    fprintf(file, "%sjmp L%d_%d\n\n", _, reg.cur_state, joined_code);

    // Easy case
    fprintf(file, "L%d_%d:;easy case no re-adjustment\n", reg.cur_state, easy_case);
    fprintf(file, "%smov rcx,%d\n", _, extend/4);
    if (move < 0) {
        fprintf(file, "%smov rdi,%s\n", _, bounds.init.Quad());
    } else {
        fprintf(file, "%slea rdi,[%s-%d+4]\n", _, bounds.init.Quad(), step);
    }

    // Joined case
    fprintf(file, "L%d_%d:;joined logic\n\n", reg.cur_state, joined_code);
    fprintf(file, "%sxor rax,rax\n", _);
    fprintf(file, "%srep stosd\n", _);

    fprintf(file, "L%d_%d:;done bounds check\n\n", reg.cur_state, ret_idx);
}

// Function to save registers and handle collisions
void save_registers(FILE *file, RegisterState &reg, std::vector<Register> &registers_to_save, std::vector<Register> &temp_registers) {
    for (const auto &reg_needed : registers_to_save) {
        Register temp_reg;
        if (reg.contains(reg_needed.name)) {
            temp_reg = reg.add_tmp();
            fprintf(file, "%smov %s, %s ; we need %s\n", _, temp_reg.Quad(), reg_needed.Quad(), reg_needed.Quad());
        } else {
            temp_reg = reg.add_tmp(reg_needed.name);
        }
        temp_registers.push_back(temp_reg);
    }
}

// Function to restore registers in reverse order
void restore_registers(FILE *file, const std::vector<Register> &registers_to_save, const std::vector<Register> &temp_registers) {
    for (int i = registers_to_save.size() - 1; i >= 0; --i) {
        if (temp_registers[i].name != registers_to_save[i].name) {
            fprintf(file, "%smov %s, %s ; restoring %s\n", _, registers_to_save[i].Quad(), temp_registers[i].Quad(), registers_to_save[i].Quad());
        }
    }
}

void load_tape_from_stack(FILE *file,RegisterState reg){
	ASSERT(reg.contains(RSP));
    int checkpoint = reg.get_tmp_count();
	Register tmp=reg.add_tmp();
	Register base=reg.add_tmp();

	fprintf(file, "%smov %s, qword [rsp] ;cur\n", _, reg.address.Quad());

	fprintf(file, "%smov %s, qword [rsp+8] ;base\n", _, base.Quad());
	// right_limit
    fprintf(file, "%smovsxd %s, dword [rsp+20]\n", _,tmp.Quad()); 
    fprintf(file, "%slea %s, [%s + 4*%s] ;right limit\n", _, reg.right.limit.Quad(), base.Quad(),tmp.Quad());

    // left_limit
    fprintf(file, "%smovsxd %s, dword [rsp+16]\n", _,tmp.Quad());
    fprintf(file, "%slea %s, [%s + 4*%s] ;left limit\n", _, reg.left.limit.Quad(), base.Quad(),tmp.Quad());

    // right_init
    fprintf(file, "%smovsxd %s, dword [rsp+24]\n", _,tmp.Quad()); 
    fprintf(file, "%slea %s, [%s + 4*%s] ;left initilized\n", _, reg.left.init.Quad(), base.Quad(),tmp.Quad());

    // left_init
    fprintf(file, "%smovsxd %s, dword [rsp+28]\n", _,tmp.Quad()); 
    fprintf(file, "%slea %s, [%s + 4*%s] ;right initilized\n", _, reg.right.init.Quad(), base.Quad(),tmp.Quad());
    reg.tmp_back_to(checkpoint);  // Reset temporary register state to avoid collisions
}

void store_tape_to_stack(FILE *file,RegisterState reg){
	ASSERT(reg.contains(RSP));
	Register base=reg.add_tmp();

	fprintf(file,"%smov [rsp],qword %s;writing current adress\n",_,reg.address.Quad());

    // Load base address into tmp
    fprintf(file, "%smov %s, qword [rsp+8];loading base\n", _, base.Quad());

    //not handeling the sign right

    //right init
    fprintf(file,"%s;moving right_init to int index\n",_);
    fprintf(file,"%ssub %s,%s\n",_,reg.right.init.Quad(),base.Quad());
    fprintf(file, "%sshr %s, 2\n", _,reg.right.init.Quad());
    fprintf(file, "%smov [rsp+28], dword %s ;storing it\n", _, reg.right.init.Double());

    //left init
    fprintf(file,"%s;moving left_init to int index\n",_);
    fprintf(file,"%ssub %s,%s\n",_,reg.left.init.Quad(),base.Quad());
    fprintf(file, "%sshr %s, 2\n", _,reg.left.init.Quad());
    fprintf(file, "%smov [rsp+24], dword %s \n", _, reg.left.init.Double());
}