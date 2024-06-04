#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>

//user optional definitions
#define CHECK_UNREACHABLE //puts an assert instead of ub 

static inline void* null_check(void* p){
	if(p==NULL){
		perror("went oom\n");
		exit(1);
	}
	return p;
}

#define max(a, b) ((a) > (b) ? (a) : (b))

#if defined(CHECK_UNREACHABLE)
#include <assert.h>
#define UNREACHABLE() assert(0 && "Unreachable code reached")
#elif defined(__GNUC__) || defined(__clang__)
#define UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define UNREACHABLE() __assume(0)
#else
//null pointer dereference to signal unreachability
#define UNREACHABLE() (*(int*)0 = 0)
#endif


#endif// UTILS_H