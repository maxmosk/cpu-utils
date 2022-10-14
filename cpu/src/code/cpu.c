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
    (status = stackCtor(&(cpu->callstack), stackInitSize)) ASSERTED;
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


enum CPU_CODES cpuExec(cpu_t *cpu)
{
    CPU_CHECK(NULL != cpu, CPU_ERROR);
    CPU_CHECK(NULL != cpu->code, CPU_ERROR);

    for (cpu->pc = 0; cpu->pc < cpu->codeSize; cpu->pc++)
    {
        switch (cpu->code[cpu->pc].opcode.cmd)
        {

#define DEFCMD(cmd_name, cmd_n, cmd_n_args, cmd_code)\
    case cmd_n:                                       \
        cmd_code;                                      \
        break;

#include "commands.h"

#undef DEFCMD

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
    (status = stackDtor(&(cpu->callstack))) ASSERTED;
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
    for (cpuAddress_t pc = 0; pc < cpu->codeSize; pc++)
    {
        LOGPRINTF("  %03X (%c%c%c r-%d #%02d) %016lX",
                    *(uint16_t *) &cpu->code[pc].opcode,
                    cpu->code[pc].opcode.mem ? 'M' : 'm',
                    cpu->code[pc].opcode.imm ? 'I' : 'i',
                    cpu->code[pc].opcode.reg ? 'R' : 'r',
                    cpu->code[pc].opcode.regNo,
                    cpu->code[pc].opcode.cmd,
                    *(uint64_t *) &cpu->code[pc].data
                 );
    }
    LOGPRINTF("\n");

    for (cpuAddress_t pc = 0; pc <= cpu->pc; pc++)
    {
        LOGPRINTF("  %33c", (pc == cpu->pc) ? '^' : ' ');
    }
    LOGPRINTF(" pc = %zu\n", cpu->pc);

    for (int i = 0; i < N_REGS; i++)
    {
        LOGPRINTF(" r%cx = %lg\n", i + 'a', cpu->reg[i]);
    }

    CLOSELOG();   
}

