#include "../includes/Memory/instructionMemory.h"

void Imem_init(Instruction_memory **memory, qword size)
{
    *memory = (Instruction_memory *)malloc(sizeof(Instruction_memory));
    if (*memory == NULL)
    {
        KSET_ERR(KMEM_INIT_FAILED, "Memory allocation failed");
        return;
    }
    (*memory)->instruction_array = (qword *)malloc(sizeof(qword) * size);
    if ((*memory)->instruction_array == NULL)
    {
        KSET_ERR(KMEM_INIT_FAILED, "Memory allocation failed"); // the same error since this IS the memory
        return;
    }
    (*memory)->ip = &(*memory)->instruction_array[0];
    (*memory)->size = size;
    (*memory)->state = UNDER_FULL;
}

void Imem_increase_size(Instruction_memory **memory, qword __size_to_add)
{
    if (memory == NULL)
    {
        return;
    }
    (*memory)->size += __size_to_add;
    *memory = (Instruction_memory *)realloc(*memory, (*memory)->size);
    if (*memory == NULL)
    {
        KSET_ERR(KMEM_MEMORY_SIZE_INCREASE_FAILED, "Memory size could not be increased");
        return;
    }
    // other fields are as is
}

void Imem_write(Instruction_memory *mem, qword addr, qword value)
{
    if (addr > mem->size || addr < 0)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing out of bounds memory");
        return;
    }
    mem->instruction_array[addr] = value;
}

qword Imem_read(Instruction_memory *mem, qword addr)
{
    if (addr > mem->size || addr < 0)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing out of bounds memory");
        return 0;
    }
    return mem->instruction_array[addr];
}

void free_Imem(Instruction_memory *memory)
{
    if (memory->instruction_array != NULL)
        free(memory->instruction_array);
    free(memory);
}
