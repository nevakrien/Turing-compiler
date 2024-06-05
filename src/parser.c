#include "parser.h"
#include "turing.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Function to read an entire file into memory
char *read_file_into_buffer(const char *filename, size_t *length) {
    FILE *file = fopen(filename, "rb");  // Open the file in binary mode
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Seek to the end of the file to determine the file size
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);  // Go back to the start of the file

    // Allocate memory for the file content
    char *buffer = null_check(malloc(*length));
    
    // Read the file into the buffer
    size_t read_size = fread(buffer, 1, *length, file);
    if (read_size < *length) {
        if (errno) {
            perror("File read error");
            free(buffer);
            fclose(file);
            return NULL;
        }
    }

    // Check if the file is already null-terminated
    if (buffer[read_size - 1] != '\0') {
        buffer = null_check(realloc(buffer, read_size + 1));
        buffer[read_size] = '\0'; // Manually null terminate if not already
    }

    fclose(file);  // Close the file
    *length = read_size; // Update length to actual bytes read
    return buffer;
}

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


static inline const char* skip_empty_lines(const char* cur,int* line_num){
	cur=skip_spaces(cur);
	while(*cur=='\n'){
		*line_num=*line_num+1;
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
	ans.lines=null_check(malloc(cap*sizeof(Line)));

	TokenNode** insert_spot=&ans.lines[0].head;

	int line_num=1;

	const char* head=raw_text;


	//while(1){
	while(*head!='\0'){
		head=skip_empty_lines(head,&line_num);
		ans.lines[ans.len].lineNum=line_num;

		const char* tok_end=get_token_end(head);
		Token tok={head,tok_end-head};
		
		//check for an earlier exit
		if(*tok_end=='\0'){
			if(tok.len!=0){
				//push token and exit
				*insert_spot=null_check(malloc(sizeof(TokenNode)));
				(*insert_spot) -> tok=tok;
				insert_spot=&((*insert_spot)->next);
				ans.len++;
			}
			break;
		}

		//push token
		*insert_spot=null_check(malloc(sizeof(TokenNode)));
		(*insert_spot) -> tok=tok;
		insert_spot=&((*insert_spot)->next);

		head=tok_end;
		head=skip_spaces(head);
		if(*head=='\n'||*head=='\0'){
			*insert_spot=NULL;

			//push line
			if(ans.len==cap){
				cap*=2;
				ans.lines=null_check(realloc(ans.lines,cap*sizeof(Line)));
			}
			ans.len++;
			insert_spot=&ans.lines[ans.len].head;

		}
	}

	if(ans.len==0){
		ans.lines=NULL;
	}
	else{
		ans.lines=null_check(realloc(ans.lines,ans.len*sizeof(Line)));
	}
	
	return ans;
}

//UNTESTED

void remove_comments(CodeLines* codes){
	
	for(int i=0;i<codes->len;i++){
		if(codes->lines[i].head==NULL){
			continue;
		}
		TokenNode** chain=&(codes->lines[i].head);


		int run=1;
		while(*chain!=NULL&&run){
			Token tok=(*chain)->tok;
			for(int j=0;j<tok.len;j++){
				if(tok.data[j]==comment_char){
					tok.len=j;
					if(tok.len!=0){
						free_chain((*chain)->next);
						(*chain)->next=NULL;
					}
					else{
						free(*chain);
						*chain=NULL;
					}

					run=0;
					goto end_while;
				}
			}
			if((*chain)->next==NULL){
				break;
			}
			chain=&((*chain)->next);
		}
		end_while:
	}
}

//splits tokens on a sep_char = ':'. 
//also makes sure there is 1 and ONLY 1 split
//fails when the left part is completly empty
static TokenNode* split_2(TokenNode* line, ParseError* error){
	if(line==NULL){
		UNREACHABLE();
	}
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
		static char* error1="expected a \" : \" to show seperation";
		error->data=error1;
		error->code=PARSE_PERROR;
		return NULL;
	}

	line=line->next;
	while(line){
		Token tok=line->tok;
		for(int i=0;i<tok.len;i++){
			if(tok.data[i]==sep_char){
				static char* error1="this line contains more than one \" : \" seperator";
				error->data=error1;
				error->code=PARSE_PERROR;
				return NULL;
			}
		}

		line=line->next;
	}

	return last_left_token;
}

TransitionEncoding parse_trans(Line* line,ParseError* error){
	if(line==NULL ||line->head==NULL){
		UNREACHABLE();
	}
	TokenNode*  end_left=split_2(line->head,error);
	
	if(error->code){
		if(error->code==PARSE_PERROR){
			printf("ERROR at line[%d]: %s\n",line->lineNum,(char*)(error->data));
		}
		else{
			printf("UNKNOWEN ERROR at line[%d]\n",line->lineNum);
		}
		error->code=PARSE_ERROR_PRINTED;
		return (TransitionEncoding){0}; 
	}

	if(line->head->next!=end_left){
		printf("ERROR at line[%d]: expected exacly 2 tokens in the left half\n",line->lineNum);
		error->code=PARSE_ERROR_PRINTED;
		return (TransitionEncoding){0};
	}

	TokenNode* right_start=end_left->next;
	
	TokenNode* cur=right_start;
	for(int i=1;i<3;i++){
		if(cur->next==NULL){
			printf("ERROR at line[%d]: not enough tokens at the right half expected exacly 3\n",line->lineNum);
			error->code=PARSE_ERROR_PRINTED;
			return (TransitionEncoding){0};
		}
		cur=cur->next;
	}

	if(cur->next!=NULL){
		printf("ERROR at line[%d]: too many tokens at the right half expected exacly 3\n",line->lineNum);
		error->code=PARSE_ERROR_PRINTED;
		return (TransitionEncoding){0};
	}

	//TokenNode* end_right=cur;

	//PLACE HOLDER
	return (TransitionEncoding){0};
}


