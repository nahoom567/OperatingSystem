#include "bitmap.hpp"
#include "printText.h"
#include "assert.h"

bool BitMap::operator[](uint64_t index)
{
    ASSERT(index >= m_bitCount, printString("Error: index out of bounds\n")) // checking whether the index is too large:
    
    uint8_t specificBitMask = 0b10000000 >> (index % 8); // shifting the mask the amount of times that it is needed to create a mask for wanted bit
    

    return (m_buffer[index / 8] & specificBitMask);        // if the bit in the wanted index is positive then we return true and if not we return false
}

void BitMap::setBit(uint64_t index, bool value)
{
    ASSERT(index >= m_bitCount, printString("Error: index out of bounds\n")) // checking whether the index is too large:


    uint8_t specificBitMask = 0b10000000 >> (index % 8); // the bit that we need is turned on
    m_buffer[index / 8] &= ~specificBitMask;
    if (value)
    {
        m_buffer[index / 8] |= specificBitMask;
    }
}