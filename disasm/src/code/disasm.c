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



static long dasmFileSize(const char *file);


static enum DASM_CODES dasmPrintArgs(FILE *ostream, const cpuInstruction_t *src);



enum DASM_CODES disasmCtor(disasm_t *dasm)
{
    DASM_CHECK(NULL != dasm, DASM_NULLPTR);

    return DASM_SUCCESS;
}


enum DASM_CODES disasmLoad(disasm_t *dasm, const char *codeFile)
{
    DASM_CHECK(NULL != dasm, DASM_NULLPTR);
    DASM_CHECK(NULL != codeFile, DASM_NULLPTR);

    long codeSize = dasmFileSize(codeFile);

    dasm->code = calloc((size_t) codeSize, 1);
    DASM_CHECK(NULL != dasm->code, DASM_ALLOCERR);

    FILE *exeFile = fopen(codeFile, "rb");
    DASM_CHECK(NULL != exeFile, DASM_FILEERR);

    signature_t sign = {0};
    size_t signReadStat = fread(&sign, 1, sizeof (signature_t), exeFile);
    DASM_CHECK(sizeof (signature_t) == (size_t) signReadStat, DASM_FILEERR);
    DASM_CHECK(signCheck(&sign), DASM_SIGNERR);

    size_t codeReadStat = fread(dasm->code, 1, (size_t) codeSize - sizeof (signature_t), exeFile);
    DASM_CHECK((size_t) codeReadStat == (unsigned) codeSize - sizeof (signature_t), DASM_FILEERR);
    DASM_CHECK(0 == fclose(exeFile), DASM_FILEERR);

    dasm->codeSize = codeSize / (signed) sizeof (cpuInstruction_t);

    return DASM_SUCCESS;
}


enum DASM_CODES disasmWrite(disasm_t *dasm, FILE *file)
{
    DASM_CHECK(NULL != dasm, DASM_NULLPTR);
    DASM_CHECK(NULL != dasm->code, DASM_NULLPTR);

    for (long i = 0; i < dasm->codeSize; i++)
    {
        fprintf(file, "%08lX || %03X %016lX || ", (unsigned long) i, *(uint16_t *) &dasm->code[i].opcode, *(uint64_t *) &dasm->code[i].data);
        switch (dasm->code[i].opcode.cmd)
        {

#define DEFCMD(cmd_name, cmd_n, cmd_n_args, ...)\
    case cmd_n:                                  \
        fprintf(file, "%s", #cmd_name);           \
        if (cmd_n_args != 0)                       \
        {                                           \
            dasmPrintArgs(file, &dasm->code[i]);     \
        }                                             \
        break;

#include "commands.h"

#undef DEFCMD

            default:
                fprintf(file, "<<< INVALID OPCODE >>>");
        }
        fprintf(file, "\n");
    }


    return DASM_SUCCESS;
}


enum DASM_CODES disasmDtor(disasm_t *dasm)
{
    DASM_CHECK(NULL != dasm, DASM_NULLPTR);

    free(dasm->code);
    dasm->code = NULL;
    dasm->codeSize = LONG_MAX;

    return DASM_SUCCESS;
}


static long dasmFileSize(const char *file)
{
    struct stat buf = {0};

    DASM_CHECK(-1 != stat(file, &buf), -1);

    return buf.st_size;
}


static enum DASM_CODES dasmPrintArgs(FILE *ostream, const cpuInstruction_t *src)
{
    DASM_CHECK(NULL != ostream, DASM_NULLPTR);
    DASM_CHECK(NULL != src, DASM_NULLPTR);

    if (ISJMP(src->opcode.cmd))
    {
        fprintf(ostream, " %%%zu", src->data.address);
    }

    else
    {
        if (0 != src->opcode.mem)
        {
            if ((0 != src->opcode.imm) && (0 != src->opcode.reg))
            {
                fprintf(ostream, " [%lld+r%cx]", src->data.integer, src->opcode.regNo + 'a');
            }
            else if (0 != src->opcode.imm)
            {
                fprintf(ostream, " [%zu]", src->data.address);
            }
            else if (0 != src->opcode.reg)
            {
                fprintf(ostream, " [r%cx]", src->opcode.regNo + 'a');
            }
        }
        else
        {
            if (0 != src->opcode.imm)
            {
                fprintf(ostream, " %lg", src->data.number);
            }
            else if (0 != src->opcode.reg)
            {
                fprintf(ostream, " r%cx", src->opcode.regNo + 'a');
            }
        }
    }


    return DASM_SUCCESS;
}

