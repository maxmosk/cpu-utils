#include <stdio.h>
#include "cpu.h"



#define CHECK(expr, pcpu)           \
do                                   \
{                                     \
    int status = (expr);               \
    if (CPU_SUCCESS != status)          \
    {                                    \
        printf("Expression %s\n", #expr); \
        cpuPrintError(pcpu, status);       \
    }                                       \
}                                            \
while (0)



int main(int argc, char **argv)
{
    if (2 != argc)
    {
        printf("Error: invalid command line arguments. Please, read docs.\n");
        return 0;
    }

    cpu_t myCPU = {0};
    CHECK(cpuCtor(&myCPU), &myCPU);
    CHECK(cpuLoad(&myCPU, argv[1]), &myCPU);
    CHECK(cpuExec(&myCPU), &myCPU);
    CHECK(cpuDtor(&myCPU), &myCPU);

    return 0;
}

