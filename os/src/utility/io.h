#pragma once
#include "typedefs.h"

void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

void remapPic();