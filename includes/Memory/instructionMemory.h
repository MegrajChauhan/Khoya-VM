#ifndef KINSTRUCTION_MEMORY_H
#define KINSTRUCTION_MEMORY_H

#include <stdlib.h>
#include "../utils/typedefs.h"

/*!
 * @enum State
 * @brief Enumeration representing the state of the instruction memory.
 */
typedef enum States
{
    ALMOST_FULL, /*!< The memory is almost full (about 10 more instructions can be stored) */
    UNDER_FULL,  /*!< The memory is under half utilized */
    ERROR_STATE  /*!< The memory access has failed */
} State;

/*!
 * @struct Instruction_memory
 * @brief Structure representing the instruction memory.
 */
typedef struct Instruction_memory
{
    qword *instruction_array; /*!< Array storing the instructions */
    qword size;               /*!< Size of the memory */
    qword *ip;                /*!< Instruction pointer */
    State state;              /*!< State of the memory[This information is useless for the VM as of now]*/
} Instruction_memory;

#include "../CPU/kCPU.h"

/*!
 * @brief Initializes the instruction memory.
 * @param memory Pointer to the instruction memory object to initialize
 * @param size Size of the memory
 */
void Imem_init(Instruction_memory **memory, qword size);

/*!
 * @brief Increases the size of the instruction memory.
 * @param memory Pointer to the instruction memory object to resize
 * @param __size_to_add Size to add to the memory
 */
void Imem_increase_size(Instruction_memory **memory, qword __size_to_add);

/*!
 * @brief Writes a value to the instruction memory at the specified address.
 * @param mem Pointer to the instruction memory object
 * @param addr Address at which to write the value
 * @param value Value to write
 */
void Imem_write(Instruction_memory *mem, qword addr, qword value);

/*!
 * @brief Reads a value from the instruction memory at the specified address.
 * @param mem Pointer to the instruction memory object
 * @param addr Address from which to read the value
 * @return The value read from the memory
 */
qword Imem_read(Instruction_memory *mem, qword addr);

/*!
 * @brief Frees the memory allocated for the instruction memory.
 * @param memory Pointer to the instruction memory object to free
 */
void free_Imem(Instruction_memory *memory);

#endif