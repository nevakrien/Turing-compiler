#ifndef ASM_HPP
#define ASM_HPP

#include "O2.hpp"
#include "linear_fuse.hpp"
#include <stdexcept>

//_ char, used a lot without a real meaning so I am making it this way
[[maybe_unused]]static const char* _="    ";


static const int BIT_SIZE=4;
static const int EXTEND_STEPS=256;
static const int EXTEND_SIZE=EXTEND_STEPS*BIT_SIZE;

// Enum to represent the different general registers
enum GeneralRegister {
    RAX, RBX, RCX, RDX,
    RSP, RBP, RSI, RDI,
    R8,  R9,  R10, R11,
    R12, R13, R14, R15,
};

// Class to hold all sizes of a given register
struct Register {
public:
	GeneralRegister name;

	Register(){}
    Register(GeneralRegister reg) : name(reg){
        switch (reg) {
            case RAX:
                names = {"rax", "eax", "ax", "al"};
                break;
            case RBX:
                names = {"rbx", "ebx", "bx", "bl"};
                break;
            case RCX:
                names = {"rcx", "ecx", "cx", "cl"};
                break;
            case RDX:
                names = {"rdx", "edx", "dx", "dl"};
                break;
            case RSP:
                names = {"rsp", "esp", "sp", "spl"};
                break;
            case RBP:
                names = {"rbp", "ebp", "bp", "bpl"};
                break;
            case RSI:
                names = {"rsi", "esi", "si", "sil"};
                break;
            case RDI:
                names = {"rdi", "edi", "di", "dil"};
                break;
            case R8:
                names = {"r8", "r8d", "r8w", "r8b"};
                break;
            case R9:
                names = {"r9", "r9d", "r9w", "r9b"};
                break;
            case R10:
                names = {"r10", "r10d", "r10w", "r10b"};
                break;
            case R11:
                names = {"r11", "r11d", "r11w", "r11b"};
                break;
            case R12:
                names = {"r12", "r12d", "r12w", "r12b"};
                break;
            case R13:
                names = {"r13", "r13d", "r13w", "r13b"};
                break;
            case R14:
                names = {"r14", "r14d", "r14w", "r14b"};
                break;
            case R15:
                names = {"r15", "r15d", "r15w", "r15b"};
                break;
            default:
                UNREACHABLE();
        }
    }

    const char* Quad() const {
        return names[0];
    }

    const char* Double() const {
        return names[1];
    }

    const char* Single() const {
        return names[2];
    }

    const char* Byte() const {
        return names[3];
    }
    operator const char*() const {
        return names[0];
    }

private:
    std::array<const char*, 4> names{};
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
    Register address; // always RDI
    Register read;
    BoundsDir left;
    BoundsDir right;
    std::vector<Register> tmp = {};

    int cur_state = 0;
    int cur_split = 0;

    RegisterState(Register address, Register read,BoundsDir left, BoundsDir right)
        : address(address), read(read), left(left), right(right) {}

    void update_state(int new_state) {
        cur_state = new_state;
        cur_split = 0;
    }

    inline bool contains(GeneralRegister reg) {
        if (address.name == reg) return true;
        if (read.name == reg) return true;
        if (left.contains(reg)) return true;
        if (right.contains(reg)) return true;
        for (const auto& r : tmp) {
            if (r.name == reg) return true;
        }
        return false;
    }

    Register add_tmp() {
        for (int i = 15; i >= 0; i--) {
            Register reg = Register((GeneralRegister)i);
            if (!contains(reg.name)) {
                tmp.push_back(reg);
                return reg;
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
    int get_tmp_count(){
    	return tmp.size();
    }
    void tmp_back_to(int removeCount) {
        tmp.erase(tmp.begin() + removeCount, tmp.end());
    }
};


#define Debug_Asm_Print(x) \
	fprintf(file, ";%s\n", x);

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

#endif //ASM_HPP