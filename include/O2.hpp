#ifndef O2_HPP
#define O2_HPP

#include "code_tree.hpp"
extern "C"{
    #include "IR.h"
    #include "turing.h"
    #include "utils.h"
    #include "compiler.h"
}

#include <cassert>
#include <vector>

using IRNode=std::unique_ptr<CodeTree::CodeNode>;
using TreeIR = std::vector<std::unique_ptr<CodeTree::StateStart>>;
#define IRNODE_CAST(node)\
    UNIQUE_POINTER_CAST(CodeTree::CodeNode,node)

TreeIR make_inital_tree(TuringIR ir);
TreeIR basic_fuse(TreeIR tree);
TreeIR linear_fuse(TreeIR tree);

void warn_unreachble(TreeIR &tree,TuringIR ir);
void Tree_IR_to_ASM(FILE *file,TreeIR ir,const char** names);


bool maybe_inline(std::unique_ptr<CodeTree::StateStart>& state);
void removeEmptyEntries(std::unordered_map<CodeTree::StateStart*, std::unordered_set<CodeTree::StateEnd*>>& stateMap);

static inline IRNode merge_nodes(IRNode prev,IRNode next){
    if(prev==nullptr){
        return next;
    }
    if(next==nullptr){
        return prev;
    }
    assert(prev->get_owned_next_len()==1);
    *prev->get_owned_next()=std::move(next);
    return prev;
}


#endif //O2_HPP