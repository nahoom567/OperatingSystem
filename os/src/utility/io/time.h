#pragma once
#include "typedefs.h"

/// @brief the function is get the cmos index the address of the cmos and retrieves the information in it
/// @param index the address indicate what value in the cmos we want to access here are the values of each address:
/// Register  Contents            Range
/// 0x00      Seconds             0–59
/// 0x02      Minutes             0–59
/// 0x04      Hours               0–23 in 24-hour mode, 
///                               1–12 in 12-hour mode, highest bit set if pm
/// 0x06      Weekday             1–7, Sunday = 1
/// 0x07      Day of Month        1–31
/// 0x08      Month               1–12
/// 0x09      Year                0–99
/// 0x32      Century (maybe)     19–20?
/// 0x0A      Status Register A
/// 0x0B      Status Register B
/// @return the value in the address
uint8_t readCMOS(uint8_t index);

enum CMOS_ADDRESSES
{
    seconds = 0x00,
    minutes = 0x02,
    hours   = 0x4
};
struct
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour  ;
} typedef TimeStamp;

/// @return the function is returning a time stamp of the current time this time is retrieved from the cmos rtc( real time clock )
TimeStamp getCMOSTimeStamp();


enum Rate
{
    HZ_65536 = 0, // ( period ≈ 0.5   ms ) 
    HZ_16384 = 1, // ( period ≈ 3.05  ms ) 
    HZ_8192  = 2, // ( period ≈ 6.1   ms ) 
    HZ_4096  = 3, // ( period ≈ 12.2  ms ) 
    HZ_2048  = 4, // ( period ≈ 24.4  ms ) 
    HZ_1024  = 5, // ( period ≈ 48.8  ms ) 
    HZ_512   = 6, // ( period ≈ 97.7  ms ) 
    HZ_256   = 7, // ( period ≈ 195.3 ms ) 
};
// non maskable interrupt are very curtail interrupts that we do not map and cpu handles by itself the reason that we are disabling it 
// is because the cmos is controlled by itself so if it is crushing the bios cannot access and repair it  
#define DISABLE_NON_MASKABLE_INTERRUPTS_VALUE 0x8B
#define PERIODIC_INTERRUPT_BIT 0x40
void turnPeriodicRtcInterrupt(Rate interruptTriggerRate);