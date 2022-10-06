#ifndef __DISASM_H_INCLUDED__
#define __DISASM_H_INCLUDED__



#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "opcodes.h"
#include "signature.h"



typedef struct
{
    cpuInstruction_t *code;
    long int codeSize;
} disasm_t;



enum DASM_CODES
{
    DASM_SUCCESS  = 0,
    DASM_ERROR    = 1,
    DASM_ALLOCERR = 2,
    DASM_SIGNERR  = 3,
    DASM_FILEERR  = 4
};



enum DASM_CODES disasmCtor(disasm_t *dasm);


enum DASM_CODES disasmLoad(disasm_t *dasm, const char *codeFile);


enum DASM_CODES disasmWrite(disasm_t *dasm, FILE *file);


enum DASM_CODES disasmDtor(disasm_t *dasm);



#endif /* __DISASM_H_INCLUDED__ */

