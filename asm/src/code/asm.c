#include "asm.h"



static size_t MAX_LABELS = 64;



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



static enum ASM_CODES asmMakeInstr(cpuInstruction_t *dest, const char *cmd, const char *arg, const label_t *labels);


static enum ASM_CODES asmRemoveComment(char *str);


static size_t asmFindLabel(const char *label, const label_t *labels);


static enum ASM_CODES asmAddLabel(const char *label, label_t *labels, size_t addr);



enum ASM_CODES asmCtor(asm_t *thisAsm)
{
    ASM_CHECK(NULL != thisAsm, ASM_ERROR);

    thisAsm->labels = calloc(sizeof *thisAsm->labels, MAX_LABELS);
    ASM_CHECK(NULL != thisAsm->labels, ASM_ALLOCERR);
    for (size_t i = 0; i < MAX_LABELS; i++)
    {
        thisAsm->labels[i].name = NULL;
    }

    return ASM_SUCCESS;
}


enum ASM_CODES asmLoad(asm_t *thisAsm, const char *sourceFile)
{
    ASM_CHECK(NULL != thisAsm, ASM_ERROR);
    ASM_CHECK(NULL != sourceFile, ASM_ERROR);

    ASM_CHECK(SUCCESS == txtRead(&thisAsm->source, sourceFile), ASM_TEXTERR);
    ASM_CHECK(NULL != (thisAsm->code = calloc(thisAsm->source.quan_lines, sizeof *thisAsm->code)), ASM_ALLOCERR);


    return ASM_SUCCESS;
}


enum ASM_CODES asmBuild(asm_t *thisAsm, const char *execFile)
{
    ASM_CHECK(NULL != thisAsm, ASM_ERROR);
    ASM_CHECK(NULL != execFile, ASM_ERROR);

    for (int i = 0; i < 2; i++)
    {
        thisAsm->codeSize = 0;

        for (size_t i = 0; i < thisAsm->source.quan_lines; i++)
        {
            char *curLine = txtGetStr(&thisAsm->source.lines[i]);
            ASM_CHECK(ASM_ERROR != asmRemoveComment(curLine), ASM_TEXTERR);

            if (NULL != strchr(curLine, ':'))
            {
                ASM_CHECK(ASM_SUCCESS == asmAddLabel(curLine, thisAsm->labels, thisAsm->codeSize), ASM_ERROR);
                continue;
            }

            char cmdBuf[64] = "";
            char argBuf[64] = "";
            char format[64] = "";
            sprintf(format, "%%%zus %%%zus", sizeof (cmdBuf) - 1, sizeof (argBuf) - 1);


            int status = sscanf(curLine, format, cmdBuf, argBuf);
            if ((status == EOF) || (status == 0))
            {
                continue;
            }
            else
            {
                ASM_CHECK(ASM_SUCCESS == asmMakeInstr(thisAsm->code + thisAsm->codeSize,
                            cmdBuf, argBuf, thisAsm->labels),
                            ASM_INSTRERR
                         );
                thisAsm->codeSize++;
            }
        }
    } 

    FILE *exec = NULL;
    ASM_CHECK(NULL != (exec = fopen(execFile, "wb")), ASM_ERROR);

    signature_t sign = {CPU_EXE_FORMAT, CPU_EXE_VERSION};
    ASM_CHECK(1 == fwrite(&sign, sizeof sign, 1, exec), (fclose(exec), ASM_FILEERR));
    ASM_CHECK(thisAsm->codeSize == fwrite(thisAsm->code, sizeof *thisAsm->code, thisAsm->codeSize, exec),
            (fclose(exec), ASM_FILEERR));
    ASM_CHECK(0 == fclose(exec), ASM_FILEERR);


    return ASM_SUCCESS;
}


enum ASM_CODES asmDtor(asm_t *thisAsm)
{
    ASM_CHECK(NULL != thisAsm, ASM_ERROR);

    free(thisAsm->code);
    free(thisAsm->labels);
    txtFree(&thisAsm->source);
    thisAsm->codeSize = SIZE_MAX;


    return ASM_SUCCESS;
}


static enum ASM_CODES asmMakeInstr(cpuInstruction_t *dest, const char *cmd, const char *arg, const label_t *labels)
{
    ASM_CHECK(NULL != labels, ASM_ERROR);
    ASM_CHECK(NULL != cmd, ASM_ERROR);
    ASM_CHECK(NULL != arg, ASM_ERROR);


