#ifndef TURING_H
#define TURING_H

#include <stdio.h>

typedef enum TuringDone{	
	HAULT=0,
	TIME_OUT,
	OUT_OF_TAPE,
}TuringDone;

typedef enum  Bit{
	Bit_0=0,
	Bit_1=1,
} Bit;

typedef enum  Dir{
	Left=-1,
	Stay=0,
	Right=1,
} Dir;

//typedef struct TRANSITION TRANSITION;
typedef struct State State;

typedef struct TRANSITION{
	Bit write;
	Dir move;
	State* NextState;
}TRANSITION;

typedef struct State {
    TRANSITION transitions[2];
} State;

typedef struct TuringMachine{
	int size;
	State* states;
}TuringMachine;

typedef struct Tape{
	Bit* cur;
	Bit* base;//base[min] =the actual base pointer to free
	
	//all indecies here are allways inclusive  base[right_limit] should not segfault
	int left_limit;
	int right_limit;

	int left_init;
	int right_init;
} Tape;

typedef struct TuringResult{
	TuringDone code;
	int steps;
	int state_id;
	//Tape* tape;
}TuringResult;



TuringResult run_turing(Tape* tape,const TuringMachine machine,int max_steps);

#endif //TURING_H