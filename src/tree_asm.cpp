#include "O2.hpp"
#include "asm.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Exit* x){
	switch(x->code){
		case HALT:
			fprintf(file,"%sjmp exit_good\n\n",_);
			break;
		case TIME_OUT:
			fprintf(file,"%sjmp exit_time_out\n\n",_);
			break;
		case OUT_OF_TAPE:
			fprintf(file,"%sjmp exit_out_of_tape\n\n",_);
			break;
		default:
			UNREACHABLE();
	}
	
}

void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::StateStart* x){
	reg.update_state(x->StateID);
	fprintf(file,"L%d:",reg.cur_state);
	if(names){
		fprintf(file," ;%s",names[reg.cur_state]);
	}
	fprintf(file,"\n");
	write_genral(file,reg,names,x->next.get());
}

void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Write* x){
	
	switch(x->val){
		case TapeVal::Unchanged:
			break;
		case TapeVal::Allways1:
			fprintf(file,"%smov [%s],dword 1\n",_,reg.address.Quad());
			break;
		case TapeVal::Allways0:
			fprintf(file,"%smov [%s],dword 0\n",_,reg.address.Quad());
			break;

		case TapeVal::Flip:
			fprintf(file,"%smov %s, [%s]\n",_,reg.read.Double(),reg.address.Quad());
			fprintf(file,"%sxor %s, 1;simple flip\n",_,reg.read.Double());
			fprintf(file,"%smov [%s], %s\n",_,reg.address.Quad(),reg.read.Double());
			break;
		default:
			UNREACHABLE();
	}
	write_genral(file,reg,names,x->next.get());
}

void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::StateEnd* x){
	int next_id=x->next->StateID;
	fprintf(file,"%sjmp L%d\n",_,next_id);
}

void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Split* x){
	fprintf(file,"%s;spliting\n",_);
	fprintf(file,"%smov %s, [%s]\n",_,reg.read.Double(),reg.address.Quad());
	fprintf(file,"%stest %s, %s\n",_,reg.read.Quad(),reg.read.Quad());
	
	int ret_spot=++reg.cur_split;

	fprintf(file,"%sjnz L%d_%d\n\n",_,reg.cur_state,ret_spot); 
	write_genral(file,reg,names,x->sides[0].get());
	
	fprintf(file,"L%d_%d:\n",reg.cur_state,ret_spot);
	write_genral(file,reg,names,x->sides[1].get());

}


void write_asm(FILE *file, RegisterState &reg, const char** names, CodeTree::Move* x) {
    int move = (x->move_value) * BIT_SIZE;

    if (move == 0) {
        write_genral(file, reg, names, x->next.get());
        return;
    }

    fprintf(file, "%sadd %s, %d\n", _, reg.address.Quad(), move);
    // Now we no longer need to modify address

    std::vector<Register> registers_to_save = { Register(RDI), Register(RCX), Register(RAX) };
    std::vector<Register> temp_registers;

    int prev_tmp_count = reg.get_tmp_count(); // So we can return

    // Save registers and handle collisions
    save_registers(file, reg, registers_to_save, temp_registers);

    // Call the bounds checking function
    unsafe_bounds_check_asm(file, reg,  reg.address, move);

    // Restore dirtied registers in reverse order
    restore_registers(file, registers_to_save, temp_registers);

    reg.tmp_back_to(prev_tmp_count);
    write_genral(file, reg, names, x->next.get());
}