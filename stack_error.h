#ifndef STACK_ERROR_H
#define STACK_ERROR_H

#define RED          "\x1b[31;1m"
#define END_OF_COLOR "\x1b[39;49m"

#include "stack.h"
#include <limits.h>

const uint64_t canary = 0xDEDADED;

const int poison = INT_MAX;

const size_t canary_shift = sizeof(canary_t)/sizeof(elem_t);

enum ERROR_CODE
{
    STK_ERROR                = 1 << 0,
    DATA_ERROR               = 1 << 1,
    SIZE_ERROR               = 1 << 2,
    SIZE_OVER_CAPACITY_ERROR = 1 << 3,
    POISON_ERROR             = 1 << 4,
    RIGHT_CANARY_STK_ERROR   = 1 << 5,
    LEFT_CANARY_STK_ERROR    = 1 << 6,
    RIGHT_CANARY_DATA_ERROR  = 1 << 7,
    LEFT_CANARY_DATA_ERROR   = 1 << 8,
    HASH_STK_ERROR           = 1 << 9,
    HASH_DATA_ERROR          = 1 << 10,
};

extern stack_result_t stack_errno;

void Stack_error_decode(uint32_t error);

void Stack_dump(struct Stack *stk, const char *file1, int line1, const char *func_name1, const char *arg_name1);

uint32_t Stack_is_invalid(const struct Stack *stk);

#define STACK_ERROR(stk, error) do                                                  \
                    {                                                               \
                        fprintf(stderr, RED"ERROR:");                               \
                        Stack_error_decode((error));                                \
                        fprintf(stderr, END_OF_COLOR);                              \
                        Stack_dump((stk), __FILE__, __LINE__, __func__, (#stk));    \
                    } while (0)


#endif