    if (0 == strcmp("push", cmd))
    {
        dest->opcode.cmd = CMD_PUSH;

        cpuData_t argVal = {NAN};
        char regChar = '\0';

        if (1 == sscanf(arg, "%lf", &argVal.number))
        {
            dest->data.number = argVal.number;
            dest->opcode.imm = 1;
        }

        else if (1 == sscanf(arg, "r%cx", &regChar))
        {
            dest->opcode.reg = 1;
            ASM_CHECK(regChar - 'a' < N_REGS, ASM_ERROR);
            dest->opcode.regNo = regChar - 'a';
        }

        else if (1 == sscanf(arg, "[%zu]", &argVal.address))
        {
            dest->opcode.imm = 1;
            dest->opcode.mem = 1;
            dest->data.address = argVal.address;
        }

        else
        {
            return ASM_ERROR;
        }
    }

    else if (0 == strcmp("hlt", cmd))
    {
        ASM_CHECK('\0' == *arg, ASM_ERROR);
        dest->opcode.cmd = CMD_HLT;
    }

    else if (0 == strcmp("add", cmd))
    {
        ASM_CHECK('\0' == *arg, ASM_ERROR);
        dest->opcode.cmd = CMD_ADD;
    }

    else if (0 == strcmp("sub", cmd))
    {
        ASM_CHECK('\0' == *arg, ASM_ERROR);
        dest->opcode.cmd = CMD_SUB;
    }

    else if (0 == strcmp("out", cmd))
    {
        ASM_CHECK('\0' == *arg, ASM_ERROR);
        dest->opcode.cmd = CMD_OUT;
    }

    else if (0 == strcmp("in", cmd))
    {
        ASM_CHECK('\0' == *arg, ASM_ERROR);
        dest->opcode.cmd = CMD_IN;
    }

    else if (0 == strcmp("mul", cmd))
    {
        ASM_CHECK('\0' == *arg, ASM_ERROR);
        dest->opcode.cmd = CMD_MUL;
    }

    else if (0 == strcmp("div", cmd))
    {
        ASM_CHECK('\0' == *arg, ASM_ERROR);
        dest->opcode.cmd = CMD_DIV;
    }

    else if (0 == strcmp("dump", cmd))
    {
        ASM_CHECK('\0' == *arg, ASM_ERROR);
        dest->opcode.cmd = CMD_DUMP;
    }

    else if (0 == strcmp("dup", cmd))
    {
        ASM_CHECK('\0' == *arg, ASM_ERROR);
        dest->opcode.cmd = CMD_DUP;
    }

    else if (0 == strcmp("jmp", cmd))
    {
        dest->opcode.cmd = CMD_JMP;

        cpuData_t argVal = {.address = SIZE_MAX};
        if (1 == sscanf(arg, "%zu", &argVal.address)) { ; }
        else if (SIZE_MAX != (argVal.address = asmFindLabel(arg, labels))) { ; }
        else { ; }

        dest->data.address = argVal.address;
    }

    else
    {
        return ASM_ERROR;
    }


    return ASM_SUCCESS;
}


static enum ASM_CODES asmRemoveComment(char *str)
{
    ASM_CHECK(NULL != str, ASM_ERROR);

    char *comStart = strchr(str, ';');
    if (NULL != comStart)
    {
        *comStart = '\0';
    }

    return ASM_SUCCESS;
}


static size_t asmFindLabel(const char *label, const label_t *labels)
{
    ASM_CHECK(NULL != label, SIZE_MAX);
    ASM_CHECK(NULL != labels, SIZE_MAX);

    size_t labLen = strlen(label);
    for (size_t i = 0; (i < MAX_LABELS) && (NULL != labels[i].name); i++)
    {
        if (0 == strncmp(label, labels[i].name, labLen))
        {
            return labels[i].address;
        }
    }

    return SIZE_MAX;
}


static enum ASM_CODES asmAddLabel(const char *label, label_t *labels, size_t addr)
{
    ASM_CHECK(NULL != label, ASM_ERROR);
    ASM_CHECK(NULL != labels, ASM_ERROR);

    if (SIZE_MAX != asmFindLabel(label, labels))
    {
        return ASM_SUCCESS;
    }


    size_t i = 0;
    for (i = 0; (i < MAX_LABELS) && (labels[i].name != NULL); i++)
    {
        continue;
    }
    
    ASM_CHECK(i != MAX_LABELS, ASM_ERROR);

    labels[i].name = label;
#if 0
    printf(">>> Label %16s [%3zu] is %3zu <<<\n", label, addr, i);
#endif
    labels[i].address = addr;


    return ASM_SUCCESS;
}

