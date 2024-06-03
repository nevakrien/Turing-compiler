#ifndef PARSER_H
#define PARSER_H

#include "turing.h"

void print_trans(TuringMachine machine,TRANSITION trans, int indent);
void print_state(TuringMachine machine,State s, int indent);
void print_machine(TuringMachine machine, int indent);
TuringMachine parse_text(char* raw_text);

#endif // PARSER_H
