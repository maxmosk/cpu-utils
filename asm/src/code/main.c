#include <stdio.h>
#include "asm.h"


#define CHECK(code)       \
do                         \
{                           \
    int status = (code);     \
    if (ASM_SUCCESS != status)\
    {                          \
        asmPrintError(status);  \
    }                            \
}                                 \
while (0)


int main(int argc, char **argv)
{
    if (3 != argc)
    {
        printf("Error: invalid command line arguments. Please, read docs.\n");
        return 0;
    }

    asm_t assem = {0};
    CHECK(asmCtor(&assem));
    CHECK(asmLoad(&assem, argv[1]));
    CHECK(asmBuild(&assem, argv[2]));

    CHECK(asmDtor(&assem));

    return 0;
}

