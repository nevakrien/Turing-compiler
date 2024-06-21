#ifndef IR_H
#define IR_H

#include "parser.h"
#include <stdint.h>



//
typedef struct{
	Bit write;
	Dir move;
	int nextState; //-1 is halt -2 is not intilized
} TransIR;

typedef struct{
	//int stateId;
	TransIR trans[2];

} StateIR;

typedef struct{
	StateIR* states;
	int len;
	const char** names;
}TuringIR;

static inline void free_TuringIR(TuringIR x){
	free(x.states);
	free(x.names);
}

static inline int eq_TransIR(TransIR a,TransIR b){
	if(a.write!=b.write){
		return 0;
	}
	if(a.move!=b.move){
		return 0;
	}
	if(a.nextState!=b.nextState){
		return 0;
	}
	return 1;
}


TuringIR make_initial_ir (TuringMachineEncoding encode);
static inline TuringIR prone_ir (TuringIR ir,int warn){
	int* new_ids=null_check(malloc(sizeof(int)*ir.len));
	for(int i=0;i<ir.len;i++){
		new_ids[i]=-2;
	}

	TuringIR ans;
	ans.len=1;
	ans.states=null_check(malloc(sizeof(StateIR)*ir.len));
	ans.names=null_check(malloc(sizeof(char*)*ir.len));

	int cur_handle=0;
	new_ids[0]=0;
	ans.states[0]=ir.states[0];
	ans.names[0]=ir.names[0];

	//populate based on traveling the tree
	while(cur_handle<ans.len)
	{
		TransIR* trans=ans.states[cur_handle].trans;
		for(int k=0;k<2;k++){
			int next=trans[k].nextState;
			if(next!=-1){
				//maybe insert
				if(new_ids[next]<0){
					new_ids[next]=ans.len;

					//insert to ans
					ans.states[ans.len]=ir.states[next];
					ans.names[ans.len]=ir.names[next];
					
					ans.len++;
				}
			}
		}
		cur_handle++;
	}

	if(warn&&ans.len!=ir.len){
		for(int i=0;i<ir.len;i++){
			if(new_ids[i]<0){
				printf("warning state \"%s\" is unsused\n",ir.names[i] );
			}
		}
	}

	//update to the new ids
	for(int i=0;i<ans.len;i++){
		int* idptr;
		
	//	idptr=&ans.states[i].stateId;
	//	*idptr=new_ids[*idptr];
		
		for(int k=0;k<2;k++){
			idptr=&ans.states[i].trans[k].nextState;
			if(*idptr!=-1){
				*idptr=new_ids[*idptr];
			}
	
		}
	}
	free(new_ids);

	ans.states=null_check(realloc(ans.states,sizeof(StateIR)*ans.len));
	ans.names=null_check(realloc(ans.names,sizeof(char*)*ans.len));
	return ans;
}




#endif // IR_H
