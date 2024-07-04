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
};

// Forward declare StateStart because StateEnd needs it
struct StateStart;

struct StateEnd : public CodeNode {
    int StateID;//parent state
    CodeNode* owner;//safe to hold since if owner is deleted we are deleted
    StateStart* next;//safe to hold because of custom behivior of StateStart

    StateEnd(int stateID, CodeNode* owner, StateStart* next_node);

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }

    ~StateEnd();
};


struct StateStart : public CodeNode {
    std::unordered_map<int, std::unordered_set<StateEnd*>> incoming;
    std::unique_ptr<CodeNode> next;
    int StateID;

    StateStart(int stateID, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::StateStart, &next, 1), next(std::move(next_node)), StateID(stateID) {}

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }

    void insert(StateEnd* x) {
        incoming[x->StateID].insert(x);
    }

    void erase(StateEnd* x) {
        incoming[x->StateID].erase(x);
    }
    ~StateStart(){
        //StateEnd holds a pointer to us we must reset.
        next=nullptr; //remove all possible incoming[StateID]

        for (const auto& pair : incoming) {
            const std::unordered_set<StateEnd*>& set = pair.second;
            for (StateEnd* x : set) {
                x->next=nullptr;
            }
        }
    }
};

// Implement StateEnd's constructor and destructor after StateStart
inline StateEnd::StateEnd(int StateID, CodeNode* owner, StateStart* next)
    : CodeNode(NodeTypes::StateEnd, nullptr, 0), StateID(StateID), owner(owner), next(next) {
    next->insert(this);
}

inline StateEnd::~StateEnd() {
    if(next){//could be StateStart was deleted, only happens rarely
        next->erase(this);
    }   
}

struct Exit : public CodeNode {
    TuringDone code;

    Exit(TuringDone code_done)
        : CodeNode(NodeTypes::Exit, nullptr, 0), code(code_done) {}

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }
};

} // namespace CodeTree

#endif // CODE_TREE_HPP
