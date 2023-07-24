/*
 * Definition of the memory heap for memory management
 */
#ifndef KMEM_HEAP_H
#define KMEM_HEAP_H

#include <unistd.h>
#include "../utils/typedefs.h"
#include <stdlib.h>

/*!
 * @struct Memory_heap
 * @brief Structure representing the memory heap for memory management.
 */
typedef struct Memory_heap
{
    qword heap_Size;   /*!< The memory pool size that we own */
    qword heap_in_use; /*!< The memory pool size that is being used */
} Memory_heap;

/*!
 * @brief Initializes the memory heap.
 * @param heap Pointer to the memory heap object to initialize
 * @param __initial_size Initial size of the memory pool
 * @return 0 if successful, -1 if an error occurred
 */
int heap_init(Memory_heap **heap, qword __initial_size);

/*!
 * @brief Increases the size of the memory pool in the heap.
 * @param heap Pointer to the memory heap object
 * @param __inc_by Additional size to increase the memory pool by
 * @return 0 if successful, -1 if an error occurred
 */
int heap_increase_pool(Memory_heap **heap, qword __inc_by);

/*!
 * @brief Requests memory from the memory heap.
 * @param heap Pointer to the memory heap object
 * @param __req_size Requested size of memory
 * @param __inc_not_avai Flag indicating whether to increase the pool if memory is not available
 * @return 0 if successful, -1 if an error occurred
 */
int heap_request_memory(Memory_heap **heap, qword __req_size, bool __inc_not_avai);

/*!
 * @brief Frees the memory allocated for the memory heap.
 * @param heap Pointer to the memory heap object to free
 */
void free_heap(Memory_heap *heap);

#endif