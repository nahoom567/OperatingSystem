#ifndef IDT_H
#define IDT_H

#include "src/utility/typedefs.h"
#include "src/utility/io/io.h"

enum InterruptIndexes
{
  page_fault_interrupt_index   = 0xE ,
  double_fault_interrupt_index = 0x8 ,
  programmable_interrupt_timer = 0x20,
  keyboard_interrupt_index     = 0x21,
  cmos_real_time_clock         = 0x27,
};

#define PAGE_FAULT_INTERRUPT_INDEX  0xE


void initSpecificInterrupt(uint64_t isrAddress, uint8_t interruptStackTable, uint8_t typesAndAttributes, uint8_t extraBits, uint8_t interruptIndex);


/**
 * @brief the function is idt ( interrupt descriptor table ) and all of the interrupts that the os currently supports
*/
void initIDT();




/* this is the definition of idt register which is 10 bytes instead of the usual 8 bytes registers that in 64 bit system
 the first 2 bytes ( dw ) are the size of the idt, and the second 8 bytes ( dq ) are the first address of the idt */
typedef struct
{
    uint16_t size;
    uint64_t idtAddress;
}__attribute__((packed)) IdtRegister;




/**
 * @brief the selector values are telling us where the isr function ( interrupt service routine ) is located at what privileges it has
 * in what kind of descriptor table it is located at and so on
*/
typedef union
{
  struct
  {
    /* Requestor Privilege Level (RPL): 
    00 - the same as the descriptor
    01 - next higher privilege level 
    10 - reserved 11 */
    uint16_t privilegeLevel   : 2 ;      
    uint16_t tableIndicator   : 1 ;     // table indicator ( false: GDT - our case, true: LDT )
    uint16_t index            : 13;     // index into the GDT or LDT, 0 - null, 1 - code segment, 2 - data segment and so on
  } attr;
  uint16_t raw;



  /** 
   * @brief the function that is setting the selector in the current way we want it to be configured:
   * gdt, the same privilege level as the descriptor table and in the code segment 
  */
  void setSelector();
  // some defines for setting the selector
  #define GDT_INDICATOR                      0
  #define SAME_PRIVILEGE_AS_DESCRIPTOR_TABLE 0
  #define CODE_SEGMENT                       1
} __attribute__((packed)) IdtSelector;



struct InterruptDescriptor64
{
  
  uint16_t offsetLow;
  IdtSelector selector; // the code segment that the isr (interrupt service routine ) function is located at
  
  uint8_t  ist;           // the interrupt stack table it is 0 for ( interrupt gate and trap gate which are explained above ) 
  #define NO_INTERRUPT_STACK_TABLE 0

  uint8_t  types_attr;    // the type of interrupt explained below
  uint16_t offsetMid;
  uint32_t offsetHigh;
  uint32_t extraBits; // extra bits that are used for alignment ( in the future may they can be used for flags ) 
  #define NO_INTERRUPT_EXTRA_BITS 0





  // ways to configure the offset, the offset is address of the isr function that is being called when the interrupt is triggered:
  #define OFFSET_LOW_BIT_MASK( offset)      (uint16_t)((offset & 0x000000000000FFFF)      )
  #define OFFSET_MID_BIT_MASK( offset)      (uint16_t)((offset & 0x00000000FFFF0000) >> 16)
  #define OFFSET_HIGH_BIT_MASK(offset)      (uint32_t)((offset & 0xFFFFFFFF00000000) >> 32)
  /**
   * @brief this function is getting an offset which is the address of the isr function that we want to be called when the interrupt is triggered
   * 
  */
  void     setOffset(uint64_t offset);
  #define  setIsrFunctionAddress(offset) setOffset(offset)
  uint64_t getOffset();



  /* there are two main types of interrupts interrupts which are interrupt gates which are hardware interrupts when certain hardware input is 
  given then the interrupt is being called.

  the second type of interrupt is trap gate interrupt which is a software interrupt, this interrupt is being performed 
  when a certain operation in the software is being performed*/ 
  #define IDT_TA_InterruptGate 0b10001110  
  #define IDT_TA_TrapGate      0b10001111

  /*there is a third type of gate known as a Call Gate, which is part of the Task State Segment(TSS) 
    and is used in task switching and calling functions across privilege levels.
    call Gate is used in the context of task switching and inter-privilege level function calls,
    it allows programs to call functions in different privilege levels.
  */
  #define IDT_TA_CallGate      0b10001100
  
} __attribute__((packed));

#endif