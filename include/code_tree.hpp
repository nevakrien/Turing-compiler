#ifndef CODE_TREE_HPP
#define CODE_TREE_HPP

extern "C" {
    #include "turing.h"
}

#include "utils.h"
#include "tape_enums.hpp"

#include <cstdint>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <memory>



enum class NodeTypes{
    Split = 0,
    Write,
    Move,
    StateStart,
    StateEnd,
    Exit,

    LinearFuse,//defined in linear_fuse.hpp
    HistoryNode,//defined in linear_fuse.hpp
};

namespace CodeTree {

struct CodeNode {
protected:
    const NodeTypes node_type;
    std::unique_ptr<CodeNode>* const owned_next;
    const int owned_next_len;
public:
    //we want to hold valid pointers to memory locations, thus copying is not alowed.
    //if A holds a pointer to B then B owns A. 
    //OR B's destructor handles all the A pointers pointing to B (see StateStart)
    CodeNode(const CodeNode&) = delete;
    CodeNode& operator=(const CodeNode&) = delete;
    //~~~~

    CodeNode(NodeTypes type, std::unique_ptr<CodeNode>* owned_next_ptr, int owned_next_len)
        : node_type(type),  owned_next(owned_next_ptr), owned_next_len(owned_next_len) {}

    virtual ~CodeNode() = default;

    inline NodeTypes type() const { return node_type; }
    inline bool is_final() const { return owned_next_len == 0; }
    inline std::unique_ptr<CodeNode>* get_owned_next() const { return owned_next; }
    inline int get_owned_next_len() const { return owned_next_len; }

    virtual TapeVal read_value() const= 0;
    virtual int read_move() const{
        return 0;
    }
};

struct Split final: public CodeNode {
    std::array<std::unique_ptr<CodeNode>, 2> sides;

    Split(std::unique_ptr<CodeNode> left, std::unique_ptr<CodeNode> right)
        : CodeNode(NodeTypes::Split, sides.data(), 2), sides{std::move(left), std::move(right)} {}

    TapeVal read_value() const override {
        return TapeVal::Unchanged;
    }
};

struct Write final: public CodeNode {
    std::unique_ptr<CodeNode> next;
    TapeVal val;

    Write(TapeVal value, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::Write, &next, 1), next(std::move(next_node)), val(value) {}


    TapeVal read_value() const override {
        return val;
    }
};

struct Move final: public CodeNode {
    std::unique_ptr<CodeNode> next;
    int move_value;

    Move(int value, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::Move, &next, 1), next(std::move(next_node)), move_value(value) {}


    TapeVal read_value() const override {
        return TapeVal::Unknown;
    }
    int read_move() const override {
        return move_value;
    }
};

// Forward declare StateStart because StateEnd needs it
struct StateStart;

struct StateEnd final: public CodeNode {
    StateStart* owning_state;
    //int StateID;//parent state
    CodeNode* owner;//safe to hold since if owner is deleted we are deleted
    StateStart* next;//safe to hold because of custom behivior of StateStart

    inline StateEnd(StateStart* owning_state,CodeNode* owner, StateStart* next);
    inline void move_owner_state(StateStart* state);
    inline void move_target_state(StateStart* state);

    TapeVal read_value() const override {
        return TapeVal::Unchanged;
    }

    ~StateEnd();
};

struct StateStart final: public CodeNode {
    std::unordered_map<StateStart*, std::unordered_set<StateEnd*>> incoming;
    std::unordered_map<StateStart*, std::unordered_set<StateEnd*>> outgoing;
    std::unique_ptr<CodeNode> next;

    int exit_counts[3]={0};
    int StateID;

    StateStart(int stateID, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::StateStart, &next, 1), next(std::move(next_node)), StateID(stateID) {}

    TapeVal read_value() const override {
        return TapeVal::Unchanged;
    }

    void insert_outgoing(StateEnd* x) {
        outgoing[x->next].insert(x);
    }

    void erase_outgoing(StateEnd* x) {
        // ASSERT(outgoing[x->next].erase(x)); //if x is null we have issues...
        if(outgoing[x->owning_state].erase(x)==1){
            return;
        }
        printf("ERROR non existing state to remove!!!\n");
        printf("ordered (%p)[ID%d](next %p)\n",x,x->next->StateID,x->next);

        for (const auto& pair : outgoing) {
            const std::unordered_set<StateEnd*>& set = pair.second;
            printf("found this(%p) :[",pair.first);
            for (StateEnd* x : set) {
                printf("(%p)[ID%d],",x,x->next->StateID);
            }
            printf("]\n");
        }
        UNREACHABLE();
    }

    void insert_incoming(StateEnd* x) {
        incoming[x->owning_state].insert(x);
    }

    void erase_incoming(StateEnd* x) {
        ASSERT(incoming[x->owning_state].erase(x));
    }


    void unsafe_clear_incoming(){
        incoming={};
    }
    //time wasted: 2 hours.
    /*
    note on testing this function:
    when states are in their destruction phase
    a lot of their data is invalid
    which means checking things about them causes UB
    its better to test this by checking that no UB occured
    */
    ~StateStart(){
        //triger all our destructors
        //StateEnd/Exit holds a pointer to us so it must die first
        next=nullptr;

        //make sure other StateEnds that hold us dont cause UB
        for (const auto& pair : incoming) {
            const std::unordered_set<StateEnd*>& set = pair.second;
            for (StateEnd* x : set) {
                x->next=nullptr;//hazard pointer
            }
        }
    }
};

inline StateEnd::StateEnd(StateStart* owning_state,CodeNode* owner, StateStart* next)
    : CodeNode(NodeTypes::StateEnd, nullptr, 0),owning_state(owning_state),owner(owner), next(next) {
    next->insert_incoming(this);
    owning_state->insert_outgoing(this);
}

inline StateEnd::~StateEnd() {
    //our onwer deleted us before deleting itself. this is safe
    owning_state->erase_outgoing(this); 
    
    //could be next was deleted 
    //if it was its table does not matter
    if(next){
        next->erase_incoming(this);
    } 
}

inline void StateEnd::move_owner_state(StateStart* new_owning_state) {
    ASSERT(new_owning_state!=nullptr);

    owning_state->erase_outgoing(this);
    owning_state = new_owning_state;
    owning_state->insert_outgoing(this);
    
}

inline void StateEnd::move_target_state(StateStart* new_next_state) {
    if (next) {
        next->erase_incoming(this);
    }
    next = new_next_state;
    if (next) {
        next->insert_incoming(this);
    }
}


struct Exit final: public CodeNode {
    TuringDone code;
    StateStart* owner;

    Exit(TuringDone code_done,StateStart* owner)
        : CodeNode(NodeTypes::Exit, nullptr, 0), code(code_done),owner(owner) {
            owner->exit_counts[code]++;
        }

    ~Exit(){
        //owner allways removes us before dying, this is safe
         owner->exit_counts[code]--;
    }

    TapeVal read_value() const override {
        return TapeVal::Unchanged;
    }
};


} // namespace CodeTree


#endif // CODE_TREE_HPP
