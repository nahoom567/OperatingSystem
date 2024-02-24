#include "io.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

/**
 * this functions is using the inline asm it allowes to write asm code from inside of a c code
 * in this specific example the %0, %1 are the operand that go into the standard instruction of outb in asm 
 * "a"(val): The "a" constraint tells the compiler to store the operand val in the eax register
 * "Nd"(port): The "Nd" constraint is a little more complex. 
 * The N means that the value must be an 8-bit constant. 
 * The d means that the compiler should use the edx register. 
 * However, in the case of the outb instruction, the port number can be specified directly in the instruction itself 
 * (this is known as an “immediate” operand). 
 * So, the edx register is not actually used here, and the port value is embedded directly into the instruction.
*/
void outb(unsigned short port, unsigned char val)
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
    // in asm it will look something like this:
    /*
        mov eax, val
        mov dx, port
        out dx, al
    */
}

unsigned char inb(unsigned short port)
{
    unsigned char returnValue;
    asm volatile("inb %1, %0" : "=a"(returnValue): "Nd"(port));
    // will look something like this:
    /*
        mov dx, port
        in al, dx
        mov val, eax
    */
    
    return returnValue;
}

void remapPic()
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);   // save the current contents of PIC1_DATA register
    a2 = inb(PIC2_DATA);   // save the current contents of PIC2_DATA register

    // initialize both PICs with ICW1_INIT and ICW1_ICW4
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // set the interrupt vector offset for each PIC
    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 8);

    // connect PIC1 to IRQ2 and PIC2 to IRQ9
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);

    // set x86 mode in ICW4 for both PICs
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // restore the saved values to PIC1_DATA and PIC2_DATA
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}