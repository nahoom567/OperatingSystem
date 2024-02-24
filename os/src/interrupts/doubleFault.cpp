#include "doubleFault.h"
#include "idt.h"
#include "src/utility/utility.h"

void isrDoubleFaultHandler()
{
    ASSERT(false, printString("Error: system reached double fault isr function pls debug the situation\n"))
}

void initDoubleFaultInterrupt()
{
    extern uint64_t asmIsrDoubleFault; // getting the address of the isr function that is written in asm
    uint64_t asmIsrDoubleFaultAddress = (uint64_t)(&asmIsrDoubleFault);

    
    initSpecificInterrupt(asmIsrDoubleFaultAddress, NO_INTERRUPT_STACK_TABLE, 
        IDT_TA_TrapGate, NO_INTERRUPT_EXTRA_BITS, InterruptIndexes::double_fault_interrupt_index);
}