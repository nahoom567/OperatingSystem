#include "test.h"
#include "src/memoryManagment/physicalMemoryManager/pageFrameAllocator.h"
#include "src/memoryManagment/virtualMemoryManager/virtualMemoryManager.h"
#include "src/utility/utility.h"

 
IF_DEBUG
(
    #define PAGES_TO_TEST 20
    void testIdentityMappedMemoryTranslation()
    {
        for(int i = 0; i < PAGES_TO_TEST; i++)
        {
            PhysicalAddress pAddr;
            pAddr.addr = requestIdentityMappedPage().addr;
            VirtualAddress vAddr;
            vAddr.raw = pAddr.raw;

            ASSERT(pAddr.raw == getPhysicalAddress(vAddr).raw,
                printString("Error: test of the translation of identity mapped pages address failed\n"));
            

        }

        printString("testIdentityMappedMemoryTranslation: successful\n");
    }

    #define VALUE_TO_PUT_IN_ADDRESS 0x5987
    void testMemoryMapping()
    {
        PhysicalAddress pAddr = requestUserPage(); // requesting a page that is not identity mapped
        VirtualAddress  vAddr;
        vAddr.raw = pAddr.raw;  // putting the same address in the virtual address as the physical address to create identity mapping
        
        // mapping the physical address and virtual address which are the same which allows as to access 
        // the physical address normally without worrying about what the translation: 
        bool success = mapMemory(pAddr, vAddr);
        ASSERT(success, printString("Error: failed to map memory\n"))
    
        // peaking another virtual address in the page after and mapping it to the same address:
        vAddr.raw += PAGE_SIZE;
        success = mapMemory(pAddr, vAddr);
        ASSERT(success, printString("Error: failed to map memory\n"))


        // putting the value VALUE_TO_PUT_IN_ADDRESS in one of the addresses and checking if there is the same address in both of them:
        *(vAddr.uint64Ptr) = VALUE_TO_PUT_IN_ADDRESS;
        ASSERT(*(pAddr.uint64Ptr) == VALUE_TO_PUT_IN_ADDRESS, 
            printString("Error: the test of mapping two different memories virtual address to the same one failed\n"));


        if(*(pAddr.uint64Ptr) == VALUE_TO_PUT_IN_ADDRESS)
        {
            printString("testMemoryMapping: successful\n");

        }
    }
)