#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>

//user optional definitions
#define CHECK_UNREACHABLE //puts an assert instead of ub 

// #define MUTATEBLE 

#ifndef __cplusplus
#include <assert.h>
#else
#include <cassert>
#endif //__cplusplus

#if defined(CHECK_UNREACHABLE)

#define UNREACHABLE() assert(0 && "Unreachable code reached")

#define ASSERT(x) assert(x) 

#elif defined(__GNUC__) || defined(__clang__)
#define UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)

#define UNREACHABLE() __assume(0)
#define ASSERT(x) __assume(x)

#else
//null pointer dereference to signal unreachability
#define UNREACHABLE() (*(int*)0 = 0)
#endif

#ifndef ASSERT
#include <stdbool.h>
static inline void ASSERT(bool x){
    if(!x){UNREACHABLE();}
}
// #define ASSERT(x) if(!x){UNREACHABLE();}
#endif

#define TODO assert(0 && "TODO");

static inline void* null_check(void* p){
	if(p==NULL){
		perror("went oom\n");
		exit(1);
	}
	return p;
}

#ifndef __cplusplus
// Used the same way as atoi but does error checking.
static inline int int_of_str(const char* str)
{
    errno = 0;
    char* end;
    long r = strtol(str, &end, 10);
    if (errno == ERANGE || end == str || *end || r < INT_MIN || r > INT_MAX) {
        fprintf(stderr, "\"%s\" is not an integer\n",str);
        exit(1);
    }
    return r;
}

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#else
#define UNIQUE_POINTER_CAST(DerivedType, basePtr) \
    std::unique_ptr<DerivedType>(static_cast<DerivedType*>(basePtr.release()))
#endif //__cplusplus




#endif// UTILS_H