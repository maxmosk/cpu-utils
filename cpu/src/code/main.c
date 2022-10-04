#include <stdio.h>
#include "cpu.h"



int main(int argc, char **argv)
{
    if (2 != argc)
    {
        printf("Error: invalid command line arguments. Please, read docs.\n");
        return 0;
    }

    cpu_t myCPU = {0};
    cpuCtor(&myCPU) ASSERTED;
    cpuLoad(&myCPU, argv[1]) ASSERTED;
    cpuExec(&myCPU) ASSERTED;
    cpuDtor(&myCPU) ASSERTED;

    return 0;
}

