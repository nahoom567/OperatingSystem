#pragma once
#include "src/interrupts/idt.h"

#define KEYBOARD_INTERRUPT_IRQ_NUMBER 1
extern "C" void isrKeyboardHandler();

/**
 * setting up the keyboard interrupt inside of the interrupt descriptor table
*/
void initKeyboardInterrupt();


// the port that is used to get information from the keyboard:
#define KEYBOARD_DATA_PORT   0x60
#define READ_KEYBOARD_DATA() inb(KEYBOARD_DATA_PORT)
