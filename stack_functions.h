#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H

#include "stack.h"
#include "stack_error.h"

#include <assert.h>

const size_t CANARY_SIZE =  2*sizeof(canary_t);
const int multiplier = 2;
const int decrease_multiplier = 2;

enum REALLOC_DATA
{
    TO_DECREASE = 0,
    TO_INCREASE = 1,
};

stack_result_t Stack_push(struct Stack *stk, elem_t value);

stack_result_t Stack_pop(struct Stack *stk, elem_t *value);

stack_result_t Stack_realloc(struct Stack *stk, int to_increase);

stack_result_t Stack_ctor(struct Stack *stk, int capacity, const char *file_name,
                          const char *func_name, const char *arg_name, int line);

stack_result_t Stack_dtor(struct Stack *stk);

stack_result_t Stack_realloc_if_needed(struct Stack *stk);

#define stack_ctor(stk, capacity) Stack_ctor((stk), (capacity),   \
                     __FILE__, __func__, (#stk), __LINE__)

#endif
