#include "turing.h"
#include "io.h"
#include "cli.h"
#include "utils.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//only for dump use
Tape unsafe_init_tape(int left_limit,int right_limit,int left_init,int right_init){
	if(left_limit>left_init || right_limit<right_init || right_init<0||left_init>0){
		printf("invalid bounds try again\n");
		exit(1);
	}

	Tape tape=(Tape){0};
	
	Bit* data=null_check(calloc(1+right_init-left_init,sizeof(Bit)));
	tape.base=data-left_init;
	tape.cur=tape.base;
	
	tape.left_limit=left_limit;
	tape.right_limit=right_limit;
	tape.left_init=left_init;
	tape.right_init=right_init;
	return tape;
}

void change_bounds(const char* filename,int left,int right){
	Tape tape=ReadTape(filename);
	int cur_index=tape.cur-tape.base;

	if(cur_index>right||cur_index<left){
		printf("invalid bounds\n");
		exit(1);
	}

	tape.left_limit=left;
	tape.right_limit=right;

	if(left>tape.left_init){
		tape.left_init=left;
	}
	if(right<tape.right_init){
		tape.right_init=right;
	}

	DumpTape(&tape,filename);

}

static inline void set_tape(const char* filename,int left,int right,Bit val){
	Tape tape=ReadTape(filename);

	if(tape.right_limit<right||tape.left_limit>left){
		printf("invalid bounds\n");
		exit(1);
	}

	if(left<tape.left_init){
		tape.left_init=left;
	}
	if(right>tape.right_init){
		tape.right_init=right;
	}

	for(int i=left;i<=right;i++){
		tape.base[i]=val;
	}
	DumpTape(&tape,filename);

}


void print_help(){
	printf("use show file.tape to see a tape\nuse new filename left_limit right_limit left_init rigt_init to make a new tape\n");
	printf("use change file.tape new_left_limit new_right_limit to change bounds\n");
}


int main(int argc, char* argv[]){
	if(argc<2){
		print_help();
		return 1;
	}

	if(strcmp(argv[1],"show")==0){
		if(argc!=3){
			printf("incorect usage %s show filename\n",argv[0]);
			return 1;
		}
		
		PrintTape(ReadTape(argv[2]));
		return 0;
	}

	if(strcmp(argv[1],"cmp")==0){
		if(argc!=4){
			printf("incorect usage %s cmp A B\n",argv[0]);
			return 1;
		}
		
		int eq=tapes_are_equal(ReadTape(argv[2]),ReadTape(argv[3]));
		if(eq){
			printf("yes\n");
		}
		else{
			printf("no\n");
		}
		return 0;
	}

	if(strcmp(argv[1],"new")==0){
		if(argc!=7){
			printf("incorect usage %s new filename left_limit right_limit left_init rigt_init\n",argv[0]);
			return 1;
		}
		Tape tape=unsafe_init_tape(int_of_str(argv[3]),int_of_str(argv[4]),int_of_str(argv[5]),int_of_str(argv[6]));
		DumpTape(&tape,argv[2] );
		return 0;

	}
	
	if(strcmp(argv[1],"change")==0){
		if(argc!=5){
			printf("incorect usage %s change filename left right\n",argv[0]);
		}
		change_bounds(argv[2],int_of_str(argv[3]),int_of_str(argv[4]));
		return 0;
	}

	if(strcmp(argv[1],"set1")==0){
		if(argc!=5){
			printf("incorect usage %s set1 filename left right\n",argv[0]);
		}
		set_tape(argv[2],int_of_str(argv[3]),int_of_str(argv[4]),1);
		return 0;
	}

	if(strcmp(argv[1],"set0")==0){
		if(argc!=5){
			printf("incorect usage %s set0 filename left right\n",argv[0]);
		}
		set_tape(argv[2],int_of_str(argv[3]),int_of_str(argv[4]),0);
		return 0;
	}

	print_help();
	return 1;
}
