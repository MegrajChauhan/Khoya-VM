#include "../includes/CPU/kMemoryHeap.h"

int heap_init(Memory_heap **heap, qword __initial_size)
{
    *heap = (Memory_heap *)malloc(sizeof(Memory_heap));
    if (*heap == NULL)
        return -1;
    void *ret = sbrk(__initial_size);
    if (ret == (void *)-1)
    {
        free(*heap);
        return -1;
    }
    (*heap)->heap_Size = __initial_size;
    (*heap)->heap_in_use = 0;
    return 0;
}

int heap_increase_pool(Memory_heap **heap, qword __inc_by)
{
    if (((*heap)->heap_Size - (*heap)->heap_in_use) >= __inc_by)
    {
        return 0; // we don't have to increase it if there is already enough memory available
    }
    void *ret = sbrk(__inc_by - (*heap)->heap_Size - (*heap)->heap_in_use);
    if (ret == (void *)-1)
    {
        return -1;
    }
    (*heap)->heap_Size += __inc_by - (*heap)->heap_Size - (*heap)->heap_in_use;
    return 0;
}

int heap_request_memory(Memory_heap **heap, qword __req_size, bool __inc_not_avai)
{
    if (((*heap)->heap_Size - (*heap)->heap_in_use) >= __req_size)
    {
        (*heap)->heap_in_use += __req_size;
        return 0;
    }
    if(__inc_not_avai == true)
    {
        if(heap_increase_pool(heap, __req_size) == 0)
        {
            (*heap)->heap_in_use += __req_size;
            return 0;
        }
        return -1;
    }
    return -1;
}

void free_heap(Memory_heap *heap)
{
    free(heap);
}
