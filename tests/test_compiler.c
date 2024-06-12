#include "compiler.h"
#include <libgen.h>
#include <stdio.h>

void code_func(FILE* f,void* data){
    fprintf(f,"%s","    ; Inserted custom code\n");
}


int main(int argc,char* argv[]){
    int code= assemble_and_link("generated",dirname(argv[0]),&code_func,NULL);
    system("rm generated.*");
    //system("rm a.out");

    printf("done test exit code %d\n", code);
    return code;
}