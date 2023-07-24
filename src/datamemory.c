#include "../includes/Memory/datamemory.h"

void init_mem(Memory **memory, qword __initial_size)
{
    (*memory) = (Memory *)malloc(sizeof(Memory));
    if (*memory == NULL)
    {
        KSET_ERR(KMEM_INIT_FAILED, "Data memory could not be initialized");
        return;
    }
    (*memory)->memArray = (byte *)malloc(sizeof(byte) * __initial_size);
    if ((*memory)->memArray == NULL)
    {
        KSET_ERR(KMEM_INIT_FAILED, "Data memory could not be initialized");
        return;
    }
    (*memory)->size = __initial_size;
}

void mem_size_increase(Memory **memory, qword __additional_size)
{
    (*memory)->size += __additional_size;
    (*memory)->memArray = (byte *)realloc((*memory)->memArray, (*memory)->size);
    if ((*memory)->memArray == NULL)
    {
        KSET_ERR(KMEM_MEMORY_SIZE_INCREASE_FAILED, "Could not increase the data memory size");
        return;
    }
}

void mem_write8(Memory *memory, qword address, qword value)
{
    if (address > memory->size || address < 0)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing memory that is out of bounds");
        return;
    }
    memory->memArray[address] = value & 255;
}

void mem_write16(Memory *memory, qword address, qword value)
{
    if (address > memory->size || address < 0 || (address + 2) > memory->size)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing memory that is out of bounds");
        return;
    }
    memory->memArray[address] = (value >> 8) & 255;
    memory->memArray[address + 1] = (value & 255);
}

void mem_write32(Memory *memory, qword address, qword value)
{
    if (address > memory->size || address < 0 || (address + 4) > memory->size)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing memory that is out of bounds");
        return;
    }
    memory->memArray[address] = (value >> 24) & 255;
    memory->memArray[address + 1] = (value >> 16) & 255;
    memory->memArray[address + 2] = (value >> 8) & 255;
    memory->memArray[address + 3] = value & 255;
}

void mem_write64(Memory *memory, qword address, qword value)
{
    if (address > memory->size || address < 0 || (address + 7) > memory->size)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing memory that is out of bounds");
        return;
    }
    memory->memArray[address] = (value >> 56) & 255;
    memory->memArray[address + 1] = (value >> 48) & 255;
    memory->memArray[address + 2] = (value >> 40) & 255;
    memory->memArray[address + 3] = (value >> 32) & 255;
    memory->memArray[address + 4] = (value >> 24) & 255;
    memory->memArray[address + 5] = (value >> 16) & 255;
    memory->memArray[address + 6] = (value >> 8) & 255;
    memory->memArray[address + 7] = value & 255;
}

qword mem_read8(Memory *memory, qword address)
{
    if (address > memory->size || address < 0)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing memory that is out of bounds");
        return 0;
    }
    return (memory->memArray[address] | 0UL);
}

qword mem_read16(Memory *memory, qword address)
{
    if (address > memory->size || address < 0 || (address + 2) > memory->size)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing memory that is out of bounds");
        return 0;
    }
    return (((memory->memArray[address] << 8) | memory->memArray[address + 1]) | 0UL);
}

qword mem_read32(Memory *memory, qword address)
{
    if (address > memory->size || address < 0 || (address + 4) > memory->size)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing memory that is out of bounds");
        return 0;
    }
    qword res = 0;
    res = memory->memArray[address];
    res = (res << 8) | memory->memArray[address + 1];
    res = (res << 8) | memory->memArray[address + 2];
    res = (res << 8) | memory->memArray[address + 3];
    return res | 0UL;
}

qword mem_read64(Memory *memory, qword address)
{
    if (address > memory->size || address < 0 || (address + 8) > memory->size)
    {
        KSET_ERR(KMEM_ACCESS_OUT_OF_BOUNDS, "Accessing memory that is out of bounds");
        return 0;
    }
    qword res = 0;
    res = memory->memArray[address];
    res = (res << 8) | memory->memArray[address + 1];
    res = (res << 8) | memory->memArray[address + 2];
    res = (res << 8) | memory->memArray[address + 3];
    res = (res << 8) | memory->memArray[address + 4];
    res = (res << 8) | memory->memArray[address + 5];
    res = (res << 8) | memory->memArray[address + 6];
    res = (res << 8) | memory->memArray[address + 7];
    return res;
}

void free_mem(Memory *memory)
{
    free(memory->memArray);
    free(memory);
}