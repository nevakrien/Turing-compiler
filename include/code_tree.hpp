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
    Always1,
    Always0,
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
    const NodeTypes node_type;
    const int len_next_nodes;
    const bool is_final_flag;

    CodeNode(NodeTypes type, int len_next, bool is_final)
        : node_type(type), len_next_nodes(len_next), is_final_flag(is_final) {}

    virtual ~CodeNode() = default;

    inline NodeTypes type() const { return node_type; }
    inline int len_next() const { return len_next_nodes; }
    inline bool is_final() const { return is_final_flag; }

    virtual std::unique_ptr<CodeNode>* next_nodes() = 0;
    virtual TapeVal read_value() = 0;
};

struct Split : public CodeNode {
    std::array<std::unique_ptr<CodeNode>, 2> sides;

    Split(std::unique_ptr<CodeNode> left, std::unique_ptr<CodeNode> right)
        : CodeNode(NodeTypes::Split, 2, false), sides{std::move(left), std::move(right)} {}

    std::unique_ptr<CodeNode>* next_nodes() override {
        return sides.data();
    }

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }
};

struct Write : public CodeNode {
    std::unique_ptr<CodeNode> next;
    TapeVal val;

    Write(TapeVal value, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::Write, 1, false), next(std::move(next_node)), val(value) {}

    std::unique_ptr<CodeNode>* next_nodes() override {
        return &next;
    }

    TapeVal read_value() override {
        return val;
    }
};

struct Move : public CodeNode {
    std::unique_ptr<CodeNode> next;
    int move_value;

    Move(int value, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::Move, 1, false), next(std::move(next_node)), move_value(value) {}

    std::unique_ptr<CodeNode>* next_nodes() override {
        return &next;
    }

    TapeVal read_value() override {
        return TapeVal::Unknown;
    }
};

// Forward declare StateStart because StateEnd needs it
struct StateStart;

struct StateEnd : public CodeNode {
    int StateID;
    std::unique_ptr<CodeNode>* next;
    std::unique_ptr<CodeNode>* replace_spot;

    StateEnd(int stateID, std::unique_ptr<CodeNode>* replace_spot, std::unique_ptr<CodeNode>* next_node);

    std::unique_ptr<CodeNode>* next_nodes() override {
        return next;
    }

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
        : CodeNode(NodeTypes::StateStart, 1, false), next(std::move(next_node)), StateID(stateID) {}

    std::unique_ptr<CodeNode>* next_nodes() override {
        return &next;
    }

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }

    void insert(StateEnd* x) {
        incoming[x->StateID].insert(x);
    }

    void erase(StateEnd* x) {
        incoming[x->StateID].erase(x);
    }
};

// Implement StateEnd's constructor and destructor after StateStart
inline StateEnd::StateEnd(int StateID, std::unique_ptr<CodeNode>* replace_spot, std::unique_ptr<CodeNode>* next)
    : CodeNode(NodeTypes::StateEnd, 1, true), StateID(StateID), replace_spot(replace_spot), next(next) {
    static_cast<StateStart*>(next->get())->insert(this);
}

inline StateEnd::~StateEnd() {
    static_cast<StateStart*>(next->get())->erase(this);
}

struct Exit : public CodeNode {
    TuringDone code;

    Exit(TuringDone code_done)
        : CodeNode(NodeTypes::Exit, 0, true), code(code_done) {}

    std::unique_ptr<CodeNode>* next_nodes() override {
        return nullptr;
    }

    TapeVal read_value() override {
        return TapeVal::Unchanged;
    }
};

} // namespace CodeTree

#endif // CODE_TREE_HPP
