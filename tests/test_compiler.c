#include "compiler.h"

int main(){
    int code= assemble_and_link("generated","    ; Inserted custom code");
    system("rm generated.*");
    system("rm a.out");

    printf("done test exit code %d\n", code);
    return code;
}