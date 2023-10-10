#include "cpu.h"
#include "stack_functions.h"
#include "stack_error.h"

#include <limits.h>
#include <stdio.h>

int main()
{
    FILE *bin_program = fopen("bin_prog.txt", "r");
    assert(bin_program != NULL);

    double* code_cs = (double*)calloc(MAX_SIZE, sizeof(double));
    assert(code_cs != NULL);

    file_read(code_cs, bin_program);
    fclose(bin_program);

    double heh = 0;
    fscanf(bin_program, "%lf", &heh);

    struct CPU cpu = {};
    CPU_CTOR(&cpu, code_cs);

    do_bin_program(&cpu);
    free(code_cs);
    return 0;
}

void do_bin_program(struct CPU *cpu)
{
    assert(cpu != NULL);

    int ncommands = *(cpu->code_cs);
    (cpu->code_cs) ++;

    for (int i = 0; i < ncommands; i++)
    {
        double command = (*cpu->code_cs);
        switch ((int)command)
        {
        case command_push:
            do_push(&(*cpu));
            break;
        case command_in:
            do_in(&(*cpu));
            break;
        case command_pop:
            do_pop(&(*cpu));
            break;
        case command_div:
            do_div(&(*cpu));
            break;
        case command_sub:
            do_sub(&(*cpu));
            break;
        case command_add:
            do_add(&(*cpu));
            break;
        case command_mul:
            do_mul(&(*cpu));
            break;
        case command_out:
            do_out(&(*cpu));
            break;
        case command_hlt:
            do_hlt(&(*cpu));
            break;
        case command_sqrt:
            do_sqrt(&(*cpu));
            break;
        default:
            break;
        }
    }
}

void CPU_CTOR(struct CPU *cpu, double* code_cs)
{
    assert(cpu != NULL);
    assert(code_cs != NULL);

    struct Stack stk = {};
    stack_ctor(&stk, 2);
    cpu->stk = stk;
    cpu->rax = 0;
    cpu->rbx = 0;
    cpu->rcx = 0;
    cpu->rdx = 0;
    cpu->ip = 0;
    cpu->code_cs = code_cs;
    Stack_push(&(cpu->stk), 0);

}

void do_in(struct CPU *cpu)
{
    assert(cpu != NULL);

    printf(GREEN "\nEnter a number for push_command: " END_OF_COLOUR);
    double in_number = 0;
    scanf("%lf", &in_number);
    Stack_push(&(cpu->stk), in_number);
    (cpu->code_cs)++;
}

void do_pop(struct CPU *cpu)
{
    assert(cpu != NULL);
    (cpu->code_cs)++;
    (cpu->code_cs)++;

    double nreg = *(cpu->code_cs);
    (cpu->code_cs)++;

    double number_from_stack = 0;
    Stack_pop(&(cpu->stk), &number_from_stack);

    switch ((int)nreg)
    {
        case reg_rax:
            (cpu->rax) = number_from_stack;
            break;
        case reg_rbx:
            (cpu->rbx) = number_from_stack;
            break;
        case reg_rcx:
            (cpu->rcx) = number_from_stack;
            break;
        case reg_rdx:
            (cpu->rdx) = number_from_stack;
            break;
        default:
            break;
    }
}

void do_push(struct CPU *cpu)
{
    assert(cpu != NULL);

    (cpu->code_cs)++;
    double mode = *(cpu->code_cs);
    (cpu->code_cs)++;

    if (mode == 1)
    {
        double number = *(cpu->code_cs);
        (cpu->code_cs)++;
        Stack_push(&(cpu->stk), number);
    }
    else
    {
        double nreg = *(cpu->code_cs);
        (cpu->code_cs)++;
        switch ((int)nreg)
        {
            case reg_rax:
                Stack_push(&(cpu->stk), cpu->rax);
                break;
            case reg_rbx:
                Stack_push(&(cpu->stk), cpu->rbx);
                break;
            case reg_rcx:
                Stack_push(&(cpu->stk), cpu->rcx);
                break;
            case reg_rdx:
                Stack_push(&(cpu->stk), cpu->rdx);
                break;
            default:
                break;
        }
    }
}

void do_div(struct CPU *cpu)
{
    assert(cpu != NULL);

    (cpu->code_cs)++;
    double number1 = 0;
    double number2 = 0;

    Stack_pop(&(cpu->stk), &number1);
    Stack_pop(&(cpu->stk), &number2);
    Stack_push(&(cpu->stk), number2/number1);
}

void do_sub(struct CPU *cpu)
{
    assert(cpu != NULL);

    (cpu->code_cs)++;
    double number1 = 0;
    double number2 = 0;

    Stack_pop(&(cpu->stk), &number1);
    Stack_pop(&(cpu->stk), &number2);
    Stack_push(&(cpu->stk), number2-number1);
}

void do_add(struct CPU *cpu)
{
    assert(cpu != NULL);

    (cpu->code_cs)++;
    double number1 = 0;
    double number2 = 0;

    Stack_pop(&(cpu->stk), &number1);
    Stack_pop(&(cpu->stk), &number2);
    Stack_push(&(cpu->stk), number2+number1);
}

void do_sqrt(struct CPU *cpu)
{
    assert(cpu != NULL);

    (cpu->code_cs)++;
    double number = 0;

    Stack_pop(&(cpu->stk), &number);
    if (number < 0)
        printf(BLUE "Extracting the root from a negative number" END_OF_COLOUR);
        return;
    Stack_push(&(cpu->stk), sqrt(number));
}

void do_mul(struct CPU *cpu)
{
    assert(cpu != NULL);

    (cpu->code_cs)++;
    double number1 = 0;
    double number2 = 0;

    Stack_pop(&(cpu->stk), &number1);
    Stack_pop(&(cpu->stk), &number2);
    Stack_push(&(cpu->stk), number2*number1);
}

void do_out(struct CPU *cpu)
{
    assert(cpu != NULL);

    (cpu->code_cs)++;
    double number = 0;
    Stack_pop(&(cpu->stk), &number);
    printf(BLUE "\nResult: %lf\n" END_OF_COLOUR, number);
}

void do_hlt(struct CPU *cpu)
{
    (cpu->code_cs)++;
    assert(cpu != NULL);
    return;
}

void file_read(double* code_cs, FILE* bin_program)
{
    assert(code_cs != NULL);
    assert(bin_program != NULL);

    size_t current = 0;
    double number = 0;
    while (!feof(bin_program))
    {
        code_cs[current] = number;
        current++;
    }
}
