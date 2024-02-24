#pragma once
#include "typedefs.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY (uint8_t*)0xb8000
extern uint16_t cursorPostion;

void setCursorPosition(uint16_t position);
uint16_t postionFromCords(uint8_t x, uint8_t y);
void printString(const char* str);
void printChar(char ch);

// priting integers:
// unsigned values:
void printInt(uint8_t  value);
void printInt(uint16_t value);
void printInt(uint32_t value);
void printInt(uint64_t value);
// signed values:
void printInt(char      value);
void printInt(short     value);
void printInt(int       value);
void printInt(long long value);


// priting hex values:
// unsigned values:
void printHex(uint8_t  value);
void printHex(uint16_t value);
void printHex(uint32_t value);
void printHex(uint64_t value);

// signed values:
void printHex(char      value);
void printHex(short     value);
void printHex(int       value);
void printHex(long long value);
