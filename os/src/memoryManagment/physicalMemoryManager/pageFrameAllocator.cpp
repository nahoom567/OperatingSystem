#include "pageFrameAllocator.h"
#include "memoryMap.h"
#include "src/utility/bitmap.hpp"

uint64_t freeMemory     = 0   ;
uint64_t usedMemory     = 0   ;
uint64_t reservedMemory = 0   ;
BitMap* pageBitMap      = NULL;

uint64_t getFreeRam()      { return freeMemory              ; }
uint64_t getUsedRam()      { return usedMemory              ; }
uint64_t getReservedRam()  { return reservedMemory          ; }
uint64_t getPageCount()    { return pageBitMap->m_PAGE_COUNT; }


// functions for initiating the bit map allocator:
void initPageFrameAllocator()
{
    // checking if the bit map allocator was already created:
    static bool initialized = false;
    if(initialized) { return; }
    initialized = true;



    // getting the region with the biggest size
    MemoryMapEntry* biggestSizeEntry = getFreeMemoryBiggestEntry();
    PhysicalAddress addressOfBiggestSizeEntry;
    addressOfBiggestSizeEntry.raw = biggestSizeEntry->baseAddress;

    // checking if the address that was provided is the same for virtual and physical:
    bool condition = addressOfBiggestSizeEntry.raw < MAX_VIRTUAL_ADDRESS_MAPPED_TO_PHYSICAL;
    ASSERT(condition, printString("Error: virtual address and physical address are not the same\n"))

    // initializing the bit map of all of the pages in memory and locking the bit map where 
    initPageBitMap(addressOfBiggestSizeEntry);
    lockPages(addressOfBiggestSizeEntry /* the starting address of the bitMap buffer */, ( pageBitMap->m_size / PAGE_SIZE ) + 1);

    // reserving the pages of the memory sections that cannot be used:
    reserveMemoryMapReservedPages();
    reserveKernelAndBootloaderCodeSections();


    // functions for debugging:
    // printTakenPages();
    // printAllRegions();
    // printDifferentMemoryCapacities();
}
void initPageBitMap(PhysicalAddress addressOfBiggestSizeEntry)
{

    // getting the total size of memory:
    uint64_t memorySize = getMemorySize();
    freeMemory = memorySize;
    uint64_t bitMapSize = (memorySize / PAGE_SIZE) + 1; // calculating the size of the bit map in bytes
    

    // initializing the bitmap for the pages:
    // creating a bit map that represents all of the pages in the system and if they are taken or not
    // we are doing it by converting the address of the biggest free memory into a bit map 
    // so we can store the bit map dynamically without the stack
    pageBitMap = (BitMap*)addressOfBiggestSizeEntry.voidPtr; 

    // setting up the size of the map:
    pageBitMap->m_size     = bitMapSize;
    pageBitMap->m_bitCount = bitMapSize / BITS_IN_BYTE;

    // setting up the buffer of the bit map:
    // adding to the the address of the free big chuck of memory the size of the bit map class to get the next free address because 
    // because we already created an instance of BitMap pointer in the start of the chucnk and now we need to created the buffer
    addressOfBiggestSizeEntry.raw += sizeof(BitMap); 
    pageBitMap->m_buffer = addressOfBiggestSizeEntry.uint8Ptr;  // setting up the buffer of the bit map class to the address where the actual bit map will be


    // putting 0 in all of the buffer because all of the pages are free right now:
    for(int i = 0; i < pageBitMap->m_size; i++)
    {
        (pageBitMap->m_buffer)[i] = 0x0;
    }
}
void reserveMemoryMapReservedPages()
{
    MemoryMapEntry* allMemoryRegionsEntries = getAllMemoryRegions();
    PhysicalAddress reservedMemoryRegionAddress;
    uint64_t pageCount = 0;


    // reserving all the pages that have are not usable ram pages:
    for(int i = 0; i < getMemoryRegionCount(); i++)
    {
        if(allMemoryRegionsEntries[i].regionType != MemoryRegion::UsableRAM)
        {
            // we subtract the modulo because we want the address of the starting page
            // for example the address 0x9FC00 will become 0x9F000 so now we know for sure that the value 0x9F is the page index:
            reservedMemoryRegionAddress.raw = 
                allMemoryRegionsEntries[i].baseAddress - (allMemoryRegionsEntries[i].baseAddress % PAGE_SIZE);
            
            // calculating the amount of pages by dividing the region length by the page size and then 
            // checking if there is a partial page by checking if the length modulo page size is grater then 1
            // if it is grater then 1 we add 1 to the page count otherwise we add 0
            pageCount = 
                (allMemoryRegionsEntries[i].regionLength / PAGE_SIZE) + (( allMemoryRegionsEntries[i].regionLength % PAGE_SIZE > 0) * 1);

            reservePages(reservedMemoryRegionAddress, pageCount);
        }
    }
}
void reserveKernelAndBootloaderCodeSections()
{
    // we are using the address 0x0 because we want to lock the first pages that are used for important things like some page tabes
    // gdt and more
    PhysicalAddress address;
    address.raw = 0x0;
    reservePages(address, PROGRAM_SPACE / PAGE_SIZE);


    // now reserving the pages of the bootloader and kernel code:
    address.raw = PROGRAM_SPACE;
    reservePages(address, (SECTOR_SIZE * SECTORS_TO_READ) / PAGE_SIZE);
}


