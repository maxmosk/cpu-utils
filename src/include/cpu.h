#ifndef __CPU_H_INCLUDED__
#define __CPU_H_INCLUDED__



#include "stack.h"
#include "text.h"



typedef struct
{
    stack_t stack;
    text_t code;
} cpu_t;



enum CPU_CODES
{
    CPU_SUCCESS = 0,
    CPU_ERROR   = 1
};



enum CPU_CODES cpuCtor(cpu_t *cpu);



#endif /* __CPU_H_INCLUDED__ */

