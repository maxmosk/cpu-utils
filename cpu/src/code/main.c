#include "cpu.h"



int main(void)
{
    cpu_t myCPU = {0};
    cpuCtor(&myCPU);
    
    cpuDtor(&myCPU);

    return 0;
}
