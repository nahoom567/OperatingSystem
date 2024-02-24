#include "idt.h"
#include "src/utility/printText.h"

extern IDT64 _idt[256];
extern uint64_t isr1;
extern "C" void LoadIDT();

// function pointer called mainKeyboardHandler
void (*mainKeyboardHandler)(uint8_t scanCode) = NULL;

void keyboardHnalder(uint8_t scanCode)
{
    
    // checking if the scanCode is outside the bound of the translation buffer
    if(scanCode > SIZE_OF_ScanCodeLookupTable)
    {
        return;
    }
    
    if(KBSet1::ScanCodeLookupTable[scanCode] != 0)
    {
        printChar(KBSet1::ScanCodeLookupTable[scanCode]);
    }
    else 
    {
        switch (scanCode) 
        {
        case 0x8E: // BackSpace hex
            if(cursorPostion % VGA_WIDTH == 0)
            {
                setCursorPosition(cursorPostion - VGA_WIDTH);
                for (uint8_t i = 0; i < VGA_WIDTH; i++)
                {
                    printChar(' ');
                }
                setCursorPosition(cursorPostion - VGA_WIDTH);
                break;
            }
            setCursorPosition(cursorPostion - 1);
            printChar(' ');
            setCursorPosition(cursorPostion - 1);

            break;
        }
    }
}


void InitializeIDT()
{
    mainKeyboardHandler = keyboardHnalder;

    _idt[1].zero = 0;
    _idt[1].offset_low = (uint16_t)(((uint64_t)&isr1 & 0x000000000000ffff));
    _idt[1].offset_mid = (uint16_t)(((uint64_t)&isr1 & 0x00000000ffff0000) >> 16);
    _idt[1].offset_high = (uint32_t)(((uint64_t)&isr1 & 0xffffffff00000000) >> 32);
    _idt[1].ist = 0;
    _idt[1].selector = 0x08;
    _idt[1].types_attr = 0x8e;

    remapPic();
	
	outb(0x21, 0xfd);
	outb(0xa1, 0xff);
	LoadIDT();
}

void isr1_handler()
{
    uint8_t scanCode = inb(0x60);

    // in c/c++ if the left side is checked first and if it is evaluated to be false then it won't check the right side 
    // so it won't go out of memory bounds: 
    if(mainKeyboardHandler != NULL)
    {
        mainKeyboardHandler(scanCode);
    }

    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}