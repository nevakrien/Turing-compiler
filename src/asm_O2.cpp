#include "O2.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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
        for (int i = 0; i < 16; i++) {
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

//_ char, used a lot without a real meaning so I am making it this way
const char* _="    ";

#define Debug_Asm_Print(x) \
	fprintf(file, ";%s\n", x);

//every node type has its own translation, we declare them ahead of time here

#define DECLARE_WRITE(NodeType) \
    static void write_asm(FILE *file, RegisterState &reg, const char** names, CodeTree::NodeType* x)

#define HANDLE_CASE(NodeType) \
    case NodeTypes::NodeType: \
    	Debug_Asm_Print(#NodeType); \
        write_asm(file, reg,names, static_cast<CodeTree::NodeType*>(x)); \
        break;

DECLARE_WRITE(Split);
DECLARE_WRITE(Write);
DECLARE_WRITE(Move);
DECLARE_WRITE(StateStart);
DECLARE_WRITE(StateEnd);
DECLARE_WRITE(Exit);

static void write_genral(FILE *file, RegisterState &reg, const char** names, CodeTree::CodeNode* x) {
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
			fprintf(file,"%sjmp exit_good\n\n",_);
			break;
		case TIME_OUT:
			fprintf(file,"%sjmp exit_time_out\n\n",_);
			break;
		case OUT_OF_TAPE:
			fprintf(file,"%sjmp exit_out_of_tape\n\n",_);
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
			fprintf(file,"%smov [%s],dword 1\n",_,reg.address.Quad());
			break;
		case TapeVal::Allways0:
			fprintf(file,"%smov [%s],dword 0\n",_,reg.address.Quad());
			break;

		case TapeVal::Flip:
			fprintf(file,"%smov %s, [%s]\n",_,reg.read.Double(),reg.address.Quad());
			fprintf(file,"%sxor %s, 1;simple flip\n",_,reg.read.Double());
			fprintf(file,"%smov [%s], %s\n",_,reg.address.Quad(),reg.read.Double());
			break;
		default:
			UNREACHABLE();
	}
	write_genral(file,reg,names,x->next.get());
}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::StateEnd* x){
	int next_id=x->next->StateID;
	fprintf(file,"%sjmp L%d\n",_,next_id);
}

static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Split* x){
	fprintf(file,"%s;spliting\n",_);
	fprintf(file,"%smov %s, [%s]\n",_,reg.read.Double(),reg.address.Quad());
	fprintf(file,"%stest %s, %s\n",_,reg.read.Quad(),reg.read.Quad());
	
	int ret_spot=++reg.cur_split;

	fprintf(file,"%sjnz L%d_%d\n\n",_,reg.cur_state,ret_spot); 
	write_genral(file,reg,names,x->sides[0].get());
	
	fprintf(file,"L%d_%d:\n",reg.cur_state,ret_spot);
	write_genral(file,reg,names,x->sides[1].get());

}

