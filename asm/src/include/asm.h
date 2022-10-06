#ifndef __ASM_H_INCLUDED__
#define __ASM_H_INCLUDED__



#include <stdio.h>
#include "opcodes.h"
#include "signature.h"
#include "text.h"



#ifndef NDEBUG

#define ASM_ASSERTED == ASM_SUCCESS || (fprintf(stderr, "\t*** Error in file %s line %d ***\n", __FILE__, __LINE__), (exit(0), 1))

#else
#define ASM_ASSERTED
#endif /* NDEBUG */



typedef struct
{
    text_t source;
    size_t codeSize;
    cpuInstruction_t code;
} asm_t;



enum ASM_CODES
{
    ASM_SUCCESS = 0,
    ASM_ERROR   = 1
};



enum ASM_CODES asmCtor(asm_t *thisAsm);


enum ASM_CODES asmLoad(asm_t *thisAsm, const char *sourceFile);


enum ASM_CODES asmBuild(asm_t *thisAsm, const char *execFile);


enum ASM_CODES asmDtor(asm_t *thisAsm);


#endif /* __ASM_H_INCLUDED__ */
