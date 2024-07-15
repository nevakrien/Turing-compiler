#include "O2.hpp"
#include "asm.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>




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