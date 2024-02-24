#ifndef IDT_H
#define IDT_H

#include "src/utility/typedefs.h"
#include "src/utility/io.h"
#include "KBScanCOdeSets.h"

struct IDT64
{
  uint16_t offset_low;
  uint16_t selector;
  uint8_t ist;
  uint8_t types_attr;
  uint16_t offset_mid;
  uint32_t offset_high;
  uint32_t zero;
};


void InitializeIDT();

extern "C" void isr1_handler();

#endif