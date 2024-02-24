ADDRESS_WITH_MEMORY_MAPS equ 0x7000

memoryRegionCount:
    db 0
    GLOBAL memoryRegionCount

detectMemory:
	mov ax, 0
	mov es, ax                       ; set the ax to the value 0 ( we are using ax because we cannot move a direct value into es)
	mov di, ADDRESS_WITH_MEMORY_MAPS ; place that we would like to store it ( it is random )
	mov edx, 0x534D4150              ; this stands for 'SMAP' in ascii 
	xor ebx, ebx

	.repeat:
		mov eax, 0xE820              ; bios function to query the memory map
		mov ecx, 24                  ; size of the structure
		int 0x15                     ; invoke BIOS interrupt for system services

		cmp ebx, 0                   ; check if the carry flag is set (indicates end of memory map)
		je .finished    

		add di, 24                   ; move to the next memory region entry
		inc byte [memoryRegionCount] ; increment the memory region count
		jmp .repeat

	.finished:
	ret