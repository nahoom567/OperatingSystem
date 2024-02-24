#include "pageFaultHandler.h"
#include "printText.h"
#include "src/memoryManagment/virtualMemoryManager/virtualMemoryManager.h"
#include "src/memoryManagment/physicalMemoryManager/pageFrameAllocator.h"


void isrPageFaultHandler(PageFaultErrorCode errorCode)
{
    printHex(errorCode.raw);
    printChar('\n');
    if(!(errorCode.controlBits.writable || errorCode.controlBits.present))
    {
        sysExit(0);
    }

    VirtualAddress addressThatTriggeredFault;

    // cr2 - has the address that triggered the page fault
    // we are moving it to a general purpose register
    asm volatile("movq %%cr2, %%rax" : : : "%rax");


    asm volatile(
        "movq %%rax, %0" 
        : "=r"(addressThatTriggeredFault.raw)  // where the output is stored
        : 
        : "%rax" // registers that are used this signals the compiler not to use this register anymore
        );
    
    ASSERT(addressThatTriggeredFault.raw != NULL, printString("Error: the address that trigger the crush is NULL\n"))

    PhysicalAddress physicalFreePageAddress = requestUserPage();
    bool success = mapMemory(physicalFreePageAddress, addressThatTriggeredFault);
    ASSERT(success, printString("Error: page fault handler failed to map memory\n"));
    
    *(addressThatTriggeredFault.uint64Ptr) = 0x6;
    printHex(*(addressThatTriggeredFault.uint64Ptr));
    printChar('\n');
    //if(publicValue == 1)
    //{
    //    asm volatile("hlt");
    //}
    //publicValue++;

    //printHex(addressThatTriggeredFault.raw);
    //printChar('\n');
//
}


void initPageFaultHandlerInterrupt()
{
    extern uint64_t asmIsrPageFault; // getting the address of the isr function that is written in asm:
    uint64_t asmIsrPageFaultAddress = (uint64_t)(&asmIsrPageFault);

    initSpecificInterrupt(asmIsrPageFaultAddress, NO_INTERRUPT_STACK_TABLE, 
       IDT_TA_TrapGate, NO_INTERRUPT_EXTRA_BITS, InterruptIndexes::page_fault_interrupt_index);
}