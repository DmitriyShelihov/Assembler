#ifndef _ASM_H_
#define _ASM_H_

#include <stdio.h>
#include <assert.h>

const int MAX_LEN = 100;
const int MAX_SIZE = 1000;
const int SIZE_OF_REGISTER = 10;

#define RED           "\x1b[31;1m"
#define REDD          "\x1b[31;4m"
#define END_OF_COLOUR "\x1b[39;49m"
#define POSITION_IN_CODE __FILE__, __PRETTY_FUNCTION__, __LINE__

/* this enumeration contains the assembly language command numbers */
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

/* a function that gives each command its own number */
void assembly(FILE* program, FILE* bin_program);

/* a function that returns the number of register or -1 || -2 if it is not register */
int check_register(char* cpu_register);

/* a function that returns the number of characters in the number */
int len_of_number(double number);

void assembly_push(FILE* bin_program, char* source);

void assembly_pop(FILE* bin_program, char* source);

void skip_comment(char* source);

#endif
