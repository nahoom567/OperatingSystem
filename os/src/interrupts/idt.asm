[extern _idt]
REGISTER_COUNT          equ 15
REGISTER_SIZE_IN_BYTES  equ 8

%include "bootloader_stage/macros.asm"

[extern idtRegister          ]
[extern isrKeyboardHandler   ]
[extern isrPageFaultHandler  ]
[extern isrDoubleFaultHandler]
[extern isrRtcHandler        ]


asmIsrKeyboard:
  PUSHALL
  call isrKeyboardHandler
  POPALL

  iretq ; returning from the interrupt and enabling the interrupt flag 
  GLOBAL asmIsrKeyboard


asmIsrPageFault:
  PUSHALL

  ; the value in rdi is tha actual value that is being passed it is not passed using the stack frame but using the rdi register ( gnu decided it )
  mov rdi, [rsp+REGISTER_COUNT*REGISTER_SIZE_IN_BYTES]
  call isrPageFaultHandler


  POPALL
  iret ; returning from the interrupt and enabling the interrupt flag 
  GLOBAL asmIsrPageFault

asmIsrDoubleFault:
  PUSHALL
  call isrDoubleFaultHandler
  POPALL
  
  iret ; returning from the interrupt and enabling the interrupt flag 
  GLOBAL asmIsrDoubleFault

asmIsrRTC:
  PUSHALL
  call isrRtcHandler
  POPALL

  iret
  GLOBAL asmIsrRTC

LoadIDT:
  lidt[idtRegister] ; this instruction is loading the the idtRegster memory into the actual idt register
  sti               ; using this instruction which enabling interrupts to occur  
  ret
  GLOBAL LoadIDT