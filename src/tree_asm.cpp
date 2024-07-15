#include "O2.hpp"
#include "asm.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>



DECLARE_WRITE(Split);
DECLARE_WRITE(Write);
DECLARE_WRITE(Move);
DECLARE_WRITE(StateStart);
DECLARE_WRITE(StateEnd);
DECLARE_WRITE(Exit);

static void write_genral(FILE *file, RegisterState &reg, const char** names, CodeTree::CodeNode* x) {
    switch(x->type()) {
        HANDLE_CASE(Split)
        HANDLE_CASE(Write)
        HANDLE_CASE(Move)
        HANDLE_CASE(StateStart)
        HANDLE_CASE(StateEnd)
        HANDLE_CASE(Exit)
        default:
            UNREACHABLE();
    }
}



static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Exit* x){
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

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::StateStart* x){
	reg.update_state(x->StateID);
	fprintf(file,"L%d:",reg.cur_state);
	if(names){
		fprintf(file," ;%s",names[reg.cur_state]);
	}
	fprintf(file,"\n");
	write_genral(file,reg,names,x->next.get());
}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Write* x){
	
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

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::StateEnd* x){
	int next_id=x->next->StateID;
	fprintf(file,"%sjmp L%d\n",_,next_id);
}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Split* x){
	fprintf(file,"%s;spliting\n",_);
	fprintf(file,"%smov %s, [%s]\n",_,reg.read.Double(),reg.address.Quad());
	fprintf(file,"%stest %s, %s\n",_,reg.read.Quad(),reg.read.Quad());
	
	int ret_spot=++reg.cur_split;

	fprintf(file,"%sjnz L%d_%d\n\n",_,reg.cur_state,ret_spot); 
	write_genral(file,reg,names,x->sides[0].get());
	
	fprintf(file,"L%d_%d:\n",reg.cur_state,ret_spot);
	write_genral(file,reg,names,x->sides[1].get());

}

//this function MAY act goffy if rcx is used as a limit register, there is no reason to do that tho...
static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Move* x){
	int move=(x->move_value)*BIT_SIZE;
	
	if(move==0){
		write_genral(file,reg,names,x->next.get());
		return;
	}
	

	fprintf(file,"%sadd %s, %d\n",_,reg.address.Quad(),move);
	//now we no longer need to modify adress 
	//so even if we need the adress register we can restore it later
	
	//setup all the registers we need
	Register tmp_rdi;
	Register tmp_rcx;
	Register tmp_rax;

	int prev_tmp_count=reg.get_tmp_count();//so we can return

	//check colisions
	if(reg.contains(RDI)){
		tmp_rdi=reg.add_tmp();
		fprintf(file,"%smov %s, rdi ;we need rdi\n",_,tmp_rdi.Quad());
	}
	else{
		tmp_rdi=reg.add_tmp(RDI);
	}
	
	if(reg.contains(RCX)){
		tmp_rcx=reg.add_tmp();
		fprintf(file,"%smov %s, rcx;we need rcx\n",_,tmp_rcx.Quad());
	}
	else{
		tmp_rcx=reg.add_tmp(RCX);
	}

	
	if(reg.contains(RAX)){
		tmp_rax=reg.add_tmp();
		fprintf(file,"%smov %s, rax;we need rax\n",_,tmp_rax.Quad());
	}
	else{
		tmp_rax=reg.add_tmp(RAX);
	}

	//useful globals
	BoundsDir bounds = move > 0 ? reg.right : reg.left;
	const char* jump=(move > 0 ? "jbe" : "jae");
	const char* tape_jump=(move > 0 ? "ja" : "jb");



	//first check
	int ret_idx=++reg.cur_split;
	fprintf(file,"%scmp %s, %s;bounds check init\n",_,reg.address.Quad(),bounds.init.Quad());
	fprintf(file, "%s%s L%d_%d\n\n", _, jump, reg.cur_state, ret_idx);


	//moving the bounds
	int extend=((abs(move)+EXTEND_STEPS-1)/EXTEND_STEPS)*EXTEND_SIZE;
	int step=extend*move/abs(move);

	fprintf(file,"%sadd %s, %d;optimistic new bounds\n",_,bounds.init.Quad(),step);

	//second check (maybe easy case)
	int easy_case=++reg.cur_split;
	int joined_code=++reg.cur_split;
	fprintf(file,"%scmp %s, %s\n",_,bounds.init.Quad(),bounds.limit.Quad());
	fprintf(file, "%s%s L%d_%d\n\n", _, jump, reg.cur_state, easy_case);

	//hard case: checking for exit
	fprintf(file,"%scmp %s, %s\n",_,reg.address.Quad(),bounds.limit.Quad());
	fprintf(file, "%s%s exit_out_of_tape\n", _, tape_jump);

	
	//hard case body
	
	if(move<0){
		fprintf(file,"%smov rdi,%s\n",_,bounds.limit.Quad());

		fprintf(file,"%s;rcx=prev_bound-rdi\n",_);
		fprintf(file,"%slea rcx,[%s+%d]\n",_,bounds.init.Quad(),extend);
		fprintf(file,"%ssub rcx,rdi\n",_);
	}
	else{
		fprintf(file,"%slea rdi,[%s-%d+4]\n",_,bounds.init.Quad(),extend);

		fprintf(file,"%s;rcx=limit-prev_bound\n",_);
		fprintf(file,"%slea rcx,[rdi+%d-4]\n",_,extend);
		fprintf(file,"%ssub rcx,%s\n",_,bounds.limit.Quad());
	}

	fprintf(file,"%sshr rcx,2;we move in groups of 4\n",_);

	//fix init and bounce
	fprintf(file,"%smov %s,%s\n",_,bounds.init.Quad(),bounds.limit.Quad());
	fprintf(file, "%sjmp L%d_%d\n\n", _, reg.cur_state, joined_code);
	
	//easy case
	fprintf(file,"L%d_%d:;easy case no re-adjustment\n",reg.cur_state,easy_case);
	fprintf(file,"%smov rcx,%d\n",_,extend);

	if(move<0){
		fprintf(file,"%smov rdi,%s\n",_,bounds.init.Quad());
	}	
	else{
		fprintf(file,"%slea rdi,[%s-%d+4]\n",_,bounds.init.Quad(),step);
	}

	//joined case
	fprintf(file,"L%d_%d:;joined logic\n\n",reg.cur_state,joined_code);
	fprintf(file, "%sxor rax,rax\n", _);
	fprintf(file, "%srep stosd\n", _);
	

	//restore dirtied registetrs
	if(tmp_rdi.name!=RDI){
		fprintf(file,"%smov rdi, %s;restoring rdi\n",_,tmp_rdi.Quad());
	}	
	if(tmp_rcx.name!=RCX){
		fprintf(file,"%smov rcx, %s;restoring rcx\n",_,tmp_rcx.Quad());
	}
	if(tmp_rax.name!=RAX){
		fprintf(file,"%smov rax, %s;restoring rax\n",_,tmp_rax.Quad());
	}

	fprintf(file,"L%d_%d:;done bounds check\n\n",reg.cur_state,ret_idx);

	reg.tmp_back_to(prev_tmp_count);
	write_genral(file,reg,names,x->next.get());
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

	//TODO: add missing global functions

	for(auto i=0u;i<ir.size();i++){
		write_asm(file,reg,names,ir[i].get());
	}

	fprintf(file,"exit_good:\n");
	store_tape_to_stack(file,reg);

	fprintf(file,O0_assembly_end_template);
}