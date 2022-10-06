#include <stdio.h>
#include "asm.h"



int main(int argc, char **argv)
{
    if (2 != argc)
    {
        printf("Error: invalid command line arguments. Please, read docs.\n");
        return 0;
    }


    asm_t assem = {0};
    asmCtor(&assem) ASM_ASSERTED;

    asmLoad(&assem, argv[1]) ASM_ASSERTED;
    asmBuild(&assem, argv[2]) ASM_ASSERTED;

    asmDtor(&assem) ASM_ASSERTED;


    return 0;
}

