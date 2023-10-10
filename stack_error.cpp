#include "stack_error.h"

#include <assert.h>

uint32_t stack_errno = 0;

const int max_bit = 32;

const char *str_error[] =
{
    "memory allocation failure cannot access memory for struct stack",
    "memory allocation failure cannot access memory for data",
    "current position is out of range",
    "size exceeds capacity",
    "free cells do not have poison inside",
    "Right canary detected invasion in the stack form the right",
    "Left canary detected invasion in the stack from the left",
    "Right canary detected invasion in the data from the right",
    "Left canary detected invasion in the data from the left"
    "hash for sruct stack mismatched",
    "hash for data mismatched",
};

void Stack_error_decode(uint32_t error)
{
    for (int shift = 0; shift < max_bit; shift++)
    {
        if((1 << shift) & error)
            fprintf(stderr, "%s\n", str_error[shift]);
    }
}

void Stack_dump (struct Stack *stk, const char *file1, int line1, const char *func_name1, const char *arg_name1)
{
    fprintf(stderr, "%s from %s:%d %s()\n"
                    "size = %d;\n"
                    "capacity = %d;\n{\n",
                    arg_name1, file1, line1, func_name1,
                    stk->size,
                    stk->capacity);

    for (int i = 0; i < stk->size; i++)
        fprintf(stderr, "   *[%d] = " ELEM_PRINT_SPEC ";\n", i, stk->data[i]);

    for (int i = stk->size; i < stk->capacity; i++)
        fprintf(stderr, "   [%d] = " ELEM_PRINT_SPEC " (POISON);\n", i, stk->data[i]);

    fprintf(stderr, "  }\n");
}

uint32_t Stack_is_invalid(const struct Stack *stk)
{
    uint32_t error = 0;

    if (!stk)
    {
        error |= STK_ERROR;
        stack_errno = error;
        return error;
    }

    if (!stk->data)                                  error |= DATA_ERROR;
    if (stk->size < 0)                               error |= SIZE_ERROR;
    if (stk->size > stk->capacity)                   error |= SIZE_OVER_CAPACITY_ERROR;
    if (stk->right_canary != canary)                 error |= RIGHT_CANARY_STK_ERROR;
    if (stk->left_canary != canary)                  error |= LEFT_CANARY_STK_ERROR;
    if (!(*(stk->data - canary_shift) == canary))    error |= LEFT_CANARY_DATA_ERROR;
    if (!(*(stk->data + stk->capacity) == canary))   error |= RIGHT_CANARY_DATA_ERROR;
    if (oat_hash(stk, stk_size) != stk->stk_hash)    error |= HASH_STK_ERROR;
    if (oat_hash(stk->data, (size_t)stk->capacity *
        sizeof(elem_t)) != stk->data_hash)           error |= HASH_DATA_ERROR;

    for (int index = stk->size; index < stk->capacity; index++)
        if (!(stk->data[index] == poison)) error |= POISON_ERROR;

    stack_errno = error;

    return error;
}
