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
    Unchanged=0,
    Always1,
    Always0,
    Flip,
    Unknown,
};

namespace CodeTree {

struct CodeNode {
    virtual ~CodeNode() = default;
    virtual std::unique_ptr<CodeNode>* next_nodes()=0;
    virtual int len_next()=0;
    virtual TapeVal read_value() = 0;
};

struct Split : public CodeNode {
    std::array<std::unique_ptr<CodeNode>, 2> sides;

    inline Split(std::unique_ptr<CodeNode> left, std::unique_ptr<CodeNode> right) 
        : sides{std::move(left), std::move(right)} {}

    inline std::unique_ptr<CodeNode>* next_nodes() override{
        return sides.data();
    }
    inline int len_next() override{
        return 2;
    }

    inline TapeVal read_value() override {
        return TapeVal::Unchanged;
    }
};

struct Write : public CodeNode {
    std::unique_ptr<CodeNode> next;
    TapeVal val;

    inline Write(std::unique_ptr<CodeNode> next, TapeVal val) 
        : next(std::move(next)), val(val) {}

    inline std::unique_ptr<CodeNode>* next_nodes() override {
        return &next;
    }
    inline int len_next() override{
        return 1;
    }
    inline TapeVal read_value() override {
        return val;
    }
};

struct Move : public CodeNode {
    std::unique_ptr<CodeNode> next;
    int val;

    inline Move(std::unique_ptr<CodeNode> next, int val) 
        : next(std::move(next)), val(val) {}

    inline std::unique_ptr<CodeNode>* next_nodes() override {
        return &next;
    }
    inline int len_next() override{
        return 1;
    }
    inline TapeVal read_value() override {
        return TapeVal::Unknown;
    }
};

struct StateStart;
struct StateEnd : public CodeNode {
    int StateID; // id=0 is the start
    std::unique_ptr<CodeNode>* next; // always StateStart
    std::unique_ptr<CodeNode>* replace_spot;

    inline StateEnd(std::unique_ptr<CodeNode>* next, int StateID, std::unique_ptr<CodeNode>* replace_spot);
    inline std::unique_ptr<CodeNode>* next_nodes() override {
        return next;
    }
    inline int len_next() override {
        return 1;
    }
    inline TapeVal read_value() override {
        return TapeVal::Unchanged;
    }
    inline ~StateEnd();
};

struct StateStart : public CodeNode {
private:
    std::unordered_map<int, std::unordered_set<StateEnd*>> incoming;

public:
    std::unique_ptr<CodeNode> next;
    int StateID;

    inline StateStart(std::unique_ptr<CodeNode> next, int StateID)
        : next(std::move(next)), StateID(StateID) {}

    inline std::unique_ptr<CodeNode>* next_nodes() override {
        return &next;
    }
    inline int len_next() override {
        return 1;
    }

    inline TapeVal read_value() override {
        return TapeVal::Unchanged;
    }

    inline void insert(StateEnd* x) {
        incoming[x->StateID].insert(x);
    }

    inline void erase(StateEnd* x) {
        incoming[x->StateID].erase(x);
    }
};

inline StateEnd::StateEnd(std::unique_ptr<CodeNode>* next, int StateID, std::unique_ptr<CodeNode>* replace_spot)
    : StateID(StateID), next(next), replace_spot(replace_spot) {
    static_cast<StateStart*>(next->get())->insert(this);
}

inline StateEnd::~StateEnd() {
    static_cast<StateStart*>(next->get())->erase(this);
}




struct Exit : public CodeNode {
    TuringDone code;

    inline Exit(TuringDone code) : code(code) {}

    inline std::unique_ptr<CodeNode>* next_nodes() override {
        return nullptr;
    }
    inline int len_next() override{
        return 0;
    }

    inline TapeVal read_value() override {
        return TapeVal::Unchanged;
    }
};

} // namespace CodeTree

#endif // CODE_TREE_HPP
