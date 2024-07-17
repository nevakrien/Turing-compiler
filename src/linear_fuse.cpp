#include"O2.hpp"
#include "linear_fuse.hpp"
#include "history_maps.hpp"



#define CaseStart(Name) \
	case NodeTypes::Name: {\
		auto node =static_cast<CodeTree::Name*>(input_node.get());

#define CaseEnd() \
	}break

static inline void show_writes(TapeValMap map){
	printf("map changed to:\n");
	for(int i= map.minKey();i<=map.maxKey();i++){
		printf("[(%d) %d]",i,(int)map[i]);
	}
	putchar('\n');
}

//need to add an offset on the read map when passing it recursivly
IRNode make_linear_nodes(IRNode input_node,RunTimeValMap read_map){
	TapeValMap write_map;
	int move=0;

	while(1){
	switch(input_node->type()){
		case NodeTypes::LinearFuse:
		{
			auto node =static_cast<LinearFuse*>(input_node.get());
			const TapeValMap& map=node->write_ops;
			for(int i=map.minKey();i<=map.maxKey();i++){
				read_map[i]=run_tapeval(read_map[i],map[i]);
				write_map[i]=combine_tapevals(write_map[i],map[i]);
			}

			move+=node->move_offset;

			input_node=std::move(node->next);
			continue;
		}

		case NodeTypes::HistoryNode:
		{
			auto node =static_cast<HistoryNode*>(input_node.get());
			input_node=std::move(node->next);
			continue;
		}

		CaseStart(StateEnd)
			IRNode ans;
			if(write_map.size()!=0){
				ans= std::make_unique<LinearFuse>(write_map,move,
					std::make_unique<HistoryNode>(read_map,std::move(input_node)
						)
					);
				node->owner=ans->get_owned_next()[0].get();
			}
			else{
				ans = std::make_unique<HistoryNode>(read_map,std::move(input_node));
				node->owner=ans.get();
			} 
			return ans;
		CaseEnd();

		CaseStart(Move)
			move+=node->move_value;
			write_map[move]=write_map[move];//update min max key if needed 
			
			input_node=std::move(node->next);
			continue;
		CaseEnd();

		CaseStart(Write)
			write_map[move]=combine_tapevals(write_map[move],node->val);
			read_map[move]=run_tapeval(read_map[move],node->val);

			//show_writes(write_map);

			input_node=std::move(node->next);
			continue;
		CaseEnd();

		CaseStart(Split)
			//like default but no history
			if(read_map[move]==RunTimeVal::Unknown){
				for(int i=0;i<2;i++){
					printf("Split side [%d]\n",i);
					node->sides[i]=make_linear_nodes(
										std::move(node->sides[i]),
										read_map.offset_copy(-move)
									);
				}
				if(write_map.size()==0){
					return input_node;
				}
				return std::make_unique<LinearFuse>(write_map,move,std::move(input_node));
			}
			
			//remove branching
			int side=(int)read_map[move];

			printf("Found a knowen split, allways %d\n",side);

			input_node=std::move(node->sides[side]);
			continue;
		CaseEnd();
		
		// //just for printing
		// case NodeTypes::StateStart: 
		// {
		// auto node =static_cast<CodeTree::StateStart*>(input_node.get());
		// printf("starting State (%d)\n",node->StateID);
		// }
		default:
			for(int i=0;i<input_node->get_owned_next_len();i++){
					input_node->get_owned_next()[i]=make_linear_nodes(
										std::move(input_node->get_owned_next()[i]),
										read_map.offset_copy(-move)
									);
				}
			if(write_map.size()==0){
					return std::make_unique<HistoryNode>(read_map,std::move(input_node));
				}
			return std::make_unique<LinearFuse>(write_map,move,
						std::make_unique<HistoryNode>(read_map,std::move(input_node))
						);


	}
}
}

// static void print_node(const CodeTree::CodeNode* node, const TuringIR ir, int depth = 0) {
//     if (!node) return;

//     const char* indent = "  ";
//     for (int i = 0; i < depth; ++i) printf("%s", indent);

//     switch (node->type()) {
//         case NodeTypes::Split:
//             printf("Split\n");
//             for (int i = 0; i < 2; ++i) {
//                 print_node(((const CodeTree::Split*)node)->sides[i].get(), ir, depth + 1);
//             }
//             break;
//         case NodeTypes::Write:
//             printf("Write (val: %d)\n", ((int)((const CodeTree::Write*)node)->read_value()));
//             print_node(((const CodeTree::Write*)node)->next.get(), ir, depth + 1);
//             break;
//         case NodeTypes::Move:
//             printf("Move (move_value: %d)\n", ((const CodeTree::Move*)node)->read_move());
//             print_node(((const CodeTree::Move*)node)->next.get(), ir, depth + 1);
//             break;
//         case NodeTypes::StateStart:
//             printf("StateStart (%s)\n", ir.names[((const CodeTree::StateStart*)node)->StateID]);
//             print_node(((const CodeTree::StateStart*)node)->next.get(), ir, depth + 1);
//             break;
//         case NodeTypes::StateEnd:
//             printf("StateEnd TO(%s)\n", ir.names[((const CodeTree::StateEnd*)node)->next->StateID]);
//             break;
//         case NodeTypes::Exit:
//             printf("Exit (code: %d)\n", ((const CodeTree::Exit*)node)->code);
//             break;
//         case NodeTypes::LinearFuse:
//             printf("LinearFuse (move_offset: %d)\n", ((const LinearFuse*)node)->move_offset);
//             print_node(((const LinearFuse*)node)->next.get(), ir, depth + 1);
//             break;
//         case NodeTypes::HistoryNode:
//             printf("HistoryNode\n");
//             print_node(((const HistoryNode*)node)->next.get(), ir, depth + 1);
//             break;
//         default:
//             printf("Unknown NodeType\n");
//             break;
//     }
// }

