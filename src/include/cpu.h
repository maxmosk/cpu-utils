#ifndef __CPU_H_INCLUDED__
#define __CPU_H_INCLUDED__



#include "stack.h"
#include "text.h"



typedef struct
{
    stack_t stack;
    text_t code;
} cpu_t;



#endif /* __CPU_H_INCLUDED__ */

