#include "spu.h"
#include "stack_functions.h"
#include "stack_error.h"

#include <limits.h>
#include <stdio.h>

int main(){

    FILE *bin_program = fopen("bin_prog.txt", "r");
    if (bin_program == NULL){
        printf("File not found\n");
        return 0;
    }
    do_bin_program(bin_program);
    fclose(bin_program);
    return 0;
}

void do_bin_program(FILE* bin_prog){
    struct stack stk = {};
    STACK_CTOR(&stk, 2);
    stack_push(&stk, 0);
    int comand = 0;

    int scanf_result = fscanf(bin_prog, " %d ", &comand);

    double variable = 0;

    while (scanf_result != EOF){
        if (comand == comand_push){
            fscanf(bin_prog, " %lf ", &variable);
            stack_push(&stk, variable);
        }
        else if (comand == comand_div){
            double a = 0;
            double b = 0;
            if (a == 0){
                printf("division by 0");
                return;
            }
            stack_pop(&stk, &a);
            stack_pop(&stk, &b);
            stack_push(&stk, b/a);
        }
        else if (comand == comand_sub){
            double a = 0;
            double b = 0;
            stack_pop(&stk, &a);
            stack_pop(&stk, &b);
            stack_push(&stk, b-a);
        }
        else if (comand == comand_add){
            double a = 0;
            double b = 0;
            stack_pop(&stk, &a);
            stack_pop(&stk, &b);
            stack_push(&stk, b+a);
        }
        else if (comand == comand_mul){
            double a = 0;
            double b = 0;
            stack_pop(&stk, &a);
            stack_pop(&stk, &b);
            stack_push(&stk, b*a);
        }
        else if (comand == comand_out){
            double a = 0;
            stack_pop(&stk, &a);
            printf("%lf\n", a);
        }
        else if (comand == comand_HLT) {
            return;
        }
        scanf_result = fscanf(bin_prog, " %d ", &comand);
    }
    stack_dtor(&stk);
}
