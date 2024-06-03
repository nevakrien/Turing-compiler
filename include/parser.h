#ifndef PARSER_H
#define PARSER_H

#include "turing.h"

void print_machine(TuringMachine machine);
TuringMachine parse_text(char* raw_text);

#endif // PARSER_H
