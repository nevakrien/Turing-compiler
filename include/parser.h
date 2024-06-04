#ifndef PARSER_H
#define PARSER_H

#include "turing.h"

typedef struct{
	const char* data;
	int len;
} Token;

typedef struct TokenNode TokenNode;
struct TokenNode{
	Token tok;
	TokenNode* next;
};

typedef struct{
	int len;
	TokenNode** lines;//array of pointers
}CodeLines;
CodeLines nevas_tokenize_text(const char* raw_text);

void print_trans(TuringMachine machine,TRANSITION trans, int indent);
void print_state(TuringMachine machine,State s, int indent);
void print_machine(TuringMachine machine, int indent);
TuringMachine parse_text(const char* raw_text);


#endif // PARSER_H
