#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H

#include "stack.h"

stack_result_t stack_push(struct stack *stk, elem_t value);

stack_result_t stack_pop(struct stack *stk, elem_t *value);

stack_result_t stack_realloc(struct stack *stk, int to_increase);

stack_result_t stack_ctor(struct stack *stk, int capacity, const char *file_name,
                          const char *func_name, const char *arg_name, int line);

stack_result_t stack_dtor(struct stack *stk);

stack_result_t stack_realloc_if_needed(struct stack *stk);

#define STACK_CTOR(stk, capacity) stack_ctor((stk), (capacity),   \
                     __FILE__, __func__, (#stk), __LINE__)

#endif
