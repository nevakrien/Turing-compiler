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
				*insert_spot=NULL;
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


typedef struct 
{
	TokenNode** tok;
	int i;
}CharLoc;

static inline CharLoc find_sep(TokenNode** line,int start){
	while(*line){
		Token tok=(*line)->tok;
		for(int i=start;i<tok.len;i++){
			if(tok.data[i]==sep_char){
				return(CharLoc){line,i};
			}
		}
		start=0;
		line=&((*line)->next);
	}
	return (CharLoc){NULL,-1};
}

TransitionEncoding parse_trans(Line* line,ParseError* error){
	if(line->head==NULL){
		UNREACHABLE();
	}

	CharLoc split=find_sep(&(line->head),0);
	if(split.tok==NULL){
		printf("ERROR at line[%d]: no \" : \" seperator\n",line->lineNum);
		error->code=PARSE_ERROR_PRINTED;
		return (TransitionEncoding){0};
	}

	CharLoc e=find_sep(split.tok,split.i+1);
	if(e.tok!=NULL){
		printf("ERROR at line[%d]: more than one \" : \" seperator\n",line->lineNum);
		error->code=PARSE_ERROR_PRINTED;
		return (TransitionEncoding){0};
	}

	if(split.tok==&(line->head)){
	err_too_few_left:
		printf("ERROR at line[%d]: less than 2 items on left half\n",line->lineNum);
		error->code=PARSE_ERROR_PRINTED;
		return (TransitionEncoding){0};
	}

	TokenNode* left[2];
	TokenNode* right[3];

	//split on the seperator

	int i=split.i;
	Token t=(*split.tok)->tok;
	int right_tok_len=t.len-i-1;

	//extend to the right.
	if(right_tok_len>0){
		Token right_tok=(Token){t.data+i+1,t.len-i-1};
		TokenNode* right_part=null_check(malloc(sizeof(TokenNode)));
		right_part->tok=right_tok;
		right_part->next=(*split.tok)->next;

		(*split.tok)->next=right_part;
	}

	//split the list
	right[0]=(*split.tok)->next;
	(*split.tok)->next=NULL;
	
	
	//cut empty node
	(*split.tok)->tok.len=i-1;
	if((*split.tok)->tok.len<1){
		free(*split.tok);
		*split.tok=NULL;
	}

	//len checks
	

	left[0]=line->head;
	if(left[0]==NULL){
		goto err_too_few_left;
	}

	left[1]=line->head->next;
	if(left[1]==NULL){
		goto err_too_few_left;
	}

	if(left[1]->next!=NULL){
		printf("ERROR at line[%d]: more than 2 items on left half\n",line->lineNum);
		error->code=PARSE_ERROR_PRINTED;
		return (TransitionEncoding){0};
	}

	//right len checks
	if(right[0]->next==NULL){
	err_too_few_right:
		printf("ERROR at line[%d]: less than 3 items on right half\n",line->lineNum);
		error->code=PARSE_ERROR_PRINTED;
		return (TransitionEncoding){0};

	}

	right[1]=right[0]->next;

	if(right[1]->next==NULL){
		goto err_too_few_right;
	}

	right[2]=right[1]->next;
	if(right[2]->next!=NULL){
		printf("ERROR at line[%d]: more than 3 items on right half\n",line->lineNum);
		error->code=PARSE_ERROR_PRINTED;
		return (TransitionEncoding){0};
	}

	printf("YAY at line[%d]\n",line->lineNum);
	//place holder
	return (TransitionEncoding){0};
}