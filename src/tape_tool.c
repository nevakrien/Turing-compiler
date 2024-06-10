#include "cli.h"
#include "io.h"
#include "turing.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// only for dump use
Tape unsafe_init_tape(int left_limit, int right_limit, int left_init, int right_init)
{
    if (left_limit > left_init || right_limit < right_init || right_init < 0 || left_init > 0) {
        printf("invalid bounds try again\n");
        exit(1);
    }

    Tape tape = (Tape) { 0 };

    Bit* data = null_check(calloc(1 + right_init - left_init, sizeof(Bit)));
    tape.base = data - left_init;
    tape.cur = tape.base;

    tape.left_limit = left_limit;
    tape.right_limit = right_limit;
    tape.left_init = left_init;
    tape.right_init = right_init;
    return tape;
}

void change_bounds(const char* filename, int left, int right)
{
    Tape tape = ReadTape(filename);
    int cur_index = tape.cur - tape.base;

    if (cur_index > right || cur_index < left) {
        printf("invalid bounds\n");
        exit(1);
    }
    if (left > tape.left_init) {
        tape.left_init = left;
    }
    if (right < tape.right_init) {
        tape.right_init = right;
    }

    DumpTape(&tape, filename);
}

void print_help()
{
    printf("use show file.tape to see a tape\nuse new filename left_limit right_limit left_init rigt_init to make a new tape\n");
    printf("use change file.tape new_left_limit new_right_limit to change bounds\n");
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        print_help();
        return 1;
    }

    if (strcmp(argv[1], "show") == 0) {
        if (argc != 3) {
            printf("incorect usage %s show filename\n", argv[0]);
            return 1;
        }

        PrintTape(ReadTape(argv[2]));
        return 0;
    }

    if (strcmp(argv[1], "new") == 0) {
        if (argc != 7) {
            printf("incorect usage %s new filename left_limit right_limit left_init rigt_init\n", argv[0]);
            return 1;
        }
        Tape tape = unsafe_init_tape(atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
        DumpTape(&tape, argv[2]);
        return 0;
    }

    if (strcmp(argv[1], "change") == 0) {
        if (argc != 5) {
            printf("incorect usage %s change filename left right\n", argv[0]);
        }
        change_bounds(argv[2], atoi(argv[3]), atoi(argv[4]));
        return 0;
    }

    print_help();
    return 1;
}
