#ifndef _ASM_H_
#define _ASM_H_

#include <stdio.h>

const int MAX_LEN = 100;

enum commands
{
    command_push = 33,
    command_div  = 2,
    command_sub  = 3,
    command_out  = 4,
    command_add  = 5,
    command_mul  = 6,
    command_HLT  = 7,
    command_sqrt = 8,
    command_sin  = 9,
    command_cos  = 10,
    command_in   = 11,
    command_pop  = 43
};

/** a function that gives each command its own number */
void assembly(FILE* program, FILE* bin_program);

#endif
