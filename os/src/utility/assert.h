#pragma once
#include "debug.h"
#include "printText.h"


// only making the assert in the case 
#ifdef DEBUG_MODE

// when the condition that is inside of the assert does not evaluate to true then the action that is passed is being performed and then
// a hlt instruction is being performed on the cpu which freezes it until an interrupt is being triggered
#define ASSERT(condition, action)          \
    if(!condition)                          \
    {                                        \
        action;                               \
        asm volatile("hlt");                   \
    }
#else
#define ASSERT(condition, action)          
#endif