#include "O2.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

static const BIT_SIZE=4;

// Enum to represent the different general registers
enum GeneralRegister {
    RAX, RBX, RCX, RDX,
    RSP, RBP, RSI, RDI,
    R8,  R9,  R10, R11,
    R12, R13, R14, R15
};

// Class to hold all sizes of a given register
struct Register {
public:
    Register(GeneralRegister reg) {
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

struct BoundsDir{
	Register init;
	Register limit;
};

struct RegisterState{
	Register adress;//allways RDI
	Register read;
	BoundsDir left;
	BoundsDir right;
	std::vector<Register> tmp={};

	int cur_state=0;
	int cur_split=0;

	RegisterState(Register read,BoundsDir left,BoundsDir right) 
		:adress(Register(RDI)), read(read) , left(left), right(right) {}

	void update_state(int new_state){
		cur_state=new_state;
		cur_split=0;
	}

};

const char* spaces="    ";

//every node type has its own translation, we declare them ahead of time here

#define DECLARE_WRITE(NodeType) \
		static void write_asm(FILE *file, RegisterState &reg,const char** names, CodeTree::NodeType* x)

#define HANDLE_CASE(NodeType) \
    case NodeTypes::NodeType: \
        write_asm(file, reg,names, static_cast<CodeTree::NodeType*>(x)); \
        break;

DECLARE_WRITE(Split);
DECLARE_WRITE(Write);
DECLARE_WRITE(Move);
DECLARE_WRITE(StateStart);
DECLARE_WRITE(StateEnd);
DECLARE_WRITE(Exit);

static void write_genral(FILE *file, RegisterState &reg,const char** names, CodeTree::CodeNode* x) {
    switch(x->type()) {
        HANDLE_CASE(Split)
        HANDLE_CASE(Write)
        HANDLE_CASE(Move)
        HANDLE_CASE(StateStart)
        HANDLE_CASE(StateEnd)
        HANDLE_CASE(Exit)
        default:
            UNREACHABLE();
    }
}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Exit* x){
	switch(x->code){
		case HALT:
			fprintf(file,"%sjmp exit_good\n\n",spaces);
			break;
		case TIME_OUT:
			fprintf(file,"%sjmp exit_time_out\n\n",spaces);
			break;
		case OUT_OF_TAPE:
			fprintf(file,"%sjmp exit_out_of_tape\n\n",spaces);
			break;
		default:
			UNREACHABLE();
	}
	
}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::StateStart* x){
	reg.update_state(x->StateID);
	fprintf(file,"L%d:",reg.cur_state);
	if(names){
		fprintf(file," ;%s",names[reg.cur_state]);
	}
	fprintf(file,"\n");
	write_genral(file,reg,names,x->next.get());
}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Write* x){
	
	switch(x->val){
		case TapeVal::Unchanged:
			break;
		case TapeVal::Allways1:
			fprintf(file,"%smov [%s],double 1\n",spaces,reg.adress.Double());
			break;
		case TapeVal::Allways0:
			fprintf(file,"%smov [%s],double 0\n",spaces,reg.adress.Double());
			break;

		case TapeVal::Flip:
			fprintf(file,"%smov [%s], %s\n",spaces,reg.read.Double(),reg.adress.Double());
			fprintf(file,"%smov %s, 1;simple flip\n",spaces,reg.adress.Double());
			fprintf(file,"%smov [%s], %s\n",spaces,reg.adress.Double(),reg.read.Double());
			break;
		default:
			UNREACHABLE();
	}
	write_genral(file,reg,names,x->next.get());
}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::StateEnd* x){
	int next_id=x->next->StateID;
	fprintf(file,"%sjmp L%d\n",spaces,next_id);
}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Split* x){
	fprintf(file,"%s;spliting\n",spaces);
	fprintf(file,"%smov [%s], %s\n",spaces,reg.read.Double(),reg.adress.Double());
	fprintf(file,"%stest %s, %s\n",spaces,reg.read.Quad(),reg.read.Quad());
	
	fprintf(file,"%sjnz L%d_%d\n\n",spaces,reg.cur_state,++reg.cur_split); 
	//TODO: figure the name out //
	write_genral(file,reg,names,x->sides[0].get());
	
	fprintf(file,"L%d_%d:\n",reg.cur_state,++reg.cur_split);
	write_genral(file,reg,names,x->sides[1].get());

}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Move* x){
	int move=(x->move_value)*BIT_SIZE;
	
	if(move!=0){
		fprintf(file,"%sadd %s, %d\n",spaces,reg.adress.Quad(),move);
		fprintf(file,"%scmp %s, %d\n",spaces,reg.adress.Quad(),reg.left.init.Quad());
	}
	if(move<0){
		fprintf(file,"%sjae L%d_%d\n\n",spaces,reg.cur_state,++reg.cur_split);
		//TODO
	}
	else if(move>0){
		fprintf(file,"%sjbe L%d_%d\n\n",spaces,reg.cur_state,++reg.cur_split);
		//TODO	
	}
	
	if(move!=0){
		fprintf(file,"L%d_%d:\n\n",reg.cur_state,++reg.cur_split);
	}
	write_genral(file,reg,names,x->next.get());
}

void Tree_IR_to_ASM(FILE *file,TreeIR ir,const char** names){
	RegisterState reg=RegisterState(
		Register(R15),
		(BoundsDir){Register(R11),Register(R9)},
		(BoundsDir){Register(R10),Register(R8)}
	);

	//TODO: add missing global functions

	for(auto i=0u;i<ir.size();i++){
		write_asm(file,reg,names,ir[i].get());
	}
}