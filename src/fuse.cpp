#include "O2.hpp"
#include <stdbool.h>

#define DECLARE_FUSE(NodeType) \
    static inline IRNode fuse(bool &changed, RunTimeVal val, std::unique_ptr<CodeTree::NodeType> node,CodeTree::CodeNode* owner)

#define HANDLE_CASE(NodeType) \
    case NodeTypes::NodeType: \
        return fuse(changed, val, UNIQUE_POINTER_CAST(CodeTree::NodeType,node),owner);


DECLARE_FUSE(Split);
DECLARE_FUSE(Write);
DECLARE_FUSE(Move);
DECLARE_FUSE(StateEnd);

//need this so StateEnd refrences stay valid
static inline IRNode fuse(bool &changed,RunTimeVal val,std::unique_ptr<CodeTree::StateEnd> node,CodeTree::CodeNode* owner){
	[[maybe_unused]] volatile auto dump0=node->owner->get_owned_next();
	node->owner=owner;
	[[maybe_unused]] volatile auto dump=owner->get_owned_next();
	return node;
}

static IRNode fuse(bool &changed,RunTimeVal val,IRNode node,CodeTree::CodeNode* owner){
	switch(node->type()){
		HANDLE_CASE(Split)
		HANDLE_CASE(Write)
		HANDLE_CASE(Move)
		HANDLE_CASE(StateEnd)
		default:
			val=run_tapeval(val,node->read_value());
			
			IRNode* children=node->get_owned_next();
			int len=node->get_owned_next_len();
			
			for(int i=0;i<len;i++){
				children[i]=fuse(changed,val,std::move(children[i]),node.get());
			}
			return node;
	}
}



static IRNode fuse(bool &changed,RunTimeVal val,std::unique_ptr<CodeTree::Split> node,CodeTree::CodeNode* owner){
	switch(val){
		case RunTimeVal::Zero:
		case RunTimeVal::One:
			changed=true;
			return fuse(changed,val,std::move(node->sides[(int)(val)]),owner);
		
		case RunTimeVal::Unknown:
			for(int i=0;i<2;i++){
				node->sides[i]=fuse(changed,val,std::move(node->sides[i]),node.get());
			}
			return node;
		default:
			UNREACHABLE();
	}
	return nullptr;
}

static inline IRNode fuse(bool &changed,RunTimeVal val,std::unique_ptr<CodeTree::Move> node,CodeTree::CodeNode* owner){
	if(node->next==nullptr){
		UNREACHABLE();
	}

	if(node->next->type()==NodeTypes::Move){
		changed=true;

		auto next=static_cast<CodeTree::Move*>(node->next.get());
		node->move_value+=next->move_value;
		node->next=std::move(next->next);
		return fuse(changed,RunTimeVal::Unknown,IRNODE_CAST(node),owner);
	}

	if(node->move_value==0){
		changed=true;
		return fuse(changed,val,std::move(node->next),owner);
	}

	node->next=fuse(changed,RunTimeVal::Unknown,std::move(node->next),node.get());
	return node;
}

static inline IRNode fuse(bool &changed,RunTimeVal runtime_val,std::unique_ptr<CodeTree::Write> node,CodeTree::CodeNode* owner){
	if(node->next==nullptr){
		UNREACHABLE();
	}

	if(node->next->type()!=NodeTypes::Write){
		runtime_val=run_tapeval(runtime_val,node->val);
		node->next=fuse(changed,runtime_val,std::move(node->next),node.get());
		return node;
	}
	
	changed=true;

	auto next=static_cast<CodeTree::Write*>(node->next.get());
	node->val=combine_tapevals(node->val,next->val);
	node->next=std::move(next->next);
	//runtime val will be updated next cakk
	return fuse(changed,runtime_val,IRNODE_CAST(node),owner); 
}


//for now this is simply a no op
//the commented out parts are all changes that SHOULD happen
//but since they invalidate StateEnd I cant do them
//need to add an update here
bool maybe_inline(std::unique_ptr<CodeTree::StateStart> &state){
	using namespace CodeTree;
	removeEmptyEntries(state->incoming);
	
	auto size=state->incoming.size();
	if(size==0){
		// state=nullptr;
		// return true;
		return false;
	}

	if(size==1){
		//check for self refrence
		StateStart* calling_state=state->incoming.begin()->first;
		if(calling_state==state.get()){
			// state=nullptr;
			// return true;
			return false;
		}

		auto set=state->incoming.begin()->second;
		if(set.size()==1){
			StateEnd* caller_end=*set.begin();
			CodeNode* caller=caller_end->owner;

			ASSERT(caller!=nullptr);

			//get the pointer to us
			IRNode* child=caller->get_owned_next();
			while(child->get()!=(CodeTree::CodeNode*)caller_end){
				child++;
			}

			// *child=std::move(state->next);
			// state=nullptr;
			// return true;
			return false;
		}
	}

	// //check for infinite loops
	// if(state->exit_counts[HALT]==0){
	// 	removeEmptyEntries(state->outgoing);
	// 	size=state->outgoing.size();
	// 	if(size==0){
	// 		state->next=std::make_unique<Exit>(TIME_OUT,state.get());
	// 		return true;
	// 	}
	// 	if(size==1){
	// 		StateStart* target_state=state->incoming.begin()->first;
	// 		if(target_state==state.get()){
	// 			state->next=std::make_unique<Exit>(TIME_OUT,state.get());
	// 			return true;
	// 		}
	// 	}
	// }

	return false;
}
TreeIR initial_fuse(TreeIR tree){
	bool changed=true;
	while(changed){
		changed=false;

		for(auto i=0u;i<tree.size();i++){
			if(tree[i]==nullptr){
				continue;
			}
			tree[i]->next=fuse(changed,
				RunTimeVal::Unknown,
				std::move(tree[i]->next),
				tree[i].get()
			);
			if(i!=0){
				changed|=maybe_inline(tree[i]);
			}
		}
	}
	//prune null states
	TreeIR ans={};
	ans.reserve(tree.size());
	ans.push_back(std::move(tree[0]));

	for(auto i=1u;i<tree.size();i++){
		if(tree[i]!=nullptr){
			ans.push_back(std::move(tree[i]));
		}
	}
	ans.shrink_to_fit();
	return ans;
}	