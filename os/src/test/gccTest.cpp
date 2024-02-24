/*
this file is only used to test how the gcc compiles things into asm it is not included in the final build only in the asm build
*/
#include "src/utility/typedefs.h"

void func()
{
    uint8_t input1 = 1;
    uint8_t input2 = 2;
    uint8_t output1 = 3;
    uint8_t output2 = 4;


    asm volatile(
        "movb %%cl, %0  \n\t"   // Use input1
        "movb %%dl, %1  \n\t"   // Use input2
        "movb %2, %%al  \n\t"   // Set output1
        "movb %3, %%bl  \n\t"   // Set output2
        : "=r" (output1), "=r" (output2)
        : "r" (input1), "r" (input2)
        : "al", "bl", "cl", "dl"
    );
}
