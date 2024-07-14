#include"O2.hpp"
#include "linear_fuse.hpp"
#include "history_maps.hpp"




static IRNode track_line(IRNode node,RunTimeValMap &read_map){
	
	unique_ptr write_map;
	int move=0;

	//TODO
	while(1||node->get_owned_next_len()==1){

	}

	// for(int i=write_map.getMinKey();i<=getMaxKey();i++){
	// 	read_map[i]=run_tapeval(read_map[i],write_map[i]);
	// }

	write_map.removeUnchangedEntries();
	IRNode ans=std::make_unique<LinearFuse>(write_map,move,node);
	
	if(type==ans->get_owned_next()[0]->type()){
		auto end =static_cast<CodeTree::StateEnd*>(ans->get_owned_next()[0].get());
		end->owner=ans.get();
	}
	
	return ans;
}

TreeIR linear_fuse(TreeIR tree){
	RunTimeValMap read_map;

}