#include "O2.hpp"

static TapeVal translate_write_val(Bit w){
	switch(w){
		case Bit_0:
			return TapeVal::Allways0;
		case Bit_1:
			return TapeVal::Allways1;
	}
}

static TapeVal translate_dual_write_val(Bit w0,Bit w1){
	if(w0==w1){
		return translate_write_val(w1);
	}
	if(w0!=0){
		return TapeVal::Flip;
	}
	return TapeVal::Unchanged;
}

static IRNode make_write(Bit w,Bit read){
	if(w==read){
		return nullptr;
	}	
	return std::make_unique<CodeTree::Write>(translate_write_val(w),nullptr);
}

static IRNode make_write(TapeVal x){
	if(x==TapeVal::Unchanged){
		return nullptr;
	}	
	return std::make_unique<CodeTree::Write>(x,nullptr);
}

static IRNode make_move(Dir m){
	if(m==Stay){
		return nullptr;
	}
	return std::make_unique<CodeTree::Move>(m,nullptr);
}

static inline IRNode make_jump(int cur_id,int next_id,TreeIR tree){
	if(next_id==-1){
		return std::make_unique<CodeTree::Exit>(HALT);
	}
	return std::make_unique<CodeTree::StateEnd>(cur_id,tree.data()+cur_id,tree.data()+next_id);
}

//needs rework

static int map_id_maybe_extend(int id,std::vector<int> &maping,TreeIR &tree,std::vector<int> &next_todo){
	if(id==-1){
		return -1;
	}

	if(maping[id]==-1){
		maping[id]=tree.size();
		tree.push_back(nullptr);
		next_todo.push_back(id);
	}
	return maping[id];
}

static void add_state_base(int add_id,const TuringIR ir,TreeIR &tree,std::vector<int> &maping,std::vector<int> &next_todo){
	StateIR state=ir.states[maping[add_id]];
	IRNode ans[2]={nullptr,nullptr};
	for(int i=0;i<2;i++){
		TransIR trans=state.trans[i];
	}
}

static void add_state_no_split(int add_id,const TuringIR ir,TreeIR &tree,std::vector<int> &maping,std::vector<int> &next_todo){
	StateIR state=ir.states[maping[add_id]];
	TapeVal w=translate_dual_write_val(state.trans[0].write,state.trans[1].write);
	Dir m = state.trans[0].move;

	int my_id=maping[add_id];
	int next_id=map_id_maybe_extend(state.trans[0].nextState,maping,tree,next_todo);

	IRNode b=append_node(make_write(w),make_move(m));
	tree[my_id]=append_node(std::move(b),make_jump(my_id,next_id,tree));
}

static void add_state(int add_id,const TuringIR ir,TreeIR &tree,std::vector<int> &maping,std::vector<int> &next_todo){
	StateIR state=ir.states[maping[add_id]];
	
	if(SemiEq_noWrite_TransIR(state.trans[0],state.trans[1])){
		return add_state_no_split(cur,ir,ans,maping,next_todo);
	}

	add_state_base(cur,ir,ans,maping,next_todo);
}

TreeIR make_inital_tree(TuringIR ir){
	TreeIR ans={};

	std::vector<int> maping={};
	maping.reserve(ir.len);
	for(int i=0;i<ir.len;i++){
		maping.push_back(-1);
	}

	std::vector<int> todo = {0};
	std::vector<int> next_todo={};

	while(todo.size()){
		while(todo.size()){
			int cur=todo.back(); todo.pop_back();

			if(ans[maping[cur]]!=nullptr){
				continue;
			}

			add_state(cur,ir,ans,maping,next_todo);
			
		}
		todo=std::move(next_todo);
		next_todo={};
	}

	return ans;
}

