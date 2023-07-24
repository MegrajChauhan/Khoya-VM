#include "../includes/CPU/kCPU.h"
#include <math.h>

void KSET_ERR(int err_code, char *err_msg)
{
    SET_ERR(cpu->err, err_code, err_msg);
}

void checkStatus()
{
    if (KGET_STATUS)
    {
        KPRINT_ERR
        stopCPU();
        exit(-1);
    }
}

void startCPU()
{
    cpu = (CPU *)malloc(sizeof(CPU));
    if (cpu == NULL)
    {
        fprintf(stderr, "Internal Error: Unable to start the CPU\n");
        exit(-1);
    }
    cpu->err = (Error *)malloc(sizeof(Error));
    if (cpu->err == NULL)
    {
        fprintf(stderr, "Internal Error: Unable to start the CPU\n");
        free(cpu);
        exit(-1);
    }
    int ret = heap_init(&cpu->heap, 1024 + 512 + 1024 + 128);
    if (ret != 0)
    {
        fprintf(stderr, "Internal Error: Could not initialize memory\n");
        stopCPU();
        exit(-1);
    }
    Imem_init(&cpu->memory, 1024);
    checkStatus();
    stack_init(&cpu->stack, 512);
    checkStatus();
    init_mem(&cpu->data_memory, 1024);
    checkStatus();
    cpu->running = false;
}

void loadInstructions(qword *instructions, qword size)
{
    if (cpu->memory->size < size)
    {
        int ret = heap_request_memory(&cpu->heap, cpu->memory->size - size, true);
        if (ret != 0)
        {
            fprintf(stderr, "Could not allocate memory for instructions\n");
            stopCPU();
            exit(-1);
        }
        Imem_increase_size(&cpu->memory, cpu->memory->size - size);
        checkStatus();
    }
    cpu->_number_of_instruction = size;
    qword addr = 0;
    while (size > 0)
    {
        Imem_write(cpu->memory, addr, *instructions);
        instructions++;
        addr++;
        size--;
        if (KGET_STATUS) // this is better here instead of a function call every time it loops
        {
            KPRINT_ERR
            stopCPU();
            exit(-1);
        }
    }
}

void loadData(byte *data, qword size)
{
    if (cpu->data_memory->size < size)
    {
        int ret = heap_request_memory(&cpu->heap, cpu->memory->size - size, true);
        if (ret != 0)
        {
            fprintf(stderr, "Could not allocate memory for instructions\n");
            stopCPU();
            exit(-1);
        }
        mem_size_increase(&cpu->data_memory, size - cpu->data_memory->size);
        checkStatus();
    }
    qword addr = 0; // the address for the data starts at 0 so the assembler must put this into consideration
    while (size > 0)
    {
        mem_write8(cpu->data_memory, addr, *data);
        data++;
        addr++;
        size--;
        if (KGET_STATUS)
        {
            KPRINT_ERR
            stopCPU();
            exit(-1);
        }
    }
}

void stopCPU()
{
    free_error(cpu->err);
    if (cpu->heap != NULL)
        free_heap(cpu->heap);
    if (cpu->memory != NULL)
        free_Imem(cpu->memory);
    if (cpu->stack != NULL)
        free_stack(cpu->stack);
    if (cpu->data_memory != NULL)
        free_mem(cpu->data_memory);
    free(cpu);
}

void inc()
{
    // xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // first 8 bits for the instruction itself
    // inc instruction works only on registers so the last four bits must be the register address
    cpu->k_registers[__current_full_instruction & 15]++;
    cpu->_number_of_instruction--;
    cpu->memory->ip++;
    cpu->_instructions_executed++;
}

void dec()
{
    // xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // first 8 bits for the instruction itself
    // inc instruction works only on registers so the last four bits must be the register address
    cpu->k_registers[__current_full_instruction & 15]--;
    cpu->_number_of_instruction--;
    cpu->memory->ip++;
    cpu->_instructions_executed++;
}