static void print_node(const CodeTree::CodeNode* node, int depth = 0) {
    if (!node) return;

    const char* indent = "  ";
    for (int i = 0; i < depth; ++i) printf("%s", indent);

    switch (node->type()) {
        case NodeTypes::Split:
            printf("Split\n");
            for (int i = 0; i < 2; ++i) {
                print_node(((const CodeTree::Split*)node)->sides[i].get(), depth + 1);
            }
            break;
        case NodeTypes::Write:
            printf("Write (val: %d)\n", ((int)((const CodeTree::Write*)node)->read_value()));
            print_node(((const CodeTree::Write*)node)->next.get(), depth + 1);
            break;
        case NodeTypes::Move:
            printf("Move (move_value: %d)\n", ((const CodeTree::Move*)node)->read_move());
            print_node(((const CodeTree::Move*)node)->next.get(), depth + 1);
            break;
        case NodeTypes::StateStart:
        {
        	auto n = static_cast<const CodeTree::StateStart*>(node);
        	printf("StateStart(%d) ",n->StateID);
        	for (const auto& pair : n->incoming) {
			        const std::unordered_set<CodeTree::StateEnd*>& set = pair.second;
			        for (CodeTree::StateEnd* x : set) {
			            printf("reciving [%d] ", x->owning_state->StateID);
			        }
			    }
			for (const auto& pair : n->outgoing) {
			        const std::unordered_set<CodeTree::StateEnd*>& set = pair.second;
			        for (CodeTree::StateEnd* x : set) {
			            printf("going [%d] ", x->next->StateID);
			        }
			    }
			printf("\n");
		}
            
            print_node(((const CodeTree::StateStart*)node)->next.get(), depth + 1);
            break;
        case NodeTypes::StateEnd:
        {	
        	auto n = static_cast<const CodeTree::StateEnd*>(node);
        	printf("StateEnd TO(%d)\n",n->next->StateID);
        }
            break;
        
        case NodeTypes::Exit:
            printf("Exit (code: %d)\n", ((const CodeTree::Exit*)node)->code);
            break;
        case NodeTypes::LinearFuse:
        {
        	auto n = static_cast<const LinearFuse*>(node);
            printf("LinearFuse (move_offset: %d)", (n->move_offset));

            for(int i =n->write_ops.minKey();i<=n->write_ops.maxKey();i++){
            	printf("[(%d) %d]",i,(int)n->write_ops[i]);
            }
           	printf("\n");
        }
           
            print_node(((const LinearFuse*)node)->next.get(), depth + 1);
            break;
        
        case NodeTypes::HistoryNode:
            printf("HistoryNode\n");
            print_node(((const HistoryNode*)node)->next.get(), depth + 1);
            break;
        default:
            printf("Unknown NodeType\n");
            break;
    }
}

static void validate(CodeTree::CodeNode* node,CodeTree::StateStart* start) {
    assert(node != nullptr);

    [[maybe_unused]] volatile TapeVal dump = node->read_value(); // make sure that reading the value works
    [[maybe_unused]] volatile bool dump2 = node->is_final();

    switch(node->type()){
    	case NodeTypes::StateStart:
    		start=(CodeTree::StateStart*)node;
    		break;
    	case NodeTypes::StateEnd:
    		{
    			auto x = static_cast<CodeTree::StateEnd*>(node);
    			assert(x->owning_state==start);
    			// printf("yes we got the right end\n");
    		}
    	default:
    		break;
    }
    for (int i = 0; i < node->get_owned_next_len(); i++) {
        auto next = node->get_owned_next()[i].get();
        assert(next != nullptr);
        validate(next,start);
    }
    return;
}

static void show(TreeIR &tree){
	printf("CURRENT SITUATION\n");
		for(auto i=0u;i<tree.size();i++){
			if(tree[i]==nullptr){
				continue;
			}
			auto x = tree[i].get();
			validate(x,x);
			print_node(x);

		}
		printf("\n\n");
}

TreeIR linear_fuse(TreeIR tree){
	//RunTimeValMap read_map;
	bool changed=true;

	printf("STARTING TREE:\n");
	show(tree);

	while(changed){
		changed=false;		
		
		for(auto i=0u;i<tree.size();i++){
			printf("starting State (%d)\n",i);
			if(tree[i]==nullptr){
				continue;
			}
			RunTimeValMap read_map={};
			tree[i]->next=make_linear_nodes(
				std::move(tree[i]->next),
				read_map
				);
		}

		printf("LINEAR FUSE PASS:\n");
		show(tree);
		
		for(auto i=1u;i<tree.size();i++){
			if(tree[i]==nullptr){
				continue;
			}
			auto x = tree[i].get();
			validate(x,x);
			// print_node(x);
			changed|=maybe_inline(tree[i]);
		}

		printf("INLINE PASS:\n");
		show(tree);
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