#ifndef PARSER_H
#define PARSER_H

#include "turing.h"
#include <stdlib.h>

static const char comment_char='#';

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
	int NextStateID;
} TransitionEncoding;

CodeLines tokenize_text(const char* raw_text);


void print_trans(TuringMachine machine,Transition trans, int indent);
void print_state(TuringMachine machine,State s, int indent);
void print_machine(TuringMachine machine, int indent);
TuringMachine parse_text(const char* raw_text);


#endif // PARSER_H
