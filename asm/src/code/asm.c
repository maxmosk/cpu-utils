#include "asm.h"
#include "asm_helper.h"



static size_t MAX_LABELS = 64;



static enum ASM_CODES asmMakeInstr(cpuInstruction_t *dest, const char *cmd,
                                    const char *arg, const label_t *labels);


static enum ASM_CODES asmRemoveComment(char *str);


static size_t asmFindLabel(const char *label, const label_t *labels);


static enum ASM_CODES asmAddLabel(const char *label, label_t *labels, size_t addr);


static enum ASM_CODES asmSetArg(cpuInstruction_t *dest, const char *arg, const label_t *labels);



enum ASM_CODES asmCtor(asm_t *thisAsm)
{
    ASM_CHECK(NULL != thisAsm, ASM_NULLPTR);

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
    ASM_CHECK(NULL != thisAsm, ASM_NULLPTR);
    ASM_CHECK(NULL != sourceFile, ASM_NULLPTR);

    ASM_CHECK(SUCCESS == txtRead(&thisAsm->source, sourceFile), ASM_TEXTERR);
    ASM_CHECK(NULL != (thisAsm->code = calloc(thisAsm->source.quan_lines, sizeof *thisAsm->code)), ASM_ALLOCERR);


    return ASM_SUCCESS;
}


enum ASM_CODES asmBuild(asm_t *thisAsm, const char *execFile)
{
    ASM_CHECK(NULL != thisAsm, ASM_NULLPTR);
    ASM_CHECK(NULL != execFile, ASM_NULLPTR);

    for (int j = 0; j < 2; j++)
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
    ASM_CHECK(NULL != thisAsm, ASM_NULLPTR);

    free(thisAsm->code);
    free(thisAsm->labels);
    txtFree(&thisAsm->source);
    thisAsm->codeSize = SIZE_MAX;


    return ASM_SUCCESS;
}


static enum ASM_CODES asmMakeInstr(cpuInstruction_t *dest, const char *cmd, const char *arg, const label_t *labels)
{
    ASM_CHECK(NULL != dest, ASM_NULLPTR);
    ASM_CHECK(NULL != cmd, ASM_NULLPTR);
    ASM_CHECK(NULL != arg, ASM_NULLPTR);
    ASM_CHECK(NULL != labels, ASM_NULLPTR);


#define DEFCMD(cmd_name, cmd_n, cmd_n_args, ...)                     \
if (strcasecmp(#cmd_name, cmd) == 0)                                  \
{                                                                      \
    dest->opcode.cmd = cmd_n;                                           \
    if (cmd_n_args != 0)                                                 \
    {                                                                     \
        ASM_CHECK(ASM_SUCCESS == asmSetArg(dest, arg, labels), ASM_ARGERR);\
    }                                                                       \
}                                                                            \
else

#include "commands.h"
 
#undef DEFCMD

    /* else */
    {
        return ASM_ERROR;
    }


    return ASM_SUCCESS;
}


static enum ASM_CODES asmRemoveComment(char *str)
{
    ASM_CHECK(NULL != str, ASM_NULLPTR);

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

    ASM_CHECK(1 == strchr(label, ':'), SIZE_MAX);

    size_t labLen = strlen(label + 1);
    for (size_t i = 0; (i < MAX_LABELS) && (NULL != labels[i].name); i++)
    {
        if (0 == strncmp(label + 1, labels[i].name, labLen))
        {
            return labels[i].address;
        }
    }

    return SIZE_MAX;
}


static enum ASM_CODES asmAddLabel(const char *label, label_t *labels, size_t addr)
{
    ASM_CHECK(NULL != label, ASM_NULLPTR);
    ASM_CHECK(NULL != labels, ASM_NULLPTR);

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
    labels[i].address = addr;


    return ASM_SUCCESS;
}


static enum ASM_CODES asmSetArg(cpuInstruction_t *dest, const char *arg, const label_t *labels)
{
    ASM_CHECK(NULL != dest, ASM_NULLPTR);
    ASM_CHECK(NULL != arg, ASM_NULLPTR);
    ASM_CHECK(NULL != labels, ASM_NULLPTR);


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
        ASM_CHECK(regChar - 'a' < N_REGS, ASM_ARGERR);
        dest->opcode.regNo = regChar - 'a';
    }

    else if (2 == sscanf(arg, "[%lld+r%cx]",  &argVal.address, &regChar))
    {
        dest->opcode.reg = 1;
        dest->opcode.imm = 1;
        dest->opcode.mem = 1;
        dest->data.address = argVal.address;
        ASM_CHECK(regChar - 'a' < N_REGS, ASM_ARGERR);
        dest->opcode.regNo = regChar - 'a';
    }

    else if (1 == sscanf(arg, "[%lld]", &argVal.address))
    {
        dest->opcode.imm = 1;
        dest->opcode.mem = 1;
        dest->data.address = argVal.address;
    }

    else if (1 == sscanf(arg, "[r%cx]", &regChar))
    {
        dest->opcode.reg = 1;
        dest->opcode.mem = 1;
        ASM_CHECK(regChar - 'a' < N_REGS, ASM_ARGERR);
        dest->opcode.regNo = regChar - 'a';
    }

    else if (1 == sscanf(arg, "%%%lld", &argVal.address))
    {
        dest->data.address = argVal.address;
    }
    
    else if (SIZE_MAX != (argVal.address = asmFindLabel(arg, labels)))
    {
        dest->data.address = argVal.address;
    }

    else
    {
        return ASM_ARGERR;
    }


    return ASM_SUCCESS;
}

