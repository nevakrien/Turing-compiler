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

	// Constructor
    GlobalVars(TreeIR& tree, std::vector<int>& maping, std::vector<unsigned int>& todo, const TuringIR& ir)
        : tree(tree), maping(maping), todo(todo), ir(ir) {
    }
};

static void print_node(const CodeTree::CodeNode* node,const TuringIR ir, int depth = 0) {
    if (!node) return;

    const char* indent = "  ";
    for (int i = 0; i < depth; ++i) printf("%s", indent);

    switch (node->type()) {
        case NodeTypes::Split:
            printf("Split\n");
            for (int i = 0; i < 2; ++i) {
                print_node(((const CodeTree::Split*)node)->sides[i].get(),ir, depth + 1);
            }
            break;
        case NodeTypes::Write:
            printf("Write (val: %d)\n", ((int)((const CodeTree::Write*)node)->read_value()));
            print_node(((const CodeTree::Write*)node)->next.get(),ir, depth + 1);
            break;
        case NodeTypes::Move:
            printf("Move (move_value: %d)\n", ((const CodeTree::Move*)node)->read_move());
            print_node(((const CodeTree::Move*)node)->next.get(),ir, depth + 1);
            break;
        case NodeTypes::StateStart:
            printf("StateStart (%s)\n", ir.names[((const CodeTree::StateStart*)node)->StateID]);
            print_node(((const CodeTree::StateStart*)node)->next.get(),ir, depth + 1);
            break;
        case NodeTypes::StateEnd:
            printf("StateEnd TO(%s)\n",ir.names[((const CodeTree::StateEnd*)node)->next->StateID]);
            break;
        case NodeTypes::Exit:
            printf("Exit (code: %d)\n", ((const CodeTree::Exit*)node)->code);
            break;
        default:
            printf("Unknown NodeType\n");
            break;
    }
}


//may need to make the state and add it to our map
//this can only triger once per state so no state would be added twice to todo
static CodeTree::StateStart* get_state(unsigned int id,GlobalVars vars){
	int maped_id=vars.maping[id];
	if(maped_id==-1){
		maped_id=vars.tree.size();
		
		vars.maping[id]=maped_id;
		vars.tree.push_back(make_state_start(id));
		vars.todo.push_back(id);
		//printf("adding state \"%s\" maped %d=>%d\n",vars.ir.names[id],id,maped_id);
	}

	return vars.tree[maped_id].get();
}

static IRNode make_end(int id_cur,int id_next,CodeTree::CodeNode* owner,GlobalVars vars){
	if(id_next==-1){
		return std::make_unique<CodeTree::Exit>(
				HALT,
				get_state((unsigned int) id_cur,vars)
			);
	}

	ASSERT(0<=id_next);
	return std::make_unique<CodeTree::StateEnd>(
		get_state((unsigned int) id_cur,vars)
		,owner,
		get_state((unsigned int) id_next,vars)
	);
}

static IRNode make_trans_body(int state_id,TapeVal w, Dir m,CodeTree::CodeNode* owner,int next_id,GlobalVars vars){
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
					vars
				);

	if(start==nullptr){
		return end;
	}
	
	end_owner->get_owned_next()[0]=std::move(end);
	return start;
}

static IRNode translate_trans(int state_id,TransIR trans,CodeTree::CodeNode* owner,GlobalVars vars){
	return make_trans_body(
		state_id,
		translate_write_val(trans.write),
		trans.move,
		owner,
		trans.nextState,

		//now the boiler plate
		vars
	);
}

static void add_state_base(unsigned int add_id,GlobalVars vars){
	//printf("state \"%s\" handled normaly\n",vars.ir.names[add_id] );

	StateIR state=vars.ir.states[add_id];
	
	CodeTree::StateStart* ans = get_state(add_id,vars);
	ans->next=std::make_unique<CodeTree::Split>(nullptr,nullptr);

	CodeTree::Split* split=(CodeTree::Split*) (ans->next.get());

	for(int i=0;i<2;i++){
		split->sides[i]=translate_trans(
							add_id,state.trans[i],
							(CodeTree::CodeNode*) split,
							
							vars
						);
	}
	
}

static void add_state_no_split(unsigned int add_id,GlobalVars vars){
	//printf("state \"%s\" handled with no split\n",vars.ir.names[add_id] );

	StateIR state=vars.ir.states[add_id];
	
	CodeTree::StateStart* ans = get_state(add_id,vars);
	
	ans->next=make_trans_body(
		add_id,
		translate_dual_write_val(state.trans[0].write,state.trans[1].write),
		state.trans[0].move,
		(CodeTree::CodeNode*) ans,
		state.trans[0].nextState,

		//now the boiler plate
		vars
	);
}

static void add_state(unsigned int add_id,GlobalVars vars){
	StateIR state=vars.ir.states[add_id];
	
	if(SemiEq_noWrite_TransIR(state.trans[0],state.trans[1])){
		return add_state_no_split(add_id,vars);
	}

	return add_state_base(add_id,vars);
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

	GlobalVars vars=GlobalVars(ans,maping,next_todo,ir);

	while(todo.size()){
		//printf("run start:\n");
		while(todo.size()){
			int cur=todo.back(); todo.pop_back();
			//printf("resolving state \"%s\"\n",vars.ir.names[cur] );
			add_state(cur,vars);
			//printf("done with state \"%s\"\n",vars.ir.names[cur] );
			
		}
		//printf("run end: updating todo\n!!!!!!!!!!!!!\n\n");
		todo=std::move(next_todo);
		next_todo={};
		vars.todo=next_todo;
	}

	// for(auto i=0u;i<ans.size();i++){
	// 	print_node(ans[i].get(),ir);
	// }

	ans.shrink_to_fit();
	return ans;
}