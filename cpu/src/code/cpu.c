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


static void cpuDump(const cpu_t *cpu);



static const size_t stackInitSize = 100;



enum CPU_CODES cpuCtor(cpu_t *cpu)
{
    enum STACK_CODES status = STACK_ERROR;
    (status = stackCtor(&(cpu->stack), stackInitSize)) ASSERTED;
    CPU_CHECK(STACK_ERROR != status, CPU_STACKERR);
    
    return CPU_SUCCESS;
}


enum CPU_CODES cpuLoad(cpu_t *cpu, const char *codeFile)
{
    long int codeSize = cpuFileSize(codeFile);

    cpu->code = calloc((size_t) codeSize, 1);
    CPU_CHECK(NULL != cpu->code, CPU_ALLOCERR);

    FILE *exeFile = fopen(codeFile, "rb");
    CPU_CHECK(NULL != exeFile, CPU_FILEERR);

    signature_t sign = {0};
    size_t signReadStat = fread(&sign, 1, sizeof (signature_t), exeFile);
    CPU_CHECK(sizeof (signature_t) == (size_t) signReadStat, CPU_FILEERR);
    CPU_CHECK(signCheck(&sign), CPU_SIGNERR);

    size_t codeReadStat = fread(cpu->code, 1, (size_t) codeSize - sizeof (signature_t), exeFile);
    CPU_CHECK((size_t) codeReadStat == codeSize - sizeof (signature_t), CPU_FILEERR);
    CPU_CHECK(0 == fclose(exeFile), CPU_FILEERR);

    cpu->codeSize = codeSize / sizeof (cpuInstruction_t);

    return CPU_SUCCESS;
}


enum CPU_CODES cpuExec(cpu_t *cpu)
{
    CPU_CHECK(NULL != cpu, CPU_ERROR);
    CPU_CHECK(NULL != cpu->code, CPU_ERROR);

    for (cpu->pc = 0; cpu->pc < cpu->codeSize; cpu->pc++)
    {
        switch (cpu->code[cpu->pc].opcode.cmd)
        {
            case CMD_HLT:
                return CPU_SUCCESS;
                break;

            case CMD_PUSH:
                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, cpu->code[cpu->pc].data.number), CPU_STACKERR);
                break;

            case CMD_ADD:
            {
                cpuData_t a = {NAN};
                cpuData_t b = {NAN};
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a.number), CPU_STACKERR);
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b.number), CPU_STACKERR);
                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, a.number + b.number), CPU_STACKERR);
            }
                break;

            case CMD_SUB:
            {
                cpuData_t a = {NAN};
                cpuData_t b = {NAN};
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a.number), CPU_STACKERR);
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b.number), CPU_STACKERR);
                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, b.number - a.number), CPU_STACKERR);
            }
                break;

            case CMD_OUT:
            {
                cpuData_t num = {NAN};
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &num.number), CPU_STACKERR);
                printf("%lg\n", num.number);
            }
                break;

            case CMD_IN:
            {
                cpuData_t num = {NAN};
                CPU_CHECK(1 == scanf("%lf", &num.number), CPU_ERROR);
                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, num.number), CPU_STACKERR);
            }
                break;

            case CMD_MUL:
            {
                cpuData_t a = {NAN};
                cpuData_t b = {NAN};
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a.number), CPU_STACKERR);
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b.number), CPU_STACKERR);
                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, b.number * a.number), CPU_STACKERR);
            }
                break;

            case CMD_DIV:
            {
                cpuData_t a = {NAN};
                cpuData_t b = {NAN};
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &a.number), CPU_STACKERR);
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &b.number), CPU_STACKERR);
                CPU_CHECK(abs(a.number) < 0.000001, CPU_ZERODIV);
                CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, b.number / a.number), CPU_STACKERR);
            }
                break;

            case CMD_DUMP:
                cpuDump(cpu);
                break;

            case CMD_DUP:
            {
                cpuData_t num = {NAN};
                CPU_CHECK(STACK_ERROR != stackPop(&cpu->stack, &num.number), CPU_STACKERR);

                for (int i = 0; i < 2; i++)
                {
                    CPU_CHECK(STACK_ERROR != stackPush(&cpu->stack, num.number), CPU_STACKERR);
                }
            }
                break;

            case CMD_JMP:
                break;

            default:
                cpuDump(cpu);
                return CPU_WRONGOP;
        }
    }


    return CPU_SUCCESS;
}


enum CPU_CODES cpuDtor(cpu_t *cpu)
{
    enum STACK_CODES status = STACK_ERROR;
    (status = stackDtor(&(cpu->stack))) ASSERTED;
    CPU_CHECK(STACK_ERROR != status, CPU_STACKERR);

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


static void cpuDump(const cpu_t *cpu)
{
    CPU_CHECK(NULL != cpu, ;);
    CPU_CHECK(NULL != cpu->code, ;);

    OPENLOG();

    LOGPRINTF("cpu_t [%p] dump\n", (void *) cpu);
    for (size_t i = 0; i < cpu->codeSize; i++)
    {
        LOGPRINTF("  %02X (%c%c%c #%02d) %016lX",
                    *(uint8_t *) &cpu->code[i].opcode,
                    cpu->code[i].opcode.mem ? 'M' : 'm',
                    cpu->code[i].opcode.imm ? 'I' : 'i',
                    cpu->code[i].opcode.reg ? 'R' : 'r',
                    cpu->code[i].opcode.cmd,
                    *(uint64_t *) &cpu->code[i].data
                 );
    }
    LOGPRINTF("\n");

    for (size_t i = 0; i <= cpu->pc; i++)
    {
        LOGPRINTF("  %29c", (i == cpu->pc) ? '^' : ' ');
    }
    LOGPRINTF("\n");

    CLOSELOG();   
}