//this function MAY act goffy if rcx is used as a limit register, there is no reason to do that tho...
static void write_asm(FILE *file,RegisterState &reg,const char** names,CodeTree::Move* x){
	int move=(x->move_value)*BIT_SIZE;
	
	if(move==0){
		write_genral(file,reg,names,x->next.get());
		return;
	}
	

	fprintf(file,"%sadd %s, %d\n",_,reg.address.Quad(),move);
	//now we no longer need to modify adress 
	//so even if we need the adress register we can restore it later
	
	//setup all the registers we need
	Register tmp_rdi;
	Register tmp_rcx;
	Register tmp_rax;

	int prev_tmp_count=reg.get_tmp_count();//so we can return

	//check colisions
	if(reg.contains(RDI)){
		tmp_rdi=reg.add_tmp();
		fprintf(file,"%smov %s, rdi ;we need rdi\n",_,tmp_rdi.Quad());
	}
	else{
		tmp_rdi=reg.add_tmp(RDI);
	}
	
	if(reg.contains(RCX)){
		tmp_rcx=reg.add_tmp();
		fprintf(file,"%smov %s, rcx;we need rcx\n",_,tmp_rcx.Quad());
	}
	else{
		tmp_rcx=reg.add_tmp(RCX);
	}

	
	if(reg.contains(RAX)){
		tmp_rax=reg.add_tmp();
		fprintf(file,"%smov %s, rax;we need rax\n",_,tmp_rax.Quad());
	}
	else{
		tmp_rax=reg.add_tmp(RAX);
	}

	//useful globals
	BoundsDir bounds = move > 0 ? reg.right : reg.left;
	const char* jump=(move > 0 ? "jbe" : "jae");



	//first check
	int ret_idx=++reg.cur_split;
	fprintf(file,"%scmp %s, %s;bounds check init\n",_,reg.address.Quad(),bounds.init.Quad());
	fprintf(file, "%s%s L%d_%d\n\n", _, jump, reg.cur_state, ret_idx);


	//moving the bounds
	int extend=((abs(move)+EXTEND_STEPS-1)/EXTEND_STEPS)*EXTEND_SIZE;
	int step=extend*move/abs(move);

	fprintf(file,"%sadd %s, %d;optimistic new bounds\n",_,bounds.init.Quad(),step);

	//second check (maybe easy case)
	int easy_case=++reg.cur_split;
	int joined_code=++reg.cur_split;
	fprintf(file,"%scmp %s, %s\n",_,reg.address.Quad(),bounds.limit.Quad());
	fprintf(file, "%s%s L%d_%d\n\n", _, jump, reg.cur_state, easy_case);

	//hard case: checking for exit
	fprintf(file,"%scmp %s, %s\n",_,reg.address.Quad(),bounds.limit.Quad());
	fprintf(file, "%s%s exit_out_of_tape\n", _, jump);

	
	//hard case body
	
	if(move<0){
		fprintf(file,"%smov rdi,%s\n",_,bounds.limit.Quad());

		fprintf(file,"%s;rcx=prev_bound-rdi\n",_);
		fprintf(file,"%slea rcx,[%s+%d]\n",_,bounds.init.Quad(),extend);
		fprintf(file,"%ssub rcx,rdi\n",_);
	}
	else{
		fprintf(file,"%slea rdi,[%s-%d+4]\n",_,bounds.init.Quad(),extend);

		fprintf(file,"%s;rcx=limit-prev_bound\n",_);
		fprintf(file,"%slea rcx,[rdi+%d-4]\n",_,extend);
		fprintf(file,"%ssub rcx,%s\n",_,bounds.limit.Quad());
	}

	fprintf(file,"%sshr rcx,2;we move in groups of 4\n",_);

	//fix init and bounce
	fprintf(file,"%smov %s,%s\n",_,bounds.init.Quad(),bounds.limit.Quad());
	fprintf(file, "%sjmp L%d_%d\n\n", _, reg.cur_state, joined_code);
	
	//easy case
	fprintf(file,"L%d_%d:;easy case no re-adjustment\n",reg.cur_state,easy_case);
	fprintf(file,"%smov rcx,%d\n",_,extend);

	if(move<0){
		fprintf(file,"%smov rdi,%s\n",_,bounds.init.Quad());
	}	
	else{
		fprintf(file,"%slea rdi,[%s-%d+4]\n",_,bounds.init.Quad(),step);
	}

	//joined case
	fprintf(file,"L%d_%d:;joined logic\n\n",reg.cur_state,joined_code);
	fprintf(file, "%sxor rax,rax\n", _);
	fprintf(file, "%srep stosd\n", _);
	

	//restore dirtied registetrs
	if(tmp_rdi.name!=RDI){
		fprintf(file,"%smov rdi, %s;restoring rdi\n",_,tmp_rdi.Quad());
	}	
	if(tmp_rcx.name!=RCX){
		fprintf(file,"%smov rcx, %s;restoring rcx\n",_,tmp_rcx.Quad());
	}
	if(tmp_rax.name!=RAX){
		fprintf(file,"%smov rax, %s;restoring rax\n",_,tmp_rax.Quad());
	}

	fprintf(file,"L%d_%d:;done bounds check\n\n",reg.cur_state,ret_idx);

	reg.tmp_back_to(prev_tmp_count);
	write_genral(file,reg,names,x->next.get());
}

