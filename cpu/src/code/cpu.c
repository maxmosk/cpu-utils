#include "cpu.h"



static long int cpuFileSize(const char *file);



static const size_t stackInitSize = 100;



enum CPU_CODES cpuCtor(cpu_t *cpu)
{
    stackCtor(&(cpu->stack), stackInitSize) ASSERTED;
    
    return CPU_SUCCESS;
}


enum CPU_CODES cpuLoad(cpu_t *cpu, const char *codeFile)
{
    long int codeSize = cpuFileSize(codeFile);
    return CPU_SUCCESS;
}


enum CPU_CODES cpuExec(cpu_t *cpu)
{
    return CPU_SUCCESS;
}


enum CPU_CODES cpuDtor(cpu_t *cpu)
{
    stackDtor(&(cpu->stack)) ASSERTED;
    
    return CPU_SUCCESS;
}


static long int cpuFileSize(const char *file)
{
    struct stat buf = {0};
    
    if (-1 == stat(file, &buf))
    {
        perror(strerror(errno));
        return -1;
    }

    return buf.st_size;
}

