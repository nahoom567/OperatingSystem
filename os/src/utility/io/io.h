#pragma once
#include "typedefs.h"

#define DISABLE_INTERRUPTS() asm volatile("cli")
#define ENABLE_INTERRUPTS()  asm volatile("sti")

void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

enum IO_PORTS
{
    //CMOS Address Port (Port 0x70): Specifies the address (register index) of the Real-Time Clock (RTC) or CMOS configuration register.
    cmos_address_port = 0x70,
    // CMOS Data Port (Port 0x71): Used for reading from or writing to the RTC or CMOS configuration register specified by the CMOS Address Port.
    cmos_data_port    = 0x71,
};