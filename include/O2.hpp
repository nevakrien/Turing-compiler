#ifndef O2_HPP
#define O2_HPP

#include "code_tree.hpp"
extern "C"{
    #define IGNORE_C_UTILS
    #include "IR.h"
}

#include <cassert>
#include <vector>

using IRNode=std::unique_ptr<CodeTree::CodeNode>;
using TreeIR = std::vector<IRNode>; //must be StateStart

TreeIR make_inital_tree(TuringIR ir);
void Tree_IR_to_ASM(FILE *file,TreeIR ir);

static inline IRNode append_node(IRNode prev,IRNode next){
    if(prev==nullptr){
        return next;
    }
    if(next==nullptr){
        return prev;
    }
    assert(prev->len_next()==1);
    *prev->next_nodes()=std::move(next);
    return prev;
}

#endif //O2_HPP