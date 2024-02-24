#pragma once
#include "src/utility/typedefs.h"


/**
 * @brief structure that represents the the ports of the pics ( programmable interrupts ) 
 * the pics handle 15 IRQs (Interrupt request ) from the hardware 
*/
enum PicPorts
{
    master_command_port = 0x20,
    master_data_port    = 0x21,
    slave_command_port  = 0xA0,
    slave_data_port     = 0xA1,
};

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

/**
 * @brief this function is used to remap the pics ( programmable interrupt chips ) to the right offsets in the idt, this is necessary
 * because when IBM created there computer they made a design mistake and 
*/
void remapPic();



// this is the value of EOI ( end of interrupt ) that is being signaled to the pic when the interrupt has been handled so interrupt can be used now:
#define END_OF_INTERRUPT           0x20 
// the indexes of the master pic is ( 0 - 7 ) and the indexes of the slave pic are ( 8 - 15 )
#define LAST_INDEX_OF_MASTER_PIC   7
#define LAST_INDEX_OF_SLAVE_PIC    15
/**
 * @brief the function is getting an interrupt request number and signaling to the right pic (programable interrupt chip ) that the interrupt 
 * has been handled and other hardware interrupts can be handled now   
*/
void picSendEOI(uint8_t irqNumber);

