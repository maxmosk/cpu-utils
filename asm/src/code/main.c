#include <stdio.h>
#include "asm.h"



int main(int argc, char **argv)
{
    asm_t assem = {};
    asmCtor(&assem) ASM_ASSERTED;

    asmLoad(&assem, argv[1]) ASM_ASSERTED;
    asmBuild(&assem, argv[2]) ASM_ASSERTED;

    asmDtor(&assem) ASM_ASSERTED;

    return 0;
}

