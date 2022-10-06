#include "cpu.h"



#define CPU_CHECK(cond, ret)                                                    \
do                                                                              \
{                                                                               \
    bool tmpcond_ = (cond);                                                     \
    assert(tmpcond_);                                                           \
    if (!(tmpcond_))                                                            \
    {                                                                           \
        return ret;                                                             \
    }                                                                           \
}                                                                               \
while (0)



static long int cpuFileSize(const char *file);


static void cpuDump(cpu_t *cpu);



static const size_t stackInitSize = 100;



enum CPU_CODES cpuCtor(cpu_t *cpu)
{
    stackCtor(&(cpu->stack), stackInitSize) ASSERTED;
    
    return CPU_SUCCESS;
}


enum CPU_CODES cpuLoad(cpu_t *cpu, const char *codeFile)
{
    long int codeSize = cpuFileSize(codeFile);

    cpu->code = calloc((size_t) codeSize, 1);
    CPU_CHECK(NULL != cpu->code, CPU_ERROR);

    FILE *exeFile = fopen(codeFile, "rb");
    CPU_CHECK(NULL != exeFile, CPU_ERROR);

    signature_t sign = {0};
    size_t signReadStat = fread(&sign, 1, sizeof (signature_t), exeFile);
    CPU_CHECK(sizeof (signature_t) == (size_t) signReadStat, CPU_ERROR);
    CPU_CHECK(signCheck(&sign), CPU_ERROR);

    size_t codeReadStat = fread(cpu->code, 1, (size_t) codeSize - sizeof (signature_t), exeFile);
    CPU_CHECK((size_t) codeReadStat == codeSize - sizeof (signature_t), CPU_ERROR);

    cpu->codeSize = codeSize / sizeof (cpuInstruction_t);

    return CPU_SUCCESS;
}


enum CPU_CODES cpuExec(cpu_t *cpu)
{
    CPU_CHECK(NULL != cpu, CPU_ERROR);
    CPU_CHECK(NULL != cpu->code, CPU_ERROR);

    for (cpu->pc = 0; cpu->pc < cpu->codeSize; cpu->pc++)
    {
        switch (cpu->code[cpu->pc].opcode)
        {
            case CMD_HLT:
                return CPU_SUCCESS;
                break;

            case CMD_PUSH:
                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, cpu->code[cpu->pc].data), CPU_ERROR);
                break;

            case CMD_ADD:
            {
                cpuData_t a = NAN;
                cpuData_t b = NAN;
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a), CPU_ERROR);
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b), CPU_ERROR);
                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, a + b), CPU_ERROR);
            }

            case CMD_OUT:
            {
                cpuData_t num = NAN;
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &num), CPU_ERROR);
                printf("%lf\n", num);
            }

            default:
                cpuDump(cpu);
                return CPU_ERROR;
        }
    }


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


static void cpuDump(cpu_t *cpu)
{
    CPU_CHECK(NULL != cpu, ;);
}

