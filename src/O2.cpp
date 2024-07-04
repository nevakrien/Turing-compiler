#include "O2.hpp"

void warn_unreachble(TreeIR &tree,TuringIR ir){
	printf("total size dif %ld\n",ir.len-tree.size());

	bool* seen = new bool[ir.len];
	for(int i=0;i<ir.len;i++){
		seen[i]=0;
	}
	for(auto i=0u;i<tree.size();i++){
		seen[tree[i]->StateID]=true;
	}
	

	for(int i=0;i<ir.len;i++){
		if(!seen[i]){
			printf("warning state \"%s\" is unsused\n",ir.names[i] );
		}
	}

	delete[] seen;
}

static TapeVal translate_write_val(Bit w){
	switch(w){
		case Bit_0:
			return TapeVal::Allways0;
		case Bit_1:
			return TapeVal::Allways1;
		default:
			UNREACHABLE();
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

static std::unique_ptr<CodeTree::StateStart> make_state_start(int id){
	return std::make_unique<CodeTree::StateStart>(id,nullptr);
}

static IRNode maybe_write(TapeVal val,IRNode next){
	if(val==TapeVal::Unchanged){
		return next;
	}
	return std::make_unique<CodeTree::Write>(val,std::move(next));
}

static IRNode maybe_move(Dir m,IRNode next){
	if(m==Stay){
		return next;
	}
	return std::make_unique<CodeTree::Move>(m,std::move(next));
}

//the next code has just ungodly long parameters... 
//look at make_inital_tree first and this would make sense
//~~~

//need to pull global vars into here
struct GlobalVars final {
	TreeIR &tree;
	std::vector<int> &maping;
	std::vector<unsigned int> &todo;
	TuringIR ir;
};

//may need to make the state and add it to our map
//this can only triger once per state so no state would be added twice to todo
static CodeTree::StateStart* get_state(unsigned int id,TreeIR &tree,std::vector<int> &maping,std::vector<unsigned int> &todo){
	int maped_id=maping[id];
	if(maped_id==-1){
		maped_id=tree.size();
		
		maping[id]=maped_id;
		tree.push_back(make_state_start(id));
		todo.push_back(id);
	}

	return tree[maped_id].get();
}

static IRNode make_end(int id_cur,int id_next,CodeTree::CodeNode* owner,TreeIR &tree,std::vector<int> &maping,std::vector<unsigned int> &todo){
	if(id_next==-1){
		return std::make_unique<CodeTree::Exit>(HALT);
	}

	ASSERT(id_next>0);
	return std::make_unique<CodeTree::StateEnd>(id_cur,owner,
		get_state(
				(unsigned int) id_next,
				tree,
				maping,
				todo
		)
	);
}

static IRNode make_trans_body(int state_id,TapeVal w, Dir m,CodeTree::CodeNode* owner,int next_id,TreeIR &tree,std::vector<int> &maping,std::vector<unsigned int> &todo){
	IRNode start= maybe_write(w,maybe_move(m,nullptr));
	CodeTree::CodeNode* end_owner;
	if(start==nullptr){
		end_owner=owner;
	}
	else{
		end_owner=start.get();
		if(end_owner->get_owned_next()[0]){
			end_owner=end_owner->get_owned_next()->get();
		}
	}
	IRNode end=make_end(
					state_id,next_id,end_owner,
					tree,maping,todo
				);

	return merge_nodes(std::move(start),std::move(end));
}

static IRNode translate_trans(int state_id,TransIR trans,CodeTree::CodeNode* owner,TreeIR &tree,std::vector<int> &maping,std::vector<unsigned int> &todo){
	return make_trans_body(
		state_id,
		translate_write_val(trans.write),
		trans.move,
		owner,
		trans.nextState,

		//now the boiler plate
		tree,maping,todo
	);
}

static void add_state_base(unsigned int add_id,TuringIR ir,TreeIR &tree,std::vector<int> &maping,std::vector<unsigned int> &next_todo){
	StateIR state=ir.states[maping[add_id]];
	
	CodeTree::StateStart* ans = get_state(add_id,tree,maping,next_todo);
	ans->next=std::make_unique<CodeTree::Split>(nullptr,nullptr);

	CodeTree::Split* split=(CodeTree::Split*) (ans->next.get());

	for(int i=0;i<2;i++){
		split->sides[i]=translate_trans(
							add_id,state.trans[i],
							(CodeTree::CodeNode*) split,
							
							tree,maping,next_todo
						);
	}
	
}

static void add_state_no_split(unsigned int add_id,TuringIR ir,TreeIR &tree,std::vector<int> &maping,std::vector<unsigned int> &next_todo){
	StateIR state=ir.states[maping[add_id]];
	
	CodeTree::StateStart* ans = get_state(add_id,tree,maping,next_todo);
	
	ans->next=make_trans_body(
		add_id,
		translate_dual_write_val(state.trans[0].write,state.trans[1].write),
		state.trans[0].move,
		(CodeTree::CodeNode*) ans,
		state.trans[0].nextState,

		//now the boiler plate
		tree,maping,next_todo
	);
}

static void add_state(unsigned int add_id,TuringIR ir,TreeIR &tree,std::vector<int> &maping,std::vector<unsigned int> &next_todo){
	StateIR state=ir.states[maping[add_id]];
	
	if(SemiEq_noWrite_TransIR(state.trans[0],state.trans[1])){
		return add_state_no_split(add_id,ir,tree,maping,next_todo);
	}

	return add_state_base(add_id,ir,tree,maping,next_todo);
}



TreeIR make_inital_tree(TuringIR ir){
	TreeIR ans={};
	ans.push_back((make_state_start(0)));//only way gcc wont think I am copying

	std::vector<int> maping={};
	maping.reserve(ir.len);

	maping.push_back(0);
	for(int i=1;i<ir.len;i++){
		maping.push_back(-1);
	}

	std::vector<unsigned int> todo = {0};
	std::vector<unsigned int> next_todo={};

	while(todo.size()){
		while(todo.size()){
			int cur=todo.back(); todo.pop_back();
			add_state(cur,ir,ans,maping,next_todo);
			
		}
		todo=std::move(next_todo);
		next_todo={};
	}

	ans.shrink_to_fit();
	return ans;
}