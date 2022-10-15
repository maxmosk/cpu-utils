#ifndef ASM_H_INCLUDED__
#define ASM_H_INCLUDED__



#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include "opcodes.h"
#include "signature.h"
#include "text.h"



typedef struct
{
    size_t address;
    const char *name;
} label_t;

typedef struct
{
    text_t source;
    size_t codeSize;
    cpuInstruction_t *code;
    label_t *labels;
} asm_t;



enum ASM_CODES
{
    ASM_SUCCESS  = 0,
    ASM_ERROR    = 1,
    ASM_ALLOCERR = 2,
    ASM_FILEERR  = 3,
    ASM_TEXTERR  = 4,
    ASM_INSTRERR = 5,
    ASM_ARGERR   = 6,
    ASM_NULLPTR  = 7
};



enum ASM_CODES asmCtor(asm_t *thisAsm);

enum ASM_CODES asmLoad(asm_t *thisAsm, const char *sourceFile);

enum ASM_CODES asmBuild(asm_t *thisAsm, const char *execFile);

enum ASM_CODES asmDtor(asm_t *thisAsm);

void asmPrintError(enum ASM_CODES code);


#endif /* ASM_H_INCLUDED__ */