void load_tape_from_stack(FILE *file,RegisterState reg){
	ASSERT(reg.contains(RSP));
	Register tmp=reg.add_tmp();
	Register base=reg.add_tmp();

	fprintf(file, "%smov %s, qword [rsp] ;cur\n", _, reg.address.Quad());

	fprintf(file, "%smov %s, qword [rsp+8] ;base\n", _, base.Quad());
	// right_limit
    fprintf(file, "%smovsxd %s, dword [rsp+20]\n", _,tmp.Quad()); 
    fprintf(file, "%slea %s, [%s + 4*%s] ;right limit\n", _, reg.right.limit.Quad(), base.Quad(),tmp.Quad());

    // left_limit
    fprintf(file, "%smovsxd %s, dword [rsp+16]\n", _,tmp.Quad());
    fprintf(file, "%slea %s, [%s + 4*%s] ;left limit\n", _, reg.left.limit.Quad(), base.Quad(),tmp.Quad());

    // right_init
    fprintf(file, "%smovsxd %s, dword [rsp+24]\n", _,tmp.Quad()); 
    fprintf(file, "%slea %s, [%s + 4*%s] ;left initilized\n", _, reg.left.init.Quad(), base.Quad(),tmp.Quad());

    // left_init
    fprintf(file, "%smovsxd %s, dword [rsp+28]\n", _,tmp.Quad()); 
    fprintf(file, "%slea %s, [%s + 4*%s] ;right initilized\n", _, reg.right.init.Quad(), base.Quad(),tmp.Quad());
}

void store_tape_to_stack(FILE *file,RegisterState reg){
	ASSERT(reg.contains(RSP));
	Register base=reg.add_tmp();

	fprintf(file,"%smov [rsp],qword %s;writing current adress\n",_,reg.address.Quad());

    // Load base address into tmp
    fprintf(file, "%smov %s, qword [rsp+8];loading base\n", _, base.Quad());

    //not handeling the sign right

    //right init
    fprintf(file,"%s;moving right_init to int index\n",_);
    fprintf(file,"%ssub %s,%s\n",_,reg.right.init.Quad(),base.Quad());
    fprintf(file, "%sshr %s, 2\n", _,reg.right.init.Quad());
    fprintf(file, "%smov [rsp+28], dword %s ;storing it\n", _, reg.right.init.Double());

    //left init
    fprintf(file,"%s;moving left_init to int index\n",_);
    fprintf(file,"%ssub %s,%s\n",_,reg.left.init.Quad(),base.Quad());
    fprintf(file, "%sshr %s, 2\n", _,reg.left.init.Quad());
    fprintf(file, "%smov [rsp+24], dword %s \n", _, reg.left.init.Double());
}

void Tree_IR_to_ASM(FILE *file,TreeIR ir,const char** names){
	RegisterState reg=RegisterState(
		Register(R14),
		Register(R15),
		(BoundsDir){Register(R11),Register(R9)},
		(BoundsDir){Register(R10),Register(R8)}
	);

	reg.add_tmp(RSP);//we need rsp for things
	
	fprintf(file,O0_assembly_start_template);

	load_tape_from_stack(file,reg);
	ASSERT(reg.tmp.size()==1);

	//TODO: add missing global functions

	for(auto i=0u;i<ir.size();i++){
		write_asm(file,reg,names,ir[i].get());
	}

	fprintf(file,"exit_good:\n");
	store_tape_to_stack(file,reg);

	fprintf(file,O0_assembly_end_template);
}