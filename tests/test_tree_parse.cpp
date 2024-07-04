#include "O2.hpp"

using namespace CodeTree;

extern "C"{
	#include "cli.h"
	#include  "IR.h"
	#include "utils.h"
}

#define FREE_FOR_DEBUG

bool validate_owned_tree(CodeNode* node) {
    assert(node != nullptr);

    [[maybe_unused]] volatile TapeVal dump = node->read_value(); // make sure that reading the value works
    [[maybe_unused]] volatile bool dump2 = node->is_final();

    for (int i = 0; i < node->get_owned_next_len(); i++) {
        CodeNode* next = node->get_owned_next()[i].get();
        if (next == nullptr) {
            return false;
        }
        if (!validate_owned_tree(next)) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]){
	if(argc!=2){
		printf("usage: %s src_file out_file\n",argv[0]);
		return 1;
	}
	CompileStepOne comp = first_compile_step(argv[1]);
	TreeIR tree= make_inital_tree(comp.ir);
	
	for(auto i=0u;i<tree.size();i++){
		validate_owned_tree((CodeNode*)tree[i].get());
	}

	warn_unreachble(tree,comp.ir);

	//no need to free anything.
	#ifdef FREE_FOR_DEBUG
	free_TuringIR(comp.ir);
	free(comp.text);
	#endif
	
	return 0;
}