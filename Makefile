include dependencies.mk 

# making it so the dir we currently will be used as an include dir:
CURRENT_DIR := $(shell pwd)
INCLUDE_DIR := -I$(CURRENT_DIR)/os -I$(CURRENT_DIR)/os/src/utility
# setting the flags for the gcc compilation:
GCC_COMPILATION_FLAGS := -Ttext 0x9000 -ffreestanding -mno-red-zone -m64 $(INCLUDE_DIR)
GCC_TEST_ASM_FLAGS := -Ttext 0x9000 -ffreestanding -mno-red-zone -m64 -S $(INCLUDE_DIR)

asm:
	mkdir -p myasmFiles
	#compiling the files into asm:
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/kernel.cpp" -o "myasmFiles/kernel.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/utility/printText.cpp" -o "myasmFiles/printText.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/utility/io/io.cpp" -o "myasmFiles/io.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/utility/bitmap.cpp" -o "myasmFiles/bitmap.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/utility/sysCalls.cpp" -o "myasmFiles/sysCalls.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/interrupts/idt.cpp" -o "myasmFiles/idt.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/interrupts/doubleFault.cpp" -o "myasmFiles/doubleFault.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/interrupts/programmableInterrupts.cpp" -o "myasmFiles/programmableInterrupts.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/interrupts/keyboardInterrupt/keyboardInterrupts.cpp" -o "myasmFiles/keyboardInterrupts.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/interrupts/pageFaultInterrupt/pageFaultHandler.cpp" -o "myasmFiles/pageFaultHandler.myasm"	
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/memoryManagment/physicalMemoryManager/memoryMap.cpp" -o "myasmFiles/memoryMap.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/memoryManagment/physicalMemoryManager/pageFrameAllocator.cpp" -o "myasmFiles/pageFrameAllocator.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/memoryManagment/virtualMemoryManager/virtualMemoryManager.cpp" -o "myasmFiles/virtualMemoryManager.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/test/test.cpp" -o "myasmFiles/test.myasm"
	x86_64-elf-gcc $(GCC_TEST_ASM_FLAGS) -c "os/src/test/gccTest.cpp" -o "myasmFiles/gccTest.myasm"

compile:
	# making a dir for the binaries in the case that it does not exist yet
	mkdir -p bin

	# comping asm files:
	nasm mbr_stage/mbr.asm -f bin -o bin/mbr.bin
	nasm bootloader_stage/bootloader.asm -f elf64 -o bin/bootloader.o

	# compiling the cpp files:
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/kernel.cpp" -o "bin/kernel.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/utility/printText.cpp" -o "bin/printText.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/utility/io/io.cpp" -o "bin/io.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/utility/io/time.cpp" -o "bin/time.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/utility/bitmap.cpp" -o "bin/bitmap.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/utility/sysCalls.cpp" -o "bin/sysCalls.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/interrupts/idt.cpp" -o "bin/idt.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/interrupts/timeInterrupts/rtcInterrupt.cpp" -o "bin/rtcInterrupt.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/interrupts/doubleFault.cpp" -o "bin/doubleFault.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/interrupts/programmableInterrupts.cpp" -o "bin/programmableInterrupts.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/interrupts/keyboardInterrupt/keyboardInterrupts.cpp" -o "bin/keyboardInterrupts.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/interrupts/pageFaultInterrupt/pageFaultHandler.cpp" -o "bin/pageFaultHandler.o"	
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/memoryManagment/physicalMemoryManager/memoryMap.cpp" -o "bin/memoryMap.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/memoryManagment/physicalMemoryManager/pageFrameAllocator.cpp" -o "bin/pageFrameAllocator.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/memoryManagment/virtualMemoryManager/virtualMemoryManager.cpp" -o "bin/virtualMemoryManager.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/test/test.cpp" -o "bin/test.o"

link: compile
	# linking
	x86_64-elf-ld -T link.ld
	cat bin/mbr.bin bin/kernel.bin > bin/osLoader.img

run: link
	qemu-system-x86_64 -drive file=bin/osLoader.img,format=raw -m 256M -cpu qemu64
