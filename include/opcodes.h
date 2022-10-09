#ifndef __OPCODES_H_INCLUDED__
#define __OPCODES_H_INCLUDED__



#include <stdint.h>
#include <math.h>



#define IS_VALID isfinite
#define N_REGS 4



enum CPU_OPCODES
{
    CMD_HLT  = 0,
    CMD_PUSH = 1,
    CMD_IN   = 2,
    CMD_OUT  = 3,
    CMD_ADD  = 4,
    CMD_SUB  = 5,
    CMD_MUL  = 6,
    CMD_DIV  = 7,
    CMD_DUP  = 8,
    CMD_JMP  = 9,

    CMD_DUMP = 31
};



typedef double cpuNumber_t;


typedef union
{
    cpuNumber_t number;
    size_t address;
} cpuData_t;


typedef struct
{
    unsigned cmd : 5;
    unsigned reg : 1;
    unsigned imm : 1;
    unsigned mem : 1;

    unsigned regN0 : 2;
} cpuOpcode_t;


typedef struct
{
    cpuOpcode_t opcode;
    cpuData_t data;
} cpuInstruction_t;



#endif /* __OPCODES_H_INCLUDED__ */

