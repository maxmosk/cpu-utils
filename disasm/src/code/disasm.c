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



static long int dasmFileSize(const char *file);



enum DASM_CODES disasmCtor(disasm_t *dasm)
{
    DASM_CHECK(NULL != dasm, DASM_ERROR);

    return DASM_SUCCESS;
}


enum DASM_CODES disasmLoad(disasm_t *dasm, const char *codeFile)
{
    long int codeSize = dasmFileSize(codeFile);

    dasm->code = calloc((size_t) codeSize, 1);
    DASM_CHECK(NULL != dasm->code, DASM_ALLOCERR);

    FILE *exeFile = fopen(codeFile, "rb");
    DASM_CHECK(NULL != exeFile, DASM_FILEERR);

    signature_t sign = {0};
    size_t signReadStat = fread(&sign, 1, sizeof (signature_t), exeFile);
    DASM_CHECK(sizeof (signature_t) == (size_t) signReadStat, DASM_FILEERR);
    DASM_CHECK(signCheck(&sign), DASM_SIGNERR);

    size_t codeReadStat = fread(dasm->code, 1, (size_t) codeSize - sizeof (signature_t), exeFile);
    DASM_CHECK((size_t) codeReadStat == codeSize - sizeof (signature_t), DASM_FILEERR);
    DASM_CHECK(0 == fclose(exeFile), DASM_FILEERR);

    dasm->codeSize = codeSize / sizeof (cpuInstruction_t);

    return DASM_SUCCESS;
}


enum DASM_CODES disasmWrite(disasm_t *dasm, FILE *file)
{
    DASM_CHECK(NULL != dasm, DASM_ERROR);
    DASM_CHECK(NULL != dasm->code, DASM_ERROR);

    for (long int i = 0; i < dasm->codeSize; i++)
    {
        fprintf(file, "  %02X %016lX | ", dasm->code[i].opcode, *(uint64_t *) &dasm->code[i].data);
        switch (dasm->code[i].opcode)
        {
            case CMD_HLT:
                fprintf(file, "hlt");
                break;

            case CMD_PUSH:
                fprintf(file, "push %lg", dasm->code[i].data);
                break;

            case CMD_ADD:
                fprintf(file, "add");
                break;

            case CMD_SUB:
                fprintf(file, "sub");
                break;

            case CMD_OUT:
                fprintf(file, "out");
                break;

            case CMD_IN:
                fprintf(file, "in");
                break;

            case CMD_MUL:
                fprintf(file, "mul");
                break;

            case CMD_DIV:
                fprintf(file, "div");
                break;

            case CMD_DUMP:
                fprintf(file, "dump");
                break;

            case CMD_DUP:
                fprintf(file, "dup");
                break;

            default:
                fprintf(file, "<<< INVALID OPCODE >>>");
        }
        fprintf(file, "\n");
    }


    return DASM_SUCCESS;
}


enum DASM_CODES disasmDtor(disasm_t *dasm)
{
    DASM_CHECK(NULL != dasm, DASM_ERROR);

    free(dasm->code);
    dasm->code = NULL;
    dasm->codeSize = LONG_MAX;

    return DASM_SUCCESS;
}


static long int dasmFileSize(const char *file)
{
    struct stat buf = {0};

    DASM_CHECK(-1 != stat(file, &buf), -1);

    return buf.st_size;
}

