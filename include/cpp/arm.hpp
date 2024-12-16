#ifndef ARM_HPP
#define ARM_HPP

#include "O2.hpp"
#include "linear_fuse.hpp"
#include <stdexcept>

//_ char, used a lot without a real meaning so I am making it this way
[[maybe_unused]]static const char* _="    ";


static const int BIT_SIZE=4;
static const int EXTEND_STEPS=256;
static const int EXTEND_SIZE=EXTEND_STEPS*BIT_SIZE;

struct Register {
    int id; // ARM register number (0-30)

    Register(int reg_number) : id(reg_number) {
        if (reg_number < 0 || reg_number > 30) {
            throw std::runtime_error("Invalid register number");
        }
    }
};

//dont use named registers here... 
struct BoundsDir{
	Register init;
	Register limit;
	bool contains(GeneralRegister reg){
		if(init.name==reg){return true;}
		if(limit.name==reg){return true;}
		return false;
	}
};

//dont hold rsp here...
class RegisterState {
public:
    Register one;   // Register to hold the value 1
    Register zero;  // Register to hold the value 0

    Register address; // Always holds the address base (RDI equivalent)
    Register read;    // A register used for reading
    BoundsDir left;   // Bounds for the left side
    BoundsDir right;  // Bounds for the right side
    std::vector<Register> tmp = {}; // Temporary registers

    int cur_state = 0;
    int cur_split = 0;

    RegisterState(Register address, Register read, BoundsDir left, BoundsDir right)
        : address(address), read(read), left(left), right(right) {
        // Initialize `one` and `zero` to fixed temporary registers
        one = Register(19);   // Assign x19 for the value 1
        zero = Register(20);  // Assign x20 for the value 0
    }

    void initialize_constants(FILE* file) {
        // Load the constant values into the reserved registers
        fprintf(file, "    mov w%d, #1\n", one.id);   // Load 1 into `one`
        fprintf(file, "    mov w%d, #0\n", zero.id); // Load 0 into `zero`
    }

    void update_state(int new_state) {
        cur_state = new_state;
        cur_split = 0;
    }

    inline bool contains(GeneralRegister reg) {
        if (address.name == reg) return true;
        if (read.name == reg) return true;
        if (one.name == reg) return true;  // Check `one`
        if (zero.name == reg) return true; // Check `zero`
        if (left.contains(reg)) return true;
        if (right.contains(reg)) return true;
        for (const auto& r : tmp) {
            if (r.name == reg) return true;
        }
        return false;
    }

    Register add_tmp() {
        for (int reg = 21; reg <= 28; reg++) { // Use x21-x28 for temporary registers
            if (!contains(reg)) {
                tmp.emplace_back(reg);
                return tmp.back();
            }
        }
        throw std::runtime_error("No available registers");
    }

    Register add_tmp(GeneralRegister reg) {
        if (contains(reg)) {
            throw std::runtime_error("register collision");
        }
        tmp.push_back(Register(reg));
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

//this function should be defined in the translation unit used.
//exmple can be find in tree_asm.cpp. 
//the core idea is to not check for non existing cases
void write_asm_genral(FILE *file, RegisterState &reg, const char** names, CodeTree::CodeNode* x);

//every node type has its own translation, we declare them ahead of time here

#define DECLARE_WRITE(NodeType) \
    void write_asm(FILE *file, RegisterState &reg, const char** names, CodeTree::NodeType* x);

DECLARE_WRITE(Split);
DECLARE_WRITE(Write);
DECLARE_WRITE(Move);
DECLARE_WRITE(StateStart);
DECLARE_WRITE(StateEnd);
DECLARE_WRITE(Exit);

void write_asm(FILE *file, RegisterState &reg, const char** names, LinearFuse* x);
void write_asm(FILE *file, RegisterState &reg, const char** names, HistoryNode* x);


#define HANDLE_CASE(NodeType) \
    case NodeTypes::NodeType: \
    	Debug_Asm_Print(#NodeType); \
        write_asm(file, reg,names, static_cast<CodeTree::NodeType*>(x)); \
        break;

#define HANDLE_SCOPED_CASE(NodeType) \
    case NodeTypes::NodeType: \
        Debug_Asm_Print(#NodeType); \
        write_asm(file, reg,names, static_cast<NodeType*>(x)); \
        break;

void save_registers(FILE *file, RegisterState &reg, std::vector<Register> &registers_to_save, std::vector<Register> &temp_registers);
void restore_registers(FILE *file, const std::vector<Register> &registers_to_save, const std::vector<Register> &temp_registers);

void load_tape_from_stack(FILE *file,RegisterState reg);
void store_tape_to_stack(FILE *file,RegisterState reg);

void unsafe_bounds_check_asm(FILE *file, RegisterState &reg, Register address, int move);

#endif //ARM_HPP