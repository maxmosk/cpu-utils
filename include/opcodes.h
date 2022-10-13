#ifndef __OPCODES_H_INCLUDED__
#define __OPCODES_H_INCLUDED__



#include <stdint.h>
#include <math.h>



#define IS_VALID isfinite
#define N_REGS 4


#define DEFCMD(NAME, CODE, ...) CMD_##NAME = CODE,
enum CPU_OPCODES
{
#include "commands.h"
    CMD_MAX
};
#undef DEFCMD



typedef double cpuNumber_t;


typedef union
{
    cpuNumber_t number;
    long long address;
} cpuData_t;


typedef struct
{
    unsigned cmd : 5;
    unsigned reg : 1;
    unsigned imm : 1;
    unsigned mem : 1;

    unsigned regNo : 2;
} cpuOpcode_t;


typedef struct
{
    cpuOpcode_t opcode;
    cpuData_t data;
} cpuInstruction_t;



#endif /* __OPCODES_H_INCLUDED__ */

