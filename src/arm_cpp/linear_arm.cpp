#include "O2.hpp"
#include "asm.hpp"

#include <iostream>
#include <vector>

void write_asm(FILE *file, RegisterState &reg, const char** names, HistoryNode* x){
	write_asm_genral(file,reg,names,x->next.get());
}

void write_asm(FILE *file, RegisterState &reg, const char** names, LinearFuse* x){	
	
	int move = (x->move_offset) * BIT_SIZE;

	std::vector<Register> registers_to_save = { Register(RDI), Register(RCX), Register(RAX) };
    std::vector<Register> temp_registers;

    int prev_tmp_count = reg.get_tmp_count(); // So we can return

    Register tmp = reg.add_tmp();
    fprintf(file,"%s//using %d as temp\n",_,tmp.id);

	bool do_left=x->write_ops.minKey()<0;
	bool do_right=x->write_ops.maxKey()>0;
	bool need_move=true;


	
	if(do_left||do_right){
		save_registers(file, reg, registers_to_save, temp_registers);
	}

	if(do_left){
		fprintf(file,"%s//left bounds check\n",_);
		Register address;

		if(0){
		// if(x->move_offset==0){
			need_move=false;
			address=reg.address;
		}
		else{
			address=tmp;
			fprintf(file,"%smov x%d, x%d\n",_,reg.address.id,address.id);
		}

		int bound_move = (x->write_ops.minKey()) * BIT_SIZE;

		fprintf(file,"%sadd %d, x%d\n",_,bound_move,address.id);
		unsafe_bounds_check_asm(file, reg,  address, bound_move);
	}


	if(do_right){
		fprintf(file,"%s;right bounds check\n",_);
		Register address;

		if(0){
		// if(x->write_ops.maxKey()==x->move_offset){
			need_move=false;
			address=reg.address;
		}
		else{
			address=tmp;
			fprintf(file,"%smov %s, %s\n",_,address.Quad(),reg.address.Quad());
		}

		int bound_move = (x->write_ops.maxKey()) * BIT_SIZE;

		fprintf(file,"%sadd %s, %d\n",_,address.Quad(),bound_move);
		unsafe_bounds_check_asm(file, reg,  address, bound_move);
	}

	if(need_move && move!=0){
		need_move=false;
		fprintf(file, "%sadd %s, %d\n", _, reg.address.Quad(), move);
	}


	fprintf(file,"%s;write ops\n",_);
	for(int i=x->write_ops.minKey();i<=x->write_ops.maxKey(); i++){
		int relative = (i - x->move_offset)*BIT_SIZE;
		TapeVal op = x->write_ops[i];

		switch(op){
			case TapeVal::Unchanged:
				break;
		    
		    case TapeVal::Allways1:
		    	fprintf(file, "%sstr w%d, [x%d + %d]\n", _,reg.one.id,reg.address.id,relative);
		    	break;
		    
		    case TapeVal::Allways0:
		    	fprintf(file, "%sstr w%d, [x%d + %d]\n", _,reg.zero.id,reg.address.id,relative);
		    	break;
		    	
			case TapeVal::Flip:
			    fprintf(file, "%s;flip\n", _);
			    fprintf(file, "%sldr w%d, [x%d, #%d]\n", _, tmp.id, reg.address.id, relative);  // Load the value from memory
			    fprintf(file, "%seor w%d, w%d, #1\n", _, tmp.id, tmp.id);                      // XOR the value with 1
			    fprintf(file, "%sstr w%d, [x%d, #%d]\n", _, tmp.id, reg.address.id, relative); // Store the flipped value back to memory
			    break;

		    default:
		    	UNREACHABLE();
		}
	}
	fprintf(file,"%s\n\n",_);

	if(do_left||do_right){
		restore_registers(file, registers_to_save, temp_registers);
	}
	

    reg.tmp_back_to(prev_tmp_count);
    write_asm_genral(file, reg, names, x->next.get());
}