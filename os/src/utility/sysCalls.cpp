#include "sysCalls.h"


void sysExit(uint32_t status)
{
    asm volatile("int $0x8"); // for no just triggering the double fault interrupt
}