#include "disasm.h"



#define DASM_CHECK(cond, ret)                                                   \
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



enum DASM_CODES disasmCtor(disasm_t *dasm)
{
    DASM_CHECK(NULL != dasm, DASM_ERROR);

    return DASM_SUCCESS;
}


enum DASM_CODES disasmLoad(disasm_t *dasm, const char *codeFile)
{
}


enum DASM_CODES disasmWrite(disasm_t *dasm, FILE *file)
{
}


enum DASM_CODES disasmDtor(disasm_t *dasm)
{
    DASM_CHECK(NULL != dasm, DASM_ERROR);

    free(dasm->code);
    dasm->code = NULL;
    dasm->codeSize = LONG_MAX;

    return DASM_SUCCESS;
}

