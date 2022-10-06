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

    return ASM_SUCCESS;
}


enum ASM_CODES asmBuild(asm_t *thisAsm, const char *execFile)
{
    ASM_CHECK(NULL != thisAsm, ASM_ERROR);
    ASM_CHECK(NULL != execFile, ASM_ERROR);

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

