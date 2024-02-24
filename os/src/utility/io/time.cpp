#include "time.h"
#include "io.h"

uint8_t readCMOS(uint8_t index)
{
    uint8_t cmosValue = 0;
    asm volatile(
        "cli               \n\t" // turning off interrupts
        "movb %%al, %1     \n\t" // moving the index address
        "outb %%al, $0x70  \n\t" // copying the address to the CMOS register
        "inb  $0x71, %%al  \n\t" // fetching one byte from the CMOS into the al
        "sti               \n\t" // turning the interrupts back on
        "mov %%al, %0      \n\t" // saving the value into 
        : "=r"(cmosValue)        // output operands
        : "r" (index)            // input operands
        : "memory", "%al"        // things that can be changed "memory" means that values in certain addresses can be changed
    );

    return cmosValue;
}


TimeStamp getCMOSTimeStamp()
{
    return {readCMOS(CMOS_ADDRESSES::seconds), readCMOS(CMOS_ADDRESSES::minutes), readCMOS(CMOS_ADDRESSES::hours)};
}

void turnPeriodicRtcInterrupt(Rate interruptTriggerRate)
{
    DISABLE_INTERRUPTS();
    
    // we are selecting the register B in the cmos and also disabling the non maskable interrupts, the 4 upper bits control the address 
    // the lower bits contain flags
    outb(IO_PORTS::cmos_address_port, DISABLE_NON_MASKABLE_INTERRUPTS_VALUE);
    // reading the current value in the register B
    char previousValue = inb(IO_PORTS::cmos_data_port); 
    // setting the index again because reading from the cmos data port is automatically increasing the index 
    outb(IO_PORTS::cmos_address_port, DISABLE_NON_MASKABLE_INTERRUPTS_VALUE);
    outb(IO_PORTS::cmos_address_port, previousValue | PERIODIC_INTERRUPT_BIT);

    ENABLE_INTERRUPTS();
}