#include "asm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE *program = fopen("prog.txt", "r");
    FILE *bin_program = fopen("bin_prog.txt", "w");

    assembly(program, bin_program);

    fclose(program);
    fclose(bin_program);
    return 0;
}


void assembly(FILE* program, FILE* bin_program)
{

    char *command = (char*)calloc(MAX_LEN, sizeof(char));
    double variable = 0;
    int scanf_result = fscanf(program, " %s ", command);

    while (scanf_result != EOF)
    {

        if (!strcmp(command, "push"))
        {
            fscanf(program, " %lf ", &variable);
            fprintf(bin_program, "%d ", command_push);
            fprintf(bin_program, "%lf\n", variable);
        }
        else if (!strcmp(command, "pop"))
        {
            fprintf(bin_program, "%d\n", command_pop);

            //here
        }
        else if (!strcmp(command, "div")) fprintf(bin_program, "%d\n", command_div);
        else if (!strcmp(command, "sub")) fprintf(bin_program, "%d\n", command_sub);
        else if (!strcmp(command, "add")) fprintf(bin_program, "%d\n", command_add);
        else if (!strcmp(command, "mul")) fprintf(bin_program, "%d\n", command_mul);
        else if (!strcmp(command, "out")) fprintf(bin_program, "%d\n", command_out);
        else if (!strcmp(command, "in"))  fprintf(bin_program, "%d\n", command_in);
        else if (!strcmp(command, "HLT")) fprintf(bin_program, "%d\n", command_HLT);
        else{
            printf("%s, Syntax error\n", command);
            return;
        }

        scanf_result = fscanf(program, " %s ", command );
    }

    free(command);
}
