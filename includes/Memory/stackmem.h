/*
 * The definition of the stack for the Khoya VM
 */

#ifndef KSTACK
#define KSTACK

#include "../utils/typedefs.h" // for types

/*!
 * @struct Stack
 * @brief Structure representing the stack of the VM.
 */
typedef struct Stack
{
    byte *stackArray;   /*!< Pointer to the stack memory */
    qword size;         /*!< Size of the stack */
    byte *sp;           /*!< Stack pointer (points to the top of the stack) */
    qword available;    /*!< Number of bytes available */
} Stack;

#include "../CPU/kCPU.h"

/*!
 * @brief Initializes the stack memory.
 * @param stack Pointer to the stack object to initialize
 * @param __size Initial size of the stack
 */
void stack_init(Stack **stack, qword __size);

/*!
 * @brief Pushes an 8-bit (1 byte) value to the top of the stack.
 * @param stack Pointer to the stack object
 * @param value Value to push
 */
void stack_push8(Stack *stack, qword value);

/*!
 * @brief Pushes a 16-bit (2 bytes) value to the top of the stack.
 * @param stack Pointer to the stack object
 * @param value Value to push
 */
void stack_push16(Stack *stack, qword value);

/*!
 * @brief Pushes a 32-bit (4 bytes) value to the top of the stack.
 * @param stack Pointer to the stack object
 * @param value Value to push
 */
void stack_push32(Stack *stack, qword value);

/*!
 * @brief Pushes a 64-bit (8 bytes) value to the top of the stack.
 * @param stack Pointer to the stack object
 * @param value Value to push
 */
void stack_push64(Stack *stack, qword value);

/*!
 * @brief Pops an 8-bit (1 byte) value from the top of the stack.
 * @param stack Pointer to the stack object
 * @return The popped value
 */
qword stack_pop8(Stack *stack);

/*!
 * @brief Pops a 16-bit (2 bytes) value from the top of the stack.
 * @param stack Pointer to the stack object
 * @return The popped value
 */
qword stack_pop16(Stack *stack);

/*!
 * @brief Pops a 32-bit (4 bytes) value from the top of the stack.
 * @param stack Pointer to the stack object
 * @return The popped value
 */
qword stack_pop32(Stack *stack);

/*!
 * @brief Pops a 64-bit (8 bytes) value from the top of the stack.
 * @param stack Pointer to the stack object
 * @return The popped value
 */
qword stack_pop64(Stack *stack);

/*!
 * @brief Frees the memory allocated for the stack.
 * @param stack Pointer to the stack object to free
 */
void free_stack(Stack *stack);

#endif