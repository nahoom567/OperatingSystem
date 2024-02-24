GLOBAL ata_read
GLOBAL ata_write
; An ATA is a kind of Hard Disk
; The LBA is the same as sector, the first block being LBA 0 and the second LBA 1

;=============================================================================
; ATA read sectors (LBA mode) 
;
; @param EAX Logical Block Address of sector
; @param ECX Size of data to read in bytes
; @param RDI The address of buffer to put data obtained from disk
;
; @return None
;=============================================================================
ata_read:
            pushfq
            and rax, 0x0FFFFFFF
            push rax
            push rbx
            push rcx
            push rdx
            push rdi

            mov rbx, rax         ; Save LBA in RBX
            mov ecx, ecx         ; Save size in ECX
            shr ecx, 9           ; Convert size to sectors (size/512)
            mov cl, cl           ; Move the lower byte of ECX to CL

            mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
            shr eax, 24          ; Get bit 24 - 27 in al
            or al, 11100000b     ; Set bit 6 in al for LBA mode
            out dx, al

            mov edx, 0x01F2      ; Port to send number of sectors
            mov al, cl           ; Get number of sectors from CL
            out dx, al

            mov edx, 0x1F3       ; Port to send bit 0 - 7 of LBA
            mov eax, ebx         ; Get LBA from EBX
            out dx, al

            mov edx, 0x1F4       ; Port to send bit 8 - 15 of LBA
            mov eax, ebx         ; Get LBA from EBX
            shr eax, 8           ; Get bit 8 - 15 in AL
            out dx, al


            mov edx, 0x1F5       ; Port to send bit 16 - 23 of LBA
            mov eax, ebx         ; Get LBA from EBX
            shr eax, 16          ; Get bit 16 - 23 in AL
            out dx, al

            mov edx, 0x1F7       ; Command port
            mov al, 0x20         ; Read with retry.
            out dx, al

.still_going:  
            in al, dx
            test al, 8           ; the sector buffer requires servicing.
            jz .still_going      ; until the sector buffer is ready.

            mov rax, 256         ; to read 256 words = 1 sector
            xor bx, bx
            mov bl, cl           ; read CL sectors
            mul bx
            mov rcx, rax         ; RCX is counter for INSW
            mov rdx, 0x1F0       ; Data port, in and out
            rep insw             ; in to [RDI]

            pop rdi
            pop rdx
            pop rcx
            pop rbx
            pop rax
            popfq
            ret
            


;=============================================================================
; ATA write sectors (LBA mode) 
;
; @param EAX Logical Block Address of sector
; @param ECX Size of data to write in bytes
; @param RDI The address of data to write to the disk
;
; @return None
;=============================================================================
 
ata_write:
    pushfq
    and rax, 0x0FFFFFFF
    push rax
    push rbx
    push rcx
    push rdx
    push rdi
 
    mov rbx, rax         ; Save LBA in RBX
    mov ecx, ecx         ; Save size in ECX
    shr ecx, 9           ; Convert size to sectors (size/512)
    mov cl, cl           ; Move the lower byte of ECX to CL

    mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
    shr eax, 24          ; Get bit 24 - 27 in al
    or al, 11100000b     ; Set bit 6 in al for LBA mode
    out dx, al
 
    mov edx, 0x01F2      ; Port to send number of sectors
    mov al, cl           ; Get number of sectors from CL
    out dx, al
 
    mov edx, 0x1F3       ; Port to send bit 0 - 7 of LBA
    mov eax, ebx         ; Get LBA from EBX
    out dx, al
 
    mov edx, 0x1F4       ; Port to send bit 8 - 15 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 8           ; Get bit 8 - 15 in AL
    out dx, al
 
 
    mov edx, 0x1F5       ; Port to send bit 16 - 23 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 16          ; Get bit 16 - 23 in AL
    out dx, al
 
    mov edx, 0x1F7       ; Command port
    mov al, 0x30         ; Write with retry.
    out dx, al
 
.still_going:  in al, dx
    test al, 8           ; the sector buffer requires servicing.
    jz .still_going      ; until the sector buffer is ready.
 
    mov rax, 256         ; to read 256 words = 1 sector
    xor bx, bx
    mov bl, cl           ; write CL sectors
    mul bx
    mov rcx, rax         ; RCX is counter for OUTSW
    mov rdx, 0x1F0       ; Data port, in and out
    mov rsi, rdi
    rep outsw            ; out
 
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    popfq
    ret