#include "parser.h"
#include "turing.h"
#include "utils.h"

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

// static void remove_comments(CodeLines* codes){
// 	for(int i=0;i<codes->len;i++){
// 		TokenNode* chain=codes->lines[i];
// 		while(chain!=NULL){
// 			Token tok=chain->tok;
// 			for(int j=0;j<tok.len;j++){
// 				if(tok.data[j]==comment_char){
// 					tok.len=j;
// 					free_chain(chain->next);
// 					chain->next=NULL;
// 				}
// 			}

// 			chain=chain->next; //would stop the loop if we freed 
// 		}
// 	}
// }



// TuringMachine parse_text(const char* raw_text){
// 	CodeLines lines=tokenize_text(raw_text);
// 	remove_comments(&lines);
// }