#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "cpu.h"

int main()
{
    cpu* myCpu = malloc(sizeof(cpu));
    memset(myCpu, 0, sizeof(cpu));

    cpu_initialize(myCpu);

    cpu_load_ROM(myCpu);

    cpu_reset(myCpu);

    //begin clock loop

    free(myCpu);
    return 0;
}