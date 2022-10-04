#ifndef __OPCODES_H_INCLUDED__
#define __OPCODES_H_INCLUDED__



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

    CMD_DUMP = 99
};



typedef double cpuData_t;


typedef uint8_t cpuOpcode_t;


typedef struct
{
    cpuOpcode_t opcode;
    cpuData_t data;
} cpuInstruction_t;



#endif /* __OPCODES_H_INCLUDED__ */

