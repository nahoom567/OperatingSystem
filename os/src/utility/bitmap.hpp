#pragma once 
#include "typedefs.h"

#define BITS_IN_BYTE 8
#define m_PAGE_COUNT m_bitCount                        
class BitMap
{
public:
    BitMap(uint8_t* buffer, uint64_t size) : m_buffer(buffer), m_size(size), m_bitCount(size * BITS_IN_BYTE) { }
    bool operator[](uint64_t index);
    void setBit(uint64_t index, bool value);
    
    uint64_t m_size;    // the size of the buffer in bytes ( the buffer can store m_size * 8 bits in it )
    uint64_t m_bitCount;  // this size is in bits
    uint8_t* m_buffer;
};