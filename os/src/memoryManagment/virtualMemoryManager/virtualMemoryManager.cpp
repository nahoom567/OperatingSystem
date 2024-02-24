#include "virtualMemoryManager.h"
#include "src/utility/printText.h"


inline PhysicalAddress getPML4Address()
{
    PhysicalAddress addr; 
    asm volatile("movq %%cr3, %0" : "=r"(addr.raw));
    return addr;
}
inline PageTable* getPageTable(uint64_t pageIndex)
{
    // making sure that the page index is not NULL:
    ASSERT(pageIndex != NULL, 
        printString("Error: page table index points to NULL\n"))


    uint64_t pageTableAddress = pageIndex * PAGE_SIZE;
    ASSERT(pageTableAddress > MAX_VIRTUAL_ADDRESS_MAPPED_TO_PHYSICAL, 
        printString("Error: physical and virtual address are different for this page therefor there is no way to access to this page table\n"))      
    return (PageTable*)(pageTableAddress); // finding the table it self from the page index
}

// memory translation:
inline uint64_t getNextPageIndex(uint64_t pageIndex, uint64_t entryIndex)
{
    PageTable* pageTable = getPageTable(pageIndex);


    // making sure that the output index of the page is even a page that is present in the system:
    ASSERT(pageTable->entries[entryIndex].attributes.present != NULL,
        printString("Error: the index of the requested page is not present in the system\n"))

    
    return pageTable->entries[entryIndex].attributes.index;    // extracting the index of the next table
}
PhysicalAddress getPhysicalAddress(VirtualAddress vAddr)
{
    // getting the page index of the first table the pml4 table:
    uint64_t pageIndex = getPML4Address().addr.pageIndex;
    // an array that has all entree indexes of every page table:
    uint64_t pageTableEntriesIndexes[PAGE_TABLE_MAX_LEVEL] = { vAddr.PML4_INDEX, vAddr.PDPI_INDEX, vAddr.PD_INDEX, vAddr.PT_INDEX }; 
    

    // going in a loop of PAGE_TABLE_MAX_LEVEL times to find the page index of the page the virtual address points to: 
    for(uint64_t i = 0; i < PAGE_TABLE_MAX_LEVEL; i++)
    {
        pageIndex = getNextPageIndex(pageIndex, pageTableEntriesIndexes[i]);  
    }


    // getting the physical address of the page we need:
    PhysicalAddress pAddress;
    pAddress.addr.pageIndex = pageIndex;
    pAddress.addr.offset    = vAddr.VIRTUAL_ADDRESS_OFFSET;
    return pAddress;
}



// mapping virtual addresses to physical ones:
inline void setNewPageEntry(PageTable* currentPageTable, uint64_t entryIndex, bool isPageTableIndex, uint64_t physicalPageIndex)
{
    // checking if the next page has to be an identity mapped page because this is a page that is going to be used for 
    // a page table or we should use the index that is being provided because we map it to the requested physical address:
    PhysicalAddress pageAddress; 
    if(isPageTableIndex) { pageAddress                = requestIdentityMappedPage(); }
    else                 { pageAddress.addr.pageIndex = physicalPageIndex          ; }


    ASSERT(pageAddress.raw != NULL, printString("Error: there are no more identity mapped pages and the page table couldn't have been created\n"))

    currentPageTable->entries[entryIndex].attributes.present  = true; // making the page present in the system
    currentPageTable->entries[entryIndex].attributes.writable = true; // making the page read and write through
    currentPageTable->entries[entryIndex].attributes.index = pageAddress.addr.pageIndex; // setting the index of the next page table
}
inline uint64_t createOrFindNextPageIndex(uint64_t pageIndex, uint64_t entryIndex, uint64_t physicalPageIndex, bool isPageTableIndex, bool overrunTableEntry)
{
    // getting the page table from the page index
    PageTable* pageTable = getPageTable(pageIndex);


    if(pageTable->entries[entryIndex].attributes.present == false)
    {
        setNewPageEntry(pageTable, entryIndex, isPageTableIndex, physicalPageIndex);
    }
    // making sure that this is the last entry in the actual pt and we can overrun the current physical address for this virtual address:
    else if (!isPageTableIndex && overrunTableEntry)
    {
        setNewPageEntry(pageTable, entryIndex, isPageTableIndex, physicalPageIndex);
    }

    return pageTable->entries[entryIndex].attributes.index;
}
bool mapMemory(PhysicalAddress pAddr, VirtualAddress vAddr, bool overrunTableEntry)
{
    // getting the page index of the first table the pml4
    uint64_t pageIndex = getPML4Address().addr.pageIndex;
    uint64_t pageTableEntriesIndexes[PAGE_TABLE_MAX_LEVEL] = { vAddr.PML4_INDEX, vAddr.PDPI_INDEX, vAddr.PD_INDEX, vAddr.PT_INDEX }; 
    bool isPageTableIndex = true; // this value says if the entry in the current page table is the page index of another page table or the index of the physical address  

    // looping on all the pages until we reach the index that represents the final page table the actual pt 
    for(uint64_t i = 0; i < PAGE_TABLE_MAX_LEVEL; i++)
    {
        // checking if we have reach the pt and the page index is now pointing to the actual physical page we want to map
        if(i == PAGE_TABLE_MAX_LEVEL - 1) { isPageTableIndex = false; }

        pageIndex = createOrFindNextPageIndex(pageIndex, pageTableEntriesIndexes[i], pAddr.addr.pageIndex, isPageTableIndex, overrunTableEntry);
    }
    
    // invalidating the tlb cashing
    FLUSH_BUFFER(vAddr.raw);

    // returning if the final index in the pt is pointing to the desired page that the physical address is in and the mapping succeeded: 
    return (pageIndex == pAddr.addr.pageIndex);
}



void unmapMemory(VirtualAddress vAddr)
{
    // getting the page index of the first table the pml4 table:
    uint64_t pageIndex = getPML4Address().addr.pageIndex;
    // an array that has all entree indexes of every page table:
    uint64_t pageTableEntriesIndexes[PAGE_TABLE_MAX_LEVEL - 1] = { vAddr.PML4_INDEX, vAddr.PDPI_INDEX, vAddr.PD_INDEX }; 
    

    // going in a loop until we get the into the actual final pt
    for(uint64_t i = 0; i < PAGE_TABLE_MAX_LEVEL - 1; i++)
    {
        pageIndex = getNextPageIndex(pageIndex, pageTableEntriesIndexes[i]);  
    }

    // getting the actual pt and not the ( pd, pdpt or pml4):
    PageTable* pt = getPageTable(pageIndex);

    // setting the present bit to false indicating the page is not in use now
    pt->entries->attributes.present = false;

    FLUSH_BUFFER(vAddr.addr);
}