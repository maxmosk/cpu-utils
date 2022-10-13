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



static long cpuFileSize(const char *file);


static void cpuDump(const cpu_t *cpu);



static const size_t stackInitSize = 100;


static const size_t cpuRAMSize = 100;



enum CPU_CODES cpuCtor(cpu_t *cpu)
{
    enum STACK_CODES status = STACK_ERROR;
    (status = stackCtor(&(cpu->stack), stackInitSize)) ASSERTED;
    CPU_CHECK(STACK_ERROR != status, CPU_STACKERR);
    
    return CPU_SUCCESS;
}


enum CPU_CODES cpuLoad(cpu_t *cpu, const char *codeFile)
{
    long codeSize = cpuFileSize(codeFile);

    cpu->code = calloc((size_t) codeSize, 1);
    CPU_CHECK(NULL != cpu->code, CPU_ALLOCERR);

    FILE *exeFile = fopen(codeFile, "rb");
    CPU_CHECK(NULL != exeFile, CPU_FILEERR);

    signature_t sign = {0};
    size_t signReadStat = fread(&sign, 1, sizeof (signature_t), exeFile);
    CPU_CHECK(sizeof (signature_t) == (size_t) signReadStat, CPU_FILEERR);
    CPU_CHECK(signCheck(&sign), CPU_SIGNERR);

    size_t codeReadStat = fread(cpu->code, 1, (size_t) codeSize - sizeof (signature_t), exeFile);
    CPU_CHECK((size_t) codeReadStat == (unsigned) codeSize - sizeof (signature_t), CPU_FILEERR);
    CPU_CHECK(0 == fclose(exeFile), CPU_FILEERR);

    cpu->RAM = calloc(cpuRAMSize, sizeof *cpu->RAM);

    cpu->codeSize = codeSize / (signed) sizeof (cpuInstruction_t);


    return CPU_SUCCESS;
}


#define cpu cpu
#define DEFCMD(CMD, N, NARGS, CODE, ...)    \
case N:                                      \
    CODE                                      \
    break;

enum CPU_CODES cpuExec(cpu_t *cpu)
{
    CPU_CHECK(NULL != cpu, CPU_ERROR);
    CPU_CHECK(NULL != cpu->code, CPU_ERROR);

    for (cpu->pc = 0; cpu->pc < cpu->codeSize; cpu->pc++)
    {
        switch (cpu->code[cpu->pc].opcode.cmd)
        {
#include <commands.h>

            default:
                cpuDump(cpu);
                return CPU_WRONGOP;
        }
    }


    return CPU_SUCCESS;
}

#undef cpu
#undef DEFCMD


enum CPU_CODES cpuDtor(cpu_t *cpu)
{
    enum STACK_CODES status = STACK_ERROR;
    (status = stackDtor(&(cpu->stack))) ASSERTED;
    CPU_CHECK(STACK_ERROR != status, CPU_STACKERR);

    free(cpu->code);
    cpu->codeSize = -1;
    
    free(cpu->RAM);
    
    return CPU_SUCCESS;
}


static long cpuFileSize(const char *file)
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

    LOGPRINTF("cpu_t [%p] dump\n", (const void *) cpu);
    for (long long i = 0; i < cpu->codeSize; i++)
    {
        LOGPRINTF("  %03X (%c%c%c r-%d #%02d) %016lX",
                    *(uint16_t *) &cpu->code[i].opcode,
                    cpu->code[i].opcode.mem ? 'M' : 'm',
                    cpu->code[i].opcode.imm ? 'I' : 'i',
                    cpu->code[i].opcode.reg ? 'R' : 'r',
                    cpu->code[i].opcode.regNo,
                    cpu->code[i].opcode.cmd,
                    *(uint64_t *) &cpu->code[i].data
                 );
    }
    LOGPRINTF("\n");

    for (long long i = 0; i <= cpu->pc; i++)
    {
        LOGPRINTF("  %33c", (i == cpu->pc) ? '^' : ' ');
    }
    LOGPRINTF(" pc = %lld \n", cpu->pc);

    CLOSELOG();   
}

