#ifndef CODE_TREE_HPP
#define CODE_TREE_HPP

extern "C" {
    #include "turing.h"
}

#include "utils.h"

#include <cstdint>
#include <array>
#include <unordered_set>
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
    virtual CodeNode* next_node(uint32_t idx) = 0;
    virtual TapeVal read_value() = 0;
};

struct Split : public CodeNode {
    std::array<std::unique_ptr<CodeNode>, 2> sides;

    inline Split(std::unique_ptr<CodeNode> left, std::unique_ptr<CodeNode> right) 
        : sides{std::move(left), std::move(right)} {}

    inline CodeNode* next_node(uint32_t i) override {
        if (i < 2) {
            return sides[i].get();
        }
        return nullptr;
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

    inline CodeNode* next_node(uint32_t i) override {
        if (i == 0) {
            return next.get();
        }
        return nullptr;
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

    inline CodeNode* next_node(uint32_t i) override {
        if (i == 0) {
            return next.get();
        }
        return nullptr;
    }
    inline TapeVal read_value() override {
        return TapeVal::Unknown;
    }
};

struct StateStart;
struct StateEnd : public CodeNode {
    int StateID;//id=0 is the start
    std::unique_ptr<StateStart>* next;
    std::unique_ptr<CodeNode>* replace_spot;
    
    

    inline StateEnd(std::unique_ptr<StateStart>* next, int StateID,std::unique_ptr<CodeNode>* replace_spot);
    inline CodeNode* next_node(uint32_t i) override;
    inline TapeVal read_value();
    inline ~StateEnd();
};


struct StateStart : public CodeNode {
private:
    std::unordered_set<StateEnd*> incoming;

public:
    std::unique_ptr<CodeNode> next;
    int StateID;

    inline StateStart(std::unique_ptr<CodeNode> next, int StateID) 
        : next(std::move(next)), StateID(StateID) {}

    inline CodeNode* next_node(uint32_t i) override {
        if (i == 0) {
            return next.get();
        }
        return nullptr;
    }
    inline TapeVal read_value() override {
        return TapeVal::Unchanged;
    }

    inline void insert(StateEnd* x){
        incoming.insert(x);
    }

    inline void erase(StateEnd* x){
        incoming.erase(x);
        optimize(x);
    }
    inline void optimize(StateEnd* x){
        if(StateID==0){
            return;
        }
        if(incoming.size()==1){
            StateEnd* caller=*incoming.begin();
            if(caller->StateID==StateID){
                //we are only alive because we are self refrencing need to clear it and bounce
                *(caller->next)=nullptr;
                return;
            }
            auto tmp=(caller->next);//make sure we properly kill us.
            *(caller->replace_spot)=std::move(next); //this will delete the StateEnd holding us thus also us
            *tmp=nullptr;
            return; 
        }
    }
};

inline StateEnd::StateEnd(std::unique_ptr<StateStart>* next, int StateID, std::unique_ptr<CodeNode>* replace_spot)
    : StateID(StateID), next(next), replace_spot(replace_spot) {
    next->get()->insert(this);
}
inline CodeNode* StateEnd::next_node(uint32_t i) {
    if (i == 0) {
        return next->get();
    }
    return nullptr;
}
inline TapeVal StateEnd::read_value() {
    return TapeVal::Unchanged;
}
inline StateEnd::~StateEnd() {
    next->get()->erase(this);
}



struct Exit : public CodeNode {
    TuringDone code;

    inline Exit(TuringDone code) : code(code) {}

    inline CodeNode* next_node(uint32_t i) override {
        return nullptr;
    }
    inline TapeVal read_value() override {
        return TapeVal::Unchanged;
    }
};

} // namespace CodeTree

#endif // CODE_TREE_HPP
