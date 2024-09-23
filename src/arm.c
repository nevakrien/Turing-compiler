#include "cli.h"
#include "IR.h"
#include "utils.h"
#include "compiler.h"
#include <libgen.h>

const char* casm="gcc -g -c -o %s.o %s";

void ARM_code(FILE *file, void *data) {
    TuringIR *ir = (TuringIR *) data;
    ARM_IR_to_ASM(file, *ir);
}

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("usage: %s src_file out_file\n",argv[0]);
        return EXIT_FAILURE;
    }
    CompileStepOne comp = first_compile_step(argv[1]);
    assemble_and_link(argv[2], dirname(argv[0]), ARM_code, casm, ".s", &comp.ir);
    return EXIT_SUCCESS;
}