// functions for changing the status of certain pages:
void changePageStatus(PhysicalAddress addr, bool releaseOrGrabPage, uint64_t* lockOrUnreserve)
{
    uint64_t pageIndex = addr.addr.pageIndex;                       // finding out in what page this address in
    if((*pageBitMap)[pageIndex] == !releaseOrGrabPage ) { return; }  // checking if the bit for the page is already in the desired state
    

    pageBitMap->setBit(pageIndex, !releaseOrGrabPage);               // setting the bit to the desired state
    freeMemory         += (releaseOrGrabPage ? 1 : -1) * PAGE_SIZE;  // adding or subtracting the amount of freeMemory based on the input of the function
    // adding or subtracting the amount of reserved memory or used memory based on the inputs of the function:
    *(lockOrUnreserve) += (releaseOrGrabPage ? -1 : 1) * PAGE_SIZE; 
}
void changePagesStatus(PhysicalAddress addr, bool releaseOrGrabPage, uint64_t* lockOrUnreserve, uint64_t pageCount)
{
    // going in a loop and changing the status of the pages that we want:
    for(uint64_t i = 0; i < pageCount; i++)
    {
        changePageStatus(addr, releaseOrGrabPage, lockOrUnreserve);
        addr.raw += PAGE_SIZE;
    }
}


// functions for asking pages:
PhysicalAddress requestUserPage()
{
    // we start from THE_AMOUNT_PAGES_THAT_HAVE_IDENTITY_MAPPING because we would like to reserve pages with identity mapping
    // to page tables and kernel mode memory 
    return getPageWithInRange(THE_AMOUNT_PAGES_THAT_HAVE_IDENTITY_MAPPING, getPageCount());
}
PhysicalAddress requestIdentityMappedPage()
{
    // we want only the pages with the identity mapping:
    return getPageWithInRange(0, THE_AMOUNT_PAGES_THAT_HAVE_IDENTITY_MAPPING);
}
inline PhysicalAddress getPageWithInRange(uint64_t startingRange, uint64_t endRange)
{
    // later optimize it a lot:
    PhysicalAddress address;
    address.raw = NULL;


    // looping on the pages to find an empty page
    for(uint64_t i = startingRange; i < endRange; i++)
    {
        if((*pageBitMap)[i]) { continue; }  // checking if this page is free
        
        address.raw = i * PAGE_SIZE; // multiply the index by the page size to get the physical address of the page
        lockPage(address);
        return address;
    }

    // if we get to here then the number of pages has ran out right now we do nothing about it
    ASSERT(true, printString("Error: there are no free pages left pls implemented code to handle this situation\n")); 
    return address;
}
void returnPage(PhysicalAddress addr)
{
    freePage(addr); // calling the function free page which is internal to this file
}

// printing functions that are used for debugging:
IF_DEBUG
(
    void printTakenPages()
    {
        for(int i = 0; i < pageBitMap->m_bitCount; i++)
        {
            // priting the index if the bit cannot be used: 
            if((*pageBitMap)[i] > 0)
            {
                printInt(i);
                printChar(',');
            }
        }
    }
    void printDifferentMemoryCapacities()
    {
        printString("\nfree     memory size: ");   printHex(freeMemory    );
        printString("\nused     memory size: ");   printHex(usedMemory    );
        printString("\nreserved memory size: ");   printHex(reservedMemory);
    }
)