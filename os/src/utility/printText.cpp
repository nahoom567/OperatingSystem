#include "printText.h"
#include "typedefs.h"
#include "src/utility/io/io.h"


uint16_t cursorPostion = 0;

void setCursorPosition(uint16_t position)
{
    // tell the VGA controller that we're setting the low byte of the cursor position
    outb(0x3D4, 0x0F);

    // set the low byte of the cursor position
    outb(0x3D5, (uint8_t)( position & 0xFF));

    // tell the VGA controller that we're setting the high byte of the cursor position
    outb(0x3D4, 0x0E);

    // set the high byte of the cursor position
    outb(0x3D5, (uint8_t)( (position >> 8 ) & 0xFF));
    
    cursorPostion = position;
}

// starting the cords from (0, 0)
uint16_t postionFromCords(uint8_t x, uint8_t y)
{
    return (y * VGA_WIDTH) + x;
}

void printString(const char* str)
{
    uint16_t index = cursorPostion;
    int strIndex = 0;

    while(str[strIndex] != '\0')
    {
        switch (str[strIndex])
        {
            case '\n':
                index += VGA_WIDTH;
                index -= (index % VGA_WIDTH + 1);
                break;
            default:
                *(VGA_MEMORY + (index * 2)) = str[strIndex];
                break;
        }
        
        index++;
        strIndex++;
    }

    setCursorPosition(index);
}

void printChar(char ch)
{
    uint16_t index = cursorPostion;


    switch (ch)
    {
        case '\n':
            index += VGA_WIDTH;
            index -= (index % VGA_WIDTH + 1);
            break;
        default:
            *(VGA_MEMORY + (index * 2)) = ch;
            break;
    }
    index++;
    

    setCursorPosition(index);
}


template< typename T>
void printInt(T value)
{
    static char intToStringOutput[128];
    if(value == 0)
    {
        printChar('0');
    }
    int strIndex = 0;
    int index = cursorPostion;
    bool isNegative = false;

    if(value < 0)
    {
        value *= -1; // converting the value to positive
        isNegative = true;    
    }
    uint64_t newValue = (uint64_t)value;


    // pushing the number into the buffer:
    while(newValue > 0)
    {
        strIndex++;
        intToStringOutput[strIndex] = ((newValue % 10) + '0');
        newValue /= 10;
    }
    
    if(isNegative)
    {
        strIndex++;
        intToStringOutput[strIndex] = '-';
    }
    
    
    while(strIndex != 0)
    {  
        *(VGA_MEMORY + (index * 2)) = intToStringOutput[strIndex];
        strIndex--;
        index++;
    }


    setCursorPosition(index);
}


// unsigned values:
void printInt(uint8_t  value) { return printInt<uint8_t >(value); }
void printInt(uint16_t value) { return printInt<uint16_t>(value); }
void printInt(uint32_t value) { return printInt<uint32_t>(value); }
void printInt(uint64_t value) { return printInt<uint64_t>(value); }

// signed values:
void printInt(char      value) { return printInt<char     >(value); }
void printInt(short     value) { return printInt<short    >(value); }
void printInt(int       value) { return printInt<int      >(value); }
void printInt(long long value) { return printInt<long long>(value); }



template<typename T>
void printHex(T value)
{
    static char hexToStringOutput[128]; 
    T* valPtr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = (sizeof(T)) * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        temp = ((*ptr & 0xF0) >> 4);
        hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
        temp = ((*ptr & 0x0F));
        hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
    }
    hexToStringOutput[size + 1] = 0;
    printString("0x");
    printString(hexToStringOutput);
}

// unsigned values:
void printHex(uint8_t value)  { return printHex<uint8_t>(value); }
void printHex(uint16_t value) { return printHex<uint16_t>(value); }
void printHex(uint32_t value) { return printHex<uint32_t>(value); }
void printHex(uint64_t value) { return printHex<uint64_t>(value); }

// signed values:
void printHex(char      value) { return printHex<char>(value); }
void printHex(short     value) { return printHex<short>(value); }
void printHex(int       value) { return printHex<int>(value); }
void printHex(long long value) { return printHex<long long>(value); }




void printSpecial(int scanCode)
{
    switch (scanCode) 
    {
    case 0x8E: // backSpace hex
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