#include "asm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    FILE *program = fopen("prog.txt", "r");
    FILE *bin_program = fopen("bin_prog.txt", "w");

    assert(program != NULL);
    assert(bin_program != NULL);
    assembly(program, bin_program);

    fclose(program);
    //fclose(bin_program);
    return 0;
}


void assembly(FILE* program, FILE* bin_program)
{
    assert(program != NULL);
    assert(bin_program != NULL);

    char *source = (char *)calloc(MAX_SIZE, sizeof(char));
    assert(source != NULL);
    fread(source, sizeof(char), MAX_SIZE, program);

    char *command = (char*)calloc(MAX_LEN, sizeof(char));
    assert(command != NULL);
    double variable = 0;

    char *cpu_register = (char*)calloc(SIZE_OF_REGISTER, sizeof(char));
    assert(cpu_register != NULL);

    int sscanf_result = sscanf(source, " %s ", command);
    source += strlen(command) + 1;

    while (sscanf_result != EOF)
    {
        puts(command);
        if (!strcmp(command, "push")) assembly_push(bin_program, source);
        else if (!strcmp(command, "pop")) assembly_pop(bin_program, source);
        else if (!strcmp(command, "div")) fprintf(bin_program, "%d\n", command_div);
        else if (!strcmp(command, "sub")) fprintf(bin_program, "%d\n", command_sub);
        else if (!strcmp(command, "add")) fprintf(bin_program, "%d\n", command_add);
        else if (!strcmp(command, "mul")) fprintf(bin_program, "%d\n", command_mul);
        else if (!strcmp(command, "out")) fprintf(bin_program, "%d\n", command_out);
        else if (!strcmp(command, "sqrt")) fprintf(bin_program, "%d\n", command_sqrt);
        else if (!strcmp(command, "in")) fprintf(bin_program, "%d\n", command_in);
        else if (!strcmp(command, "HLT")) fprintf(bin_program, "%d\n", command_HLT);
        else
        {
            printf(REDD "%s, Syntax error\n" END_OF_COLOUR, command);
            return;
        }

        sscanf_result = sscanf(source, " %s ", command);
        source += strlen(command) + 1;
    }

    free(command);
    free(source);
    free(cpu_register);
}

int check_register(char* cpu_register)
{
    for (int i = 0; i < strlen(cpu_register) - 2; i++)
    {
        if (cpu_register[i] != ' ')
        {
            if ((cpu_register[i] == 'r' || cpu_register[i] == 'R') &&
                (cpu_register[i+2] == 'x' || cpu_register[i+2] == 'X'))
            {
                return (cpu_register[i+1] - 'a' + 1);
            }
            else
            {
                fprintf(stderr, RED "Incorrect name of register\n" END_OF_COLOUR);
                return -1;
            }
        }
    }
    fprintf(stderr, RED "Name if register was not found\n" END_OF_COLOUR);
    return -2;
}

int len_of_number(double number)
{
    if (number > 0)
        return (int)floor (log10 (abs (number)));
    else if (number < 0)
        return (int)floor (log10 (abs (number))) + 1;
    else
        return 1;
}

void assembly_push(FILE* bin_program, char* source)
{
    assert(bin_program != NULL);
    assert(source != NULL);

    fprintf(bin_program, "%d ", command_push);

    double variable = 0;
    char cpu_register[SIZE_OF_REGISTER] = {};

    if (sscanf(source, "%lf", &variable))
    {
        source += len_of_number(variable) + 2;
        fprintf(bin_program, "%d %lf\n", 1, variable);
    }
    else
    {
        sscanf(source, " %s ", cpu_register);
        source += strlen(cpu_register) + 1;
        fprintf(bin_program, "%d %d\n", 2, check_register(cpu_register));
    }
    skip_comment(source);
}

void assembly_pop(FILE* bin_program, char* source)
{
    assert(bin_program != NULL);
    assert(source != NULL);

    fprintf(bin_program, "%d ", command_pop);
    char cpu_register[SIZE_OF_REGISTER] = {};

    sscanf(source, " %s ", cpu_register);
    source += strlen(cpu_register) + 1;

    fprintf(bin_program, "%d %d\n", 2, check_register(cpu_register));
    skip_comment(source);
    printf("(%c%c%c)", *source, *(source+1), *(source+2));
}

void skip_comment(char* source)
{
    assert(source != NULL);

    char symbol = *source;
    while (symbol != ' ' && symbol != '\n' && symbol != '\0')
        if (symbol == ';')
        {
            while (symbol != ' ' && symbol != '\n' && symbol != '\0')
                symbol = *(source++);
        }
        else
            symbol = *(source++);
    return;
}
