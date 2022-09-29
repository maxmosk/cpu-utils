#include "cpu.h"


static const size_t stackInitSize = 100;



enum CPU_CODES cpuCtor(cpu_t *cpu)
{
    stackCtor(&(cpu->stack), stackInitSize) ASSERTED;
    
    return CPU_SUCCESS;
}


enum CPU_CODES cpuDtor(cpu_t *cpu)
{
    stackDtor(&(cpu->stack)) ASSERTED;
    
    return CPU_SUCCESS;
}

