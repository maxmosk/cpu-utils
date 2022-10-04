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
    cpuCtor(&myCPU) CPU_ASSERTED;
    cpuLoad(&myCPU, argv[1]) CPU_ASSERTED;
    cpuExec(&myCPU) CPU_ASSERTED;
    cpuDtor(&myCPU) CPU_ASSERTED;

    return 0;
}

