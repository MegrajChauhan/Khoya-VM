/*
 * Definition of the data memory for the Khoya VM
 */
#ifndef KDATA_MEM
#define KDATA_MEM

#include "../utils/errors.h"
#include "../utils/typedefs.h"

/*!
 * @struct Memory
 * @brief Structure representing the data memory of the VM.
 */
typedef struct Memory
{
    byte *memArray; /*!< Pointer to the memory array */
    qword size;     /*!< Size of the memory */
} Memory;

#include "../CPU/kCPU.h"

/*!
 * @brief Initializes the data memory.
 * @param memory Pointer to the memory object to initialize
 * @param __initial_size Initial size of the memory
 */
void init_mem(Memory **memory, qword __initial_size);

/*!
 * @brief Increases the size of the data memory.
 * @param memory Pointer to the memory object
 * @param __additional_size Additional size to increase the memory by
 */
void mem_size_increase(Memory **memory, qword __additional_size);

/*!
 * @brief Writes an 8-bit (1 byte) value to the memory at the specified address.
 * @param memory Pointer to the memory object
 * @param address Address to write the value to
 * @param value Value to write
 */
void mem_write8(Memory *memory, qword address, qword value);

/*!
 * @brief Writes a 16-bit (2 bytes) value to the memory at the specified address.
 * @param memory Pointer to the memory object
 * @param address Address to write the value to
 * @param value Value to write
 */
void mem_write16(Memory *memory, qword address, qword value);

/*!
 * @brief Writes a 32-bit (4 bytes) value to the memory at the specified address.
 * @param memory Pointer to the memory object
 * @param address Address to write the value to
 * @param value Value to write
 */
void mem_write32(Memory *memory, qword address, qword value);

/*!
 * @brief Writes a 64-bit (8 bytes) value to the memory at the specified address.
 * @param memory Pointer to the memory object
 * @param address Address to write the value to
 * @param value Value to write
 */
void mem_write64(Memory *memory, qword address, qword value);

/*!
 * @brief Reads an 8-bit (1 byte) value from the memory at the specified address.
 * @param memory Pointer to the memory object
 * @param address Address to read the value from
 * @return The read value
 */
qword mem_read8(Memory *memory, qword address);

/*!
 * @brief Reads a 16-bit (2 bytes) value from the memory at the specified address.
 * @param memory Pointer to the memory object
 * @param address Address to read the value from
 * @return The read value
 */
qword mem_read16(Memory *memory, qword address);

/*!
 * @brief Reads a 32-bit (4 bytes) value from the memory at the specified address.
 * @param memory Pointer to the memory object
 * @param address Address to read the value from
 * @return The read value
 */
qword mem_read32(Memory *memory, qword address);

/*!
 * @brief Reads a 64-bit (8 bytes) value from the memory at the specified address.
 * @param memory Pointer to the memory object
 * @param address Address to read the value from
 * @return The read value
 */
qword mem_read64(Memory *memory, qword address);

/*!
 * @brief Frees the memory allocated for the data memory.
 * @param memory Pointer to the memory object to free
 */
void free_mem(Memory *memory);
#endif