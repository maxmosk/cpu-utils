#include "asm.h"



#define ASM_CHECK(cond, ret)                                                    \
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



enum ASM_CODES asmMakeInstr(cpuInstruction_t *dest, const char *cmd, cpuData_t arg);


enum ASM_CODES asmCtor(asm_t *thisAsm)
{
    ASM_CHECK(NULL != thisAsm, ASM_ERROR);

    return ASM_SUCCESS;
}


enum ASM_CODES asmLoad(asm_t *thisAsm, const char *sourceFile)
{
    ASM_CHECK(NULL != thisAsm, ASM_ERROR);
    ASM_CHECK(NULL != sourceFile, ASM_ERROR);

    ASM_CHECK(SUCCESS == txtRead(&thisAsm->source, sourceFile), ASM_ERROR);
    ASM_CHECK(NULL != (thisAsm->code = calloc(thisAsm->source.quan_lines, sizeof *thisAsm->code)), ASM_ERROR);


    return ASM_SUCCESS;
}


enum ASM_CODES asmBuild(asm_t *thisAsm, const char *execFile)
{
    ASM_CHECK(NULL != thisAsm, ASM_ERROR);
    ASM_CHECK(NULL != execFile, ASM_ERROR);


    for (size_t i = 0; i < thisAsm->source.quan_lines; i++)
    {
        char *curLine = txtGetStr(&thisAsm->source.lines[i]);

        char cmdBuf[64] = "";
        char format[64] = "";
        sprintf(format, "%%%zus %%n", sizeof (cmdBuf) - 1);

        cpuData_t value = NAN;
        int readChar = -1;
        if (0 == sscanf(curLine, format, cmdBuf, &readChar))
        {
            continue;
        }
        else
        {
            ASM_CHECK(ASM_SUCCESS == asmMakeInstr(thisAsm->code + thisAsm->codeSize, cmdBuf, value),
                    ASM_ERROR);
            thisAsm->codeSize++;
        }
    }
    

    FILE *exec = NULL;
    ASM_CHECK(NULL != (exec = fopen(execFile, "wb")), ASM_ERROR);

    signature_t sign = {CPU_EXE_FORMAT, CPU_EXE_VERSION};
    ASM_CHECK(1 == fwrite(&sign, sizeof sign, 1, exec), (fclose(exec), ASM_ERROR));
    ASM_CHECK(thisAsm->codeSize == fwrite(thisAsm->code, sizeof *thisAsm->code, thisAsm->codeSize, exec),
            (fclose(exec), ASM_ERROR));
    ASM_CHECK(0 == fclose(exec), ASM_ERROR);


    return ASM_SUCCESS;
}


enum ASM_CODES asmDtor(asm_t *thisAsm)
{
    ASM_CHECK(NULL != thisAsm, ASM_ERROR);

    free(thisAsm->code);
    txtFree(&thisAsm->source);
    thisAsm->codeSize = SIZE_MAX;


    return ASM_SUCCESS;
}


enum ASM_CODES asmMakeInstr(cpuInstruction_t *dest, const char *cmd, cpuData_t arg)
{
    return ASM_SUCCESS;
}

