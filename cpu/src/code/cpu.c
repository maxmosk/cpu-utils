#include "cpu.h"



#define CPU_CHECK(cond, ret)                                                    \
do                                                                              \
{                                                                               \
    if (!(cond))                                                                \
    {                                                                           \
        return (ret);                                                           \
    }                                                                           \
}                                                                               \
while (0)



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

    cpu->code = calloc(codeSize, 1);
    CPU_CHECK(NULL != cpu->code, CPU_ERROR);

    FILE *exeFile = fopen(codeFile, "rb");
    CPU_CHECK(NULL != exeFile, CPU_ERROR);

    size_t readed = fread(cpu->code, 1, codeSize, exeFile);
    CPU_CHECK(readed == codeSize, CPU_ERROR);

    cpu->codeSize = codeSize;

    return CPU_SUCCESS;
}


enum CPU_CODES cpuExec(cpu_t *cpu)
{
    return CPU_SUCCESS;
}


enum CPU_CODES cpuDtor(cpu_t *cpu)
{
    stackDtor(&(cpu->stack)) ASSERTED;

    free(cpu->code);
    cpu->codeSize = -1;
    
    return CPU_SUCCESS;
}


static long int cpuFileSize(const char *file)
{
    struct stat buf = {0};
    
    CPU_CHECK(-1 != stat(file, &buf), -1);

    return buf.st_size;
}

