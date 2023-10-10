#include "stack_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

stack_result_t Stack_ctor(struct Stack *stk, int capacity, const char *file_name,
                                const char *func_name, const char *arg_name, int line)
{
    assert(stk != NULL);
    assert(file_name != NULL);
    assert(func_name != NULL);
    assert(arg_name != NULL);

    stk->right_canary = canary;
    stk->left_canary = canary;
    stk->file_name = file_name;
    stk->func_name = func_name;
    stk->arg_name = arg_name;
    stk->line = line;
    stk->size = 0;
    stk->capacity = capacity;

    elem_t *data = (elem_t *)calloc((size_t)capacity*sizeof(elem_t) + CANARY_SIZE, sizeof(char));
    if(!data)
    {
        stack_errno |= DATA_ERROR;
        STACK_ERROR(stk, stack_errno);
        return stack_errno;
    }
    *data = canary;
    stk->data = data + canary_shift;

    for(int i = 0; i < stk->capacity; i++)
        stk->data[i] = poison;

    *(data + capacity + canary_shift) = canary;

    stk->stk_hash = oat_hash(stk, stk_size);
    stk->data_hash = oat_hash(stk->data, (size_t)stk->capacity * sizeof(elem_t));

    if(Stack_is_invalid(stk))
    {
        STACK_ERROR(stk, stack_errno);
        return stack_errno;
    }

    return stack_errno;
}

stack_result_t Stack_dtor(struct Stack *stk)
{
    if (Stack_is_invalid(stk))
    {
        STACK_ERROR(stk, stack_errno);
        return stack_errno;
    }

    stk->data -= canary_shift;

    for (int i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = poison;
    }

    free(stk->data);

    return stack_errno;
}

stack_result_t Stack_realloc(struct Stack *stk, int to_increase)
{
    if(Stack_is_invalid(stk))
    {
        STACK_ERROR(stk, stack_errno);
        return stack_errno;
    }

    if (to_increase == TO_INCREASE)
    {
        elem_t *data = (elem_t *)realloc(stk->data - canary_shift,
                        sizeof(char)*(((size_t)(stk->capacity * multiplier))*sizeof(elem_t) + CANARY_SIZE));

        if (!data)
        {
            stack_errno |= DATA_ERROR;
            STACK_ERROR(stk, stack_errno);
            return stack_errno;
        }

        stk->capacity *=multiplier;
        *(data + canary_shift + stk->capacity) = canary;
        stk->data = data + canary_shift;

        for (int i = stk->size; i < stk->capacity; i++)
            stk->data[i] = poison;
    }

    else
    {
        elem_t *data = (elem_t *)realloc(stk->data - canary_shift,
        (sizeof(char))*((size_t)(stk->capacity/decrease_multiplier)*sizeof(elem_t) + CANARY_SIZE));

        if (!data)
        {
            stack_errno |= DATA_ERROR;
            STACK_ERROR(stk, stack_errno);
            return stack_errno;
        }

        stk->capacity /=decrease_multiplier;
        stk->data = data + canary_shift;
        *(stk->data + stk->capacity) = canary;
    }

    return stack_errno;
}

stack_result_t Stack_push(struct Stack *stk, elem_t value)
{
    if (Stack_is_invalid(stk))
    {
        STACK_ERROR(stk, stack_errno);
        return stack_errno;
    }

    if (stk->size >= stk->capacity)
    {
        if (Stack_realloc(stk, TO_INCREASE))
        {
            STACK_ERROR(stk, stack_errno);
            return stack_errno;
        }
    }

    stk->data[(stk->size)++] = value;
    stk->stk_hash = oat_hash(stk, stk_size);
    stk->data_hash = oat_hash(stk->data, (size_t)stk->capacity * sizeof(elem_t));

    return stack_errno;
}

stack_result_t Stack_pop(struct Stack *stk, elem_t *value)
{
    if (Stack_is_invalid(stk))
    {
        STACK_ERROR(stk, stack_errno);
        return stack_errno;
    }
    if (stk->size <= 0)
    {
        stack_errno = SIZE_ERROR;
        STACK_ERROR(stk, stack_errno);
        return stack_errno;
    }
    stk->size--;

    *value = stk->data[stk->size];

    stk->data[stk->size] = poison;

    stk->stk_hash = oat_hash(stk, stk_size);
    stk->data_hash = oat_hash(stk->data, (size_t)stk->capacity * sizeof(elem_t));

    if (stk->size > 0)
    {
        if ((stk->size) * multiplier * decrease_multiplier <= stk->capacity)
        {
            if (Stack_realloc(stk, TO_DECREASE))
            {
                STACK_ERROR(stk, stack_errno);
                return stack_errno;
            }
        }
    }

    stk->stk_hash = oat_hash(stk, stk_size);
    stk->data_hash = oat_hash(stk->data, (size_t)stk->capacity * sizeof(elem_t));

    return stack_errno;
}
