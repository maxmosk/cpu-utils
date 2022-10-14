#ifndef OPCODES_H_INCLUDED__
#define OPCODES_H_INCLUDED__



#include <stdint.h>
#include <math.h>



#define N_REGS 4
#define ACCURACY 0.0001
#define ISJMP(cmd) (((cmd) == CMD_JMP) || ((cmd) == CMD_JE) ||          \
        ((cmd) == CMD_JNE) || ((cmd) == CMD_JA) || ((cmd) == CMD_JAE)    \
        || ((cmd) == CMD_JB) || ((cmd) == CMD_JBE) || ((cmd) == CMD_CALL))


#define DEFCMD(NAME, CODE, ...) CMD_##NAME = CODE,
enum CPU_OPCODES
{
#include "commands.h"
    CMD_MAX
};
#undef DEFCMD



typedef double cpuNumber_t;

typedef size_t cpuAddress_t;

typedef long long cpuInteger_t;


typedef union
{
    cpuNumber_t number;
    cpuAddress_t address;
    cpuInteger_t integer;
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



#endif /* OPCODES_H_INCLUDED__ */

