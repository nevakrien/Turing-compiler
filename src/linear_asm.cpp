#include "O2.hpp"
#include "asm.hpp"

#include <iostream>
#include <vector>

void write_asm(FILE *file, RegisterState &reg, const char** names, HistoryNode* x){
	write_genral(file,reg,names,x->next.get());
}

void write_asm(FILE *file, RegisterState &reg, const char** names, LinearFuse* x){	
	
	int move = (x->move_offset) * BIT_SIZE;

	std::vector<Register> registers_to_save = { Register(RDI), Register(RCX), Register(RAX) };
    std::vector<Register> temp_registers;

    int prev_tmp_count = reg.get_tmp_count(); // So we can return

    Register tmp = reg.add_tmp();

	bool do_left=x->write_ops.minKey()<0;
	bool do_right=x->write_ops.maxKey()>0;
	bool need_move=true;


	
	if(do_left||do_right){
		save_registers(file, reg, registers_to_save, temp_registers);
	}

	if(do_left){
		Register address;

		if(x->write_ops.minKey()==x->move_offset){
			need_move=false;
			address=reg.address;
		}
		else{
			address=tmp;
			fprintf(file,"%smov %s, %s\n",_,address.Quad(),reg.address.Quad());
		}

		fprintf(file,"%sadd %s, %d\n",_,address.Quad(),move);
		unsafe_bounds_check_asm(file, reg,  address, move);
	}


	if(do_right){
		Register address;

		if(x->write_ops.maxKey()==x->move_offset){
			need_move=false;
			address=reg.address;
		}
		else{
			address=tmp;
			fprintf(file,"%smov %s, %s\n",_,address.Quad(),reg.address.Quad());
		}

		fprintf(file,"%sadd %s, %d\n",_,address.Quad(),move);
		unsafe_bounds_check_asm(file, reg,  address, move);
	}

	if(need_move){
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
		    	fprintf(file, "%smov [%s + %d], dword 1\n", _,reg.address.Quad(),relative);
		    	break;
		    
		    case TapeVal::Allways0:
		    	fprintf(file, "%smov [%s + %d], dword 0\n", _,reg.address.Quad(),relative);
		    	break;
		    case TapeVal::Flip:
		    	fprintf(file,"%s;flip\n",_);
		    	fprintf(file, "%smov %s,[%s + %d]\n", _,tmp.Double(),reg.address.Quad(),relative);
		    	fprintf(file, "%sxor %s,1\n",_,tmp.Double());
		    	fprintf(file, "%smov [%s + %d], %s\n\n", _,reg.address.Quad(),relative,tmp.Double());
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
    write_genral(file, reg, names, x->next.get());
}