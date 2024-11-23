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
	//[[maybe_unused]] volatile auto dump0=node->owner->get_owned_next();
	node->owner=owner;
	//[[maybe_unused]] volatile auto dump=owner->get_owned_next();
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
				//we know what the val is because we check for it
				node->sides[i]=fuse(changed,(RunTimeVal)i,std::move(node->sides[i]),node.get());
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

	if(node->val==TapeVal::Unchanged){
	skip_me:
		changed=true;
		return fuse(changed,runtime_val,std::move(node->next),owner);
	}
	
	//check for useless write	
	switch(runtime_val){
		case RunTimeVal::One:
			if(node->val==TapeVal::Allways1){
				goto skip_me;
			}
			break;
		case RunTimeVal::Zero:
			if(node->val==TapeVal::Allways0){
				goto skip_me;
			}
			break;
		default:
			break;
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
	//runtime val will be updated next call
	return fuse(changed,runtime_val,IRNODE_CAST(node),owner); 
}


bool maybe_inline(std::unique_ptr<CodeTree::StateStart> &state){
	using namespace CodeTree;
	removeEmptyEntries(state->incoming);
	
	auto size=state->incoming.size();
	if(size==0){
		state=nullptr;
		return true;

	}

	if(size==1){
		//check for self refrence
		StateStart* calling_state=state->incoming.begin()->first;
		if(calling_state==state.get()){
			state=nullptr;
			return true;
		}

		auto set=state->incoming.begin()->second;
		if(set.size()==1){
			// printf("inlining state %d\n",state->StateID);

			StateEnd* caller_end=*set.begin();
			CodeNode* caller=caller_end->owner;


			ASSERT(caller!=nullptr);

			//get the pointer to us
			IRNode* child=caller->get_owned_next();
			while(child->get()!=(CodeTree::CodeNode*)caller_end){
				child++;
			}

			ASSERT(child->get()==(CodeTree::CodeNode*)caller_end);

			//replace pointers to us
			if(state->next->type()==NodeTypes::StateEnd){
				StateEnd* out = static_cast<StateEnd*>(state->next.get());
				out->owner=caller;
				out->move_owner_state(caller_end->owning_state);

			}

			else {
				// printf("expecting error...\n");
			    //move outgoing to another container
			    std::vector<StateEnd*> state_end_vec;
			    for (const auto& pair : state->outgoing) {
			        const std::unordered_set<StateEnd*>& set = pair.second;
			        for (StateEnd* x : set) {
			            state_end_vec.push_back(x);
			        }
			    }

			    //now we can modify it
			    for (StateEnd* x : state_end_vec) {
			        x->move_owner_state(caller_end->owning_state);
			    }

			}

			// removeEmptyEntries(state->outgoing);	
			// ASSERT(state->outgoing.size()==0);

			// printf("our caller %p\n",caller_end);
			// ASSERT(state->incoming.size()==1);
			// for (const auto& pair : state->incoming) {
			//         const std::unordered_set<StateEnd*>& set = pair.second;
			//         printf("at the start[");
			//         for (StateEnd* x : set) {
			//             printf("%p,",x);
			//         }

			//         printf("]\n");
			//     }
			// ASSERT(caller_end->next==state.get());
			caller_end->move_target_state(nullptr);
			// removeEmptyEntries(state->incoming);
			// for (const auto& pair : state->incoming) {
			//         const std::unordered_set<StateEnd*>& set = pair.second;
			//         printf("found this [");
			//         for (StateEnd* x : set) {
			//             printf("%p,",x);
			//         }
			//         printf("]\n");
			//     }
			// ASSERT(state->incoming.size()==0);


			//compelte the transfer
			*child=std::move(state->next);

			ASSERT(state->next==nullptr);
			
			
			
			// ASSERT(state->incoming.size()==0);
			state->unsafe_clear_incoming();
			state=nullptr;
			return true;
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


TreeIR basic_fuse(TreeIR tree){
	validate_tree(tree);

	bool changed=true;
	while(changed){
		changed=false;


		for(auto i=0u;i<tree.size();i++){
			if(tree[i]==nullptr){
				continue;
			}
			// printf("i=%d\n",i);
			// validate_tree(tree);

			tree[i]->next=fuse(changed,
				RunTimeVal::Unknown,
				std::move(tree[i]->next),
				tree[i].get()
			);

			// validate_tree(tree);

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