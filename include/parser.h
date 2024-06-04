#ifndef PARSER_H
#define PARSER_H

#include "turing.h"
#include <stdlib.h>

//syntax specific definitions
static const char comment_char='#';
static const char sep_char=':';

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

typedef struct{
	int len;
	TokenNode** lines;//array of pointers
}CodeLines;

typedef struct{
	Bit write;
	Dir move;
	Token NextStateTok;
} TransitionEncoding;

typedef struct{
	TransitionEncoding state[2];
}StateEncoding;

CodeLines tokenize_text(const char* raw_text);
TransitionEncoding parse_trans(TokenNode* line,const char**error);

void print_trans(TuringMachine machine,Transition trans, int indent);
void print_state(TuringMachine machine,State s, int indent);
void print_machine(TuringMachine machine, int indent);
TuringMachine parse_text(const char* raw_text);


#endif // PARSER_H
