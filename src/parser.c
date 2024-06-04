#include "parser.h"
#include "turing.h"
#include "utils.h"

#include <errno.h>
#include <errno.h>

// Function to print the details of a Transition with specified indentation
void print_trans(TuringMachine machine,Transition trans, int indent) {
    const char *dir_str;
    switch (trans.move) {
        case Right:
            dir_str = "R";
            break;
        case Left:
            dir_str = "L";
            break;
        case Stay:
            dir_str = "S";
            break;
        default:
            dir_str = "?";
    }

    for (int i = 0; i < indent; i++) {
        printf(" ");
    }

    int next_state_id;
    if(trans.NextState==NULL){next_state_id=-1;}
    else{next_state_id=trans.NextState -machine.states;}

    printf("Write: %d, Move: %s, Next State: %d\n", trans.write, dir_str, next_state_id);
}

// Function to print the states and their transitions with specified indentation
void print_state(TuringMachine machine,State s, int indent) {
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }
    printf("State:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < indent; j++) {
            printf(" ");
        }
        printf("  Transition %d:\n", i + 1);
        print_trans(machine,s.transitions[i], indent + 4);
    }
}

// Function to print the entire Turing machine's states and their details with specified indentation
void print_machine(TuringMachine machine, int indent) {
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }
    printf("Turing Machine Size: %d\n", machine.size);
    for (int i = 0; i < machine.size; i++) {
        print_state(machine,machine.states[i], indent + 2);
    }
}


static inline const char* get_token_end(const char* cur){
	while(*cur!='\0'&&*cur!='\t'&&*cur!=' '&&*cur!='\n'){
		cur++;
	}

	return cur;
}
static inline const char* skip_spaces(const char* cur){
	while(*cur=='\t'||*cur==' '){
		cur++;
	}

	return cur;
}

static inline const char* skip_empty_lines(const char* cur){
	cur=skip_spaces(cur);
	while(*cur=='\n'){
		cur++;
		cur=skip_spaces(cur);
	}
	return cur;
}

//note that some information is lost BUT its recovrble since we are using refrences and no copying
CodeLines tokenize_text(const char* raw_text){
	CodeLines ans;

	int cap=128;//vector style
	ans.len=0;
	ans.lines=null_check(malloc(cap*sizeof(TokenNode*)));

	TokenNode** insert_spot=&ans.lines[0];


	const char* head=skip_empty_lines(raw_text);
	if(*head=='\0'){
		free(ans.lines);
		ans.lines=NULL;
		return ans;
	}

	while(*head!='\0'){
		const char* tok_end=get_token_end(head);
		Token tok={head,tok_end-head};
		
		//push token
		*insert_spot=null_check(malloc(sizeof(TokenNode)));
		(*insert_spot) -> tok=tok;
		insert_spot=&((*insert_spot)->next);

		head=tok_end;
		head=skip_spaces(head);
		if(*head=='\n'){
			*insert_spot=NULL;
			//push line
			if(ans.len==cap){
				cap*=2;
				ans.lines=null_check(realloc(ans.lines,cap*sizeof(TokenNode*)));
			}
			ans.len++;
			insert_spot=&ans.lines[ans.len];
			
			head=skip_empty_lines(head);
		}
	}
	ans.len++;
	ans.lines=null_check(realloc(ans.lines,ans.len*sizeof(TokenNode*)));
	return ans;
}

static void remove_comments(CodeLines* codes){
	for(int i=0;i<codes->len;i++){
		TokenNode* chain=codes->lines[i];
		while(chain!=NULL){
			Token tok=chain->tok;
			for(int j=0;j<tok.len;j++){
				if(tok.data[j]==comment_char){
					tok.len=j;
					free_chain(chain->next);
					chain->next=NULL;
				}
			}

			chain=chain->next; //would stop the loop if we freed 
		}
	}
}

//errors are -2 and below
static inline int parse_id(char* str,int len){
	if(str[0]=='-'){
		if(len!=2){
			return -2;
		}
		if(str[1]=='1'){
			return -1;
		}
		return -2;
	}

	int ans=0;
	int mul=1;
	for(int i=0;i<len;i++){
		if (str[i] >= '0' && str[i] <= '9') {
	        ans+=mul*(str[i] - '0');
	    } else {
	        return -2; 
	    }
	    mul*=10;
	}

	return ans;
}

#define raise_error( text) printf(text); exit(1);
//#define raise_error( text) error*=(text); return {0};

//old parser version
// static TransitionEncoding pop_next_state(TokenNode* line,char** error){
// 	TransitionEncoding ans;

// 	int found_write=0;
// 	int found_read=0;
// 	int found_dir=0;
// 	int found_state=0;

// 	while(line){
// 		Token tok=line->tok;
// 		if(tok.data[0]=='S'){
// 			if(found_state==1){
// 				raise_error("double definition of next state\n");
// 			}
// 			//read the index and use it
// 			found_state=1;

// 			ans.NextStateID=parse_id(tok.data+1,tok.len-1);
// 			if(found_state<-1){
// 				raise_error("invalid state id\n");
// 			}
// 			continue;
// 		}
// 		switch(tok.len){

// 		case 4://read next move
// 			if(memcmp("read",tok.data,4)==0){
// 				if(found_read){
// 					raise_error("double definition of write\n");
// 				}
// 				found_read=1;
// 				break;
// 			}
// 			if(memcmp("next",tok.data,4)==0){
// 				if(found_read){
// 					raise_error("double definition of write\n");
// 				}
// 				found_read=1;
// 				break;
// 			}
// 			if(memcmp("move",tok.data,4)==0){
// 				break;
// 			}
// 			break;
// 		case 5://write
// 			if(memcmp("write",tok.data,5)==0){
// 				if(found_write){
// 					raise_error("double definition of write\n");
// 				}
// 				found_write=1;
// 			}
// 			break;

// 		default:
// 			//printf();
// 			break;
// 		}
// 	}
// }

// static StateEncoding pop_next_state(CodeLines* lines){
// 	StateEncoding ans;


// }

// TuringMachine parse_text(const char* raw_text){
// 	CodeLines lines=tokenize_text(raw_text);
// 	remove_comments(&lines);
// 	TuringMachine ans;
// }