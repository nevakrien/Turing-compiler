#ifndef PARSER_H
#define PARSER_H

#include "turing.h"
#include <stdlib.h>

//syntax specific definitions
static const char comment_char='#';
static const char sep_char=':'; //if this is changed split_2 would print wrong messages

#define COMP_SEPS(cur, comp, ag) \
    ( ((cur) comp '\t') ag \
      ((cur) comp ' ') ag \
      ((cur) comp '(') ag \
      ((cur) comp ')') ag \
      ((cur) comp '[') ag \
      ((cur) comp ']') ag \
      ((cur) comp '{') ag \
      ((cur) comp '}') ag \
      ((cur) comp '\"') ag \
      ((cur) comp '\'') ag \
      ((cur) comp ';') ag \
      ((cur) comp ',') )

// const char* right_chars={'r','R'};
// const char* left_chars={'l','L'};
// const char* stay_chars={'s','S'};

//0 and 1 are trivialy read

typedef struct{
	const char* data;
	int len;
} Token;

typedef struct TokenNode TokenNode;
struct TokenNode{
	Token tok;
	TokenNode* next;
};

static inline void free_chain(TokenNode* list){
	while(list!=NULL){
		TokenNode* tmp=list->next;
		free(list);
		list=tmp;
	}
}

//TODO need to add this
typedef struct{
	TokenNode* head;
	int lineNum;//1 indexed
}Line;

typedef struct{
	int len;
	Line* lines;//array of pointers
}CodeLines;

static inline void print_code(CodeLines code){
	for(int i=0;i<code.len;i++){
        TokenNode* chain=code.lines[i].head;
        while(chain!=NULL){
            Token tok=chain->tok;
            printf(" ");
            for(int j=0;j<tok.len;j++){
                printf("%c",tok.data[j]);
            }
            printf(" ");
            chain=chain->next;
        }
        printf("\n");
    } 
}

typedef enum PARSE_ERROR{
	PARSE_WORKED=0,
	PARSE_PERROR=1,
	PARSE_ERROR_PRINTED=2,
}PARSE_ERROR;


typedef struct{
	void * data; //probably just a char* message but we can get creative
	PARSE_ERROR code;
}ParseError;

typedef struct{
	Bit write;
	Dir move;
	Token NextStateTok;
} TransitionEncoding;

typedef struct{
	TransitionEncoding state[2];
}StateEncoding;

char *read_file_into_buffer(const char *filename, size_t *length);

void print_trans(TuringMachine machine,Transition trans, int indent);
void print_state(TuringMachine machine,State s, int indent);
void print_machine(TuringMachine machine, int indent);

CodeLines tokenize_text(const char* raw_text);
void remove_comments(CodeLines* codes);
TransitionEncoding parse_trans(Line* line,ParseError* error);


TuringMachine parse_text(const char* raw_text,ParseError* error);


#endif // PARSER_H
