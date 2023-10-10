#ifndef _CPU_H_
#define _CPU_H_

#include "stack.h"
#include "stack_error.h"
#include "stack_functions.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#define RED           "\x1b[31;1m"
#define REDD          "\x1b[31;4m"
#define GREEN         "\x1b[32;4m"
#define YELLOW        "\x1b[33;4m"
#define BLUE          "\x1b[34;4m"
#define END_OF_COLOUR "\x1b[39;49m"
#define POSITION_IN_CODE __FILE__, __PRETTY_FUNCTION__, __LINE__

struct CPU
{
    struct Stack stk;
    double rax;
    double rbx;
    double rcx;
    double rdx;
    int ip;
    double* code_cs;
};

const int MAX_SIZE = 1000;

/* This enumeration contains the assembly language command numbers */
enum COMMANDS
{
    command_push = 33,
    command_div  = 2,
    command_sub  = 3,
    command_out  = 4,
    command_add  = 5,
    command_mul  = 6,
    command_hlt  = 7,
    command_sqrt = 8,
    command_sin  = 9,
    command_cos  = 10,
    command_in   = 11,
    command_pop  = 43
};

enum REGISTER
{
    reg_rax = 1,
    reg_rbx = 2,
    reg_rcx = 3,
    reg_rdx = 4
};

/* The function that executes commands from a file */
void do_bin_program(struct CPU *cpu);

void CPU_CTOR(struct CPU *cpu, double* code_cs);

void do_in(struct CPU *cpu);

void do_pop(struct CPU *cpu);

void do_push(struct CPU *cpu);

void do_div(struct CPU *cpu);

void do_sub(struct CPU *cpu);

void do_add(struct CPU *cpu);

void do_sqrt(struct CPU *cpu);

void do_mul(struct CPU *cpu);

void do_out(struct CPU *cpu);

void do_hlt(struct CPU *cpu);

void file_read(double* code_cs, FILE* bin_program);

#endif
