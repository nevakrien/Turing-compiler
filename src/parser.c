#include "parser.h"
#include "turing.h"
#include "utils.h"

//#include <string.h>

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
	while(*cur!='\0'&&*cur!='\n'&&COMP_SEPS(*cur,!=,&&)){
		cur++;
	}

	return cur;
}
static inline const char* skip_spaces(const char* cur){
	while(COMP_SEPS(*cur,==,||)){
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
		
		//check for an earlier exit
		if(tok.data[tok.len-1]=='\0'){
			tok.len--;//no need fot the null terminator
			//push token and exit
			*insert_spot=null_check(malloc(sizeof(TokenNode)));
			(*insert_spot) -> tok=tok;
			insert_spot=&((*insert_spot)->next);
			break;
		}

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

//UNTESTED

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

//splits tokens on a specific char. 
//also makes sure there is 1 and ONLY 1 split
//fails when the left part is completly empty
static TokenNode* separate_char(TokenNode* line,const char**error){
	TokenNode* last_left_token;

	while(line){
		Token tok=line->tok;
		for(int i=0;i<tok.len;i++){
			if(tok.data[i]==sep_char){
				//
				int split_start=i+1;
				int split_len=tok.len-split_start;

				if(split_len>0){
					TokenNode* second_part=null_check(malloc(sizeof(TokenNode)));
					second_part->next=line->next;
					second_part->tok=(Token){&tok.data[split_start],split_len};

					line->next=second_part;
					line->tok.len=i;
					last_left_token=line;
					
					line=second_part;
					break;
				}

				line->tok.len=i;
				last_left_token=line;
				break;
			}
		}

		line=line->next;
	}

	if(line==NULL){
		static const char* error1="expected a \" : \" to show seperation";
		*error=error1;
		return NULL;
	}

	line=line->next;
	while(line){
		Token tok=line->tok;
		for(int i=0;i<tok.len;i++){
			if(tok.data[i]==sep_char){
				static const char* error1="this line contains more than one \" : \" seperator";
				*error=error1;
				return NULL;
			}
		}

		line=line->next;
	}

	return last_left_token;
}

TransitionEncoding parse_trans(TokenNode* line,const char**error){
	TokenNode*  end_left=separate_char(line,error);
	if(*error!=NULL){
		return (TransitionEncoding){0};
	}

	if(line->next!=end_left){
		static const char* error1="expected exacly 2 tokens in the left half";
		*error=error1;
		return (TransitionEncoding){0};
	}

	TokenNode* right_start=end_left->next;
	
	static const char* error2="expected exacly 3 tokens in the right half";
	TokenNode* cur=right_start;
	for(int i=1;i<3;i++){
		if(cur->next==NULL){
			*error=error2;
			return (TransitionEncoding){0};
		}
		cur=cur->next;
	}

	if(cur->next!=NULL){
		*error=error2;
		return (TransitionEncoding){0};
	}

	TokenNode* end_right=cur;

	//PLACE HOLDER
	return (TransitionEncoding){0};
}


