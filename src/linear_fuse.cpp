#include"O2.hpp"
#include "linear_fuse.hpp"
#include "history_maps.hpp"



#define CaseStart(Name) /
	case NodeTypes::Name:/
		auto node =static_cast<CodeTree::Name*>(input_node.get());

//need to add an offset on the read map when passing it recursivly
IRNode make_linear_nodes(IRNode input_node,RunTimeValMap &read_map){
	unique_ptr write_map;
	int move=0;

	switch(input_node->type()){
		CaseStart(StateEnd)
			IRNode ans= std::make_unique<LinearFuse>(write_map,move
					std::make_unique<HistoryNode>(read_map,std::move(input_node)
						)
					);
			node->owner=ans->get_owned_next()[0]->get_owned_next()[0].get();
			return ans;

		CaseStart(Move)
			move+=node->move_value;
			write_map[move]=write_map[move];//update min max key if needed 
			
			input_node=std::move(node->next);
			continue;
		
		CaseStart(Write)
			write_map[move]=combine_tapevals(write_map[move],node->value);
			read_map[move]=run_tapeval(read_map[move],node->value);

			input_node=std::move(node->next);
			continue;

		CaseStart(Split)
			if(read_map[move]==RunTimeVal::Unknown){
				for(int i=0;i<2;i++){
					node->sides[i]=make_linear_nodes(
										std::move(node->sides[i]),
										std::copy(read_map)
									);
				}
				return std::make_unique<LinearFuse>(write_map,move,std::move(input_node));
			}

			int side=(int)read_map[move];
			input_node=std::move(node->sides[side]);
			continue;

		CaseStart(LinearFuse)
			const TapeValMap& map=node->write_ops;
			for(int i=map.getMinKey();i<map.getMaxKey();i++){
				read_map[i]=run_tapeval(read_map[i],map[i]);
				write_map[i]=combine_tapevals(write_map[i],map[i]);
			}

			input_node=std::move(node->next);
			continue;

		CaseStart(HistoryNode)
			input_node=std::move(node->next);
			continue;

		default:
			for(int i=0;i<input_node->get_owned_next_len();i++){
					input_node->get_owned_next[i]=make_linear_nodes(
										std::move(input_node->get_owned_next[i]),
										std::copy(read_map)
									);
				}
			return std::make_unique<LinearFuse>(write_map,move,
						std::make_unique<HistoryNode>(read_map,std::move(input_node)
						);


	}
}

TreeIR linear_fuse(TreeIR tree){
	//RunTimeValMap read_map;
	bool changed=true;
	while(changed){
		changed=false;

		for(auto i=0u;i<tree.size();i++){
			if(tree[i]==nullptr){
				continue;
			}
			RunTimeValMap read_map={};
			tree[i]->next=make_linear_nodes(
				std::move(tree[i]->next),
				read_map
				);
		}
		for(auto i=1u;i<tree.size();i++){
			changed|=maybe_inline(tree[i]);
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