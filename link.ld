OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT
(
    bin/bootloader.o
    bin/kernel.o
    bin/printText.o
    bin/io.o
    bin/bitmap.o
    bin/idt.o
    bin/memoryMap.o
    bin/virtualMemoryManager.o
    bin/pageFrameAllocator.o
    bin/test.o
    bin/keyboardInterrupts.o
    bin/programmableInterrupts.o
    bin/pageFaultHandler.o
    bin/doubleFault.o
    bin/sysCalls.o
    bin/time.o
    bin/rtcInterrupt.o
)

OUTPUT
(
    bin/kernel.bin
)

SECTIONS
{
    _KernelStart = .; /* the starting address of the kernel */
    . = 0x9000;

    .text : ALIGN(0x1000)
    {
        . = . + SIZEOF_HEADERS;
        *(.text)
    }

    .data : ALIGN(0x1000)
    {
        *(.data)
    }

    
    .bss : ALIGN(0x1000)
    {
        *(COMMON)
        *(.bss)
    }

    
    _KernelEnd = .; /* the last address of the kernel */
}