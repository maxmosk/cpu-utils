#include <stdio.h>
#include "disasm.h"



int main(int argc, char **argv)
{
    if (2 != argc)
    {
        printf("Error: invalid command line arguments. Please, read docs.\n");
        return 0;
    }

    disasm_t dasm = {0};
    disasmCtor(&dasm);
    disasmLoad(&dasm, argv[1]);
    disasmWrite(&dasm, stdout);
    disasmDtor(&dasm);

    return 0;
}

