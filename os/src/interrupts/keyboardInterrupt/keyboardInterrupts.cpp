#include "keyboardInterrupts.h"
#include "src/utility/utility.h"
#include "KBScanCodeSets.h"
#include "src/interrupts/programmableInterrupts.h"


// function pointer called mainKeyboardHandler
void (*mainKeyboardHandler)(uint8_t scanCode) = NULL;
void keyboardHandler(uint8_t scanCode)
{
    // checking if the scanCode is outside the bound of the translation buffer
    if(scanCode > SIZE_OF_ScanCodeLookupTable)
    {
        printSpecial(scanCode); // a special print function that is getting a scan code and acts accordingly to it in the case it is supported
        return;
    }

    // making sure that the code is different from zero and then printing it:
    if(ScanCodeLookupTable[scanCode] != 0)
    {
        printChar(ScanCodeLookupTable[scanCode]);
    }
}



void isrKeyboardHandler()
{
    // getting the scan code from the keyboard that will be used to tell what kind of key the user has pressed:
    uint8_t scanCode = READ_KEYBOARD_DATA(); 


    // making sure that our function to deal with the scan codes is not null and calling
    if(mainKeyboardHandler != NULL)
    {
        mainKeyboardHandler(scanCode);
    }


    
    picSendEOI(KEYBOARD_INTERRUPT_IRQ_NUMBER);
}


void initKeyboardInterrupt()
{
    // setting up the keyboard handler function
    mainKeyboardHandler = keyboardHandler;

    extern uint64_t asmIsrKeyboard; // getting the address of the isr function that is written in asm:
    uint64_t asmIsrKeyboardAddress = (uint64_t)(&asmIsrKeyboard);


    initSpecificInterrupt(asmIsrKeyboardAddress, NO_INTERRUPT_STACK_TABLE, 
        IDT_TA_InterruptGate, NO_INTERRUPT_EXTRA_BITS, InterruptIndexes::keyboard_interrupt_index);
}