#ifndef ARM_HPP
#define ARM_HPP

#include "O2.hpp"
#include "arm_templates.h"
#include "linear_fuse.hpp"
#include <stdexcept>
#include <vector>
#include <cstdio>


//_ char, used a lot without a real meaning so I am making it this way
[[maybe_unused]]static const char* _="    ";


static const int BIT_SIZE=4;
static const int EXTEND_STEPS=256;
static const int EXTEND_SIZE=EXTEND_STEPS*BIT_SIZE;


namespace ARM {

// Define ARM General-Purpose Registers
enum Register {
    R0 = 0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15
};

// Structure to represent register bounds
struct BoundsDir {
    Register init;
    Register limit;

    bool contains(Register reg) {
        return reg == init || reg == limit;
    }
};

// Class to manage the current state of registers
class RegisterState {
public:
    Register address;  // Register for address (e.g., R0 or R1)
    Register zero;     // Dedicated register holding the value 0
    Register one;      // Dedicated register holding the value 1
    Register read;     // Register for reading

    BoundsDir left;           // Bounds for left-side operations
    BoundsDir right;          // Bounds for right-side operations

    std::vector<Register> tmp = {};  // Temporary registers

    int cur_state = 0;  // Current state of the machine
    int cur_split = 0;  // Current split index

    RegisterState(Register address, Register zero, Register one, Register read,
                  BoundsDir left, BoundsDir right)
        : address(address), zero(zero), one(one), read(read), left(left), right(right) {}

    void update_state(int new_state) {
        cur_state = new_state;
        cur_split = 0;
    }

    bool contains(Register reg) {
        if (address == reg || zero == reg || one == reg || read == reg) return true;
        if (left.contains(reg) || right.contains(reg)) return true;
        for (const auto& r : tmp) {
            if (r == reg) return true;
        }
        return false;
    }

    Register add_tmp() {
        for (int i = R0; i >= R14; i--) {  // Exclude R13 (SP), R14 (LR), R15 (PC)
            Register reg = static_cast<Register>(i);
            if (!contains(reg)) {
                tmp.push_back(reg);
                return reg;
            }
        }
        throw std::runtime_error("No available registers");
    }

    Register add_tmp(Register reg) {
        if (contains(reg)) {
            throw std::runtime_error("Register collision");
        }
        tmp.push_back(reg);
        return tmp.back();
    }

    int get_tmp_count() {
        return tmp.size();
    }

    void tmp_back_to(int removeCount) {
        tmp.erase(tmp.begin() + removeCount, tmp.end());
    }
};

#define Debug_Asm_Print(x) \
    fprintf(file, "//%s\n", x);

// Function declarations for generating ARM assembly
void write_asm_general(FILE* file, RegisterState& reg, const char** names, CodeTree::CodeNode* x);

#define DECLARE_WRITE(NodeType) \
    void write_asm(FILE* file, RegisterState& reg, const char** names, CodeTree::NodeType* x);

DECLARE_WRITE(Split);
DECLARE_WRITE(Write);
DECLARE_WRITE(Move);
DECLARE_WRITE(StateStart);
DECLARE_WRITE(StateEnd);
DECLARE_WRITE(Exit);

void write_asm(FILE* file, RegisterState& reg, const char** names, LinearFuse* x);
void write_asm(FILE* file, RegisterState& reg, const char** names, HistoryNode* x);

#define HANDLE_CASE(NodeType) \
    case NodeTypes::NodeType: \
        Debug_Asm_Print(#NodeType); \
        write_asm(file, reg, names, static_cast<CodeTree::NodeType*>(x)); \
        break;

#define HANDLE_SCOPED_CASE(NodeType) \
    case NodeTypes::NodeType: \
        Debug_Asm_Print(#NodeType); \
        write_asm(file, reg, names, static_cast<NodeType*>(x)); \
        break;

void save_registers(FILE* file, RegisterState& reg, std::vector<Register>& registers_to_save, std::vector<Register>& temp_registers);
void restore_registers(FILE* file, const std::vector<Register>& registers_to_save, const std::vector<Register>& temp_registers);

void load_tape_from_stack(FILE* file, RegisterState reg);
void store_tape_to_stack(FILE* file, RegisterState reg);

void bounds_check_asm(FILE* file, RegisterState& reg, Register address, int move);

} // namespace ARM

#endif // ARM_HPP