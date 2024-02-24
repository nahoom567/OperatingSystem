#include "programmableInterrupts.h"
#include "src/utility/utility.h"
#include "idt.h"

void remapPic()
{
    // saving the current data of the master and salve pics 
    uint8_t masterPicPreviousData = inb(PicPorts::master_data_port);   
    uint8_t slavePicPreviousData  = inb(PicPorts::slave_data_port );   

    // initialize both PICs with ICW1_INIT and ICW1_ICW4
    outb(PicPorts::master_command_port, ICW1_INIT | ICW1_ICW4);
    outb(PicPorts::slave_command_port , ICW1_INIT | ICW1_ICW4);

    // set the interrupt vector offset for each PIC
    outb(PicPorts::master_data_port, InterruptIndexes::programmable_interrupt_timer);
    outb(PicPorts::slave_data_port , InterruptIndexes::cmos_real_time_clock);

    // connect PIC1 to IRQ2 and PIC2 to IRQ9
    outb(PicPorts::master_data_port, 4);
    outb(PicPorts::slave_data_port , 2);

    // set x86 mode in ICW4 for both PICs
    outb(PicPorts::master_data_port, ICW4_8086);
    outb(PicPorts::slave_data_port , ICW4_8086);


    // restoring the values of the master and slave pics:
    outb(PicPorts::master_data_port, masterPicPreviousData);
    outb(PicPorts::slave_data_port , slavePicPreviousData );
}


void picSendEOI(uint8_t irqNumber)
{
    // making sure that the provided irq number is not outside of bounds:
    ASSERT((irqNumber <= LAST_INDEX_OF_SLAVE_PIC), 
        printString("Error: the provided interrupt request number is grater then the the max slave pic irq number\n") );


    // in the case that the handled interrupt was on the slave pic:
    if(irqNumber > LAST_INDEX_OF_MASTER_PIC)
    {
       outb(PicPorts::slave_command_port, END_OF_INTERRUPT);
       return;
    }
    

    // in the case that the handled interrupt was on the master pic:
    outb(PicPorts::master_command_port, END_OF_INTERRUPT);
}