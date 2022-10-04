#ifndef __CPU_H_INCLUDED__
#define __CPU_H_INCLUDED__



#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "stack.h"
#include "opcodes.h"
#include "signature.h"



#define CPU_ASSERTED ASSERTED



typedef struct
{
    stack_t stack;
    cpuInstruction_t *code;
    size_t pc;
    long int codeSize;
} cpu_t;



enum CPU_CODES
{
    CPU_SUCCESS = 0,
    CPU_ERROR   = 1
};



enum CPU_CODES cpuCtor(cpu_t *cpu);


enum CPU_CODES cpuLoad(cpu_t *cpu, const char *codeFile);


enum CPU_CODES cpuExec(cpu_t *cpu);


enum CPU_CODES cpuDtor(cpu_t *cpu);



#endif /* __CPU_H_INCLUDED__ */

