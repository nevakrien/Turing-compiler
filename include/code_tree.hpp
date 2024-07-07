#ifndef CODE_TREE_HPP
#define CODE_TREE_HPP

extern "C" {
    #include "turing.h"
}

#include "utils.h"

#include <cstdint>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <memory>

enum class TapeVal {
    Unchanged = 0,
    Allways1,
    Allways0,
    Flip,
    Unknown,
};

enum class NodeTypes {
    Split = 0,
    Write,
    Move,
    StateStart,
    StateEnd,
    Exit,
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

    virtual TapeVal read_value() = 0;
    virtual int read_move(){
        return 0;
    }
};

struct Split : public CodeNode {
    std::array<std::unique_ptr<CodeNode>, 2> sides;

    Split(std::unique_ptr<CodeNode> left, std::unique_ptr<CodeNode> right)
        : CodeNode(NodeTypes::Split, sides.data(), 2), sides{std::move(left), std::move(right)} {}

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }
};

struct Write : public CodeNode {
    std::unique_ptr<CodeNode> next;
    TapeVal val;

    Write(TapeVal value, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::Write, &next, 1), next(std::move(next_node)), val(value) {}


    TapeVal read_value() override {
        return val;
    }
};

struct Move : public CodeNode {
    std::unique_ptr<CodeNode> next;
    int move_value;

    Move(int value, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::Move, &next, 1), next(std::move(next_node)), move_value(value) {}


    TapeVal read_value() override {
        return TapeVal::Unknown;
    }
    int read_move() override {
        return move_value;
    }
};

// Forward declare StateStart because StateEnd needs it
struct StateStart;

struct StateEnd : public CodeNode {
    StateStart* owning_state;
    //int StateID;//parent state
    CodeNode* owner;//safe to hold since if owner is deleted we are deleted
    StateStart* next;//safe to hold because of custom behivior of StateStart

    inline StateEnd(StateStart* owning_state,CodeNode* owner, StateStart* next);

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }

    ~StateEnd();
};


struct StateStart : public CodeNode {
    std::unordered_map<StateStart*, std::unordered_set<StateEnd*>> incoming;
    std::unordered_map<StateStart*, std::unordered_set<StateEnd*>> outgoing;
    std::unique_ptr<CodeNode> next;

    int exit_counts[3]={0};
    int StateID;

    StateStart(int stateID, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::StateStart, &next, 1), next(std::move(next_node)), StateID(stateID) {}

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }

    void insert_outgoing(StateEnd* x) {
        outgoing[x->next].insert(x);
    }

    void erase_outgoing(StateEnd* x) {
        outgoing[x->next].erase(x);
    }

    void insert_incoming(StateEnd* x) {
        incoming[x->owning_state].insert(x);
    }

    void erase_incoming(StateEnd* x) {
        incoming[x->owning_state].erase(x);
    }
    ~StateStart(){
        //StateEnd holds a pointer to us we must reset.
        next=nullptr; //triger ower owned StateEnd*/Exit* destructors before we die

        for(const auto& pair : incoming) {
            const std::unordered_set<StateEnd*>& set = pair.second;
            ASSERT(set.size()==0);
        }
        for(int i=0;i<3;i++){
            ASSERT(exit_counts[i]==0);
        }

        //make sure other StateEnds that hold us dont cause UB
        for (const auto& pair : incoming) {
            const std::unordered_set<StateEnd*>& set = pair.second;
            for (StateEnd* x : set) {
                x->next=nullptr;
            }
        }
    }
};

// Implement StateEnd's constructor and destructor after StateStart
inline StateEnd::StateEnd(StateStart* owning_state,CodeNode* owner, StateStart* next)
    : CodeNode(NodeTypes::StateEnd, nullptr, 0),owning_state(owning_state),owner(owner), next(next) {
    next->insert_incoming(this);
    owning_state->insert_outgoing(this);
}

inline StateEnd::~StateEnd() {
    owning_state->erase_outgoing(this); //our onwer deleted us before deleting itself. this is safe
    if(next){//could be StateStart was deleted, only happens rarely
        next->erase_incoming(this);
    } 
}

struct Exit : public CodeNode {
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

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }
};

} // namespace CodeTree

#endif // CODE_TREE_HPP
