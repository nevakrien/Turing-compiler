// #include "O2.hpp"

// static TapeVal translate_write_val(Bit w){
// 	switch(w){
// 		case Bit_0:
// 			return TapeVal::Allways0;
// 		case Bit_1:
// 			return TapeVal::Allways1;
// 	}
// }

// static IRNode make_write(Bit w,Bit read){
// 	if(w==read){
// 		return nullptr;
// 	}	
// 	return std::make_unique<CodeTree::Write>(translate_write_val(write),nullptr);
// }

// static IRNode make_move(Dir m){
// 	if(m==Stay){
// 		return nullptr;
// 	}
// 	return std::make_unique<CodeTree::Move>(m,nullptr);
// }

// static IRNode make_jump(int cur_id,int next_id,TreeIR tree){
// 	if(next_id==-1){
// 		return std::make_unique<CodeTree::Exit>(Halt);
// 	}
// 	return std::make_unique<CodeTree::StateEnd>(cur_id,tree.data()+cur_id,tree.data()+next_id);
// }

// TreeIR make_inital_tree(TuringIR ir){
// 	TreeIR ans;

// 	std::vector<int> maping={};
// 	maping.reserve(ir.len);
// 	for(int i=0;i<ir.len;i++){
// 		maping.push_back(-1);
// 	}

// 	int num_states=0;

// 	std::vector<int> todo = {0};
// 	std::vector<int> next_todo={};

// 	while(todo.size()){
// 		while(todo.size()){
// 			int cur=todo.back(); todo.pop_back();

// 			if(ans[maping[cur]]!=nullptr){
// 				continue;
// 			}
// 			//add the next states
// 			for(int b=0;b<2;b++){
// 				int nextStateID=ir.states[cur].trans[b].nextState;
// 				if(nextStateID==-1){
// 					continue;
// 				}
// 				if(maping[nextStateID]==-1){
// 					next_todo.push_back(nextStateID);
// 					maping[nextStateID]=num_states;
// 					num_states++;
// 				}
// 				nextStateID=maping[nextStateID];
// 			}
// 		}
// 		todo=std::move(next_todo);
// 		next_todo={};
// 	}
	
// 	assert(num_states==ans.size());
// 	return ans;
// }

