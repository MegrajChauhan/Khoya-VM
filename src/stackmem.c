#include "../includes/Memory/stackmem.h"

/*
* stack_init : The function that initializes the stack memory for the VM to work properly
* stack - a pointer to a pointer of a Stack object
* __size - the initial size of the stack
*/
void stack_init(Stack **stack, qword __size)
{
    (*stack) = (Stack *)malloc(sizeof(Stack)); // allocate memory
    if (*stack == NULL)
    {
        KSET_ERR(KSTACK_INITIALIZATION_FAILED, "Could not initialize stack\n"); //set error
        return;
    }
    (*stack)->stackArray = (byte *)malloc(sizeof(byte) * __size); // initialize memory for the stack array
    if ((*stack)->stackArray == NULL)
    {
        KSET_ERR(KSTACK_INITIALIZATION_FAILED, "Could not initialize stack\n"); // set error 
        return;
    }
    (*stack)->size = __size;
    (*stack)->sp = NULL;
    (*stack)->available = __size;
}

/*
* stack_push8: Function to push an 8 bits[1 byte or a byte] value to the top of stack
* stack - pointer to a Stack object
* value - the value to push
*/
void stack_push8(Stack *stack, qword value)
{
    if (stack->available >= 1) // is 1 byte available
    {
        qword index = stack->size - stack->available; // get the topmost free address
        stack->stackArray[index] = value & 255; // push the value
        stack->sp = &stack->stackArray[index]; // point to the new top
        stack->available--; // available -1 bytes
        return;
    }
    KSET_ERR(KSTACK_OVERFLOW, "The stack memory was exhausted"); // else set error
}

// same as stack_push8 but modified for 16 bits values
void stack_push16(Stack *stack, qword value)
{
    if (stack->available >= 2)
    {
        qword index = stack->size - stack->available;
        stack->stackArray[index] = (value >> 8) & 255;
        stack->stackArray[index + 1] = value & 255;
        stack->sp = &stack->stackArray[index + 1];
        stack->available -= 2;
        return;
    }
    KSET_ERR(KSTACK_OVERFLOW, "The stack memory was exhausted");
}

// same as stack_push8 but modified for 32 bits values
void stack_push32(Stack *stack, qword value)
{
    if (stack->available >= 4)
    {
        qword index = stack->size - stack->available;
        stack->stackArray[index] = (value >> 24) & 255;
        stack->stackArray[index + 1] = (value >> 16) & 255;
        stack->stackArray[index + 2] = (value >> 8) & 255;
        stack->stackArray[index + 3] = value & 255;
        stack->sp = &stack->stackArray[index + 3];
        stack->available -= 4;
        return;
    }
    KSET_ERR(KSTACK_OVERFLOW, "The stack memory was exhausted");
}

// same as stack_push8 but modified for 64 bits values
void stack_push64(Stack *stack, qword value)
{
    if (stack->available >= 8)
    {
        qword index = stack->size - stack->available;
        stack->stackArray[index] = (value >> 56) & 255;
        stack->stackArray[index + 1] = (value >> 48) & 255;
        stack->stackArray[index + 2] = (value >> 40) & 255;
        stack->stackArray[index + 3] = (value >> 32) & 255;
        stack->stackArray[index + 4] = (value >> 24) & 255;
        stack->stackArray[index + 5] = (value >> 16) & 255;
        stack->stackArray[index + 6] = (value >> 8) & 255;
        stack->stackArray[index + 7] = value & 255;
        stack->sp = &stack->stackArray[index + 7];
        stack->available -= 8;
        return;
    }
    KSET_ERR(KSTACK_OVERFLOW, "The stack memory was exhausted");
}


qword stack_pop8(Stack *stack)
{
    if (stack->available == stack->size)
    {
        KSET_ERR(KSTACK_UNDERFLOW, "The stack is empty to pop values");
        return 0;
    }
    stack->available++;
    stack->sp--;
    return (stack->stackArray[stack->size - stack->available + 1] | 0UL);
}

qword stack_pop16(Stack *stack)
{
    if ((stack->size - stack->available) < 2)
    {
        KSET_ERR(KSTACK_UNDERFLOW, "The stack isn't big enough to pop value of that size");
        return 0;
    }
    stack->available += 2;
    stack->sp -= 2;
    return (((stack->stackArray[stack->size - stack->available + 1] << 8) | (stack->stackArray[stack->size - stack->available + 2]) | 0UL));
}

qword stack_pop32(Stack *stack)
{
    if ((stack->size - stack->available) < 4)
    {
        KSET_ERR(KSTACK_UNDERFLOW, "The stack isn't big enough to pop value of that size");
        return 0;
    }
    qword value = 0;
    value = (value << 8) | *(stack->sp - 3);
    value = (value << 8) | *(stack->sp - 2);
    value = (value << 8) | *(stack->sp - 1);
    value = (value << 8) | *(stack->sp);
    stack->available += 4;
    stack->sp -= 4;
    return value | 0UL;
}

qword stack_pop64(Stack *stack)
{
    if ((stack->size - stack->available) < 4)
    {
        KSET_ERR(KSTACK_UNDERFLOW, "The stack isn't big enough to pop value of that size");
        return 0;
    }
    qword value = 0;
    value = (value << 8) | *(stack->sp - 7);
    value = (value << 8) | *(stack->sp - 6);
    value = (value << 8) | *(stack->sp - 5);
    value = (value << 8) | *(stack->sp - 4);
    value = (value << 8) | *(stack->sp - 3);
    value = (value << 8) | *(stack->sp - 2);
    value = (value << 8) | *(stack->sp - 1);
    value = (value << 8) | *(stack->sp);
    stack->available += 8;
    stack->sp -= 8;
    return value;
}

void free_stack(Stack *stack)
{
    free(stack->stackArray);
    free(stack);
}
