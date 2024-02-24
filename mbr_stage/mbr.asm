[org 0x7c00]



mov [BOOT_DISK], dl ; in the start of the programm we are the driver number is in dl so we save it for later use

mov bp, 0x7c00
mov sp, bp

call readDisk


jmp PROGRAM_SPACE

%include "mbr_stage/print.asm"
%include "mbr_stage/diskRead.asm"

times 510-($-$$) db 0

dw 0xaa55