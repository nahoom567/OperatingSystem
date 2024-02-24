#include "memoryMap.h"
#include "src/utility/printText.h"
#include "src/utility/assert.h"

uint8_t usableMemoryRegionCount    = 0;
uint8_t reservedMemoryRegionsCount = 0;
extern uint8_t memoryRegionCount;

uint8_t getCertainMemoryRegionCount(MemoryRegion memoryRegion)
{
    if(memoryRegion == MemoryRegion::UsableRAM ) { return usableMemoryRegionCount   ; }
    if(memoryRegion == MemoryRegion::Reserved  ) { return reservedMemoryRegionsCount; }

    ASSERT(true, printString("the choosen memory region is not supported by this function")) 
    return 0;
}
MemoryMapEntry** getCertainMemoryRegions(MemoryRegion memoryRegion)
{
    // defining arrays for usable memory regions and reserved memory regions: 
    static MemoryMapEntry* usableMemoryRegions[AMOUNT_OF_USABLE_MEMORY_REGIONS];  
    static MemoryMapEntry* reservedMemoryRegions[AMOUNT_OF_USABLE_MEMORY_REGIONS]; 
    static bool alreadyCalculatedFreeMemoryRegions     = false;
    static bool alreadyCalculatedReservedMemoryRegions = false;

    // returning the wanted memory regions if they are already calculated: 
    if(memoryRegion == MemoryRegion::UsableRAM && alreadyCalculatedFreeMemoryRegions    ) { return usableMemoryRegions  ; }
    if(memoryRegion == MemoryRegion::Reserved  && alreadyCalculatedReservedMemoryRegions) { return reservedMemoryRegions; }
    
    // checking if the wanted region is supported by the function:
    ASSERT(memoryRegion == MemoryRegion::Reserved && memoryRegion == MemoryRegion::UsableRAM, 
        printString("Error: function only calculates reserved and free memory regions\n"));

    
    // pointing to the right array and the right counter
    MemoryMapEntry** memoryRegions     = (memoryRegion == MemoryRegion::UsableRAM) ?  usableMemoryRegions      : reservedMemoryRegions     ;
    uint8_t*         wantedRegionCount = (memoryRegion == MemoryRegion::UsableRAM) ? &usableMemoryRegionCount : &reservedMemoryRegionsCount;

    
    // going in a loop and storing the wanted regions in right arrays:
    MemoryMapEntry* memMap;
    for (uint8_t i = 0; i < memoryRegionCount && i < AMOUNT_OF_USABLE_MEMORY_REGIONS; i++)
    {
        memMap = (MemoryMapEntry*)ADDRESS_WITH_MEMORY_MAPS + i;

        // checking if the current region is type 1 ( usable ram region ):
        if(memMap->regionType == memoryRegion)
        {
            memoryRegions[(*wantedRegionCount)] = memMap;   // setting the region in the array
            (*wantedRegionCount)++;     // increasing the amount of regions whatever was choosen

            // checking if there are too many regions to fit in the array
            ASSERT((!((*wantedRegionCount) + 1 == AMOUNT_OF_USABLE_MEMORY_REGIONS)), 
                printString("Error: too many regions and not enough place to store all regions\n"))
        }
    }

    
    
    return memoryRegions;
}


MemoryMapEntry* getAllMemoryRegions()
{
    // we cannot use a static variable since the static variable is initialized to early and does not give the wanted result:
    MemoryMapEntry* allMemoryRegions = (MemoryMapEntry*)ADDRESS_WITH_MEMORY_MAPS;
    return allMemoryRegions;
}
uint8_t getMemoryRegionCount()
{
    return memoryRegionCount;
}


uint64_t getMemorySize()
{
    static uint64_t memorySizeBytes = 0;
    if(memorySizeBytes != 0) { return memorySizeBytes; };

    // getting all of the regions:
    MemoryMapEntry* freeMemoryMapEntries = getAllMemoryRegions();

    // counting the amount of memory whether it is usable or not:
    for(int i = 0; i < memoryRegionCount; i++)
    {
        memorySizeBytes += freeMemoryMapEntries[i].regionLength;
    }


    return memorySizeBytes;
}


MemoryMapEntry* getFreeMemoryBiggestEntry()
{
    MemoryMapEntry** entries = getCertainMemoryRegions(MemoryRegion::UsableRAM);
    MemoryMapEntry* biggestSizeEntry = entries[0];

    // checking what is the address of region with the most memory:
    for(int i = 1; i < usableMemoryRegionCount; i++)
    {
        if(entries[i]->regionLength > biggestSizeEntry->regionLength)
        {
            biggestSizeEntry = entries[i];
        }
    }

    return biggestSizeEntry;
}


IF_DEBUG
(
    void printAllRegions()
    {
        uint64_t count = 0;
        uint64_t countUsable = 0;
        MemoryMapEntry* allMemoryRegions = (MemoryMapEntry*)ADDRESS_WITH_MEMORY_MAPS;

        // priting the regions themself:
        for(int i = 0; i < memoryRegionCount; i++)
        {
            printInt(i); printString(":      ");
            printMemoryMap(&(allMemoryRegions[i]));
            if(allMemoryRegions[i].regionType == MemoryRegion::UsableRAM) { countUsable += allMemoryRegions[i].regionLength; }
            else                                                          { count       += allMemoryRegions[i].regionLength; }
        }

        // printing the amout of memory each region has:
        printString("the amount of non usable ram: ");
        printHex(count);
        printString("\nthe amount of usable ram    : ");
        printHex(countUsable);
    }

    void printMemoryMap(MemoryMapEntry* memoryMap)
    {
        printString("memory base: ");
        printHex(memoryMap->baseAddress);
        printChar('\n');

        printString("region length: ");
        printHex(memoryMap->regionLength);
        printChar('\n');

        printString("memory type: ");
        printInt(memoryMap->regionType);
        printChar('\n');

        printString("memory attributes: ");
        printInt(memoryMap->extendedAttributes);
        printChar('\n');
    }
)