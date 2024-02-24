include dependencies.mk 

# making it so the dir we currently will be used as an include dir:
CURRENT_DIR := $(shell pwd)
INCLUDE_DIR := -I$(CURRENT_DIR)/os -I$(CURRENT_DIR)/os/src/utility
# setting the flags for the gcc compilation:
GCC_COMPILATION_FLAGS := -Ttext 0x9000 -ffreestanding -mno-red-zone -m64 $(INCLUDE_DIR)

compile:

	# making a dir for the binaries in the case that it does not exist yet
	mkdir -p bin

	# comping asm files:
	nasm mbr_stage/mbr.asm -f bin -o bin/mbr.bin
	nasm bootloader_stage/bootloader.asm -f elf64 -o bin/bootloader.o

	# compiling the cpp files:
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/kernel.cpp" -o "bin/kernel.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/utility/printText.cpp" -o "bin/printText.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/utility/io.cpp" -o "bin/io.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/utility/bitmap.cpp" -o "bin/bitmap.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/IDT/idt.cpp" -o "bin/idt.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/memoryManagment/physicalMemoryManager/memoryMap.cpp" -o "bin/memoryMap.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/memoryManagment/physicalMemoryManager/pageFrameAllocator.cpp" -o "bin/pageFrameAllocator.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/memoryManagment/virtualMemoryManager/virtualMemoryManager.cpp" -o "bin/virtualMemoryManager.o"
	x86_64-elf-gcc $(GCC_COMPILATION_FLAGS) -c "os/src/test/test.cpp" -o "bin/test.o"

	
	
	# '=' is used for recursive assignment, and the right-hand side is evaluated every time the variable is referenced.
	# ':=' is used for simple assignment, and the right-hand side is evaluated once at the time of the assignment, and the result is stored for later use.
	# SRC_DIR = os
	# this command is making it so all of the files inside of SRC_DIR and in the sub folders of SRC_DIR will inside of SRC_FILES
	# SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.cpp")

link: compile
	# linking
	x86_64-elf-ld -T link.ld
	cat bin/mbr.bin bin/kernel.bin > bin/osLoader.img

run: link
	qemu-system-x86_64 -drive file=bin/osLoader.img,format=raw -m 256M -cpu qemu64
