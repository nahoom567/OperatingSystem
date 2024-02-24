#pragma once
#include "src/utility/typedefs.h"
#include "src/memoryManagment/physicalMemoryManager/pageFrameAllocator.h"

#define PAGE_TABLE_MAX_LEVEL 4

#define VIRTUAL_ADDRESS_OFFSET      addr.offset
#define PT_INDEX                    addr.ptIndex
#define PD_INDEX                    addr.pdIndex
#define PDPI_INDEX                  addr.pdpIndex
#define PML4_INDEX                  addr.plm4Index
#define SIGN_EXTENSION_BITMASK      addr.signExtension
typedef union
{
    struct 
    {
        uint64_t offset              : 12; 
        uint64_t ptIndex             : 9 ;
        uint64_t pdIndex             : 9 ;
        uint64_t pdpIndex            : 9 ;
        uint64_t plm4Index           : 9 ;
        uint64_t signExtension      : 16; 
    } addr;

    uint64_t   raw;
    uint8_t*   uint8Ptr;
    uint16_t*  uint16Ptr;
    uint32_t*  uint32Ptr;
    uint64_t*  uint64Ptr;
    void*      voidPtr;
    
} __attribute__((packed)) VirtualAddress;
// __attribute__((packed)) is used to tell the compiler that the members in the struct need to be packed tightly with no aliment because the structure should
// be aligned byte by byte



typedef union
{
    struct
    {
        uint64_t present          : 1;  // page present in memory
        uint64_t writable         : 1;  // read-only if clear, read-write if set
        uint64_t user_access      : 1;  // supervisor level only if clear
        uint64_t write_through    : 1;  // write-through caching enabled if set
        uint64_t cache_disable    : 1;  // cache disabled if set
        uint64_t accessed         : 1;  // page accessed since last refresh
        uint64_t dirty            : 1;  // page written to since last refresh

        // page table with the size (512 * 0x1000 = 2048KB = 2MB ) a typical page is only the size 4KB but this page is special and
        // and it can hold 512 page tables ( 1 page - for table )
        uint64_t huge_page        : 1;  
        uint64_t global           : 1;  // if set, prevents TLB from updating
        uint64_t reserved         : 3;  // reserved bits
        uint64_t index            : 40; // index of entry in the pages of the memory
        uint64_t avl2             : 7;  
        uint64_t pk               : 4;
        uint64_t xd               : 1;
    } __attribute__((packed)) attributes;

    uint64_t raw;
} __attribute__((packed)) PageTableEntry    ;
typedef PageTableEntry    PageDirectoryEntry;
typedef PageTableEntry    PagePDPTEntry     ;
typedef PageTableEntry    PagePML4Entry     ;

// page table is a generic way to call the page table, page directory
#define PAGE_TABLE_ENTRIES_IN_ONE_PAGE 512
typedef struct
{
    PageTableEntry entries[PAGE_TABLE_ENTRIES_IN_ONE_PAGE];
} __attribute__((packed, aligned(PAGE_SIZE))) PageTable;
typedef PageTable PageDirectoryTable;
typedef PageTable PagePDPTTable;
typedef PageTable PagePML4Table;



// functions for translation of virtual memory to physical memory:
/**
 * @brief returning the value in cr3 register that is a pointer to the plm4 table 
*/
inline PhysicalAddress getPML4Address();
/**
 * @brief the function is getting a page index and returning the corrsponding page table that in this page index 
*/
inline PageTable* getPageTable(uint64_t pageIndex);
/**
 * @brief this functions gets the page a page index that contains some variant of a page table and an index in this page table 
 * which are the components to get a page table entree and then it returns the index of the page of the next page table page or the requested page
 * @param pageIndex this index represents the index of the page that the page table is in if the entire memory is considered 
 * as buffer where each entree is at the size PAGE_SIZE
 * @param entryIndex this is an entree index inside of the page table that the value page index points to
 * @return the page index of the next page table or the page that we are looking for it's memory
*/
inline uint64_t getNextPageIndex(uint64_t pageIndex, uint64_t entryIndex);
/**
 * @brief the function is getting a virtual address and converts it back to a physical one
*/
PhysicalAddress getPhysicalAddress(VirtualAddress vAddr);



// this is used when we want to switch pte ( page table entry to a new page ) this is only doing it for the current process
// if multiprocessing is being used then it will not work for the other processors ( this invalidates the automatic tlb )
#define FLUSH_BUFFER(addr) asm volatile("invlpg (%0)" ::"r" (addr) : "memory")
/**
 * @brief the function is getting a physical address and a virtual address and makes it so the entire page that the virtual address 
 * points to, will be mapped to the entire page that the physical address points to.
 * @param pAddr some physical address inside of a specific page we want to map
 * @param vAddr some virtual address inside of a specific page we want to map
 * @param overrunTableEntry defines what is happening in the case that the last page table entree ( in the actual pte ) is already mapped to another page
 * @return whether the mapping succeeded or not
*/
bool mapMemory(PhysicalAddress pAddr, VirtualAddress vAddr, bool overrunTableEntry = false);



void unMapMemory(VirtualAddress vAddr);