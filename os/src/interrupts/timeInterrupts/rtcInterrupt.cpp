#include "rtcInterrupt.h"
#include "src/interrupts/idt.h"
#include "src/interrupts/programmableInterrupts.h"
#include "src/test/test.h"
#include "src/utility/utility.h"

void isrRtcHandler()
{
    //testTime();
    printString("value");


    
    picSendEOI(RTC_INTERRUPT_IRQ_NUMBER);
}

void initRtcInterrupt()
{
    extern uint64_t asmIsrRTC; // getting the address of the isr function that is written in asm:
    uint64_t asmIsrRTCAddress = (uint64_t)(&asmIsrRTC);


    initSpecificInterrupt(asmIsrRTCAddress, NO_INTERRUPT_STACK_TABLE, 
        IDT_TA_InterruptGate, NO_INTERRUPT_EXTRA_BITS, InterruptIndexes::cmos_real_time_clock);
}