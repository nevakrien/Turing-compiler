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

//debuging
static inline void validate_incoming(CodeTree::StateStart* state){
    for (const auto& pair : state->incoming) {
        const std::unordered_set<CodeTree::StateEnd*>& set = pair.second;
        for (CodeTree::StateEnd* x : set) {
            if(x->owning_state!=pair.first){
                printf("ERROR: wrong key for incoming (%p) for (%p)\n",pair.first,x->owning_state);
                UNREACHABLE();
            }
        }
    }
}

static inline void validate_outgoing(CodeTree::StateStart* state){
    for (const auto& pair : state->outgoing) {
        const std::unordered_set<CodeTree::StateEnd*>& set = pair.second;
        for (CodeTree::StateEnd* x : set) {
            if(x->next!=pair.first){
                printf("ERROR: wrong key for outging (%p) for (%p)\n",pair.first,x->next);
                UNREACHABLE();
            }
        }
    }
}


static inline void validate_state_ends( CodeTree::CodeNode* node) {
    if (node == nullptr) return;

    if (node->type() == NodeTypes::StateEnd) {
        auto n = static_cast< CodeTree::StateEnd*>(node);
        //hacky fix 
        ASSERT(n->next->incoming[n->owning_state].count(n));
        ASSERT(n->owning_state->outgoing[n->next].count(n));
        return;
    }


    for (int i = 0; i < node->get_owned_next_len(); ++i) {
        validate_state_ends(node->get_owned_next()[i].get());
    }
}



static inline void validate_tree( TreeIR &tree){

    for(auto i=0u;i<tree.size();i++){
        if(tree[i]==nullptr){
            continue;
        }
        // printf("i=%d\n",i);
        validate_state_ends(tree[i].get());
        validate_incoming(tree[i].get());
        validate_outgoing(tree[i].get());
    }
}


#endif //O2_HPP