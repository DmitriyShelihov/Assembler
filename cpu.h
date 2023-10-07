#ifndef _SPU_H_
#define _SPU_H_

#include "stack_ctor_dtor.h"
#include "stack_error.h"
#include "stack_push_pop.h"
#include "stack.h"
#include <stdio.h>

enum commands {
    command_push = 1,
    command_div = 2,
    command_sub = 3,
    command_out = 4,
    command_add = 5,
    command_mul = 6,
    command_HLT = 7,
    command_sqrt = 8,
    command_sin = 9,
    command_cos = 10,
    command_in = 11
};

/** a function that executes commands from a file*/
void do_bin_program(FILE* bin_prog);

#endif
