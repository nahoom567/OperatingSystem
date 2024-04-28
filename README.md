# OS
> [!NOTE]
> The project will work on Linux out of the box (mainly Ubuntu, the OS it was developed with); it was not tested on Windows and Mac.

This is an Operating System that specializes mainly in managing memory using paging, virtual memory, and a free memory pool for dynamic allocation of memory.

# Installation
#### Ubuntu:
1. Clone the projecct
2. cd project
3. sudo apt install `make`
4. make `dependencies`
5. cd `_linker_compiler_installer`
6. make `compiler_and_linker_dependencies`
7. In the case that it fails to run add the following to the bashrc PATH: "/usr/local/x86_64elfgcc/bin" with the command:
`echo` 'export PATH="$PATH:/usr/local/x86_64elfgcc/bin"' >> ~/.bashrc;

#### Other Linux distributions:
1. Clone the projecct
2. cd project
3. install `make` `qemu-system-x86` `nasm`
4. cd `_linker_compiler_installer`
5. make `compiler_and_linker_dependencies`
6. In the case that it fails to run add the following to the bashrc PATH: "/usr/local/x86_64elfgcc/bin" with the command:
`echo` 'export PATH="$PATH:/usr/local/x86_64elfgcc/bin"' >> ~/.bashrc;

### Running the project:
#### Regular configuration:
1. `cd project`
2. `make run`
#### Debug configuration:
1. `cd project`
2. `make debug`

### Cleaning the bin files:
1. `cd project`
2. `make clean`

### Creating new interrupts:
1. Navigate to the file: `project/scripts/createIsrHandlerFiles.py`.
2. Run the python script using your preferred method (e.g., command line or integrated development environment).
3. Follow the prompts to enter the interrupt type and interrupt name.
    - The interrupt name should be written in CamelCase convention. For example: `PageFault`, `Keyboard`.

### creating asm files:
1. `cd project`
2. `make compile_asm`

# Goals/Features:
- [x] mbr
- [x] bootloader
- [x] idt
- [x] physical memory manager
- [x] virtual memory manager
- [x] process free memory (heap)
- [*] process managemnt - almost done
- [*] filing system - in the middle of development, will soon be finished
