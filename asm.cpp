#include "asm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int main()
{
    FILE *program = fopen("prog.txt", "r");
    FILE *bin_program = fopen("bin_prog.txt", "w");

    assert(program != NULL);
    assert(bin_program != NULL);
    assembly(program, bin_program);

    fclose(program);
    fclose(bin_program);
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
    to_lower(command);

    while (sscanf_result != EOF)
    {
        to_lower(command);
        if (!strcmp(command, "push"))      assembly_push(bin_program, &source);
        else if (!strcmp(command, "pop"))  assembly_pop(bin_program,  &source);
        else if (!strcmp(command, "div"))  assembly_div(bin_program,  &source);
        else if (!strcmp(command, "sub"))  assembly_sub(bin_program,  &source);
        else if (!strcmp(command, "add"))  assembly_add(bin_program,  &source);
        else if (!strcmp(command, "mul"))  assembly_mul(bin_program,  &source);
        else if (!strcmp(command, "out"))  assembly_out(bin_program,  &source);
        else if (!strcmp(command, "sqrt")) assembly_sqrt(bin_program, &source);
        else if (!strcmp(command, "in"))   assembly_in(bin_program,   &source);
        else if (!strcmp(command, "hlt"))  assembly_hlt(bin_program,  &source);
        else
        {
            printf(REDD "%s, Incorrect name of function\n" END_OF_COLOUR, command);
            return;
        }

        sscanf_result = sscanf(source, " %s ", command);
        source += strlen(command) + 1;

    }
    free(command);
    free(source);
    free(cpu_register);
}

REGISTER check_register(char* c_register)
{
    for (int i = 0; i < strlen(c_register) - 2; i++)
    {
        if (c_register[i] != ' ')
        {
            if ((c_register[i] == 'r' || c_register[i] == 'R') &&
                (c_register[i+2] == 'x' || c_register[i+2] == 'X'))
            {
                int nregister = c_register[i+1] - 'a' + 1;
                switch (nregister)
                {
                    case 1: case 2: case 3: case 4:
                        return (REGISTER)nregister;
                    default:
                        fprintf(stderr, RED "There is no such register\n" END_OF_COLOUR);
                        return register_not_exist;
                }
            }
            else
            {
                fprintf(stderr, RED "Incorrect name of register\n" END_OF_COLOUR);
                return register_incorrect;
            }
        }
    }
    fprintf(stderr, RED "Register not specified\n" END_OF_COLOUR);
    return register_not_found;
}

int len_of_number(double number)
{
    if (number > 0)
        return (int)floor (log10 (abs (number))) + 1;
    else if (number < 0)
        return (int)floor (log10 (abs (number))) + 2;
    else
        return 1;
}

void assembly_push(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d ", command_push);
    double variable = 0;
    char* c_register = (char*)calloc(SIZE_OF_REGISTER, sizeof(char));

    assert(c_register != NULL);

    if (sscanf(*source, "%lf", &variable))
    {
        *source += len_of_number(variable) + 1;
        fprintf(bin_program, "%d %lf\n", 1, variable);
    }
    else
    {
        sscanf(*source, " %s ", c_register);
        *source += strlen(c_register) + 1;
        fprintf(bin_program, "%d %d\n", 2, check_register(c_register));
    }
    (*source)--;
    skip_comment(source);
    free(c_register);
}

void assembly_pop(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d ", command_pop);
    char* c_register = (char*)calloc(SIZE_OF_REGISTER, sizeof(char));
    sscanf(*source, " %s ", c_register);
    *source += strlen(c_register) + 1;

    fprintf(bin_program, "%d %d\n", 2, check_register(c_register));
    (*source)--;
    skip_comment(source);
}

void assembly_mul(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d\n", command_mul);
    (*source)--;
    skip_comment(source);
}

void assembly_div(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d\n", command_div);
    (*source)--;
    skip_comment(source);
}

void assembly_sub(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d\n", command_sub);
    (*source)--;
    skip_comment(source);
}

void assembly_add(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d\n", command_add);
    (*source)--;
    skip_comment(source);
}

void assembly_out(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d\n", command_out);
    (*source)--;
    skip_comment(source);
}

void assembly_sqrt(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d\n", command_sqrt);
    (*source)--;
    skip_comment(source);
}

void assembly_in(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d\n", command_in);
    (*source)--;
    skip_comment(source);
}

void assembly_hlt(FILE* bin_program, char** source)
{
    assert(bin_program != NULL);
    assert(*source != NULL);

    fprintf(bin_program, "%d\n", command_hlt);
    (*source)--;
    skip_comment(source);
}

void skip_comment(char** source)
{
    assert(*source != NULL);

    char symbol = *(*source);
    while (symbol != '\n' && symbol != '\0')
    {
        if (symbol == ';')
        {
            while (symbol != '\n' && symbol != '\0')
                symbol = *((*source)++);
        }
        else
            symbol = *((*source)++);
    }
}

void to_lower(char* str)
{
    assert(str != NULL);
    while (*str != '\0')
    {
        *str = tolower(*str);
        str++;
    }
}
