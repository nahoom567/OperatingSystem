#include "src/utility/printText.h"
#include "src/utility/bitmap.hpp"
#include "src/interrupts/idt.h"
#include "src/memoryManagment/physicalMemoryManager/pageFrameAllocator.h"
#include "src/memoryManagment/virtualMemoryManager/virtualMemoryManager.h"
#include "src/memoryManagment/physicalMemoryManager/memoryMap.h"
#include "src/test/test.h"
#include "src/interrupts/keyboardInterrupt/KBScanCodeSets.h"







extern "C" void _start()
{
    setCursorPosition(0);
    initPageFrameAllocator();
    initIDT();
    
    
    //testIdentityMappedMemoryTranslation(); 
    //testMemoryMapping();
    //testPageFaultHandlerInterrupt();
    testWritingRamHardDisk();
    testTime();
    turnPeriodicRtcInterrupt(Rate::HZ_256);
    
    while(true);
}


// non urgent tasks:
// 1. make it so the compiler and linker will be part of project 
// 2. make a readme to the project
// 3. switch the idt to support interrupt stack table ( a special stack that interrupts can use when regular stack is not available )