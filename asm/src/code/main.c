#include <stdio.h>
#include "asm.h"



int main(int argc, char **argv)
{
    if (3 != argc)
    {
        printf("Error: invalid command line arguments. Please, read docs.\n");
        return 0;
    }

    asm_t assem = {0};
    asmCtor(&assem);

    asmLoad(&assem, argv[1]);
    asmBuild(&assem, argv[2]);

    asmDtor(&assem);

    return 0;
}

