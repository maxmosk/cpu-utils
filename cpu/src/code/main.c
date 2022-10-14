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
    cpuCtor(&myCPU);
    cpuLoad(&myCPU, argv[1]);
    cpuExec(&myCPU);
    cpuDtor(&myCPU);

    return 0;
}

