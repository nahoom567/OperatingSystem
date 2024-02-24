#include "idt.h"
#include "src/utility/printText.h"
#include "src/memoryManagment/physicalMemoryManager/pageFrameAllocator.h"
#include "keyboardInterrupt/keyboardInterrupts.h"
#include "pageFaultInterrupt/pageFaultHandler.h"
#include "doubleFault.h"
#include "timeInterrupts/rtcInterrupt.h"
#include "programmableInterrupts.h"

// defining the interrupt descriptor table and making some functions for it:
InterruptDescriptor64* mainInterruptDescriptorTablePointer;
IdtRegister            idtRegister;
void InterruptDescriptor64::setOffset(uint64_t offset)
{
    this->offsetLow  = OFFSET_LOW_BIT_MASK( offset);
    this->offsetMid  = OFFSET_MID_BIT_MASK( offset);
    this->offsetHigh = OFFSET_HIGH_BIT_MASK(offset);
}
uint64_t InterruptDescriptor64::getOffset()
{
    uint64_t offset = 0;

    // using bitwise or in order to set the value offset with the right bits:
    offset |= ((uint64_t)offsetLow )       ;
    offset |= ((uint64_t)offsetMid )  << 16;
    offset |= ((uint64_t)offsetHigh)  << 32;

    return offset;
}
void IdtSelector::setSelector()
{
    this->attr.tableIndicator = GDT_INDICATOR;
    this->attr.privilegeLevel = SAME_PRIVILEGE_AS_DESCRIPTOR_TABLE;
    this->attr.index          = CODE_SEGMENT;
}



extern "C" void LoadIDT(); // a function that is written in asm and imported into c 
void initIDT()
{
    // getting an identity mapped page where the idt will sit in
    PhysicalAddress address = requestIdentityMappedPage();
    mainInterruptDescriptorTablePointer = (InterruptDescriptor64*)address.voidPtr;

    // setting the idt register with the size of the idt and the address of it ( idt register is 10 bytes and not 8 like usually )
    idtRegister.size      = PAGE_SIZE   ;
    idtRegister.idtAddress = address.raw;
    

    // loading all of the interrupts that the os supports right now:
    initKeyboardInterrupt();
    initRtcInterrupt();
    initPageFaultHandlerInterrupt();
    initDoubleFaultInterrupt();


    // remapping the programmable interrupt chips so it will correspond to the right index inside of the idt:  
    remapPic();


    outb(0x21, 0xfd);
	outb(0xa1, 0xff);

    LoadIDT(); // loading the structure of idt into memory
}



void initSpecificInterrupt(uint64_t isrAddress, uint8_t interruptStackTable, uint8_t typesAndAttributes, uint8_t extraBits, uint8_t interruptIndex)
{
    mainInterruptDescriptorTablePointer[interruptIndex].setIsrFunctionAddress(isrAddress);
    mainInterruptDescriptorTablePointer[interruptIndex].selector.setSelector();
    mainInterruptDescriptorTablePointer[interruptIndex].ist        = interruptStackTable;
    mainInterruptDescriptorTablePointer[interruptIndex].types_attr = typesAndAttributes;
    mainInterruptDescriptorTablePointer[interruptIndex].extraBits  = extraBits;          // this are extra 32 bits that we can use as we please
}