void mov()
{
    // xxxxxxxx xxxxxxx x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // first 8 bits for the instruction itself
    // 1 bit for the instruction format
    // move only takes registers and immediates as operands
    // 0 means register-register, 1 means register-imm
    switch (((__current_full_instruction >> 48) & 1))
    {
    case 0:
    {
        // so we move a value from a source register to destination register
        cpu->k_registers[(__current_full_instruction >> 4) & 15] = cpu->k_registers[__current_full_instruction & 15];
        break;
    }
    case 1:
    {
        // we need the immediate value now. The immediate is 5 bytes long
        cpu->k_registers[__current_full_instruction & 15] = (__current_full_instruction >> 8) & 0xFFFFFFFFFF;
    }
    }
    cpu->_number_of_instruction--;
    cpu->memory->ip++;
    cpu->_instructions_executed++;
}

void cmp()
{
    // xxxxxxxx xxxxxxx x xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    // first 8 bits for the instruction itself
    // comparison also does the same as mov but instead it does comparisons and not move
    qword val1, val2;
    switch ((__current_full_instruction >> 48) & 1)
    {
    case 0:
    {
        val1 = cpu->k_registers[(__current_full_instruction >> 4) & 15];
        val2 = cpu->k_registers[__current_full_instruction & 15];
        break;
    }
    case 1:
    {
        // we need the immediate value now. The immediate is 5 bytes long
        val1 = cpu->k_registers[__current_full_instruction & 15];
        val2 = (__current_full_instruction >> 8) & 0xFFFFFFFFFF;
    }
    }
    // now we compare the values we got
    // zero flag cannot be set here but other flags can be
    cpu->k_flags[EQUAL] = (val1 == val2) ? 1 : 0;
    cpu->k_flags[GREATER] = (val1 > val2) ? 1 : 0;
    cpu->k_flags[GREATER_EQ] = (val1 >= val2) ? 1 : 0;
    cpu->_number_of_instruction--;
    cpu->memory->ip++;
    cpu->_instructions_executed++;
}

void jmp()
{
    // this only takes address which should be in the last 4 bytes. The address should be of the instruction memory for no errors
    dword address = __current_full_instruction & 0xFFFFFFFF;
    dword __current_address = cpu->_instructions_executed;
    if (address > cpu->memory->size)
    {
        cpu->_number_of_instruction--;
        cpu->memory->ip++;
        cpu->_instructions_executed++;
        return; // don't prank the cpu, it can skip it
    }
    cpu->memory->ip = &cpu->memory->instruction_array[address];
    cpu->_number_of_instruction += __current_address - address;
    cpu->_instructions_executed = address;
}

void jne()
{
    if (cpu->k_flags[EQUAL] == 0)
    {
        jmp(__current_full_instruction);
        return;
    }
    cpu->_number_of_instruction--;
    cpu->memory->ip++;
    cpu->_instructions_executed++;
}

void execute()
{
    if (cpu->_number_of_instruction == 0)
        return;          // there are no instructions to execute
    cpu->running = true; // start the execution
    cpu->_instructions_executed = 0;
    while (cpu->running != false && cpu->_number_of_instruction != 0)
    {
        __current_full_instruction = *(cpu->memory->ip);          // get the instruction [imem_read was an experimental function but is left if it comes of use in future]
        __current_instruction = __current_full_instruction >> 56; // get the instruction byte which is the first byte
        switch (__current_instruction)
        {
        case NOP:
            break;
        case HALT:
            cpu->running = false;
            break;
        case INC:
            inc();
            break;
        case DEC:
            dec();
            break;
        case MOV:
            mov();
            break;
        case JNE:
            jne();
            break;
        case CMP:
            cmp();
            break;
        default:
            fprintf(stderr, "Unknown Instruction\n");
            cpu->k_registers[ka] = 1;
            break;
        }
        checkStatus();
    }
    stopCPU();
}
