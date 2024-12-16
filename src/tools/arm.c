#include "cli.h"
#include "IR.h"
#include "utils.h"
#include "compiler.h"
#include <libgen.h>

// const char *casm = "gcc -g -c -o %s.o %s";
// const char *cld = "gcc -o %s.out %s.o %s/io.o";
#if defined(__arm__)
    const char *casm = "gcc -g -c -o %s.o %s";
    const char *cld = "gcc -o %s.out %s.o %s/io.o";
#else
    const char *casm = "arm-linux-gnueabihf-gcc -march=armv7-a -c -o %s.o %s -static";
    const char *cld = "arm-linux-gnueabihf-gcc -march=armv7-a -o %s.out %s.o %s/arm_io.o -static";
#endif

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
    assemble_and_link(argv[2], dirname(argv[0]), ARM_code, casm, cld, ".s", &comp.ir);
    return EXIT_SUCCESS;
}
