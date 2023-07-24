#ifndef KCPU_H
#define KCPU_H

#include "../utils/errors.h"
#include <stdlib.h>
#include <string.h>
#include "kMemoryHeap.h"
#include "../Memory/instructionMemory.h"
#include "../Memory/stackmem.h"
#include "../Memory/datamemory.h"

enum Registers
{
    // since in an instruction, we will be having 4 bits for a single register, a maximum of 15 possible registers
    ka, // the ka general purpose register - used as the place for return values - can be used for anything
    kb, // the kb general purpose register - can be used for anything
    kc, // the kc general purpose register - can be used for anything - suggested to be used for looping
    kd, // the kd general purpose register - can be used for anything
    k1, // the k1 special purpose register - can be used for anything but should be used for function arguments
    k2, // same as k1
    k3, // same as k1
    k4, // asme as k1
    k5, // asme as k1
    k6, // same as k1
        // 5 more registers left for potential future registers
    REGR_COUNT
};

enum Flags
{
    // the flags register is not addressable and hence, these flags can only be changed indirectly
    ZERO,       // if add or subtract or multiply produced zero
    EQUAL,      // if the compared operands are equal
    GREATER,    // if, among the compared operands, first one is greater [GREATER == 0 means smaller]
    GREATER_EQ, // if, among the compared operands, first one is greater or equal
    NEGATIVE,
    CARRY,
    OVERFLOW,
    FLAG_COUNT
};

enum Instructions
{
    // the instructions
    NOP, // no instruction which is 0
    ADD,
    SUB,
    MUL,
    DIV,
    IADD,
    ISUB,
    IMUL,
    IDIV,
    INC, // add 1 to a given register
    DEC,
    MOV,  // move a value to the destination register
    CMP,  // compare two given values together
    JNE,  // jump if not equal
    HALT, // this can be used to stop the execution permanently
};

typedef struct __attribute__((aligned(256))) CPU
{
    Error *err;
    Memory_heap *heap;
    Instruction_memory *memory;
    Stack *stack;
    Memory *data_memory;
    qword _number_of_instruction; // the number of instructions that was loaded
    qword _instructions_executed;
    bool running;
    qword k_registers[REGR_COUNT];
    byte k_flags[FLAG_COUNT];
} CPU;

static qword __current_full_instruction; // the one read from memory
static int __current_instruction;        // the actual instruction that is going to be executed

static CPU *cpu;

void KSET_ERR(int err_code, char *err_msg);

#define KGET_STATUS cpu->err->__status == 1

#define KPRINT_ERR print_err(cpu->err);

void startCPU();

void loadInstructions(qword *instructions, qword size);
void loadData(byte *data, qword size);

void stopCPU();

void checkStatus();

// the actual execution functions

void execute();

// the functions that actually execute the instructions

void inc();
void dec();
void mov();
void cmp();
void jmp();
void jne();
void je();


#endif