#include "code_tree.hpp"
#include <cassert>
#include <unordered_set>
#include <memory>
#include <iostream>

#include <random>

using namespace CodeTree;

bool validate_tree(CodeNode* node,std::unordered_set<CodeNode*>* visited){
	assert(node!=nullptr); assert(visited!=nullptr);
	
	volatile TapeVal dump = node->read_value();//make sure that reading the value works

	if(dynamic_cast<Exit*>(node)){
		return true;
	}

	if(!visited->insert(node).second){
		if(dynamic_cast<StateStart*>(node)){
			return true;//only states are alowed to be reachble from more than 1 angle
		}
		return false;
	}

	if(!node->next_node(0)){
		return false;
	}

	for(int i=0;;i++){
		CodeNode* next=node->next_node(i);
		if(next==nullptr){
			break;
		}
		if(!validate_tree(next,visited)){
			return false;
		}
	}
	return true;
}

// Global random number generator setup
std::random_device rd;
std::mt19937 rng(rd()); 

std::unique_ptr<CodeNode> make_random_stuff(int len){
	if(len<=0){
		return std::make_unique<Exit>(HALT);
	}
	switch(rng()%4){
		case 0:
			return std::make_unique<Split>(make_random_stuff(len-4),make_random_stuff(len-4));
		case 1:
			return std::make_unique<Write>(make_random_stuff(len-1),(TapeVal)(rng()%4));
		case 2:
			return std::make_unique<Move>(make_random_stuff(len-1),(int) rng()%10-5);
		case 3:
			return std::make_unique<Exit>(HALT);
	}
	return nullptr;
}

CodeNode* get_random_pre_end(StateStart* start){
	CodeNode* cur=start;
	CodeNode* next=cur->next_node(0);
	while(next){
		if(dynamic_cast<StateEnd*>(next)){
			return cur;
		}
		cur=next;
		next=cur->next_node(0);
		if(rng()%3<3){
			CodeNode* tmp=cur->next_node(1);
			if(tmp){
				next=tmp;
			}
		}
	}
	return cur;

}

int main_test() {
    std::unique_ptr<StateStart> root = std::make_unique<StateStart>(std::make_unique<Exit>(HALT), 0);
    std::unordered_set<CodeNode*> visited;

    if (!validate_tree(root.get(), &visited)) {
        std::cerr << "Error with simple case\n";
        return 1;
    }
    visited.clear();

    std::vector<std::unique_ptr<StateStart>> states={};
    states.push_back(std::make_unique<StateStart>(make_random_stuff(32), 0));

    if (!validate_tree(states[0].get(), &visited)) {
        std::cerr << "Error with tree case\n";
        return 1;
    }

    for(int i=1;i<100;i++){
    	states.push_back(std::make_unique<StateStart>(make_random_stuff(8), i));
    	

    	int insert_state=rng()%states.size();
    	CodeNode* insert_spot=get_random_pre_end(states[insert_state].get());
    	//auto ref=std::make_unique<StateEnd>(&states[i],i,insert_spot);

    	auto t1=dynamic_cast<Write*>(insert_spot);
    	if(t1){
    		t1->next=std::make_unique<StateEnd>(&states[i],i,&t1->next);
    		continue;
    	}

    	auto t2=dynamic_cast<Move*>(insert_spot);
    	if(t2){
    		t2->next=std::make_unique<StateEnd>(&states[i],i,&t2->next);
    		continue;
    	}

    	auto t3=dynamic_cast<Split*>(insert_spot);
    	if(t3){
    		int selected=rng()%2;
    		CodeNode* check=t3->sides[selected].get();
    		if(dynamic_cast<Exit*>(check)||dynamic_cast<StateEnd*>(check)){
    			t3->sides[selected]=std::make_unique<StateEnd>(&states[i],i,&t3->sides[selected]);
    		}
    		else{
    			t3->sides[1-selected]=std::make_unique<StateEnd>(&states[i],i,&t3->sides[1-selected]);
    		}
    		continue;
    	}
    }
	
	for(int i=0;i<states.size();i++){
		 visited.clear();
		 if (!validate_tree(states[i].get(), &visited)) {
	        std::cerr << "Error with tree case\n";
	        return 1;
	    }
	}
   	

    return 0;
}

int main(){
	for(int i=0; i<100; i++){
		if(main_test()){
			return 1;
		}
	}
	std::cout <<"Passed code_tree test\n";
	return 0;
}