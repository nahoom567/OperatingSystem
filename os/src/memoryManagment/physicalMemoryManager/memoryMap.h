#pragma once
#include "src/utility/typedefs.h"
#include "utility.h"

#define AMOUNT_OF_USABLE_MEMORY_REGIONS 10
#define AMOUNT_OF_REGIONS_IN_MEMORY     20

// this values are values that were choosen inside of the bootloader if the code in it will change then so are this defines:
#define ADDRESS_WITH_MEMORY_MAPS 0x7000  // address with the memory maps ( regions that are reserved and free to use are stored)
#define PROGRAM_SPACE  0x9000            // the address in ram where the bootloader and the kernel code are loaded
#define SECTORS_TO_READ 64               // the amount of sectors that the 

#define SECTOR_SIZE 512                  // the size of sector that is loaded in the mbr

/*
Memory Map Types:
Usable RAM (Type 1):
Description: This type indicates regions of memory that are available for general use. It's typically used for normal system and application operations.
Use: Usable RAM regions are suitable for allocating memory for the operating system kernel and user applications.

Reserved (Type 2):
Description: Reserved regions are portions of memory set aside for specific purposes, often determined by the hardware or firmware.
Use: Reserved areas can include regions reserved by the BIOS, ACPI tables, and other hardware-related data.

ACPI Reclaimable (Type 3):
Description: ACPI (Advanced Configuration and Power Interface) reclaimable regions are memory areas that can be reclaimed or repurposed by the operating system.
Use: These regions might be used by the operating system for dynamic memory allocation.

ACPI Non-volatile (Type 4):
Description: ACPI non-volatile regions are used for storing ACPI data that needs to persist across system reboots.
Use: Data in these regions typically includes system state information that needs to survive power cycles.

Unusable (Type 5):
Description: Unusable regions are memory areas that are not available for use. This could be due to hardware limitations or reserved regions that cannot be repurposed.
Use: These regions are typically avoided for memory
*/
enum MemoryRegion
{
    UsableRAM      = 1,
    Reserved       = 2,
    Reclaimable    = 3,
    NonReclaimble  = 4,
    Unusable       = 5,
};

struct MemoryMapEntry
{
    uint64_t baseAddress;
    uint64_t regionLength;
    uint32_t regionType;
    uint32_t extendedAttributes;
};


/**
 * @param memoryRegion the type of the region that we would like to get its array size
 * @return the size of the array that the function getCertainMemoryRegions() returns
*/
uint8_t getCertainMemoryRegionCount(MemoryRegion memoryRegion);
/**
 * @brief gets a type of memory region we would like to to get array of and returns an array of pointer to the regions of this type
 * supports MemoryRegion::UsableRAM and MemoryRegion::Reserved, also the amount of regions will be stored in the 
 * values usableMemoryRegionCount and reservedMemoryRegionsCount respectively
 * @param memoryRegion the type of the region we would like to get an array of
 * @return the array of MemoryMapEntry pointers to the type of region we want. the size of the array can be extracted from the function getCertainMemoryRegionCount()
*/
MemoryMapEntry** getCertainMemoryRegions(MemoryRegion memoryRegion);


/**
 * @return returns the size of the array from the function getAllMemoryRegions()
*/
uint8_t getMemoryRegionCount();
/**
 * @brief use the value memoryRegionCount in order to know how many elements are there
 * @return an array of MemoryMapEntry which has the size that can be brough from the function getMemoryRegionCount
*/
MemoryMapEntry* getAllMemoryRegions();



/**
 * @return the accumulative memory size of all the regions
*/
uint64_t getMemorySize();




/**
 * @return pointer of type MemoryMapEntry to the region UsableRAM region with the most memory
*/
MemoryMapEntry* getFreeMemoryBiggestEntry();

IF_DEBUG
(
    /**
     * @brief this function is priting all of the regions
    */
    void printAllRegions();

    
    /**
     * @brief gets an address of MemoryMapEntry and prints info about it
     * @param memoryMap pointer to the address
    */
    void printMemoryMap(MemoryMapEntry* memoryMap);
)