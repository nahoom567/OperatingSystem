
jmp EnterProtectedMode

%include "bootloader_stage/gdt.asm"
%include "mbr_stage/print.asm"
%include "os/src/memoryManagment/physicalMemoryManager/detectMemory.asm"

EnterProtectedMode:
	call detectMemory
	call enableA20    		   
	cli 					   ; clearing the interrupt flag
	lgdt [gdt_descriptor]      ; this loads the gdt into the gdt registers
	mov eax, cr0			   ; this moves the control register (cr0) which controls aspects in the os
	or eax, 1				   ; setting the least sagnificit bit which says that switching to protected mode 
	mov cr0, eax	

	jmp codeseg:startProtectedMode  ; preforming far jump which is necessary because the cpu needs to switch to a new code segment after switching to 32 bit
	
; this gives the option to access more then 1 mb in real mode this is enabled to be comptiable with older version
enableA20:
	in al, 0x92  ; read from port 0x92
	or al, 2     ; set bit 1 in the value read
	out 0x92, al ; write the modified value back to port 0x92
	ret



[bits 32]
%include "bootloader_stage/cpuDetection.asm"
%include "bootloader_stage/identityPaging.asm"

startProtectedMode:
	; setting the data segment to all the semnet registers:
	mov ax, dataseg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; small testing:
	; mov ebx, 0xb8008
	; mov [ebx], byte ' '
	; sub ebx, 2
	; 
	; xor eax,gdt_codedesc 
	; xor edx,0xFF
	; 
	; mov ecx, 4
	; .looping
	; 	and edx,eax
	; 	shr eax, 8
	; 	add edx, 48 
	; 	mov [ebx], dl
	; 	sub bx, 2 
	; 	loop .looping
		
	mov [0xb8000], byte 'H'
	mov [0xb8002], byte 'e'
	mov [0xb8004], byte 'l'
	mov [0xb8006], byte 'l'
	mov [0xb8008], byte 'o'
	mov [0xb800a], byte ' '
	mov [0xb800c], byte 'W'
	mov [0xb800e], byte 'o'
	mov [0xb8010], byte 'r'
	mov [0xb8012], byte 'l'
	mov [0xb8014], byte 'd'
	mov [0xb8016], byte '!'
	mov [0xb8018], byte ' '

	
	call detectCpuId
	call detectLongMode
	call setUpIdentityPaging
	call editGDT
	jmp codeseg:start64Bit ; preforming far jump which is necessary because the cpu needs to switch to a new code segment after switching to 64 bit

[bits 64]
[extern _start]

%include "os/src/IDT/idt.asm"

start64Bit:
	; changing the background of my os into the color blue:
	mov edi, 0xb8000
	mov rax, 0x0a200a200a200a20
	mov ecx, 500
	rep stosq


	call _start
	jmp $

times 21000-($-$$) db 0 ; 10000