#pragma once
#include "src/utility/utility.h"

IF_DEBUG
(
    /**
     * @brief testing whether taking identity mapped pages from the physical memory and then translating them into
     * from physical memory gives us the same result
    */
    void testIdentityMappedMemoryTranslation();

    /** 
     * @brief the function is trying to tests mapping virtual memory to physical memory
    */
    void testMemoryMapping();

    void testPageFaultHandlerInterrupt();

    void testTime();

    void testWritingRamHardDisk();
)
