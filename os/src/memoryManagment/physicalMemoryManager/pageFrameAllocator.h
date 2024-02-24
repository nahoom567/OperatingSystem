#pragma once
#include "src/utility/utility.h"


#define THE_AMOUNT_PAGES_THAT_HAVE_IDENTITY_MAPPING  0x200
#define MAX_VIRTUAL_ADDRESS_MAPPED_TO_PHYSICAL       0x1FFFFF
#define PAGE_SIZE                                    0x1000
typedef union
{
    struct 
    {
        uint64_t offset    : 12;
        uint64_t pageIndex : 52;
    } addr;

    uint64_t        raw;
    uint8_t*        uint8Ptr;
    uint16_t*       uint16Ptr;
    uint32_t*       uint32Ptr;
    uint64_t*       uint64Ptr;
    void*           voidPtr;
} __attribute__((packed)) PhysicalAddress;



/**
 * @brief creating page frame allocator of typeBit map and setting the settings for it
*/
void initPageFrameAllocator();
/**
 * @brief setting the a bitMap class instance with the buffer of the bit map in the inputted address this bit map is used in
 * a way that each represents one page in the system and it's says if the page is taken or not
 * @param addressOfBiggestSizeEntry the address where the buffer of the bitmap is being created
*/
void initPageBitMap(PhysicalAddress addressOfBiggestSizeEntry);
/**
 * @brief this function is reserving pages of unusable ram sections that are given to us in the memory map file
 * this sections are retrieved by performing bios instructions to sections that the cpu needs 
*/
void reserveMemoryMapReservedPages();
/**
 * @brief this function is reserving pages of unusable ram sections that are being deduced by knowing that there are important information
 * in the memory up until PROGRAM_SPACE ( PROGRAM_SPACE defined in memoryMap.h ) and in program space the first SECTORS_TO_READ bytes 
 * ( SECTORS_TO_READ is defined in memoryMap.h ) are used for kernel code that is being executed here so we need to lock this pages as well 
*/
void reserveKernelAndBootloaderCodeSections();



/**
 * @brief this function is changing the status of a specific page by changing it's value in the bit map
 * and also updating the usedMemory, reservedMemory and freeMemory according to the options the user select
 * @param addr the physical address of the page
 * @param releaseOrGrabPage this value is telling whether to to turn the bit of that says that the page is taken on or off, 
 * true - release, false - grab
 * @param lockOrUnreserve if we lock the page or free the page we specify put the address freeMemory because we would like the status of the freeMemory
 * to be updated and if we want to reserve the memory or unreserve we specify the address of reservedMemory
*/
void changePageStatus(PhysicalAddress addr, bool releaseOrGrabPage, uint64_t* lockOrUnreserve);
// setting up some macros for ease of use: 
#define freePage(addr)      changePageStatus(addr,true,  &usedMemory    )
#define lockPage(addr)      changePageStatus(addr,false, &usedMemory    )
#define reservePage(addr)   changePageStatus(addr,false, &reservedMemory)
#define unreservePage(addr) changePageStatus(addr,true,  &reservedMemory)
/**
 * @brief this function is changing the status of a specific pages by changing there value in the bit map
 * and also updating the usedMemory, reservedMemory and freeMemory according to the options the user select
 * @param addr the physical address of the page
 * @param releaseOrGrabPage this value is telling whether to to turn the bit of that says that the page is taken on or off, 
 * true - release, false - grab
 * @param lockOrUnreserve if we lock the page or free the page we specify put the address freeMemory because we would like the status of the freeMemory
 * to be updated and if we want to reserve the memory or unreserve we specify the address of reservedMemory
 * @param pageCount the amount of pages in a raw that we would like to change their status
*/
void changePagesStatus(PhysicalAddress addr, bool releaseOrGrabPage, uint64_t* lockOrUnreserve, uint64_t pageCount);
#define freePages(addr, pageCount)      changePagesStatus(addr,true,  &usedMemory    , pageCount)
#define lockPages(addr, pageCount)      changePagesStatus(addr,false, &usedMemory    , pageCount)
#define reservePages(addr, pageCount)   changePagesStatus(addr,false, &reservedMemory, pageCount)
#define unreservePages(addr, pageCount) changePagesStatus(addr,true,  &reservedMemory, pageCount)



// @return the amount of free ram of the system
uint64_t getFreeRam();
// @return the amount of used ram of the system
uint64_t getUsedRam();
// @return the amount of used ram that is reserved for memory of the cpu and other unusable sections
uint64_t getReservedRam();
// @return the amount of pages that could possibly be on the ram
uint64_t getPageCount();

/**
 * @brief this function checks if there is a page that has a page which does not have identity paging and returns it 
 * we use this function for processors where the user wants to make because we want to reserve the pages with identity mapping for other pur
*/
PhysicalAddress requestUserPage();

/**
 * @brief this function is used to retrieve pages that would be used mainly for page tables or kernel level process 
*/
PhysicalAddress requestIdentityMappedPage();

/**
 * @brief the function provides pages within a specific range 
 * @param startingRange the page index to start searching from
 * @param endRange the page index to end the search
*/
inline PhysicalAddress getPageWithInRange(uint64_t startingRange, uint64_t endRange);


IF_DEBUG
(
    /**
     * @brief function that prints all of the index of the pages that cannot be used in the following format:
     * 5, 52, 67 ( example )
    */
    // change it later so when I press on enter or something it will give me the next parts because now it is filling the screen
    void printTakenPages();

    /**
     * @brief prints the freeMemory, reservedMemory and usedMemory
    */
    void printDifferentMemoryCapacities();
)
