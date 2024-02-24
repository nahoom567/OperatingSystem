#include "src/utility/printText.h"
#include "src/utility/bitmap.hpp"
#include "src/IDT/idt.h"
#include "src/memoryManagment/physicalMemoryManager/pageFrameAllocator.h"
#include "src/memoryManagment/virtualMemoryManager/virtualMemoryManager.h"
#include "src/memoryManagment/physicalMemoryManager/memoryMap.h"
#include "src/test/test.h"

extern "C" void _start()
{
    setCursorPosition(0);
    InitializeIDT();
    initPageFrameAllocator();

    
    testIdentityMappedMemoryTranslation(); 
    testMemoryMapping();
}
