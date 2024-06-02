#ifndef TURING_H
#define TURING_H

#include <stdio.h>

typedef enum TuringDone{	
	HAULT,
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

typedef struct TRANSITION TRANSITION;
typedef struct TRANSITION{
	Bit write;
	Dir move;
	TRANSITION* NextTRANSITION;
}TRANSITION;

typedef  TRANSITION State[2];

typedef struct TuringMachine{
	int size;
	State states[];
}TuringMachine;

typedef struct Tape{
	State* cur;
	State* base;//base-min=the actual base pointer to free
	int max;
	int min;
	int left_init;
	int right_init;
} Tape;

typedef struct TuringResult{
	TuringDone code;
	int steps;
	Tape* tape;
}TuringResult;



TuringResult run_turing(Tape* tape,int max_steps);

#endif //TURING_H